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
# include "G729Decoder.hpp"
# include <iostream>
# include "spdlog/spdlog.h"
# define FRAME_SIZE 80
# define MAX_DECODER_INPUT 10
# define MAX_BIT_STREAM_SIZE 10

void decode_g729(bcg729DecoderChannelContextStruct *decoderContext, uint8_t * g729, int length, int16_t * pcm) {
    int numberOfDecoderPayloads = length / MAX_DECODER_INPUT;

    int remainingPacketLength = length;
    uint8_t frameErased = 0;
    uint8_t sidFrame = 0;
    int bitStreamSize = MAX_BIT_STREAM_SIZE;

    for (int i = 0; i < numberOfDecoderPayloads; i++) {
        if (remainingPacketLength >= MAX_BIT_STREAM_SIZE) {
            bitStreamSize = MAX_BIT_STREAM_SIZE;
        } else if (remainingPacketLength == 2) {
            sidFrame = 1;
            frameErased = 0;
            bitStreamSize = 2;
        } else {
            frameErased = 1;
            sidFrame = 1;
            bitStreamSize = 0;
        }
        remainingPacketLength -= bitStreamSize;

        bcg729Decoder(decoderContext, frameErased == 1 ? NULL: &g729[i*MAX_DECODER_INPUT], bitStreamSize, frameErased, sidFrame, 0, &pcm[i*FRAME_SIZE]);
    }
}
G729Decoder::G729Decoder() {
    decoderContext = initBcg729DecoderChannel();
    voiceActive = false;
}

G729Decoder::~G729Decoder() {
    closeBcg729DecoderChannel(decoderContext);
}

void G729Decoder::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame) {
    int sizeInBytes = inputFrame.size;
    int16_t * pcm = new int16_t[FRAME_SIZE * 2]();
    decode_g729(decoderContext, (uint8_t *) inputFrame.data, sizeInBytes, pcm);
    outputFrame.data = (int8_t *) pcm;
    outputFrame.size = (FRAME_SIZE) * 4;
}

void G729Decoder::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame, VADEvent &vadEvent) {
    int sizeInBytes = inputFrame.size;
    spdlog::trace("got frame of length = {}", sizeInBytes);
    if (sizeInBytes == 20 && !voiceActive) {
      vadEvent = VADEvent::START_OF_SPEECH;
      voiceActive = true;
              spdlog::debug("speech started");

    } else if (sizeInBytes <= 12 && sizeInBytes >= 2 && voiceActive) {
      vadEvent = VADEvent::END_OF_SPEECH;
              spdlog::debug("speech ended");

      voiceActive = false;
    } else {
      vadEvent = VADEvent::NONE;
    }
    int16_t * pcm = new int16_t[FRAME_SIZE * 2]();
    decode_g729(decoderContext, (uint8_t *) inputFrame.data, sizeInBytes, pcm);
    outputFrame.data = (int8_t *) pcm;
    outputFrame.size = (FRAME_SIZE) * 4;
    spdlog::trace("generated frame of size = {}", outputFrame.size);
}
