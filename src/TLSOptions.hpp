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

struct TLSOptions {
  std::string certFileName;
  std::string keyFileName;
  bool enabled = false;
};

TLSOptions * getTLSOptions() {
  TLSOptions * tlsOptions = new TLSOptions();

  if (const char *certFileName = std::getenv("WEBSOCKET_SERVER_CERT_FILE_NAME")) {
    tlsOptions->certFileName = certFileName;
    tlsOptions->enabled = true;
  }

  if (const char *keyFileName = std::getenv("WEBSOCKET_SERVER_KEY_FILE_NAME")) {
    tlsOptions->keyFileName = keyFileName;
    tlsOptions->enabled = true;
  }

  return tlsOptions;
}
