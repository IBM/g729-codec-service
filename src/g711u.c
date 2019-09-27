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
#include "g711u.h"

#define SEG_LENGTH 8
static int16_t seg_end[SEG_LENGTH] = { 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF, 0x3FFF, 0x7FFF };
#define NEGATIVE_MASK 0x7F;
#define POSITIVE_MASK 0xFF;


uint8_t encode_pcm_to_mulaw_sample(int16_t pcm_sample) {
    uint8_t mask = 0;
    uint8_t mulaw = 0;

    if (pcm_sample > 0) {
        pcm_sample = pcm_sample + 0x84;
        mask = POSITIVE_MASK;
    } else {
        pcm_sample = 0x84 - pcm_sample;
        mask = NEGATIVE_MASK;
    }

    int16_t segment = 0;

    for (int i = 0; i < SEG_LENGTH; i++) {
        if (pcm_sample < seg_end[i]) {
            segment = i;
            break;
        }
        segment = SEG_LENGTH;
    }
    if (segment >= SEG_LENGTH) {
        mulaw = 0x7F;
    } else {
        mulaw = (segment) | ((pcm_sample >> (segment + 3)) & 0xF);
    }
    return mulaw ^ mask;
}
void encode_pcm_to_mulaw(int16_t pcm_input[], int pcm_length, uint8_t mulaw_output[]) {
    int samples = pcm_length / sizeof(int16_t);

    for (int i = 0; i < samples; i++) {
        mulaw_output[i] = encode_pcm_to_mulaw_sample(pcm_input[i]);
    }
}


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
    printf("samples size = %i", samples);
    for (int i = 0; i < samples; i++) {
        pcm_output[i] = decode_mulaw_to_pcm_sample(mulaw_input[i]);
    }
}
