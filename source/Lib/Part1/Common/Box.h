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

/** \file     Box.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-23
 */


#ifndef JPLM_LIB_PART1_COMMON_BOX_H__
#define JPLM_LIB_PART1_COMMON_BOX_H__

#include <any>
#include <cstdint>
#include <memory>
#include <optional>
#include <variant>
#include <vector>
#include "BoxDataHolder.h"


class LBox : public BoxDataHolder<uint32_t> {
 public:
  LBox() = default;
  LBox(const uint32_t& value) : BoxDataHolder<uint32_t>(value) {
  }
  ~LBox() = default;
};

class TBox : public BoxDataHolder<uint32_t> {
 public:
  TBox() = default;
  TBox(const uint32_t& value) : BoxDataHolder<uint32_t>(value) {
  }
  ~TBox() = default;
};


class XLBox : public BoxDataHolder<uint64_t> {
 public:
  XLBox() = default;
  XLBox(const uint64_t& value) : BoxDataHolder<uint64_t>(value) {
  }
  ~XLBox() = default;
};


//Box contents (probably DataBox)
//what is this?
class DBox {
 protected:
  //i'm not sure yet how to handle the contents of this box;
  std::any contents;

 public:
  DBox() = default;


  DBox(const std::any& contents) : contents(contents) {
  }


  virtual ~DBox(){};
  virtual uint64_t get_size() const noexcept = 0;

  virtual DBox* clone() const = 0;

  std::any& get_ref_to_contents() {
    return contents;
  }
};


class CharArrayDBox : public DBox {
 public:
  CharArrayDBox(std::vector<uint8_t> array)
      : DBox(std::make_any<std::vector<uint8_t>>(array)) {
  }

  CharArrayDBox(const CharArrayDBox& other)
      : DBox(std::make_any<std::vector<uint8_t>>(
            std::any_cast<std::vector<uint8_t>>(other.contents))) {
  }

  ~CharArrayDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<std::vector<uint8_t>>(this->contents).size();
  }

  CharArrayDBox* clone() const override {
    return new CharArrayDBox(*this);
  }
};


class Box {
 protected:
  LBox l_box;
  TBox t_box;
  std::optional<XLBox> xl_box;
  std::unique_ptr<DBox> d_box;

  Box() = delete;

 public:
  Box(TBox t_box, const DBox& d_box)
      : t_box(t_box), d_box(std::unique_ptr<DBox>(d_box.clone())) {
    constexpr uint64_t LBox_size = 4;  //bytes = 32 bits
    constexpr uint64_t TBox_size = 4;  //bytes = 32 bits
    //this means that the LBox will contain at least 8 bytes and thus
    //never use the reserved for ISO use values;

    auto total_box_size = LBox_size + TBox_size + d_box.get_size();
    //assuming that size fits in LBox (i.e., 32 bits)

    if (total_box_size > std::numeric_limits<uint32_t>::max()) {
      l_box.set_value(1);
      constexpr uint64_t XLBox_size = 8;  //bytes = 64 bits
      total_box_size += XLBox_size;  //needs more XLBox_size bytes
      xl_box = XLBox(total_box_size);
    } else {
      l_box.set_value(total_box_size);
    }
  }
  ~Box() = default;

  std::variant<LBox, XLBox> get_lenght();

  LBox get_lbox() const noexcept;
  TBox get_tbox() const noexcept;
  std::optional<XLBox> get_xlbox() const noexcept;
  std::unique_ptr<DBox> get_dbox() const noexcept;


  //LBox (required) 4-byte big-endian usigned integer: uint32_t
  //TBox (required) 4-byte big-endian usigned integer: uint32_t
  //XLBox (optional) if LBox.value = 1, 8-byte big-endian usigned integer: uint64_t
  //DBox (required) variable size: depends on what this contains
};


std::ostream& operator<<(std::ostream& stream, const Box& Box);

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOX_H__ */
