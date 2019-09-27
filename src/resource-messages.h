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
#define CWSCS0001I_MSG "CWSCS0001I: g729-codec-service is listening for requests on address {} and port {}."

#define CWSCS0002I_MSG "CWSCS0002I: g729-codec-service received a request with session-id {} to encode the audio from PCMU to G.729 with config = {}."

#define CWSCS0003I_MSG "CWSCS0003I: g729-codec-service received a request with session-id {} to decode the audio from G.729 to PCMU with config = {}."

#define CWSCS004I_MSG "CWSCS004I: g729-codec-service received a request with session-id {} to close down the encoding session."

#define CWSCS005I_MSG "CWSCS005I: g729-codec-service received a request with session-id {} to close down the decoding session."

#define CWSCS0050E_MSG "CWSCS0050E: g729-codec-service encountered the following error: {}, for a {} session with session-id {}."

#define CWSCW0051E_MSG "CWSCW0051E: g729-codec-service failed to listen on address {} and port {}."
