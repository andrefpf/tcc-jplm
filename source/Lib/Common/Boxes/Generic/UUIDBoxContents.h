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

/** \file     UUIDBoxContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef UUIDBOXCONTENTS_H__
#define UUIDBOXCONTENTS_H__

//UUID

#include "Lib/Common/Boxes/InMemoryDBoxContents.h"
#include "UniversalUniqueIdentifier.h"

class UUIDBoxContents : public InMemoryDBoxContents {
 protected:
  UniversalUniqueIdentifier id;
  std::vector<uint8_t> data;

 public:
  UUIDBoxContents() = default;


  virtual UUIDBoxContents* clone() const override {
    return new UUIDBoxContents(*this);
  }


  virtual ~UUIDBoxContents() = default;


  virtual uint64_t size() const noexcept override {
    return id.size() + data.size() * sizeof(uint8_t);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const UUIDBoxContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const UUIDBoxContents& other) const {
    return (this->id == other.id) && (this->data == other.data);
  }


  bool operator!=(const UUIDBoxContents& other) const {
    return !this->operator==(other);
  }


  void add_data(const std::vector<uint8_t>& data_to_add) {
    data.reserve(data_to_add.size());
    data.insert(data.end(), data_to_add.begin(), data_to_add.end());
  }
};


#endif /* end of include guard: UUIDBOXCONTENTS_H__ */