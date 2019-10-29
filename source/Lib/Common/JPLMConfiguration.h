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

/** \file     JPLMConfiguration.h
 *  \brief    General configuration data object
 *  \details  JPLMConfiguration is the most basic abstraction of configuration
 *            data object for being used by the JPLM Encoder and Decoders. It
 *            contains only the input and output paths extracted from the
 *            command line interface. All configuration objects are derived
 *            from JPLMConfiguration.
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-09-26
 */
#ifndef JPLM_JPLMConfiguration_H
#define JPLM_JPLMConfiguration_H

#include <iostream>
#include <string>
#include "CLI/CLI.hpp"
#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Part2/Common/TransformMode/BorderBlocksPolicy.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using Type = CompressionTypeLightField;

enum class JpegPlenoPart {
  LightField = 2,
};


class JPLMConfiguration {
 public:
  JPLMConfiguration();

  JPLMConfiguration(int argc, char **argv);

  const std::string &get_input_filename() const;

  const std::string &get_output_filename() const;

 protected:
  CLI::App app{"JPLM"};
  std::string input;
  std::string output;
  JpegPlenoPart part;
  std::string config;
  uint32_t number_of_rows_t;
  uint32_t number_of_columns_s;
  uint32_t view_height_v;
  uint32_t view_width_u;
  ColorSpaces::ColorSpace colorspace;
  double lambda;
  Type type;
  BorderBlocksPolicy border_policy = BorderBlocksPolicy::truncate;


  uint32_t maximal_transform_size_inter_view_vertical_t = 100000;
  uint32_t maximal_transform_size_inter_view_horizontal_s = 100000;
  uint32_t maximal_transform_size_intra_view_vertical_v= 100000;
  uint32_t maximal_transform_size_intra_view_horizontal_u= 100000;

  uint32_t minimal_transform_size_inter_view_vertical_t= 100000;
  uint32_t minimal_transform_size_inter_view_horizontal_s= 100000;
  uint32_t minimal_transform_size_intra_view_vertical_v= 100000;
  uint32_t minimal_transform_size_intra_view_horizontal_u= 100000;

  double transform_scale_t = 1.0;
  double transform_scale_s = 1.0;
  double transform_scale_v = 1.0;
  double transform_scale_u = 1.0;

 private:
  void parse_cli(int argc, char **argv);
};

const std::string &JPLMConfiguration::get_input_filename() const {
  return input;
}

const std::string &JPLMConfiguration::get_output_filename() const {
  return output;
}

JPLMConfiguration::JPLMConfiguration(int argc, char **argv) {
  parse_cli(argc, argv);
}

void JPLMConfiguration::parse_cli(int argc, char **argv) {
  // Belongs to JPLMConfiguration
  app.add_option("-i,--input", input,
      "Input (If Part II, it is a directory containing a set of uncompressed "
      "light-field images xxx_yyy.ppm).");
  app.add_option("-o,--output", output, "Output compressed bitstream");

  // Belongs to JPLMEncoderConfiguration
  app.add_option("-c,--config", config, "Path to config file");
  app.add_set(
         "-p,--part", part, {JpegPlenoPart::LightField}, "Part of JPEG Pleno")
      ->type_name("enum/JpegPlenoPart in { LightField=2 }");

  app.add_option(
      "-t,--number_of_rows", number_of_rows_t, "Number of light-field rows");
  app.add_option("-s,--number_of_columns", number_of_columns_s,
      "Number of light-field columns");
  app.add_option(
      "-v,--view_height", view_height_v, "Single-view height dimension");
  app.add_option(
      "-u,--view_width", view_width_u, "Single-view width dimension");

  // Belongs to JPLMEncoderConfigurationLightField4DTransformMode
  app.add_option("-l,--lambda", lambda,
      "Lagrangian multiplier used in the RDO process of 4D Transform mode.");

  // Belongs to JPLMEncoderConfigurationLightField
  app.add_set("-T,--type", type, {Type::transform_mode, Type::prediction_mode},
         "Codec type")
      ->type_name(
          "enum/CompressionTypeLightField in {transform_mode=0, "
          "prediction_mode=1}");

  app.add_set("-B,--border_policy", border_policy, {BorderBlocksPolicy::padding, BorderBlocksPolicy::truncate},
         "Policy to treat border 4D blocks.")
      ->type_name(
          "enum/BorderBlocksPolicy in {padding=0, "
          "truncate=1}");


  app.add_option("--transform_size_maximum_inter_view_vertical",
      maximal_transform_size_inter_view_vertical_t,
      "Maximum 4D transform size in inter-view vertical direction.");
  app.add_option("--transform_size_maximum_inter_view_horizontal",
      maximal_transform_size_inter_view_horizontal_s,
      "Maximum 4D transform size in inter-view horizontal direction.");
  app.add_option("--transform_size_maximum_intra_view_vertical",
      maximal_transform_size_intra_view_vertical_v,
      "Maximum 4D transform size in intra-view vertical direction.");
  app.add_option("--transform_size_maximum_intra_view_horizontal",
      maximal_transform_size_intra_view_horizontal_u,
      "Maximum 4D transform size in intra-view horizontal direction.");


  app.add_option("--transform_size_minimum_inter_view_vertical",
      minimal_transform_size_inter_view_vertical_t,
      "Minimum 4D transform size in inter-view vertical direction.");
  app.add_option("--transform_size_minimum_inter_view_horizontal",
      minimal_transform_size_inter_view_horizontal_s,
      "Minimum 4D transform size in inter-view horizontal direction.");
  app.add_option("--transform_size_minimum_intra_view_vertical",
      minimal_transform_size_intra_view_vertical_v,
      "Minimum 4D transform size in intra-view vertical direction.");
  app.add_option("--transform_size_minimum_intra_view_horizontal",
      minimal_transform_size_intra_view_horizontal_u,
      "Minimum 4D transform size in intra-view horizontal direction.");


  try {
    app.parse(argc, argv);
  } catch (const CLI::CallForHelp &e) {
    app.exit(e);
    std::cout << __FILE__ << " " << __func__ << " " << __LINE__ << e.what()
              << std::endl;
  }
}

#endif  //JPLM_JPLMConfiguration_H
