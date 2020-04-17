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

#include <iostream>
#include <stdlib.h>

const std::string defaultAddress = "0.0.0.0";
const int defaultPort = 9001;
const int defaultIdleTimeout = 600; // Seconds

struct NetworkOptions {
  std::string addr;
  int port;
  int idleTimeout;
};

NetworkOptions generateNetworkOptions() {
  NetworkOptions networkOptions = {defaultAddress, defaultPort, defaultIdleTimeout};

  if (const char *websocketServerHost = std::getenv("WEBSOCKET_SERVER_HOST")) {
    networkOptions.addr = websocketServerHost;

    if (strcmp(websocketServerHost, "localhost") == 0) {
      networkOptions.addr = "127.0.0.1";
    }
  }

  if (const char *port = std::getenv("WEBSOCKET_SERVER_PORT")) {
    networkOptions.port = (int)std::strtol(port, NULL, 10);
  }

  if (const char *idleTimeout = std::getenv("WEBSOCKET_SERVER_IDLE_TIMEOUT")) {
    networkOptions.idleTimeout = (int) std::strtol(idleTimeout, NULL, 10);
  }

  return networkOptions;
}
