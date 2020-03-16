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
#include "Lib/Common/JPLMEncoderConfigurationLightField.h"
#include "Lib/Part2/Common/LightfieldDimension.h"
#include "Lib/Part2/Common/TransformMode/BorderBlocksPolicy.h"
#include "Lib/Part2/Common/TransformMode/ComponentSsizParameter.h"

// \todo: Refactor and improve the redundancies in this class
class JPLMEncoderConfigurationLightField4DTransformMode
    : public JPLMEncoderConfigurationLightField {
 private:
  static constexpr std::size_t current_hierarchy_level = 2;
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

  uint32_t maximal_transform_size_inter_view_vertical_t = 13;
  uint32_t maximal_transform_size_inter_view_horizontal_s = 13;
  uint32_t maximal_transform_size_intra_view_vertical_v = 64;
  uint32_t maximal_transform_size_intra_view_horizontal_u = 64;

  uint32_t minimal_transform_size_inter_view_vertical_t = 13;
  uint32_t minimal_transform_size_inter_view_horizontal_s = 13;
  uint32_t minimal_transform_size_intra_view_vertical_v = 4;
  uint32_t minimal_transform_size_intra_view_horizontal_u = 4;

  double lambda = 1000.0;
  BorderBlocksPolicy border_policy = BorderBlocksPolicy::truncate;

  double transform_scale_t = 1.0;
  double transform_scale_s = 1.0;
  double transform_scale_v = 1.0;
  double transform_scale_u = 1.0;

  bool show_estimated_error_flag = false;

 protected:
  virtual void add_options() override;
  JPLMEncoderConfigurationLightField4DTransformMode(
      int argc, char **argv, std::size_t level);
  void parse_json(std::string path);

 public:
  JPLMEncoderConfigurationLightField4DTransformMode(int argc, char **argv);
  double get_lambda() const;
  virtual CompressionTypeLightField get_compression_type() const override;
  uint32_t get_minimal_transform_size_intra_view_vertical();
  uint32_t get_maximal_transform_size_intra_view_vertical();
  uint32_t get_minimal_transform_size_intra_view_horizontal();
  uint32_t get_maximal_transform_size_intra_view_horizontal();
  uint32_t get_minimal_transform_size_inter_view_vertical();
  uint32_t get_maximal_transform_size_inter_view_vertical();
  uint32_t get_minimal_transform_size_inter_view_horizontal();
  uint32_t get_maximal_transform_size_inter_view_horizontal();

  BorderBlocksPolicy get_border_blocks_policy();

  bool show_error_estimate() const {
    return show_estimated_error_flag;
  }

  std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
  get_maximal_transform_sizes() const;

  LightfieldDimension<uint32_t> get_maximal_transform_dimension() const;

  std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
  get_minimal_transform_sizes() const;

  LightfieldDimension<uint32_t> get_minimal_transform_dimension() const;

  std::tuple<double, double, double, double> get_transform_scalings() const;

  std::vector<ComponentSsizParameter> get_bitdepths() const {
    return std::vector<ComponentSsizParameter>(
        this->get_number_of_colour_channels(), ComponentSsizParameter(10));
  }

  std::vector<uint8_t> get_max_bitplanes() const {
    return std::vector<uint8_t>(this->get_number_of_colour_channels(), 30);
  }

  TransformSize transform_size;


  virtual uint16_t get_level() const override {
    auto level_for_resolution = JPLMEncoderConfigurationLightField::get_level();

    // auto max_transform_size = std::max(get_maximal_transform_sizes());

    return 0;
  }
};


#endif /* end of include guard: JPLMENCODERCONFIGURATIONLIGHTFIELD4DTRANSFORMMODE_H__ */
