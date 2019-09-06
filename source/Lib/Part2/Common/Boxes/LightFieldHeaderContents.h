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

/** \file     LightFieldHeaderContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-22
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_LIGHTFIELDHEADERCONTENTS_H__
#define JPLM_LIB_PART2_COMMON_BOXES_LIGHTFIELDHEADERCONTENTS_H__

#include <tuple>  //std::tie
#include "CompressionTypeLightField.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Common/Boxes/GenericBox.h"
#include "Lib/Utils/Stream/BinaryTools.h"
#include "Lib/Part2/Common/LightfieldDimension.h"

using colour_space_unknown_flag_type = uint8_t;

enum class ColourSpaceUnknownFlag : colour_space_unknown_flag_type {
  known =
      0,  //the colour space is known and correctly specified in the Colourspace Specification boxes
  unknown = 1,  //the colour space is unknown
  //other values are reserved for ISO use
};

using intelectual_property_flag_type = uint8_t;

enum class IntelectualPropertyFlag : intelectual_property_flag_type {
  has_no_rights_information = 0,  //
  has_rights_information = 1,  //
  //other values are reserved for ISO use
};


class LightFieldHeaderContents : public InMemoryDBox {
 protected:
  // uint32_t rows;  //T number of rows of the subaperture view array
  // uint32_t columns;  //S number of columns of the subaperture view array
  // uint32_t height;  //V subaperture view height
  // uint32_t width;  //U subaperture view width
  LightfieldDimension<uint32_t> light_field_dimension;
  uint16_t nc;  // number of components
  uint8_t bpc;  // bits per component
  //! \todo bits per component has a different synthax
  CompressionTypeLightField c;  // compression type (i.e., the mode)
  ColourSpaceUnknownFlag unkc;  //
  IntelectualPropertyFlag ipr;  //
 public:
  LightFieldHeaderContents(
      const LightfieldDimension<uint32_t>& light_field_dimension, uint16_t nc,
      uint8_t bpc, CompressionTypeLightField c,
      ColourSpaceUnknownFlag unkc = ColourSpaceUnknownFlag::known,
      IntelectualPropertyFlag ipr =
          IntelectualPropertyFlag::has_no_rights_information)
      : light_field_dimension(light_field_dimension), nc(nc), bpc(bpc), c(c),
        unkc(unkc), ipr(ipr) {
  }


  LightFieldHeaderContents(uint32_t rows, uint32_t columns, uint32_t height,
      uint32_t width, uint16_t nc, uint8_t bpc, CompressionTypeLightField c,
      ColourSpaceUnknownFlag unkc = ColourSpaceUnknownFlag::known,
      IntelectualPropertyFlag ipr =
          IntelectualPropertyFlag::has_no_rights_information)
      : LightFieldHeaderContents(
            {rows, columns, height, width}, nc, bpc, c, unkc, ipr) {
  }


  virtual ~LightFieldHeaderContents() = default;


  auto get_number_of_components() const noexcept {
    return nc;
  }


  auto get_bits_per_component() const noexcept {
    return bpc;
  }


  auto get_light_field_dimension() const noexcept {
    return light_field_dimension;
  }


  auto get_compression_type() const noexcept {
    return c;
  }


  auto get_color_space_unknown_flag() const noexcept {
    return unkc;
  }


  auto get_intelectual_property_flag() const noexcept {
    return ipr;
  }


  virtual LightFieldHeaderContents* clone() const override {
    return new LightFieldHeaderContents(*this);
  }


  uint64_t size() const noexcept override {
    return 4 * sizeof(uint32_t) + sizeof(uint16_t) + 4 * sizeof(uint8_t);
  }


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const LightFieldHeaderContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const LightFieldHeaderContents& other) const noexcept {
    return (std::tie(this->light_field_dimension, this->nc, this->bpc, this->c,
                this->unkc, this->ipr) == std::tie(other.light_field_dimension,
                                              other.nc, other.bpc, other.c,
                                              other.unkc, other.ipr));
  }


  bool operator!=(const LightFieldHeaderContents& other) const noexcept {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());

    const auto& [rows, columns, height, width] = light_field_dimension;

    BinaryTools::append_big_endian_bytes(bytes, rows);
    BinaryTools::append_big_endian_bytes(bytes, columns);
    BinaryTools::append_big_endian_bytes(bytes, height);
    BinaryTools::append_big_endian_bytes(bytes, width);
    BinaryTools::append_big_endian_bytes(bytes, nc);
    BinaryTools::append_big_endian_bytes(bytes, bpc);

    BinaryTools::append_big_endian_bytes(
        bytes, static_cast<compression_type_data>(c));
    BinaryTools::append_big_endian_bytes(
        bytes, static_cast<colour_space_unknown_flag_type>(unkc));
    BinaryTools::append_big_endian_bytes(
        bytes, static_cast<intelectual_property_flag_type>(ipr));

    return bytes;
  }
};


using LightFieldHeaderBox = GenericBox<0x6c686472, LightFieldHeaderContents>;

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_LIGHTFIELDHEADERCONTENTS_H__ */