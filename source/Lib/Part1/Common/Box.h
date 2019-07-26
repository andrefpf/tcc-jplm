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
#include <iostream>
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
  bool operator==(const LBox& other) const {
    return this->is_equal(other);
  }

  bool operator!=(const LBox& other) const {
    return !operator==(other);
  }
};


class TBox : public BoxDataHolder<uint32_t> {
 public:
  TBox() = default;
  TBox(const uint32_t& value) : BoxDataHolder<uint32_t>(value) {
  }
  ~TBox() = default;

  bool operator==(const TBox& other) const {
    return this->is_equal(other);
  }

  bool operator!=(const TBox& other) const {
    return !operator==(other);
  }
};


class XLBox : public BoxDataHolder<uint64_t> {
 public:
  XLBox() = default;
  XLBox(const uint64_t& value) : BoxDataHolder<uint64_t>(value) {
  }
  ~XLBox() = default;

  bool operator==(const XLBox& other) const {
    return this->is_equal(other);
  }

  bool operator!=(const XLBox& other) const {
    return !operator==(other);
  }
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
  virtual uint64_t size() const noexcept = 0;

  virtual DBox* clone() const = 0;

  const std::any& get_ref_to_contents() const {
    return contents;
  }

  virtual bool is_equal(const DBox& other) const = 0;

  bool operator==(const DBox& other) const {
    return this->is_equal(other);
  }

  bool operator!=(const DBox& other) const {
    return !this->operator==(other);
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


  uint64_t size() const noexcept override {
    return std::any_cast<std::vector<uint8_t>>(this->contents).size();
  }

  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (std::any_cast<std::vector<uint8_t>>(this->get_ref_to_contents()) ==
            std::any_cast<std::vector<uint8_t>>(other.get_ref_to_contents()));
  }

  CharArrayDBox* clone() const override {
    return new CharArrayDBox(*this);
  }
};


class EmptyDBox : public DBox {
 public:
  EmptyDBox() = default;
  EmptyDBox(const EmptyDBox& other) = default;
  ~EmptyDBox() = default;
  uint64_t size() const noexcept override {
    return 0;
  }

  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return true;
  }

  EmptyDBox* clone() const override {
    return new EmptyDBox(*this);
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
  Box(TBox t_box, const DBox& d_box = EmptyDBox())
      : t_box(t_box), d_box(std::unique_ptr<DBox>(d_box.clone())) {
    constexpr uint64_t LBox_size = 4;  //bytes = 32 bits
    constexpr uint64_t TBox_size = 4;  //bytes = 32 bits
    //this means that the LBox will contain at least 8 bytes and thus
    //never use the reserved for ISO use values;

    auto total_box_size = LBox_size + TBox_size + d_box.size();
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

  std::variant<LBox, XLBox> get_lenght() const noexcept;
  std::uint64_t size() const noexcept;

  LBox get_lbox() const noexcept;
  TBox get_tbox() const noexcept;
  std::optional<XLBox> get_xlbox() const noexcept;
  std::unique_ptr<DBox> get_dbox() const noexcept;


  bool has_same_type(const Box& other) const noexcept {
    if (other.t_box == this->t_box)
      return true;
    return false;
  }


  bool has_same_lenght(const Box& other) const noexcept {
    if (other.l_box != this->l_box) {
      return false;
    }
    if (this->l_box.get_value() == 1) {
      if (this->xl_box != other.xl_box) {
        return false;
      }
    }
    return true;
  }

  bool holds_same_data(const Box& other) const noexcept {
    if (*(other.d_box) != *(this->d_box)) {
      return false;
    }
    return true;
  }


  bool is_equal(const Box& other) const noexcept {
    if (this->holds_same_data(other) && this->has_same_type(other) &&
        this->has_same_lenght(other))
      return true;
    return false;
  }


  bool operator==(const Box& other) const {
    return this->is_equal(other);
  }

  bool operator!=(const Box& other) const {
    return !this->operator==(other);
  }

  //LBox (required) 4-byte big-endian usigned integer: uint32_t
  //TBox (required) 4-byte big-endian usigned integer: uint32_t
  //XLBox (optional) if LBox.value = 1, 8-byte big-endian usigned integer: uint64_t
  //DBox (required) variable size: depends on what this contains
};


std::ostream& operator<<(std::ostream& stream, const Box& Box);

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOX_H__ */
