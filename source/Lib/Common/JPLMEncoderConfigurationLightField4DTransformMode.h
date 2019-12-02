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

/** \file     JPLMEncoderConfigurationLightField4DTransformMode.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-11
 */

#ifndef JPLMENCODERCONFIGURATIONLIGHTFIELD4DTRANSFORMMODE_H__
#define JPLMENCODERCONFIGURATIONLIGHTFIELD4DTRANSFORMMODE_H__

#include <string>
#include "JPLMEncoderConfigurationLightField.h"
#include "Lib/Part2/Common/LightfieldDimension.h"

// \todo: Refactor and improve the redundancies in this class
class JPLMEncoderConfigurationLightField4DTransformMode
    : public JPLMEncoderConfigurationLightField {
 private:
  struct IntraView {
    uint32_t vertical, horizontal;
  };
  struct InterView {
    uint32_t vertical, horizontal;
  };
  struct Maximum {
    IntraView intra_view;
    InterView inter_view;
  };
  struct Minimum {
    IntraView intra_view;
    InterView inter_view;
  };
  struct TransformSize {
    Maximum maximum;
    Minimum minimum;
  };
  void init_transform_size();

  uint32_t maximal_transform_size_inter_view_vertical_t = 100000;
  uint32_t maximal_transform_size_inter_view_horizontal_s = 100000;
  uint32_t maximal_transform_size_intra_view_vertical_v = 100000;
  uint32_t maximal_transform_size_intra_view_horizontal_u = 100000;

  uint32_t minimal_transform_size_inter_view_vertical_t = 100000;
  uint32_t minimal_transform_size_inter_view_horizontal_s = 100000;
  uint32_t minimal_transform_size_intra_view_vertical_v = 100000;
  uint32_t minimal_transform_size_intra_view_horizontal_u = 100000;


  double lambda;

  double transform_scale_t = 1.0;
  double transform_scale_s = 1.0;
  double transform_scale_v = 1.0;
  double transform_scale_u = 1.0;


 protected:
  void parse_json(string path);
  void parse_minimal_transform_size_intra_view_vertical(const json &conf);
  void parse_maximal_transform_size_intra_view_vertical(const json &conf);
  void parse_minimal_transform_size_intra_view_horizontal(const json &conf);
  void parse_maximal_transform_size_intra_view_horizontal(const json &conf);
  void parse_minimal_transform_size_inter_view_vertical(const json &conf);
  void parse_maximal_transform_size_inter_view_vertical(const json &conf);
  void parse_minimal_transform_size_inter_view_horizontal(const json &conf);
  void parse_maximal_transform_size_inter_view_horizontal(const json &conf);
  void parse_lambda(const json &conf);


 public:
  JPLMEncoderConfigurationLightField4DTransformMode(int argc, char **argv);
  double get_lambda() const;
  virtual Type get_compression_type() const override;
  uint32_t get_minimal_transform_size_intra_view_vertical();
  uint32_t get_maximal_transform_size_intra_view_vertical();
  uint32_t get_minimal_transform_size_intra_view_horizontal();
  uint32_t get_maximal_transform_size_intra_view_horizontal();
  uint32_t get_minimal_transform_size_inter_view_vertical();
  uint32_t get_maximal_transform_size_inter_view_vertical();
  uint32_t get_minimal_transform_size_inter_view_horizontal();
  uint32_t get_maximal_transform_size_inter_view_horizontal();


  std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
  get_maximal_transform_sizes() const {
    return {maximal_transform_size_inter_view_vertical_t,
        maximal_transform_size_inter_view_horizontal_s,
        maximal_transform_size_intra_view_vertical_v,
        maximal_transform_size_intra_view_horizontal_u};
  }

  LightfieldDimension<uint32_t> get_maximal_transform_dimension() const {
    return {get_maximal_transform_sizes()};
  }


  std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
  get_minimal_transform_sizes() const {
    return {minimal_transform_size_inter_view_vertical_t,
        minimal_transform_size_inter_view_horizontal_s,
        minimal_transform_size_intra_view_vertical_v,
        minimal_transform_size_intra_view_horizontal_u};
  }


  LightfieldDimension<uint32_t> get_minimal_transform_dimension() const {
    return {get_minimal_transform_sizes()};
  }


  std::tuple<double, double, double, double> get_transform_scalings() const {
    return {transform_scale_t, transform_scale_s, transform_scale_v,
        transform_scale_u};
  }


  TransformSize transform_size;
};


