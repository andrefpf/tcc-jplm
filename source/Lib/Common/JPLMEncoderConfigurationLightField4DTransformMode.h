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

#include "JPLMEncoderConfigurationLightField.h"

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


 public:
  JPLMEncoderConfigurationLightField4DTransformMode(int argc, char **argv);
  double get_lambda() const;
  const Type &get_compression_type() const;
  uint32_t get_minimal_transform_size_intra_view_vertical() const;
  uint32_t get_maximal_transform_size_intra_view_vertical() const;
  uint32_t get_minimal_transform_size_intra_view_horizontal() const;
  uint32_t get_maximal_transform_size_intra_view_horizontal() const;
  uint32_t get_minimal_transform_size_inter_view_vertical() const;
  uint32_t get_maximal_transform_size_inter_view_vertical() const;
  uint32_t get_minimal_transform_size_inter_view_horizontal() const;
  uint32_t get_maximal_transform_size_inter_view_horizontal() const;
  TransformSize transform_size;
};

JPLMEncoderConfigurationLightField4DTransformMode::
    JPLMEncoderConfigurationLightField4DTransformMode(int argc, char **argv)
    : JPLMEncoderConfigurationLightField(argc, argv) {
  if (!config.empty())
    parse_json(config);
  init_transform_size();
}

const Type &
JPLMEncoderConfigurationLightField4DTransformMode::get_compression_type()
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
}


uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_intra_view_vertical() const {
  return minimal_transform_size_intra_view_vertical_v;
}

uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_intra_view_vertical() const {
  return maximal_transform_size_intra_view_vertical_v;
}

uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_intra_view_horizontal() const {
  return minimal_transform_size_intra_view_horizontal_u;
}

uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_intra_view_horizontal() const {
  return maximal_transform_size_intra_view_horizontal_u;
}

uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_inter_view_vertical() const {
  return minimal_transform_size_inter_view_vertical_t;
}

uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_inter_view_vertical() const {
  return maximal_transform_size_inter_view_vertical_t;
}

uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_minimal_transform_size_inter_view_horizontal() const {
  return minimal_transform_size_inter_view_horizontal_s;
}

uint32_t JPLMEncoderConfigurationLightField4DTransformMode::
    get_maximal_transform_size_inter_view_horizontal() const {
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
          minimal_transform_size_intra_view_horizontal_u =
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
          minimal_transform_size_intra_view_horizontal_u =
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
          minimal_transform_size_intra_view_horizontal_u =
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
          minimal_transform_size_intra_view_horizontal_u =
              conf["transform_size"]["maximum"]["inter-view"]["horizontal"]
                  .get<uint32_t>();
}


#endif /* end of include guard: JPLMENCODERCONFIGURATIONLIGHTFIELD4DTRANSFORMMODE_H__ */