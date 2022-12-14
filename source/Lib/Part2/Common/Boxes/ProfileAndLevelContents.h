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

/** \file     ProfileAndLevelContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-22
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_PROFILEANDLEVELCONTENTS_H__
#define JPLM_LIB_PART2_COMMON_BOXES_PROFILEANDLEVELCONTENTS_H__


#include <tuple>  //std::tie
#include "Lib/Common/Boxes/GenericBox.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Utils/Stream/BinaryTools.h"


class ProfileAndLevelContents : public InMemoryDBox {
 protected:
  uint16_t ppih;  // profile of the codestream
  uint16_t plev;  // level of the codestream
 public:
  ProfileAndLevelContents(uint16_t ppih, uint16_t plev)
      : ppih(ppih), plev(plev) {
  }

  /**
   * \brief      Constructs the object with default values
   * \warning    The current standard is not defining any default value
   */
  ProfileAndLevelContents() : ppih(0), plev(0) {
  }


  auto get_profile_of_the_codestream() const noexcept {
    return ppih;
  }


  auto get_level_of_the_codestream() const noexcept {
    return plev;
  }


  auto get_ppih() const noexcept {
    return get_profile_of_the_codestream();
  }


  auto get_plev() const noexcept {
    return get_level_of_the_codestream();
  }


  virtual ~ProfileAndLevelContents() = default;


  virtual ProfileAndLevelContents* clone() const override {
    return new ProfileAndLevelContents(*this);
  }


  uint64_t size() const noexcept override {
    return 2 * sizeof(uint16_t);
  }


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const ProfileAndLevelContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const ProfileAndLevelContents& other) const noexcept {
    return (
        std::tie(this->ppih, this->plev) == std::tie(other.ppih, other.plev));
  }


  bool operator!=(const ProfileAndLevelContents& other) const noexcept {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    BinaryTools::append_big_endian_bytes(bytes, ppih);
    BinaryTools::append_big_endian_bytes(bytes, plev);
    return bytes;
  }
};


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_PROFILEANDLEVELCONTENTS_H__ */