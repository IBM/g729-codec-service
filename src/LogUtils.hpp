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
#include "spdlog/spdlog.h"
#include <strings.h>

spdlog::level::level_enum charToLogLevelEnum(const char *logLevelChar) {
  if (strcasecmp(logLevelChar, "TRACE") == 0) {
    return spdlog::level::trace;
  }

  if (strcasecmp(logLevelChar, "DEBUG") == 0) {
    return spdlog::level::debug;
  }

  if (strcasecmp(logLevelChar, "INFO") == 0) {
    return spdlog::level::info;
  }

  if (strcasecmp(logLevelChar, "WARN") == 0) {
    return spdlog::level::warn;
  }

  if (strcasecmp(logLevelChar, "ERROR") == 0) {
    return spdlog::level::err;
  }

    if (strcasecmp(logLevelChar, "OFF") == 0) {
    return spdlog::level::off;
  }
}


spdlog::level::level_enum getLogLevel() {
  spdlog::level::level_enum logLevelEnum = spdlog::level::info;
  if (const char *env_loglevel = std::getenv("LOG_LEVEL")) {
    logLevelEnum = charToLogLevelEnum(env_loglevel);
  }
  return logLevelEnum;
}
