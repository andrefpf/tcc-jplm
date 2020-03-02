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

/** \file     PPMToPGX.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-14
 */

#include <filesystem>
#include <iostream>
#include "Lib/Utils/BasicConfiguration/BasicConfiguration.h"
#include "Lib/Utils/Image/ImageIO.h"

class PPMToPGXConfiguration : public BasicConfiguration {
 private:
  static constexpr std::size_t current_hierarchy_level = 0;

 protected:
  std::string input;
  std::string output;
  PPMToPGXConfiguration(int argc, char **argv, std::size_t level)
      : BasicConfiguration(argc, argv, level) {
  }


  virtual void add_options() override {
    BasicConfiguration::add_options();


    this->add_cli_json_option(
        {"--input", "-i", "Input PPM (P6) file to be converted to PGX",
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
        "Directory where the output PGX files will be saved.",
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
  PPMToPGXConfiguration(int argc, char **argv)
      : PPMToPGXConfiguration(
            argc, argv, PPMToPGXConfiguration::current_hierarchy_level) {
    this->init(argc, argv);
  }


  virtual ~PPMToPGXConfiguration() = default;


  const std::string &get_input_filename() const {
    return input;
  }


  const std::string &get_output_filename() const {
    return output;
  }
};


int main(int argc, char const *argv[]) {
  auto configuration = PPMToPGXConfiguration(argc, const_cast<char **>(argv));


  auto image_file = ImageIO::open(configuration.get_input_filename());

  /**
   * \todo include option that allows for changing the colour space of the output image
   */
  auto image_from_file = ImageIO::read<BT601Image, uint16_t>(*image_file);


  auto split_images = ImageUtils::get_splitting_of(*image_from_file);

  auto output = std::filesystem::path(configuration.get_output_filename());
  auto input = std::filesystem::path(configuration.get_input_filename());

  auto number_of_channels = image_from_file->get_number_of_channels();

  for (auto i = decltype(number_of_channels){0}; i < number_of_channels; ++i) {
    auto path_for_output = output;
    path_for_output /=
        std::to_string(i) / input.filename().replace_extension(".pgx");


    if (!std::filesystem::exists(path_for_output.parent_path())) {
      std::filesystem::create_directory(path_for_output.parent_path());
    }
    ImageIO::imwrite(*split_images[i], path_for_output.string());
  }

  return 0;
}