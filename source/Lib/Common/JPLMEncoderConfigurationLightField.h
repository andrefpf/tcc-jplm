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

/** \file     JPLMEncoderConfigurationLightField.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-11
 */

#ifndef JPLMENCODERCONFIGURATIONLIGHTFIELD_H__
#define JPLMENCODERCONFIGURATIONLIGHTFIELD_H__

#include <cstdint>
#include <filesystem>
#include <optional>
#include <tuple>
#include "CLI/CLI.hpp"
#include "Lib/Common/JPLMConfiguration.h"
#include "Lib/Common/JPLMConfigurationExceptions.h"
#include "Lib/Common/JPLMEncoderConfiguration.h"
#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Part2/Common/Lightfield.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "nlohmann/json.hpp"

class JPLMEncoderConfigurationLightField : public JPLMEncoderConfiguration {
 public:
  JPLMEncoderConfigurationLightField(int argc, char **argv);
  LightfieldIOConfiguration get_lightfield_io_configurations() const;
  uint32_t get_number_of_rows_t() const;
  uint32_t get_number_of_columns_s() const;
  uint32_t get_view_height_v() const;
  uint32_t get_view_width_u() const;
  virtual Type get_type() const;
  virtual Type get_compression_type() const;


 protected:
  uint32_t number_of_rows_t;
  uint32_t number_of_columns_s;
  uint32_t view_height_v;
  uint32_t view_width_u;

  JPLMEncoderConfigurationLightField(int argc, char **argv, std::size_t level)
      : JPLMEncoderConfiguration(argc, argv, level) {
    arguments.push_back({"--view_height", "-v", "Single-view height dimension",
        [this](std::any value) {
          this->view_height_v = std::stoi(std::any_cast<string>(value));
        },
        this->current_hierarchy_level});

    arguments.push_back({"--view_width", "-u", "Single-view width dimension",
        [this](std::any value) {
          this->view_width_u = std::stoi(std::any_cast<string>(value));
        },
        this->current_hierarchy_level});

    arguments.push_back(
        {"--number_of_rows", "-t", "Number of light-field views rows",
            [this](std::any value) {
              this->number_of_rows_t = std::stoi(std::any_cast<string>(value));
            },
            this->current_hierarchy_level});

    arguments.push_back({"--number_of_columns", "-s",
        "Number of light-field views columns",
        [this](std::any value) {
          this->number_of_columns_s = std::stoi(std::any_cast<string>(value));
        },
        this->current_hierarchy_level});

    arguments.push_back({"--type", "-T",
        "Codec type enum/CompressionTypeLightField in {transform_mode=0, "
        "prediction_mode=1}",
        [this](std::any v) {
          std::string typed_string = std::any_cast<std::string>(v);
          int type = std::stoi(typed_string);
          this->type = static_cast<Type>(type);
        },
        this->current_hierarchy_level});

    this->parse_cli(argc, argv);

    this->message = "Options for Part 2, Light Field ( -p,--part 2 ): ";
  }

  void parse_json(string path);
  void parse_number_of_rows_t(const json &conf);
  void parse_number_of_columns_s(const json &conf);
  void parse_view_height_v(const json &conf);
  void parse_view_width_u(const json &conf);


 private:
  void parse_mode_type(const json &conf);
  void check_inconsistencies();
  CompressionTypeLightField type;
  static constexpr std::size_t current_hierarchy_level = 1;
};


JPLMEncoderConfigurationLightField::JPLMEncoderConfigurationLightField(
    int argc, char **argv)
    : JPLMEncoderConfigurationLightField(argc, argv, 1) {
  run_help();
}


LightfieldIOConfiguration
JPLMEncoderConfigurationLightField::get_lightfield_io_configurations() const {
  // \todo check this constants
  auto dimension = LightfieldDimension<std::size_t>(
      number_of_rows_t, number_of_columns_s, view_height_v, view_width_u);
  auto config = LightfieldIOConfiguration(input, dimension);
  return config;
}


Type JPLMEncoderConfigurationLightField::get_type() const {
  return type;
}


void JPLMEncoderConfigurationLightField::parse_mode_type(const json &conf) {
  if (conf.contains("type")) {
    string t = conf["type"].get<string>();
    std::transform(t.begin(), t.end(), t.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (t == "transform mode" || t == "transform_mode" || t == "mule") {
      type = Type::transform_mode;
    } else if (t == "prediction mode" || t == "prediction_mode" ||
               t == "wasp") {
      type = Type::prediction_mode;
    } else {
      //! \todo check if this is the right exception to be thown here...
      throw NotImplementedYetInputTypeParseException(t);
    }
  }
}


void JPLMEncoderConfigurationLightField::parse_json(string path) {
  JPLMEncoderConfiguration::parse_json(path);
  ifstream ifs(path);
  json conf = json::parse(ifs);
  parse_mode_type(conf);
  parse_number_of_columns_s(conf);
  parse_number_of_rows_t(conf);
  parse_view_height_v(conf);
  parse_view_width_u(conf);
}

/**
 * \brief      check if it was created with wrong params, if so, uses a default
 */
void JPLMEncoderConfigurationLightField::check_inconsistencies() {
  if (part != JpegPlenoPart::LightField) {
    this->part = JpegPlenoPart::LightField;
    std::cout << "Inconsistent configuration. Jpeg Pleno Part was set to "
              << static_cast<int>(part)
              << ". Using Part 2 (Light Field) as default" << std::endl;
  }
  if ((type != Type::transform_mode) && (type != Type::prediction_mode)) {
    std::cout << "Inconsistent configuration. Compression type was set to "
              << static_cast<int>(type) << ". Using transform mode as default"
              << std::endl;
    this->type = Type::transform_mode;
  }
}


Type JPLMEncoderConfigurationLightField::get_compression_type() const {
  return type;
}

void JPLMEncoderConfigurationLightField::parse_number_of_rows_t(
    const json &conf) {
  if (conf.contains("number_of_rows"))
    number_of_rows_t = conf["number_of_rows"].get<uint32_t>();
}

void JPLMEncoderConfigurationLightField::parse_number_of_columns_s(
    const json &conf) {
  if (conf.contains("number_of_columns"))
    number_of_columns_s = conf["number_of_columns"].get<uint32_t>();
}

void JPLMEncoderConfigurationLightField::parse_view_height_v(const json &conf) {
  if (conf.contains("view_height") || conf.contains("image_height"))
    view_height_v = (conf.contains("view_height"))
                        ? conf["view_height"].get<uint32_t>()
                        : conf["image_height"].get<uint32_t>();
}

void JPLMEncoderConfigurationLightField::parse_view_width_u(const json &conf) {
  if (conf.contains("view_width") || conf.contains("image_width"))
    view_width_u = (conf.contains("view_width"))
                       ? conf["view_width"].get<uint32_t>()
                       : conf["image_width"].get<uint32_t>();
}

uint32_t JPLMEncoderConfigurationLightField::get_number_of_rows_t() const {
  return number_of_rows_t;
}

uint32_t JPLMEncoderConfigurationLightField::get_number_of_columns_s() const {
  return number_of_columns_s;
}

uint32_t JPLMEncoderConfigurationLightField::get_view_height_v() const {
  return view_height_v;
}

uint32_t JPLMEncoderConfigurationLightField::get_view_width_u() const {
  return view_width_u;
}

#endif /* end of include guard: JPLMENCODERCONFIGURATIONLIGHTFIELD_H__ */