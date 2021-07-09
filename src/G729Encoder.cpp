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
# include "spdlog/spdlog.h"

#include "G729Encoder.hpp"
#include <iostream>
# define FRAME_SIZE 80 // 80 samples per frame
# define MAX_ENCODER_OUTPUT 10
//# define
int encode_g729_frame(bcg729EncoderChannelContextStruct *encoderChannelContext, int16_t * pcm, int numberOfEncodingFrames, uint8_t * data) {
    int totalSize = 0;
    // 160 samples
    // 2 frames to decode
    // each frame has 80 samples, each sample is 16 bits (2 bytes)
    for (int i = 0; i < numberOfEncodingFrames; i++) {
         uint8_t bitStreamLength = 0;
         bcg729Encoder(encoderChannelContext, &pcm[i * (FRAME_SIZE)], &data[i * MAX_ENCODER_OUTPUT], &bitStreamLength);
         totalSize += bitStreamLength;
    }
    return totalSize;
}
// Sample size = 20ms => 320 bytes

//bcg729Encoder(bcg729EncoderChannelContextStruct *encoderChannelContext, int16_t inputFrame[], uint8_t bitStream[], uint8_t *bitStreamLength);

G729Encoder::G729Encoder(uint8_t vadEnabled) {
    encoderContext = initBcg729EncoderChannel(vadEnabled);
    voiceActive = false;
}

G729Encoder::~G729Encoder() {
    closeBcg729EncoderChannel(encoderContext);
}

void G729Encoder::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame) {
    int frameSize = inputFrame.size / 2;
    int numberOfEncodingFrames = frameSize / FRAME_SIZE;
    uint8_t * g729_frames = new uint8_t[numberOfEncodingFrames * MAX_ENCODER_OUTPUT]();
    int byteSize = encode_g729_frame(encoderContext, (int16_t *) inputFrame.data, numberOfEncodingFrames, g729_frames);
    outputFrame.data = (int8_t *) g729_frames;
    outputFrame.size = byteSize;
}


void G729Encoder::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame, VADEvent &vadEvent) {
    int frameSize = inputFrame.size / 2;
    int numberOfEncodingFrames = frameSize / FRAME_SIZE;
    uint8_t * g729_frames = new uint8_t[numberOfEncodingFrames * MAX_ENCODER_OUTPUT]();
    int byteSize = encode_g729_frame(encoderContext, (int16_t *) inputFrame.data, numberOfEncodingFrames, g729_frames);
    vadEvent = VADEvent::NONE;
    if (byteSize >= 2 && byteSize <= 12) {
      if (voiceActive) {
        voiceActive = false;
        vadEvent = VADEvent::END_OF_SPEECH;
      }
    } else {
      if (!voiceActive) {
        voiceActive = true;
        vadEvent = VADEvent::START_OF_SPEECH;
      }
    }
    outputFrame.data = (int8_t *) g729_frames;
    outputFrame.size = byteSize;
}

