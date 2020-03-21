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

/** \file     JpegPlenoSignatureContents.h
 *  \brief    This files contains the JPEG Pleno Signature Contents class
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-02
 */

#ifndef JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOSIGNATURECONTENTS_H__
#define JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOSIGNATURECONTENTS_H__

#include <array>
#include <cstddef>  //std::byte
#include <vector>
#include "Lib/Common/Boxes/GenericBox.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"


/**
 * @brief      This class describes JPEG Pleno Signature Box contents.
 */
class JpegPlenoSignatureContents : public InMemoryDBox {
 protected:
  const std::array<std::byte, 4> signature = {
      std::byte{0x0d}, std::byte{0x0a}, std::byte{0x87}, std::byte{0x0a}};

 public:
  /**
   * @brief      Constructs a new instance.
   */
  JpegPlenoSignatureContents() = default;


  /**
   * @brief      Destroys the object.
   */
  virtual ~JpegPlenoSignatureContents() = default;


  /**
   * @brief      Creates a new instance of the object with same properties than original.
   *
   * @return     Copy of this object.
   */
  virtual JpegPlenoSignatureContents* clone() const override;


  /**
   * @brief      Gets the size (in bytes) of the contents of this JPEG Pleno Signature box
   *
   * @return     Size in bytes
   */
  virtual uint64_t size() const noexcept override;


  /**
   * @brief      Determines whether the specified other is equal.
   *
   * @param[in]  other  The other
   *
   * @return     True if the specified other is equal, False otherwise.
   */
  virtual bool is_equal(const DBox& other) const override;


  /**
   * @brief      Equality operator.
   *
   * @param[in]  other  The other
   *
   * @return     The result of the equality
   */
  bool operator==(const JpegPlenoSignatureContents& other) const;


  /**
   * @brief      Inequality operator.
   *
   * @param[in]  other  The other
   *
   * @return     The result of the inequality
   */
  bool operator!=(const JpegPlenoSignatureContents& other) const;


  /**
   * @brief      Determines whether the specified bytes is valid.
   *
   * @param[in]  bytes  The bytes
   *
   * @return     True if the specified bytes is valid, False otherwise.
   */
  bool is_valid(const std::vector<std::byte>& bytes);


  /**
   * @brief      Gets the reference to signature.
   *
   * @return     The reference to signature.
   */
  const std::array<std::byte, 4>& get_ref_to_signature() const noexcept;


  /**
   * @brief      Gets the bytes with the contents of JPEG Pleno Signature Box.
   *
   * @return     The bytes.
   */
  virtual std::vector<std::byte> get_bytes() const override;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOSIGNATURECONTENTS_H__ */