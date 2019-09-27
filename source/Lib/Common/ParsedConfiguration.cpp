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
#include "Lib/Common/ParsedConfiguration.h"
#include <functional>
#include <iostream>
#include "Lib/Common/ParseExceptions.h"

using namespace std;
using json = nlohmann::json;


ParsedConfiguration::ParsedConfiguration(const int argc, const char** argv) {
  parse_cli(argc, argv);
  if (!config->empty())
    parse_json(*config);
}

void ParsedConfiguration::parse_cli(const int argc, const char** argv) {
  CLI::App app{"JPLM"};
  app.add_option("-i,--input", input,
      "Input (If Part II, it is a directory containing a set of uncompressed "
      "light-field images xxx_yyy.ppm).");
  app.add_option("-o,--output", output, "Output compressed bitstream");
  app.add_option("-c,--config", config, "Path to config file");
  app.add_set(
         "-p,--part", part, {JpegPlenoPart::LightField}, "Part of JPEG Pleno")
      ->type_name("enum/JpegPlenoPart in { LightField=2 }");
  app.add_set("-T,--type", type, {Type::transform_mode, Type::prediction_mode},
         "Codec type")
      ->type_name(
          "enum/CompressionTypeLightField in {transform_mode=0, "
          "prediction_mode=1}");
  app.add_option(
      "-t,--number_of_rows", number_of_rows_t, "Number of light-field rows");
  app.add_option("-s,--number_of_columns", number_of_columns_s,
      "Number of light-field columns");
  app.add_option(
      "-v,--view_height", view_height_v, "Single-view height dimension");
  app.add_option(
      "-u,--view_width", view_width_u, "Single-view width dimension");
  app.add_option("-l,--lambda", lambda,
      "Lagrangian multiplier used in the RDO process of 4D Transform mode.");
  app.parse(argc, argv);
}

void ParsedConfiguration::parse_json(string config_file_path) {
  ifstream ifs(config_file_path);
  json conf = json::parse(ifs);
  parse_standard_part(conf);
  parse_mode_type(conf);
  parse_number_of_columns_s(conf);
  parse_number_of_rows_t(conf);
  parse_view_height_v(conf);
  parse_view_width_u(conf);
  parse_lambda(conf);
}

void ParsedConfiguration::parse_standard_part(const json& conf) {
  if (conf.contains("part")) {
    JpegPlenoPart p = conf["part"].get<JpegPlenoPart>();
    if (p == JpegPlenoPart::LightField)
      part = JpegPlenoPart::LightField;
    else
      throw NotImplementedYetInputTypeParseException<JpegPlenoPart>(p);
  }
}

void ParsedConfiguration::parse_mode_type(const json& conf) {
  if (conf.contains("type")) {
    Type t = conf["type"].get<Type>();
    if (t == Type::transform_mode)
      type = Type::transform_mode;
    else if (t == Type::prediction_mode)
      type = Type::prediction_mode;
    else
      throw NotImplementedYetInputTypeParseException<Type>(t);
  }
}

void ParsedConfiguration::parse_number_of_rows_t(const json& conf) {
  if (conf.contains("number_of_rows"))
    number_of_rows_t = conf["number_of_rows"].get<uint32_t>();
}

void ParsedConfiguration::parse_number_of_columns_s(const json& conf) {
  if (conf.contains("number_of_columns"))
    number_of_columns_s = conf["number_of_columns"].get<uint32_t>();
}

void ParsedConfiguration::parse_view_height_v(const json& conf) {
  if (conf.contains("view_height"))
    view_height_v = conf["view_height"].get<uint32_t>();
}

void ParsedConfiguration::parse_view_width_u(const json& conf) {
  if (conf.contains("view_width"))
    view_width_u = conf["view_width"].get<uint32_t>();
}

void ParsedConfiguration::parse_lambda(const nlohmann::basic_json<> &conf) {
  if (conf.contains("lambda"))
    lambda = conf["lambda"].get<double>();
}

void ParsedConfiguration::check_inconsistencies(void) {
  if (lambda && (part != JpegPlenoPart::LightField))
    throw InconsistentOptionsException();
}
