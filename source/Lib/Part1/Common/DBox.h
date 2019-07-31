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

/** \file     DBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-29
 */

#ifndef JPLM_LIB_PART1_COMMON_DBOX_H__
#define JPLM_LIB_PART1_COMMON_DBOX_H__

#include <any>
#include <cstdint>
#include <iostream>
#include "DBoxContents.h"

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


  bool operator==(const DBox& other) const;


  bool operator!=(const DBox& other) const;

  
  virtual std::vector<std::byte> get_bytes() const noexcept = 0;


  friend std::ostream& operator<<(std::ostream& os, const DBox& d_box);
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
  

  virtual std::vector<std::byte> get_bytes() const noexcept override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    for(const auto& value: std::any_cast<std::vector<uint8_t>>(this->get_ref_to_contents())) {
      bytes.emplace_back(std::byte{value});
    }
    return bytes;
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


  virtual std::vector<std::byte> get_bytes() const noexcept override {
    return std::vector<std::byte>();
  }


};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_DBOX_H__ */