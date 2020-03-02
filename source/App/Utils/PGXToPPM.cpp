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

/** \file     PGXToPPM.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-16
 */

#include <filesystem>
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include "Lib/Utils/BasicConfiguration/BasicConfiguration.h"
#include "Lib/Utils/Image/ImageIO.h"


class PGXToPPMConfiguration : public BasicConfiguration {
 private:
  static constexpr std::size_t current_hierarchy_level = 0;

 protected:
  std::string input;
  std::string output;
  PGXToPPMConfiguration(int argc, char **argv, std::size_t level)
      : BasicConfiguration(argc, argv, level) {
  }


  virtual void add_options() override {
    BasicConfiguration::add_options();


    this->add_cli_json_option({"--input", "-i",
        "Input directory that contains at least three subdirectories with PGX "
        "files.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
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
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("output")) {
            return conf["output"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          this->output = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});
  }

 public:
  PGXToPPMConfiguration(int argc, char **argv)
      : PGXToPPMConfiguration(
            argc, argv, PGXToPPMConfiguration::current_hierarchy_level) {
    this->init(argc, argv);
  }


  virtual ~PGXToPPMConfiguration() = default;


  const std::string &get_input_filename() const {
    return input;
  }


  const std::string &get_output_filename() const {
    return output;
  }
};


int main(int argc, char const *argv[]) {
  auto configuration = PGXToPPMConfiguration(argc, const_cast<char **>(argv));
  if (configuration.is_help_mode()) {
    exit(0);
  }

  auto input = std::filesystem::path(configuration.get_input_filename());

  auto output = std::filesystem::path(configuration.get_output_filename());

  std::vector<std::unique_ptr<UndefinedImage<uint16_t>>> images;

  for (auto i = 0; i < 3; ++i) {
    auto file_name =
        input / std::to_string(i) / output.filename().replace_extension(".pgx");
    auto image_file = ImageIO::open(file_name.string());
    images.push_back(ImageIO::read<UndefinedImage, uint16_t>(*image_file));
  }

  auto image_sum = ImageUtils::get_undefined_images_as<BT601Image>(
      *(images[0]), *(images[1]), *(images[2]));
  ImageIO::imwrite(*image_sum, output.string());

  return 0;
}