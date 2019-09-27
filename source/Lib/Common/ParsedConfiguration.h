/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     ParsedConfiguration.h
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-09-26
 */
#ifndef JPLM_PARSEDCONFIGURATION_H
#define JPLM_PARSEDCONFIGURATION_H

#include <optional>
#include <cstdint>
#include "CLI/CLI.hpp"
#include "Lib/Part1/Common/JPLMConfiguration.h"
#include "Lib/Part2/Encoder/JPLMEncoderConfigurationLightField.h"
#include "nlohmann/json.hpp"

using namespace std;

using Type = CompressionTypeLightField;

class ParsedConfiguration {
protected:
  optional<string> input;
  optional<string> output;
  optional<JpegPlenoPart> part;
  optional<Type> type;
  optional<string> config;
  optional<uint32_t> number_of_rows_t;
  optional<uint32_t> number_of_columns_s;
  optional<uint32_t> view_height_v;
  optional<uint32_t> view_width_u;
  optional<double> lambda;

  void parse_cli(const int argc, const char** argv);
  void parse_json(string path);
  void parse_standard_part(const nlohmann::basic_json<> &conf);
  void parse_mode_type(const nlohmann::basic_json<> &conf);
  void parse_number_of_rows_t(const nlohmann::basic_json<> &conf);
  void parse_number_of_columns_s(const nlohmann::basic_json<> &conf);
  void parse_view_height_v(const nlohmann::basic_json<> &conf);
  void parse_view_width_u(const nlohmann::basic_json<> &conf);
  void parse_lambda(const nlohmann::basic_json<> &conf);
  void check_inconsistencies(void);
 public:
  ParsedConfiguration(const int argc, const char** argv);
};


#endif  //JPLM_PARSEDCONFIGURATION_H
