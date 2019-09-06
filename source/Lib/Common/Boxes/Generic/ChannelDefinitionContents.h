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

/** \file     ChannelDefinitionContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_CHANNELDEFINITIONCONTENTS_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_CHANNELDEFINITIONCONTENTS_H__

#include "Lib/Common/Boxes/Generic/ChannelDescription.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"

/**
 * \brief      Class for channel definition contents.
 * \sa ChannelDefinitionBox
 * \todo It is necessary to define the get_bytes() method of ChannelDefinitionContents.
 */
class ChannelDefinitionContents : public InMemoryDBox {
 protected:
  std::vector<ChannelDescription>
      channel_descriptions;  //!< Vector of channel description

 public:
  ChannelDefinitionContents() = default;


  /**
   * \brief      Copy constructor
   *
   * \param[in]  other  The other channel definition content
   */
  ChannelDefinitionContents(const ChannelDefinitionContents& other)
      : channel_descriptions(other.channel_descriptions) {
  }


  /**
   * \brief      Move constructor
   *
   * \param      other  The other
   */
  ChannelDefinitionContents(ChannelDefinitionContents&& other)
      : channel_descriptions(std::move(other.channel_descriptions)) {
  }


  /**
   * \brief      Destroys the object.
   */
  virtual ~ChannelDefinitionContents() = default;


  /**
   * \brief      Determines if equal.
   *
   * \param[in]  other  The other
   *
   * \return     True if equal, false otherwise.
   */
  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const ChannelDefinitionContents&>(other);
    return *this == cast_other;
  }


  /**
   * \brief      Creates a new instance of the object with same properties than original.
   *
   * \return     Copy of this object.
   */
  virtual ChannelDefinitionContents* clone() const override {
    return new ChannelDefinitionContents(*this);
  }


  /**
   * \brief      Gets the size, in bytes, of this contents
   *
   * \return     Size in bytes
   * \details    There are 2 bytes to represent N (number of channel descriptions) and 3 uint16_t for each channel descriptor.
   */
  uint64_t size() const noexcept override {
    //2 is from N (number of channel descriptions)
    //3 is from the 3 fields in ChannelDescription
    return 2 + this->channel_descriptions.size() * 3 * sizeof(uint16_t);
  }


  /**
   * \brief      Equal operator
   *
   * \param[in]  other  The other ChannelDefinitionContents
   *
   * \return     True if equal, false otherwise.
   */
  bool operator==(const ChannelDefinitionContents& other) const {
    return this->channel_descriptions == other.channel_descriptions;
  }


  /**
   * \brief      Not equal operator
   *
   * \param[in]  other  The other ChannelDefinitionContents
   *
   * \return     False if equal, true otherwise.
   */
  bool operator!=(const ChannelDefinitionContents& other) const {
    return !this->operator==(other);
  }
};


#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_CHANNELDEFINITIONCONTENTS_H__ */