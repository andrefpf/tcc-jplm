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

/** \file     JPLMEncoderConfiguration.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-11
 */

#ifndef JPLMENCODERCONFIGURATION_H__
#define JPLMENCODERCONFIGURATION_H__

#include <cstdint>
#include <optional>
#include <filesystem>
#include "CLI/CLI.hpp"
#include "Lib/Common/JPLMConfiguration.h"
#include "Lib/Common/JPLMConfigurationExceptions.h"
#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;
using Type = CompressionTypeLightField;
namespace fs = std::filesystem;

class JPLMEncoderConfiguration : public JPLMConfiguration {
 public:
  const JpegPlenoPart &get_jpeg_pleno_part() const;

  const string &getConfig() const;

  const uint32_t &getNumberOfRowsT() const;

  const uint32_t &getNumberOfColumnsS() const;

  const uint32_t &getViewHeightV() const;

  const uint32_t &getViewWidthU() const;


  const ColorSpaces::ColorSpace &getColorspace() const;

  JPLMEncoderConfiguration(int argc, char **argv);

 protected:
  void parse_json(string path);
  void parse_jpeg_pleno_part(const nlohmann::basic_json<> &conf);

  void parse_number_of_rows_t(const nlohmann::basic_json<> &conf);
  void parse_number_of_columns_s(const nlohmann::basic_json<> &conf);
  void parse_view_height_v(const nlohmann::basic_json<> &conf);
  void parse_view_width_u(const nlohmann::basic_json<> &conf);
  void parse_colorspace(const nlohmann::basic_json<> &conf);
};

JPLMEncoderConfiguration::JPLMEncoderConfiguration(int argc, char **argv)
    : JPLMConfiguration(argc, argv) {
  if (!config.empty())
    if (fs::exists(config))
      parse_json(config);
    else
      throw ConfigFileDoesNotExistException(config);
}

void JPLMEncoderConfiguration::parse_json(string config_file_path) {
  ifstream ifs(config_file_path);
  json conf = json::parse(ifs);
  parse_jpeg_pleno_part(conf);
  parse_number_of_columns_s(conf);
  parse_number_of_rows_t(conf);
  parse_view_height_v(conf);
  parse_view_width_u(conf);
  parse_colorspace(conf);
}

void JPLMEncoderConfiguration::parse_jpeg_pleno_part(const json &conf) {
  if (conf.contains("part")) {
    string p = conf["part"].get<string>();
    std::transform(p.begin(), p.end(), p.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (p == "part 2" || p == "part2" || p == "part_2" || p == "light_fields")
      part = JpegPlenoPart::LightField;
    throw NotImplementedYetInputTypeParseException(p);
  }
}



void JPLMEncoderConfiguration::parse_number_of_rows_t(const json &conf) {
  if (conf.contains("number_of_rows"))
    number_of_rows_t = conf["number_of_rows"].get<uint32_t>();
}

void JPLMEncoderConfiguration::parse_number_of_columns_s(const json &conf) {
  if (conf.contains("number_of_columns"))
    number_of_columns_s = conf["number_of_columns"].get<uint32_t>();
}

void JPLMEncoderConfiguration::parse_view_height_v(const json &conf) {
  if (conf.contains("view_height"))
    view_height_v = conf["view_height"].get<uint32_t>();
}

void JPLMEncoderConfiguration::parse_view_width_u(const json &conf) {
  if (conf.contains("view_width"))
    view_width_u = conf["view_width"].get<uint32_t>();
}

void JPLMEncoderConfiguration::parse_colorspace(
    const nlohmann::basic_json<> &conf) {
  if (conf.contains("colorspace")) {
    string c = conf["colorspace"].get<string>();
    std::transform(c.begin(), c.end(), c.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (c == "ycbcr" || c == "bt601")
      colorspace = ColorSpaces::ColorSpace::BT601;
    else if (c == "rgb")
      colorspace = ColorSpaces::ColorSpace::RGB;
    else if (c == "bt709")
      colorspace = ColorSpaces::ColorSpace::BT709;
    else if (c == "bt2020")
      colorspace = ColorSpaces::ColorSpace::BT2020;
    else if (c == "ycocg")
      colorspace = ColorSpaces::ColorSpace::YCoCg;
    else
      throw NotImplementedYetInputTypeParseException(c);
  }
}






const JpegPlenoPart &JPLMEncoderConfiguration::get_jpeg_pleno_part() const {
  return part;
}



const string &JPLMEncoderConfiguration::getConfig() const {
  return config;
}

const uint32_t &JPLMEncoderConfiguration::getNumberOfRowsT() const {
  return number_of_rows_t;
}

const uint32_t &JPLMEncoderConfiguration::getNumberOfColumnsS() const {
  return number_of_columns_s;
}

const uint32_t &JPLMEncoderConfiguration::getViewHeightV() const {
  return view_height_v;
}

const uint32_t &JPLMEncoderConfiguration::getViewWidthU() const {
  return view_width_u;
}

const ColorSpaces::ColorSpace &JPLMEncoderConfiguration::getColorspace() const {
  return colorspace;
}

#endif /* end of include guard: JPLMENCODERCONFIGURATION_H__ */