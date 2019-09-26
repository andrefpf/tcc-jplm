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

/** \file     ParsedConfiguration.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-09-26
 */
#include <iostream>
#include "Lib/Common/ParsedConfiguration.h"
#include "Lib/Common/ParseExceptions.h"

using namespace std;
using json = nlohmann::json;


ParsedConfiguration::ParsedConfiguration(const int argc, const char** argv) {
  parse_cli(argc, argv);
  if (!config.empty())
    parse_json(config);
}

void ParsedConfiguration::parse_cli(const int argc, const char** argv) {
  CLI::App app{"JPLM"};
  app.add_option("-i,--input", input,
      "Input (If Part II, it is a directory containing a set of uncompressed "
      "light-field images xxx_yyy.ppm).");
  app.add_option("-o,--output", output, "Output compressed bitstream.");
  app.add_option("-c,--config", config, "Path to config file");
  app.add_set(
         "-p,--part", part, {JpegPlenoPart::LightField}, "Part of JPEG Pleno")
      ->type_name("enum/JpegPlenoPart in { LightField=2 }");
  app.add_set("-t,--type", type, {Type::transform_mode, Type::prediction_mode},
         "Codec type")
      ->type_name(
          "enum/CompressionTypeLightField in {transform_mode=0, "
          "prediction_mode=1}");
  app.parse(argc, argv);
}

void ParsedConfiguration::parse_json(string config_file_path) {
  ifstream ifs(config_file_path);
  json conf = json::parse(ifs);
  parse_part(conf);
}

void ParsedConfiguration::parse_part(const json &conf) {
  JpegPlenoPart p = conf["part"].get<JpegPlenoPart>();
  if (p == JpegPlenoPart::LightField)
    part = JpegPlenoPart::LightField;
  else
    throw NotImplementedYetInputTypeParseException(p);
}
