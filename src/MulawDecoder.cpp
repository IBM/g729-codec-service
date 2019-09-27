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
#include "MulawDecoder.hpp"


int16_t decode_mulaw_to_pcm_sample(uint8_t mulaw_sample) {
    mulaw_sample = ~mulaw_sample;
    int sign_bit = mulaw_sample & 0x80;
    int16_t exponent = (mulaw_sample & 0x70) >> 4;

    int16_t pcm_sample = mulaw_sample & 0x0F;
    pcm_sample |= 0x10;

    pcm_sample = pcm_sample << 1;
    pcm_sample = pcm_sample + 1;
    pcm_sample = pcm_sample << (exponent + 2);
    pcm_sample = pcm_sample - 0x84;

    return sign_bit ? pcm_sample: pcm_sample * -1;
}

void decode_mulaw_to_pcm(uint8_t mulaw_input[], int mulaw_length, int16_t pcm_output[]) {
    int samples = mulaw_length / sizeof(uint8_t);
    for (int i = 0; i < samples; i++) {
        pcm_output[i] = decode_mulaw_to_pcm_sample(mulaw_input[i]);
    }
}


void MulawDecoder::processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame) {
   int inputFrameSize = inputFrame.size;
   int16_t * pcm_output = new int16_t[inputFrameSize * 2]();

   uint8_t * inputFrameData = (uint8_t * )inputFrame.data;

   decode_mulaw_to_pcm(inputFrameData, inputFrameSize, pcm_output);

   outputFrame.data = (int8_t * ) pcm_output;
   outputFrame.size = inputFrameSize * 2;
}

MulawDecoder::~MulawDecoder() {}

