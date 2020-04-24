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

/** \file     JPLMEncoderConfigurationLightField4DTransformMode.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */

#include "Lib/Common/JPLMEncoderConfigurationLightField4DTransformMode.h"


void JPLMEncoderConfigurationLightField4DTransformMode::add_options() {
  /**
   * Runs the method to add options of the base class
   * \todo Add a tutorial explaining this point
   */
  JPLMEncoderConfigurationLightField::add_options();

  this->add_cli_json_option({"--transform_size_maximum_inter_view_vertical",
      "-TNIv", "Maximum 4D transform size in inter-view vertical direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("maximum")
                                    .at("inter-view")
                                    .at("vertical")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->maximal_transform_size_inter_view_vertical_t =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "13"; }}});

  this->add_cli_json_option({"--transform_size_maximum_inter_view_horizontal",
      "-TMIh", "Maximum 4D transform size in inter-view horizontal direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("maximum")
                                    .at("inter-view")
                                    .at("horizontal")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->maximal_transform_size_inter_view_horizontal_s =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "13"; }}});

  this->add_cli_json_option({"--transform_size_maximum_intra_view_vertical",
      "-TMiv", "Maximum 4D transform size in intra-view vertical direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("maximum")
                                    .at("intra-view")
                                    .at("vertical")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->maximal_transform_size_intra_view_vertical_v =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "64"; }}});

  this->add_cli_json_option({"--transform_size_maximum_intra_view_horizontal",
      "-TMih", "Maximum 4D transform size in intra-view horizontal direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("maximum")
                                    .at("intra-view")
                                    .at("horizontal")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->maximal_transform_size_intra_view_horizontal_u =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "64"; }}});


  this->add_cli_json_option({"--transform_size_minimum_inter_view_vertical",
      "-TmIv", "Minimum 4D transform size in inter-view vertical direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("minimum")
                                    .at("inter-view")
                                    .at("vertical")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->minimal_transform_size_inter_view_vertical_t =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "13"; }}});

  this->add_cli_json_option({"--transform_size_minimum_inter_view_horizontal",
      "-TmIh", "Minimum 4D transform size in inter-view horizontal direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("minimum")
                                    .at("inter-view")
                                    .at("horizontal")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->minimal_transform_size_inter_view_horizontal_s =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "13"; }}});

  this->add_cli_json_option({"--transform_size_minimum_intra_view_vertical",
      "-Tmiv", "Minimum 4D transform size in intra-view vertical direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("minimum")
                                    .at("intra-view")
                                    .at("vertical")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->minimal_transform_size_intra_view_vertical_v =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "4"; }}});


  this->add_cli_json_option({"--transform_size_minimum_intra_view_horizontal",
      "-Tmih", "Minimum 4D transform size in intra-view horizontal direction.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        try {
          return std::to_string(conf.at("transform_size")
                                    .at("minimum")
                                    .at("intra-view")
                                    .at("horizontal")
                                    .get<uint32_t>());
        } catch (nlohmann::json::out_of_range &e) {
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        this->minimal_transform_size_intra_view_horizontal_u =
            static_cast<uint32_t>(std::stoul(arg));
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "4"; }}});


  this->add_cli_json_option({"--border_policy", "-B",
      "Policy to treat border blocks, i.e., the blocks ranging outside "
      "the 4D limits of the light field. Available options are: " +
      this->get_valid_enumerated_options_str<BorderBlocksPolicy>(),
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        if (conf.contains("border_policy")) {
          return conf["border_policy"].get<std::string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        std::transform(arg.begin(), arg.end(), arg.begin(),
            [](unsigned char c) { return std::tolower(c); });
          this->border_policy = this->parse_enum_option_as<BorderBlocksPolicy>(arg);
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "truncate"; }}});


  this->add_cli_json_option({"--lambda", "-l",
      "Lagrangian multiplier used in the RDO process of 4D Transform "
      "mode (floating point value).",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        if (conf.contains("lambda")) {
          return std::to_string(conf["lambda"].get<double>());
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        std::string::size_type sz;
        this->lambda = std::stod(arg, &sz);
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "1000.0"; }}});

  this->add_cli_json_option({"--show-error-estimate", "-errorest",
      "Shows error estimates computed during RDO. Although close to the real "
      "error figures, they are only estimates (do not account for rounding "
      "errors in the inverse transform, for instance). For proper error "
      "reports, please decode the generated file and use the provided external "
      "tool.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        if (conf.contains("show-error-estimate")) {
          return conf["show-error-estimate"].get<std::string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        if (arg == "false") {
          this->show_estimated_error_flag = false;
        } else {
          this->show_estimated_error_flag = true;
        }
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "false"; }}});


  this->add_cli_json_option({"--insert-codestream-pointer-set", "-pnt",
      "Inserts a codestream pointer set marker segment before the first 4D "
      "block. This enables the encoder to known where each 4D block starts. ",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        if (conf.contains("insert-codestream-pointer-set")) {
          return conf["insert-codestream-pointer-set"].get<std::string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        if (arg == "false") {
          this->insert_codestream_pointer_set_flag = false;
        } else {
          this->insert_codestream_pointer_set_flag = true;
        }
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "false"; }}});
}

