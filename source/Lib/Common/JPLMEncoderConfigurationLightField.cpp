/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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

/** \file     JPLMEncoderConfigurationLightField.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */

#include "Lib/Common/JPLMEncoderConfigurationLightField.h"

using json = nlohmann::json;
using namespace std;

void JPLMEncoderConfigurationLightField::add_options_to_cli() {
  this->add_cli_json_option(
      {"--view_height", "-v", "Single-view height dimension. Mandatory.",
          [this](const json &conf) -> std::optional<std::string> {
            if (conf.contains("view_height")) {
              return std::to_string(conf["view_height"].get<uint32_t>());
            }
            if (conf.contains("image_height")) {
              return std::to_string(conf["image_height"].get<uint32_t>());
            }
            return std::nullopt;
          },
          [this](std::string arg) { this->view_height_v = std::stoi(arg); },
          this->current_hierarchy_level});

  this->add_cli_json_option(
      {"--view_width", "-u", "Single-view width dimension. Mandatory.",
          [this](const json &conf) -> std::optional<std::string> {
            if (conf.contains("view_width")) {
              return std::to_string(conf["view_width"].get<uint32_t>());
            }
            if (conf.contains("image_width")) {
              return std::to_string(conf["image_width"].get<uint32_t>());
            }
            return std::nullopt;
          },
          [this](std::string arg) { this->view_width_u = std::stoi(arg); },
          this->current_hierarchy_level});

  this->add_cli_json_option(
      {"--number_of_rows", "-t", "Number of light-field views rows. Mandatory.",
          [this](const json &conf) -> std::optional<std::string> {
            if (conf.contains("number_of_rows")) {
              return std::to_string(conf["number_of_rows"].get<uint32_t>());
            }
            return std::nullopt;
          },
          [this](std::string arg) { this->number_of_rows_t = std::stoi(arg); },
          this->current_hierarchy_level});

  this->add_cli_json_option({"--number_of_columns", "-s",
      "Number of light-field views columns",
      [this](const json &conf) -> std::optional<std::string> {
        if (conf.contains("number_of_columns")) {
          return std::to_string(conf["number_of_columns"].get<uint32_t>());
        }
        return std::nullopt;
      },
      [this](std::string arg) { this->number_of_columns_s = std::stoi(arg); },
      this->current_hierarchy_level});

  this->add_cli_json_option({"--type", "-T",
      "Codec type enum/CompressionTypeLightField in {transform_mode=0, "
      "prediction_mode=1}",
      [this](const json &conf) -> std::optional<std::string> {
        if (conf.contains("type")) {
          return conf["type"].get<std::string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        auto mode = std::string(arg.size(), ' ');
        std::transform(arg.begin(), arg.end(), mode.begin(),
            [](unsigned char c) { return std::tolower(c); });
        if (mode == "transform mode" || mode == "transform_mode" ||
            mode == "mule" || mode == "0") {
          this->type = CompressionTypeLightField::transform_mode;
        } else if (mode == "prediction mode" || mode == "prediction_mode" ||
                   mode == "wasp" || mode == "1") {
          this->type = CompressionTypeLightField::prediction_mode;
        } else {
          //! \todo check if this is the right exception to be thown here...
          throw JPLMConfigurationExceptions::
              NotImplementedYetInputTypeParseException(mode);
        }
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "transform_mode"; }}});
}


/**
 * @brief      Constructs a new instance. (protected)
 *
 * @param[in]  argc   The count of arguments
 * @param      argv   The arguments array
 * @param[in]  level  The level
 */
JPLMEncoderConfigurationLightField::JPLMEncoderConfigurationLightField(
    int argc, char **argv, std::size_t level)
    : JPLMEncoderConfiguration(argc, argv, level) {
  add_options_to_cli();

  this->parse_cli(argc, argv);

  this->message = "Options for Part 2, Light Field ( -p,--part 2 ): ";
}


JPLMEncoderConfigurationLightField::JPLMEncoderConfigurationLightField(
    int argc, char **argv)
    : JPLMEncoderConfigurationLightField(argc, argv,
          JPLMEncoderConfigurationLightField::current_hierarchy_level) {
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


CompressionTypeLightField JPLMEncoderConfigurationLightField::get_type() const {
  return type;
}


/**
 * \todo      check if it was created with wrong params, if so, uses a default
 */
void JPLMEncoderConfigurationLightField::check_inconsistencies() {
  if (part != JpegPlenoPart::LightField) {
    this->part = JpegPlenoPart::LightField;
    std::cout << "Inconsistent configuration. Jpeg Pleno Part was set to "
              << static_cast<int>(part)
              << ". Using Part 2 (Light Field) as default" << std::endl;
  }
  if ((type != CompressionTypeLightField::transform_mode) &&
      (type != CompressionTypeLightField::prediction_mode)) {
    std::cout << "Inconsistent configuration. Compression type was set to "
              << static_cast<int>(type) << ". Using transform mode as default"
              << std::endl;
    this->type = CompressionTypeLightField::transform_mode;
  }
}


CompressionTypeLightField
JPLMEncoderConfigurationLightField::get_compression_type() const {
  return type;
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