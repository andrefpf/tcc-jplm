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

/** \file     JPLMEncoderConfiguration.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */

#include "Lib/Common/JPLMEncoderConfiguration.h"

using namespace std;

using json = nlohmann::json;
using Type = CompressionTypeLightField;
namespace fs = std::filesystem;


JPLMEncoderConfiguration::JPLMEncoderConfiguration(int argc, char **argv)
    : JPLMEncoderConfiguration(
          argc, argv, JPLMEncoderConfiguration::current_hierarchy_level) {
  this->init(argc, argv);
}


JpegPlenoPart JPLMEncoderConfiguration::get_jpeg_pleno_part() const {
  return part;
}


const string &JPLMEncoderConfiguration::get_config() const {
  return config;
}


ColorSpaces::ColorSpace JPLMEncoderConfiguration::get_colorspace() const {
  return colorspace;
}

JPLMEncoderConfiguration::JPLMEncoderConfiguration(
    int argc, char **argv, std::size_t level)
    : JPLMConfiguration(argc, argv, level) {
  this->message =
      "JPLM Encoder\nUsage: " + this->executable_name + " [OPTIONS]\nOptions: ";
}


void JPLMEncoderConfiguration::add_options() {
  JPLMConfiguration::add_options();
  std::cout << "adding options of JPLMEncoderConfiguration" << std::endl;
  this->add_cli_json_option({"--input", "-i",
      "Input directory containing the plenoptic data to be compressed "
      "(according to the JPEG Pleno Part). "
      "\n\tFor Part 2, light field, the input is a directory containing a "
      "set of directories (one for each color channel). Each one of those "
      "directories contains a set of views in PGX format.",
      [this](const json &conf) -> std::optional<std::string> {
        if (conf.contains("input")) {
          return conf["input"].get<string>();
        }
        return std::nullopt;
      },
      [this]([[maybe_unused]] std::any v) {
        this->input = std::any_cast<std::string>(v);
      },
      this->current_hierarchy_level});


  this->add_cli_json_option({"--output", "-o",
      "Output, i.e., the compressed JPEG Pleno bitstream (filename.jpl).",
      [this](const json &conf) -> std::optional<std::string> {
        if (conf.contains("output")) {
          return conf["output"].get<string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->output = arg;
        //checking the file extension for showing a warning message
        if (this->current_hierarchy_level == hierarchy_level) {
          if (std::filesystem::path(this->output).extension() !=
              std::filesystem::path(".jpl")) {
            std::cerr << "Warning: the recommended extension is .jpl: "
                      << std::endl;
            std::cerr << "\tWhen stored in traditional computer file systems, "
                         "JPL files should be given the file extension \".jpl\""
                      << std::endl;
            std::cerr << "\tThe used extension was: "
                      << std::filesystem::path(this->output).extension()
                      << std::endl;
          }
        }
      },
      this->current_hierarchy_level});


  this->add_cli_json_option({"--part", "-p",
      "The JPEG Pleno part. Mandatory. enum/JpegPlenoPart in { LightField=2 }",
      [this](const json &conf) -> std::optional<std::string> {
        if (conf.contains("part")) {
          return conf["part"].get<string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        auto part = std::string(arg.size(), ' ');
        std::transform(arg.begin(), arg.end(), part.begin(),
            [](unsigned char c) { return std::tolower(c); });
        if (part == "2" || part == "part 2" || part == "part2" ||
            part == "part_2" || part == "light_fields" ||
            part == "lightfields" || part == "light fields" ||
            part == "light_field" || part == "lightfield" ||
            part == "light field") {
          this->part = JpegPlenoPart::LightField;
        }
      },
      this->current_hierarchy_level});

  //<! \todo the colorspace is a part of the jpl encoder? Or it depends on the part?
  //<! \todo include options to input the EnumCS for the ColourDefinitionBox
  this->add_cli_json_option({"--colourspace", "-cs",
      "Colour space to be signalized in EnumCS. It shall be the colour space "
      "of the inputs.",
      [this](const json &conf) -> std::optional<std::string> {
        if (conf.contains("colourspace")) {
          return conf["colourspace"].get<string>();
        }
        if (conf.contains("colorspace")) {
          return conf["colorspace"].get<string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        auto colourspace = std::string(arg.size(), ' ');
        std::transform(arg.begin(), arg.end(), colourspace.begin(),
            [](unsigned char c) { return std::tolower(c); });
        if (colourspace == "ycbcr" || colourspace == "bt601")
          this->colorspace = ColorSpaces::ColorSpace::BT601;
        else if (colourspace == "rgb")
          this->colorspace = ColorSpaces::ColorSpace::RGB;
        else if (colourspace == "bt709")
          this->colorspace = ColorSpaces::ColorSpace::BT709;
        else if (colourspace == "bt2020")
          this->colorspace = ColorSpaces::ColorSpace::BT2020;
        else if (colourspace == "ycocg")
          this->colorspace = ColorSpaces::ColorSpace::YCoCg;
        else
          throw JPLMConfigurationExceptions::
              NotImplementedYetInputTypeParseException(arg);
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "bt601"; }}});
}