bool JPLMEncoderConfigurationLightField4DTransformMode::show_error_estimate()
    const noexcept {
  return show_estimated_error_flag;
}


bool JPLMEncoderConfigurationLightField4DTransformMode::
    insert_codestream_pointer_set() const noexcept {
  return insert_codestream_pointer_set_flag;
}


JPLMEncoderConfigurationLightField4DTransformMode::
    JPLMEncoderConfigurationLightField4DTransformMode(
        int argc, char **argv, std::size_t level)
    : JPLMEncoderConfigurationLightField(argc, argv, level) {
  this->message = "* Options for Transform mode ( -T,--type=0 ):";
}


JPLMEncoderConfigurationLightField4DTransformMode::
    JPLMEncoderConfigurationLightField4DTransformMode(int argc, char **argv)
    : JPLMEncoderConfigurationLightField4DTransformMode(argc, argv,
          JPLMEncoderConfigurationLightField4DTransformMode::
              current_hierarchy_level) {
  this->init(argc, argv);
  init_transform_size();  //<! \todo check if this may bring problems when this class is derived
}


CompressionTypeLightField
JPLMEncoderConfigurationLightField4DTransformMode::get_compression_type()
    const {
  return CompressionTypeLightField::transform_mode;
}


double JPLMEncoderConfigurationLightField4DTransformMode::get_lambda() const {
  return lambda;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_intra_view_vertical() {
  return minimal_transform_size_intra_view_vertical_v;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_intra_view_vertical() {
  return maximal_transform_size_intra_view_vertical_v;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_intra_view_horizontal() {
  return minimal_transform_size_intra_view_horizontal_u;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_intra_view_horizontal() {
  return maximal_transform_size_intra_view_horizontal_u;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_inter_view_vertical() {
  return minimal_transform_size_inter_view_vertical_t;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_inter_view_vertical() {
  return maximal_transform_size_inter_view_vertical_t;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_inter_view_horizontal() {
  return minimal_transform_size_inter_view_horizontal_s;
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_inter_view_horizontal() {
  return maximal_transform_size_inter_view_horizontal_s;
}


BorderBlocksPolicy
JPLMEncoderConfigurationLightField4DTransformMode::get_border_blocks_policy() {
  return border_policy;
}


void JPLMEncoderConfigurationLightField4DTransformMode::init_transform_size() {
  IntraView transform_size_maximum_intra_view{
      get_maximal_transform_size_intra_view_vertical(),
      get_maximal_transform_size_intra_view_horizontal()};
  InterView transform_size_maximum_inter_view{
      get_maximal_transform_size_inter_view_vertical(),
      get_maximal_transform_size_inter_view_horizontal()};
  IntraView transform_size_minimum_intra_view{
      get_minimal_transform_size_intra_view_vertical(),
      get_minimal_transform_size_intra_view_horizontal()};
  InterView transform_size_minimum_inter_view{
      get_minimal_transform_size_inter_view_vertical(),
      get_minimal_transform_size_inter_view_horizontal()};
  Maximum maximum{
      transform_size_maximum_intra_view, transform_size_maximum_inter_view};
  Minimum minimum{
      transform_size_minimum_intra_view, transform_size_minimum_inter_view};
  TransformSize t{maximum, minimum};
  this->transform_size = t;
}


std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
JPLMEncoderConfigurationLightField4DTransformMode::get_maximal_transform_sizes()
    const {
  return {maximal_transform_size_inter_view_vertical_t,
      maximal_transform_size_inter_view_horizontal_s,
      maximal_transform_size_intra_view_vertical_v,
      maximal_transform_size_intra_view_horizontal_u};
}


LightfieldDimension<uint32_t>
JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_dimension() const {
  return {get_maximal_transform_sizes()};
}


std::tuple<double, double, double, double>
JPLMEncoderConfigurationLightField4DTransformMode::get_transform_scalings()
    const {
  return {transform_scale_t, transform_scale_s, transform_scale_v,
      transform_scale_u};
}


std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
JPLMEncoderConfigurationLightField4DTransformMode::get_minimal_transform_sizes()
    const {
  return {minimal_transform_size_inter_view_vertical_t,
      minimal_transform_size_inter_view_horizontal_s,
      minimal_transform_size_intra_view_vertical_v,
      minimal_transform_size_intra_view_horizontal_u};
}


LightfieldDimension<uint32_t>
JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_dimension() const {
  return {get_minimal_transform_sizes()};
}
