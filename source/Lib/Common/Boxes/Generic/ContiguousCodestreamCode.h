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

/** \file     ContiguousCodestreamCode.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-29
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODE_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODE_H__

#include <cstddef>  //for std::byte
#include <cstdint>
#include <ostream>
#include <vector>

/**
 * \brief      Class for contiguous codestream code.
 */
class ContiguousCodestreamCode {
 public:
  /**
   * \brief      Gets the size (in bytes) of the contiguous codestream code
   *
   * \return     The size (in bytes)
   */
  virtual uint64_t size() const noexcept = 0;


  /**
   * \brief      Creates a new instance of the object with same properties than original.
   *
   * \return     Copy of this object.
   */
  virtual ContiguousCodestreamCode* clone() const = 0;


  /**
   * \brief      Determines whether the specified other is equal.
   *
   * \param[in]  other  The other
   *
   * \return     True if the specified other is equal, False otherwise.
   */
  virtual bool is_equal(const ContiguousCodestreamCode& other) const = 0;


  /**
   * \brief      Writes to the specified stream.
   *
   * \param      stream  The stream
   *
   * \return     Returns a reference to the stream (param)
   */
  virtual std::ostream& write_to(std::ostream& stream) const = 0;


  /**
   * \brief      Pushes a byte.
   *
   * \param[in]  byte  The byte
   */
  virtual void push_byte(const std::byte byte) = 0;


  /**
   * \brief      Gets the byte at the specified position.
   *
   * \param[in]  pos   The position
   *
   * \return     The byte at pos.
   */
  virtual std::byte get_byte_at(const uint64_t pos) const = 0;


  virtual std::size_t get_current_position() const = 0;


  /**
   * \brief      Gets the next byte.
   *
   * \return     The next byte.
   */
  virtual std::byte get_next_byte() const = 0;


  /**
   * \brief      Gets the next n bytes.
   *
   * \param[in]  n     Number of bytes to be get.
   *
   * \return     The next n bytes.
   */
  virtual std::vector<std::byte> get_next_n_bytes(std::size_t n) const = 0;


  /**
   * \brief      Determines if the next byte is valid.
   *
   * \return     True if next valid, False otherwise.
   */
  virtual bool is_next_valid() const = 0;


  /**
   * \brief      Gets the next byte without advancing the pointer to the next byte
   *
   * \return     The next byte
   */
  virtual std::byte peek_next_byte() const = 0;


  /**
   * \brief      Rewinds the pointer to next byte according to the param
   *
   * \param[in]  n_bytes_to_rewind  The number of bytes to rewind
   */
  virtual void rewind(std::size_t n_bytes_to_rewind) const = 0;


  /**
   * \brief      Inserts bytes in the desired position
   *
   * \param[in]  initial_position  The initial position
   * \param[in]  bytes_to_insert   The bytes to insert
   */
  virtual void insert_bytes(std::size_t initial_position,
      const std::vector<std::byte>& bytes_to_insert) = 0;


  /**
   * \brief      Equality operator.
   *
   * \param[in]  other  The other
   *
   * \return     The result of the equality
   */
  bool operator==(const ContiguousCodestreamCode& other) const;


  /**
   * \brief      Inequality operator.
   *
   * \param[in]  other  The other
   *
   * \return     The result of the inequality
   */
  bool operator!=(const ContiguousCodestreamCode& other) const;

  //! \todo define a iterator interface for ContiguousCodestreamCode
};


std::ostream& operator<<(
    std::ostream& stream, const ContiguousCodestreamCode& code);

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODE_H__ */