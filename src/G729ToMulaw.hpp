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
#ifndef G729ToMulaw_hpp
#define G729ToMulaw_hpp

#include <stdio.h>

#include <stdio.h>
#include "G729Decoder.hpp"
#include "MulawEncoder.hpp"
#include "VADCodec.hpp"

class G729ToMulaw: public virtual VADCodec {
private:
    G729Decoder * g729Decoder;
    MulawEncoder *  mulawEncoder;
public:
    void processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame);
    void processFrame(AudioFrame& inputFrame, AudioFrame& outputFrame, VADEvent& vadEvent);

    G729ToMulaw();
    ~G729ToMulaw();
};
#endif /* G729ToMulaw_hpp */
