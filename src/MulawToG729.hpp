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
#ifndef MulawToG729_hpp
#define MulawToG729_hpp


#include <stdio.h>
#include "G729Encoder.hpp"
#include "MulawDecoder.hpp"
#include "VADCodec.hpp"

class MulawtoG729: public virtual VADCodec {
private:
    G729Encoder * g729Encoder;
    MulawDecoder *  mulawDecoder;
public:
    void processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame);
    void processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame, VADEvent& vadEvent);
    MulawtoG729(bool annexbEnabled);
    ~MulawtoG729();
};
#endif /* MulawToG729_hpp */