JPLMEncoderConfigurationLightField4DTransformMode::
    JPLMEncoderConfigurationLightField4DTransformMode(int argc, char **argv)
    : JPLMEncoderConfigurationLightField(argc, argv) {
  arguments.push_back({"--lambda", "-l",
      "Lagrangian multiplier used in the RDO process of 4D Transform mode.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        std::string::size_type sz;
        this->lambda = std::stod(typed_string, &sz);
      }});

  arguments.push_back({"--transform_size_maximum_inter_view_vertical",
      "--transform_size_maximum_inter_view_vertical",
      "Maximum 4D transform size in inter-view vertical direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->maximal_transform_size_inter_view_vertical_t =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});

  arguments.push_back({"--transform_size_maximum_inter_view_horizontal",
      "--transform_size_maximum_inter_view_horizontal",
      "Maximum 4D transform size in inter-view horizontal direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->maximal_transform_size_inter_view_horizontal_s =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});

  arguments.push_back({"--transform_size_maximum_intra_view_vertical",
      "--transform_size_maximum_intra_view_vertical",
      "Maximum 4D transform size in intra-view vertical direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->maximal_transform_size_intra_view_vertical_v =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});

  arguments.push_back({"--transform_size_maximum_intra_view_horizontal",
      "--transform_size_maximum_intra_view_horizontal",
      "Maximum 4D transform size in intra-view horizontal direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->maximal_transform_size_intra_view_horizontal_u =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});


  arguments.push_back({"--transform_size_minimum_inter_view_vertical",
      "--transform_size_minimum_inter_view_vertical",
      "Minimum 4D transform size in inter-view vertical direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->minimal_transform_size_inter_view_vertical_t =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});

  arguments.push_back({"--transform_size_minimum_inter_view_horizontal",
      "--transform_size_minimum_inter_view_horizontal",
      "Minimum 4D transform size in inter-view horizontal direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->minimal_transform_size_inter_view_horizontal_s =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});

  arguments.push_back({"--transform_size_minimum_intra_view_vertical",
      "--transform_size_minimum_intra_view_vertical",
      "Minimum 4D transform size in intra-view vertical direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->minimal_transform_size_intra_view_vertical_v =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});

  arguments.push_back({"--transform_size_minimum_intra_view_horizontal",
      "--transform_size_minimum_intra_view_horizontal",
      "Minimum 4D transform size in intra-view horizontal direction.",
      [this](std::any v) {
        std::string typed_string = std::any_cast<std::string>(v);
        this->minimal_transform_size_intra_view_horizontal_u =
            static_cast<uint32_t>(std::stoul(typed_string));
      }});

  this->parse_cli(argc, argv);
  if (!config.empty())
    parse_json(config);
  init_transform_size();
}

Type JPLMEncoderConfigurationLightField4DTransformMode::get_compression_type()
    const {
  return Type::transform_mode;
}

double JPLMEncoderConfigurationLightField4DTransformMode::get_lambda() const {
  return lambda;
}

void JPLMEncoderConfigurationLightField4DTransformMode::parse_json(string p) {
  JPLMEncoderConfigurationLightField::parse_json(p);
  ifstream ifs(p);
  json conf = json::parse(ifs);
  parse_minimal_transform_size_intra_view_vertical(conf);
  parse_maximal_transform_size_intra_view_vertical(conf);
  parse_minimal_transform_size_intra_view_horizontal(conf);
  parse_maximal_transform_size_intra_view_horizontal(conf);
  parse_minimal_transform_size_inter_view_vertical(conf);
  parse_maximal_transform_size_inter_view_vertical(conf);
  parse_minimal_transform_size_inter_view_horizontal(conf);
  parse_maximal_transform_size_inter_view_horizontal(conf);
  parse_lambda(conf);
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

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_minimal_transform_size_intra_view_vertical(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("minimum"))
      if (conf["transform_size"]["minimum"].contains("intra-view"))
        if (conf["transform_size"]["minimum"]["intra-view"].contains(
                "vertical"))
          minimal_transform_size_intra_view_vertical_v =
              conf["transform_size"]["minimum"]["intra-view"]["vertical"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_maximal_transform_size_intra_view_vertical(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("maximum"))
      if (conf["transform_size"]["maximum"].contains("intra-view"))
        if (conf["transform_size"]["maximum"]["intra-view"].contains(
                "vertical"))
          maximal_transform_size_intra_view_vertical_v =
              conf["transform_size"]["maximum"]["intra-view"]["vertical"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_minimal_transform_size_intra_view_horizontal(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("minimum"))
      if (conf["transform_size"]["minimum"].contains("intra-view"))
        if (conf["transform_size"]["minimum"]["intra-view"].contains(
                "horizontal"))
          minimal_transform_size_intra_view_horizontal_u =
              conf["transform_size"]["minimum"]["intra-view"]["horizontal"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_maximal_transform_size_intra_view_horizontal(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("maximum"))
      if (conf["transform_size"]["maximum"].contains("intra-view"))
        if (conf["transform_size"]["maximum"]["intra-view"].contains(
                "horizontal"))
          maximal_transform_size_intra_view_horizontal_u =
              conf["transform_size"]["maximum"]["intra-view"]["horizontal"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_minimal_transform_size_inter_view_vertical(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("minimum"))
      if (conf["transform_size"]["minimum"].contains("inter-view"))
        if (conf["transform_size"]["minimum"]["inter-view"].contains(
                "vertical"))
          minimal_transform_size_inter_view_vertical_t =
              conf["transform_size"]["minimum"]["inter-view"]["vertical"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_maximal_transform_size_inter_view_vertical(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("maximum"))
      if (conf["transform_size"]["maximum"].contains("inter-view"))
        if (conf["transform_size"]["maximum"]["inter-view"].contains(
                "vertical"))
          maximal_transform_size_inter_view_vertical_t =
              conf["transform_size"]["maximum"]["inter-view"]["vertical"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_minimal_transform_size_inter_view_horizontal(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("minimum"))
      if (conf["transform_size"]["minimum"].contains("inter-view"))
        if (conf["transform_size"]["minimum"]["inter-view"].contains(
                "horizontal"))
          minimal_transform_size_inter_view_horizontal_s =
              conf["transform_size"]["minimum"]["inter-view"]["horizontal"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::
    parse_maximal_transform_size_inter_view_horizontal(const json &conf) {
  if (conf.contains("transform_size"))
    if (conf["transform_size"].contains("maximum"))
      if (conf["transform_size"]["maximum"].contains("inter-view"))
        if (conf["transform_size"]["maximum"]["inter-view"].contains(
                "horizontal"))
          maximal_transform_size_inter_view_horizontal_s =
              conf["transform_size"]["maximum"]["inter-view"]["horizontal"]
                  .get<uint32_t>();
}

void JPLMEncoderConfigurationLightField4DTransformMode::parse_lambda(
    const json &conf) {
  if (conf.contains("lambda"))
    this->lambda = conf["lambda"].get<double>();
}


#endif /* end of include guard: JPLMENCODERCONFIGURATIONLIGHTFIELD4DTRANSFORMMODE_H__ */