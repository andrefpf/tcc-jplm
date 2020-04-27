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

/** \file     Lenslet13x13Shifter.cpp
 *  \brief    Test of PPM Binary File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-31
 */

#include <algorithm>
#include <filesystem>
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "Lib/Utils/BasicConfiguration/BasicConfiguration.h"
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageMetrics.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"


std::map<std::string, ImageType> string_to_image_type_map;

void fill_map() {
  string_to_image_type_map["rgb"] = ImageType::RGB;
  string_to_image_type_map["bt601"] = ImageType::BT601;
  string_to_image_type_map["bt709"] = ImageType::BT709;
  string_to_image_type_map["bt2020"] = ImageType::BT2020;
}

class PSNRComputerConfiguration : public BasicConfiguration {
 private:
  static constexpr std::size_t current_hierarchy_level = 0;

 protected:
  std::string input;
  std::string output;
  ImageType type = ImageType::RGB;

  PSNRComputerConfiguration(int argc, char** argv, std::size_t level)
      : BasicConfiguration(argc, argv, level) {
  }


  virtual void add_options() override {
    BasicConfiguration::add_options();


    this->add_cli_json_option({"--input", "-i",
        "Input directory that contains at least three subdirectories with PGX "
        "files.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          if (conf.contains("input")) {
            return conf["input"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          this->input = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--output", "-o",
        "Output PPM (P6) filename. The name of the output file will be used to "
        "detect the input name in each channel directory. ",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          if (conf.contains("output")) {
            return conf["output"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          this->output = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--color_space", "-cs",
        "Colorspace used in metric computation.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          if (conf.contains("color_space")) {
            return conf["color_space"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          std::string colorspace = std::any_cast<std::string>(v);
          std::transform(colorspace.begin(), colorspace.end(),
              colorspace.begin(), ::tolower);

          auto it = string_to_image_type_map.find(colorspace);
          if (it == string_to_image_type_map.end()) {
            std::cerr << "Unable to find color space " << colorspace
                      << ". The available ones are: \n";
            for (const auto& map_iterator : string_to_image_type_map) {
              std::cerr << '\t' << std::get<0>(map_iterator) << '\n';
            }
            std::cerr << "Using RGB as default:\n";
          } else {
            this->type = it->second;
          }
        },
        this->current_hierarchy_level, {[this]() -> std::string {
          auto colorspace_name = magic_enum::enum_name(this->type);
          return std::string(colorspace_name);
        }}});
  }

 public:
  PSNRComputerConfiguration(int argc, char** argv)
      : PSNRComputerConfiguration(
            argc, argv, PSNRComputerConfiguration::current_hierarchy_level) {
    this->init(argc, argv);
  }


  virtual ~PSNRComputerConfiguration() = default;


  const std::string& get_input_filename() const {
    return input;
  }


  const std::string& get_output_filename() const {
    return output;
  }


  //const std::string& get_colorspace() const {
  //  return colorspace;
  //}


  ImageType get_type() const {
    return type;
  }
};


void show_psnr(const std::string& filename_original,
    const std::string& filename_decoded,
    const ImageType type = ImageType::RGB) {
  auto original_file = PixelMapFileIO::open(filename_original);
  auto decoded_file = PixelMapFileIO::open(filename_decoded);

  auto original_image_variant = original_file->read_full_image();
  auto original_image =
      PixelMapFileIO::extract_image_with_type_from_variant<RGBImage, uint16_t>(
          original_image_variant);

  auto decoded_image_variant = decoded_file->read_full_image();
  auto decoded_image =
      PixelMapFileIO::extract_image_with_type_from_variant<RGBImage, uint16_t>(
          decoded_image_variant);

  auto printer = ImageMetrics::visitors::PSNRPrinter();
  auto max_error_printer = ImageMetrics::visitors::MaximumErrorPrinter();

  switch (type) {
    case ImageType::RGB:
      printer(original_image, decoded_image);
      max_error_printer(original_image, decoded_image);
      break;
    case ImageType::BT601:
      printer.operator()<BT601Image>(original_image, decoded_image);
      max_error_printer.operator()<BT601Image>(original_image, decoded_image);
      break;
    case ImageType::BT709:
      printer.operator()<BT709Image>(original_image, decoded_image);
      max_error_printer.operator()<BT709Image>(original_image, decoded_image);
      break;
    case ImageType::BT2020:
      printer.operator()<BT2020Image>(original_image, decoded_image);
      max_error_printer.operator()<BT2020Image>(original_image, decoded_image);
      break;
    default:
      std::cerr << "Error default" << std::endl;
  }
}


int main(int argc, char const* argv[]) {
  fill_map();

  auto configuration =
      PSNRComputerConfiguration(argc, const_cast<char**>(argv));
  if (configuration.is_help_mode()) {
    exit(0);
  }

  std::string filename_original = configuration.get_input_filename();
  std::string filename_decoded = configuration.get_output_filename();
  //std::string color_space = configuration.get_colorspace();
  ImageType type = configuration.get_type();

  show_psnr(filename_original, filename_decoded, type);

  return 0;
}
