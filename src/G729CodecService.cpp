/*
 * Copyright (C) 2019 IBM Corp.
 *
 * This file is part of g729-codec-service.
 *
 * g729-codec-service is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * g729-codec-service is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with g729-codec-service.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "App.h"
#include "G729ToMulaw.hpp"
#include "MulawToG729.hpp"
#include "VADCodec.hpp"
#include "g711_extern.h"
#include "bcg729-extern.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "resource-messages.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "string.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "LogUtils.hpp"
#include "TLSOptions.hpp"
#include "NetworkOptions.hpp"

#include <signal.h>
#include <utility>
#define BIT_STREAM_SIZE 10
#define FRAME_SIZE 160
#define PACKET_SIZE 20 // ms
#define FRAME_SIZE_MS 10
#define COMPRESSION_RATE 16
#define FRAME_SIZE_IN_BYTES 160
#define MAX_ENCODER_OUTPUT_SIZE 10

auto logger = spdlog::stdout_color_mt("g729-codec-service", spdlog::color_mode::automatic);

const std::string START_OF_SPEECH_EVENT_MESSAGE =
    "{\"event\": \"START_OF_SPEECH\"}";
const std::string END_OF_SPEECH_EVENT_MESSAGE =
    "{\"event\": \"END_OF_SPEECH\"}";
const char * defaultSessionId = "N/A";
struct PerSocketData {
  VADCodec *audioProcessor = NULL;
  bool processing;
  std::string * sessionId;
};

std::string capabilities;


bool isAnnexBEnabled() {
  bool annexbEnabled = true;
  if (const char *annexbEnabledEnv = std::getenv("ANNEXB_ENABLED")) {
    if (strcmp(annexbEnabledEnv, "false") == 0) {
      annexbEnabled = false;
    }
  }
  return annexbEnabled;
}
std::string loadDefaultCapabilities(bool annexb) {
  std::string jsonStr =
      "{\"type\":\"Codec\",\"inbound\":{\"encodingName\":\"G729\",\"channels\":"
      "1,\"sampleRate\":8000,\"payloadType\":18,\"config\":\"annexb=yes\"},"
      "\"outbound\":{\"encodingName\":\"PCMU\",\"channels\":\"1\","
      "\"clockRate\":8000}}";
  rapidjson::Document document;
  document.Parse(jsonStr.c_str());

  if (!annexb) {
    document["inbound"]["config"] = "annexb=no";
  }
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);
  return std::string(buffer.GetString());
}

template <typename App>
void addWebSocketBehavior(App *server, std::string pattern) {
  typename App::WebSocketBehavior behavior = {};
  bool annexbEnabled = isAnnexBEnabled();

  capabilities = loadDefaultCapabilities(annexbEnabled);
  behavior.idleTimeout = 30;
  behavior.compression = uWS::SHARED_COMPRESSOR;
  behavior.maxPayloadLength = 16 * 1024;
  behavior.open = [](auto *ws, auto *req) {
    logger->debug("onWebSocketOpen");
    PerSocketData *perSocketData = (PerSocketData *)ws->getUserData();
    perSocketData->processing = false;
    std::string_view sessionIdHeaderVal = req->getHeader("session-id");
    logger->debug("session-header: {}", sessionIdHeaderVal);
    if (sessionIdHeaderVal.length()) {
      perSocketData->sessionId = new std::string(sessionIdHeaderVal);
    } else {
      perSocketData->sessionId = new std::string("N/A");
    }
  };

  behavior.message = [annexbEnabled](auto *ws, std::string_view message,
                        uWS::OpCode opCode) {
    if (opCode == uWS::BINARY) {
      PerSocketData *perSocketData = (PerSocketData *)ws->getUserData();
      VADCodec *audioProcessor = perSocketData->audioProcessor;
      VADEvent vadEvent;
      std::unique_ptr<AudioFrame> inputFrame(new AudioFrame());
      std::unique_ptr<AudioFrame> outputFrame(new AudioFrame());

      size_t payloadLength = message.length();
      inputFrame->size = (int)payloadLength;
      inputFrame->data = (int8_t *)message.data();

      logger->trace("got binary data of size = {}", payloadLength);
      audioProcessor->processFrame(*inputFrame, *outputFrame, vadEvent);
      if (vadEvent == VADEvent::START_OF_SPEECH) {
        ws->send(START_OF_SPEECH_EVENT_MESSAGE, uWS::OpCode::TEXT);
      } else if (vadEvent == VADEvent::END_OF_SPEECH) {
        ws->send(END_OF_SPEECH_EVENT_MESSAGE, uWS::OpCode::TEXT);
      }
      logger->trace("generated binary data of size = {}", outputFrame->size);

      std::string_view data =
          std::string_view((char *)outputFrame->data, outputFrame->size);
      ws->send(data, opCode);
      audio_frame_destroy(*outputFrame);
    } else {
      logger->trace("Got non-binary message");
      logger->debug("onMessage: {}", message);

      std::string_view listeningMessage = "{\"state\":\"listening\"}";
      rapidjson::Document d;

      d.Parse(std::string(message).c_str());

      if (d.HasMember("method")) {
        rapidjson::Value::MemberIterator method = d.FindMember("method");
        if (method->value.IsString()) {

          logger->debug("Sending Capabilities: {}", capabilities);
          rapidjson::Document c;
          const char *capabilitiesChar = capabilities.c_str();
          c.Parse(capabilitiesChar);
          rapidjson::StringBuffer buffer;
          rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
          c.Accept(writer);
          logger->debug("Capabilities Str: {}", buffer.GetString());
          ws->send(buffer.GetString(), opCode);
        }
      } else if (d.HasMember("inbound")) {
        logger->trace("hasInbound");
        const rapidjson::Value &inbound = d["inbound"];
        const rapidjson::Value &outbound = d["outbound"];

        if (inbound.HasMember("encodingName")) {
          logger->trace("hasEncodingName");
          rapidjson::Value::ConstMemberIterator encodingName =
              inbound.FindMember("encodingName");
          if (encodingName->value.IsString()) {
            const char *encodingNameStr = encodingName->value.GetString();
            PerSocketData *perSocketData = (PerSocketData *)ws->getUserData();
            if (strcmp(encodingNameStr, "G729") == 0) {
              const char * configValueStr = "N/A";
              if (inbound.HasMember("config") && inbound.FindMember("config")->value.IsString()) {
                rapidjson::Value::ConstMemberIterator configField = inbound.FindMember("config");
                configValueStr = configField->value.GetString();
              }

              VADCodec *decoder = new G729ToMulaw();

              logger->info(CWSCS0003I_MSG, *(perSocketData->sessionId), configValueStr);
              logger->debug("Building decoder...");
              perSocketData->audioProcessor = decoder;
              logger->debug("Built decoder...");
            } else {
              const char * configValueStr = "N/A";

              if (outbound.HasMember("config") && outbound.FindMember("config")->value.IsString()) {
                rapidjson::Value::ConstMemberIterator configField = outbound.FindMember("config");
                configValueStr = configField->value.GetString();
              }
              logger->info(CWSCS0002I_MSG, *(perSocketData->sessionId), configValueStr);
              logger->debug("Building encoder...");
              VADCodec *encoder = new MulawtoG729(annexbEnabled);
              perSocketData->audioProcessor = encoder;
              logger->debug("Built encoder...");
            }
            perSocketData->processing = true;
            logger->trace("Sending 'listening' response...");
            ws->send(listeningMessage, opCode);
          } else {
            logger->warn("'EncodingName', is not a string");
          }
        } else {
          logger->warn("Invalid Message received...");
        }
      } else {
        logger->warn("Invalid Message body...");
      }
    }
  };

  behavior.close = [](auto *ws, int code, std::string_view message) {
    logger->debug("onWebSocketClose code = {}, message = {}", code, message);

    PerSocketData *perSocketData = (PerSocketData *)ws->getUserData();
    logger->trace("processing = {}", perSocketData->processing);
    if (perSocketData->processing) {
      logger->trace("deleting audio processor");
      delete perSocketData->audioProcessor;
      delete perSocketData->sessionId;
    }
  };
  server->template ws<PerSocketData>(pattern, std::move(behavior));
}

template <typename AppType> class G729CodecService {
private:
  AppType *wsApp;
  void init() { addWebSocketBehavior<AppType>(wsApp, "/*"); }

public:
  G729CodecService() {
    wsApp = new AppType();
    init();
  }
  G729CodecService(TLSOptions *options) {
    wsApp = new AppType({
      .key_file_name = options->keyFileName.c_str(),
      .cert_file_name = options->certFileName.c_str()
    });
    init();
  };

  G729CodecService &&listen(std::string host, int port) {
    wsApp->listen(host, port, [host = host, port = port](auto *token) {
      if (token) {
        logger->info(CWSCS0001I_MSG, host, port);
      } else {
        logger->error(CWSCW0051E_MSG, host, port);
      }
    });
    return std::move(*this);
  }

  G729CodecService &&run() {
    wsApp->run();
    return std::move(*this);
  }
};

int main() {
  spdlog::level::level_enum logLevelEnum = getLogLevel();
  logger->set_level(logLevelEnum);

  NetworkOptions networkOptions = generateNetworkOptions();
  TLSOptions * tlsOptions = getTLSOptions();
  if (tlsOptions->enabled) {
    auto * service = new G729CodecService<uWS::SSLApp>(tlsOptions);
    service->listen(networkOptions.addr, networkOptions.port).run();
  } else {
    auto * service = new G729CodecService<uWS::App>();
    service->listen(networkOptions.addr, networkOptions.port).run();
  }
}
