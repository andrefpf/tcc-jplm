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

/** \file     JPLMEncoderConfigurationLightField.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-11
 */

#ifndef JPLMENCODERCONFIGURATIONLIGHTFIELD_H__
#define JPLMENCODERCONFIGURATIONLIGHTFIELD_H__

#include <cstdint>
#include <filesystem>
#include <magic_enum.hpp>
#include <optional>
#include <tuple>
#include "Lib/Common/Boxes/Generic/EnumCS.h"
#include "Lib/Common/JPLMConfiguration.h"
#include "Lib/Common/JPLMEncoderConfiguration.h"
#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoProfileBrand.h"
#include "Lib/Part2/Common/Lightfield.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "nlohmann/json.hpp"

class JPLMEncoderConfigurationLightField : public JPLMEncoderConfiguration {
 private:
  void parse_mode_type(const nlohmann::json &conf);
  void check_inconsistencies();
  CompressionTypeLightField type;
  static constexpr std::size_t current_hierarchy_level = 1;

 protected:
  uint32_t number_of_rows_t;
  uint32_t number_of_columns_s;
  uint32_t view_height_v;
  uint32_t view_width_u;

  EnumCS enum_cs = EnumCS::YCbCr_2;
  uint16_t number_of_colour_channels =
      3;  //<! \todo check the type of number of colour channels

  JPLMEncoderConfigurationLightField(int argc, char **argv, std::size_t level);
  void parse_number_of_rows_t(const nlohmann::json &conf);
  void parse_number_of_columns_s(const nlohmann::json &conf);
  void parse_view_height_v(const nlohmann::json &conf);
  void parse_view_width_u(const nlohmann::json &conf);
  virtual void add_options() override;

 public:
  JPLMEncoderConfigurationLightField(int argc, char **argv);
  LightfieldIOConfiguration get_lightfield_io_configurations() const;
  uint32_t get_number_of_rows_t() const;
  uint32_t get_number_of_columns_s() const;
  uint32_t get_view_height_v() const;
  uint32_t get_view_width_u() const;
  virtual CompressionTypeLightField get_type() const;
  virtual CompressionTypeLightField get_compression_type() const;

  EnumCS get_enum_cs() const;
  uint16_t get_number_of_colour_channels() const;

  JpegPlenoProfileBrand get_profile() const {
    if (this->get_type() == CompressionTypeLightField::transform_mode) {
      return JpegPlenoProfileBrand::baseline_block_based_profile;
    }
    if (this->get_type() == CompressionTypeLightField::prediction_mode) {
      return JpegPlenoProfileBrand::baseline_prediction_based_profile;
    }
    throw JPLMConfigurationExceptions::InvalidPartException();
  }

  virtual uint16_t get_level() const {
    std::size_t total_samples = get_number_of_rows_t() *
                                get_number_of_columns_s() *
                                get_view_height_v() * get_view_width_u();

    if (total_samples <= 64000000) {
      return 1;
    }
    if (total_samples <= 256000000) {
      return 2;
    }
    if (total_samples <= 1024000000) {
      return 3;
    }
    if (total_samples <= 4096000000) {
      return 4;
    }
    if (total_samples <= 16384000000) {
      return 5;
    }
    //\todo should throw
    return 0;
  }
};


#endif /* end of include guard: JPLMENCODERCONFIGURATIONLIGHTFIELD_H__ */