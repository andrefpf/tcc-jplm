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
#ifndef JPLM_CONFIGURATIONPARSER_H
#define JPLM_CONFIGURATIONPARSER_H

#include <cstdint>
#include <optional>
#include "CLI/CLI.hpp"
#include "Lib/Common/ParseExceptions.h"
#include "Lib/Part1/Common/JPLMConfiguration.h"
#include "Lib/Part2/Encoder/JPLMEncoderConfigurationLightField.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;
using Type = CompressionTypeLightField;

class ConfigurationParser {
 public:
  const string &getInput() const;

  const string &getOutput() const;

  const JpegPlenoPart &getPart() const;

  const Type &getType() const;

  const string &getConfig() const;

  const uint32_t &getNumberOfRowsT() const;

  const uint32_t &getNumberOfColumnsS() const;

  const uint32_t &getViewHeightV() const;

  const uint32_t &getViewWidthU() const;

  const double &getLambda() const;

  const ColorSpaces::ColorSpace &getColorspace() const;

  ConfigurationParser(int argc, char **argv);

 protected:
  string input;
  string output;
  JpegPlenoPart part;
  Type type;
  string config;
  uint32_t number_of_rows_t;
  uint32_t number_of_columns_s;
  uint32_t view_height_v;
  uint32_t view_width_u;
  double lambda;
  ColorSpaces::ColorSpace colorspace;

  void parse_cli(int argc, char **argv);
  void parse_json(string path);
  void parse_standard_part(const nlohmann::basic_json<> &conf);
  void parse_mode_type(const nlohmann::basic_json<> &conf);
  void parse_number_of_rows_t(const nlohmann::basic_json<> &conf);
  void parse_number_of_columns_s(const nlohmann::basic_json<> &conf);
  void parse_view_height_v(const nlohmann::basic_json<> &conf);
  void parse_view_width_u(const nlohmann::basic_json<> &conf);
  void parse_lambda(const nlohmann::basic_json<> &conf);
  void parse_colorspace(const nlohmann::basic_json<> &conf);
  void check_inconsistencies(void);
};

ConfigurationParser::ConfigurationParser(int argc, char **argv) {
  parse_cli(argc, argv);
  if (!config.empty())
    parse_json(config);
}

void ConfigurationParser::parse_cli(int argc, char **argv) {
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

void ConfigurationParser::parse_json(string config_file_path) {
  ifstream ifs(config_file_path);
  json conf = json::parse(ifs);
  parse_standard_part(conf);
  parse_mode_type(conf);
  parse_number_of_columns_s(conf);
  parse_number_of_rows_t(conf);
  parse_view_height_v(conf);
  parse_view_width_u(conf);
  parse_lambda(conf);
  parse_colorspace(conf);

  check_inconsistencies();
}

void ConfigurationParser::parse_standard_part(const json &conf) {
  if (conf.contains("part")) {
    string p = conf["part"].get<string>();
    std::transform(p.begin(), p.end(), p.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (p == "part 2" || p == "part2" || p == "part_2" || p == "light_fields")
      part = JpegPlenoPart::LightField;
    throw NotImplementedYetInputTypeParseException(p);
  }
}

void ConfigurationParser::parse_mode_type(const json &conf) {
  if (conf.contains("type")) {
    string t = conf["type"].get<string>();
    std::transform(t.begin(), t.end(), t.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (t == "transform mode" || t == "transform_mode" || t == "mule")
      type = Type::transform_mode;
    else if (t == "prediction mode" || t == "prediction_mode" || t == "wasp")
      type = Type::prediction_mode;
    else
      throw NotImplementedYetInputTypeParseException(t);
  }
}

void ConfigurationParser::parse_number_of_rows_t(const json &conf) {
  if (conf.contains("number_of_rows"))
    number_of_rows_t = conf["number_of_rows"].get<uint32_t>();
}

void ConfigurationParser::parse_number_of_columns_s(const json &conf) {
  if (conf.contains("number_of_columns"))
    number_of_columns_s = conf["number_of_columns"].get<uint32_t>();
}

void ConfigurationParser::parse_view_height_v(const json &conf) {
  if (conf.contains("view_height"))
    view_height_v = conf["view_height"].get<uint32_t>();
}

void ConfigurationParser::parse_view_width_u(const json &conf) {
  if (conf.contains("view_width"))
    view_width_u = conf["view_width"].get<uint32_t>();
}

void ConfigurationParser::parse_lambda(const nlohmann::basic_json<> &conf) {
  if (conf.contains("lambda"))
    lambda = conf["lambda"].get<double>();
}

void ConfigurationParser::parse_colorspace(const nlohmann::basic_json<> &conf) {
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


void ConfigurationParser::check_inconsistencies(void) {
  bool rule1 = lambda && (part != JpegPlenoPart::LightField);
  bool rule2 = lambda && (type != Type::transform_mode);
  bool rule3 =
      (part != JpegPlenoPart::LightField) && (type != Type::transform_mode);
  if (rule1 || rule2 || rule3)
    throw InconsistentOptionsException();
}

const string &ConfigurationParser::getInput() const {
  return input;
}

const string &ConfigurationParser::getOutput() const {
  return output;
}

const JpegPlenoPart &ConfigurationParser::getPart() const {
  return part;
}

const Type &ConfigurationParser::getType() const {
  return type;
}

const string &ConfigurationParser::getConfig() const {
  return config;
}

const uint32_t &ConfigurationParser::getNumberOfRowsT() const {
  return number_of_rows_t;
}

const uint32_t &ConfigurationParser::getNumberOfColumnsS() const {
  return number_of_columns_s;
}

const uint32_t &ConfigurationParser::getViewHeightV() const {
  return view_height_v;
}

const uint32_t &ConfigurationParser::getViewWidthU() const {
  return view_width_u;
}

const double &ConfigurationParser::getLambda() const {
  return lambda;
}

const ColorSpaces::ColorSpace &ConfigurationParser::getColorspace()
    const {
  return colorspace;
}

#endif  //JPLM_CONFIGURATIONPARSER_H
