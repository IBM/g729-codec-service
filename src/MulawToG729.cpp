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
#include "MulawToG729.hpp"
# include <memory>

MulawtoG729::MulawtoG729(bool annexbEnabled) {
    g729Encoder = new G729Encoder(annexbEnabled);
    mulawDecoder = new MulawDecoder();
}

MulawtoG729::~MulawtoG729() {
  delete g729Encoder;
  delete mulawDecoder;
}


void MulawtoG729::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame) {
    std::unique_ptr<AudioFrame> pcmOutputFrame(new AudioFrame());
    mulawDecoder->processFrame(inputFrame, *pcmOutputFrame);
    g729Encoder->processFrame(*pcmOutputFrame, outputFrame);
    audio_frame_destroy(*pcmOutputFrame);
}


void MulawtoG729::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame, VADEvent& vadEvent) {
    std::unique_ptr<AudioFrame> pcmOutputFrame(new AudioFrame());
    mulawDecoder->processFrame(inputFrame, *pcmOutputFrame);
    g729Encoder->processFrame(*pcmOutputFrame, outputFrame, vadEvent);
    audio_frame_destroy(*pcmOutputFrame);

}

