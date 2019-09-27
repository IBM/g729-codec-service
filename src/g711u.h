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
#ifndef g711_encode_h
#define g711_encode_h

#include <stdio.h>
#include <stdint.h>
uint8_t encode_pcm_to_mulaw_sample(int16_t pcm_sample);
void encode_pcm_to_mulaw(int16_t pcm_input[], int pcm_length, uint8_t mulaw_output[]);

int16_t decode_mulaw_to_pcm_sample(uint8_t mulaw_sample);

void decode_mulaw_to_pcm(uint8_t mulaw_input[], int mulaw_length, int16_t pcm_output[]);
#endif /* g711_encode_h */
