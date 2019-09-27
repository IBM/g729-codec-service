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
#include "G729ToMulaw.hpp"
# include <memory>
# include "spdlog/spdlog.h"

G729ToMulaw::G729ToMulaw() {
    g729Decoder = new G729Decoder();
    mulawEncoder = new MulawEncoder();
}

G729ToMulaw::~G729ToMulaw() {
    delete g729Decoder;
    delete mulawEncoder;
}

void G729ToMulaw::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame) {
    std::unique_ptr<AudioFrame> pcmOutputFrame(new AudioFrame());
    g729Decoder->processFrame(inputFrame, *pcmOutputFrame);
    mulawEncoder->processFrame(*pcmOutputFrame, outputFrame);
    audio_frame_destroy(*pcmOutputFrame);
}

void G729ToMulaw::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame, VADEvent &vadEvent) {
    std::unique_ptr<AudioFrame> pcmOutputFrame(new AudioFrame());
    g729Decoder->processFrame(inputFrame, *pcmOutputFrame, vadEvent);
    mulawEncoder->processFrame(*pcmOutputFrame, outputFrame);
    audio_frame_destroy(*pcmOutputFrame);
}
