/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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

/** \file     BoxParserHelperBase.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-06
 */

#include "Lib/Common/Boxes/Parsers/BoxParserHelperBase.h"


uint32_t BoxParserHelperBase::get_l_box_value_from_stream() {
  //ensuring the stream is at the expected position. i.e., the next get_bytes<sizeof(l_box_t)> will get l_box data..
  managed_stream.rewind();
  using namespace BinaryTools;
  return get_value_from_big_endian_byte_vector<uint32_t>(
      managed_stream.get_bytes<4>());
}

uint64_t BoxParserHelperBase::get_xl_box_value_from_stream() {
  //ensuring the stream is at the expected position. i.e., the next get_bytes<sizeof(l_box_t)> will get l_box data..
  using namespace BinaryTools;
  has_xl_field = true;
  return get_value_from_big_endian_byte_vector<uint64_t>(
      managed_stream.get_bytes<8>());
}

uint32_t BoxParserHelperBase::get_t_box_value_from_stream() {
  using namespace BinaryTools;
  return get_value_from_big_endian_byte_vector<uint32_t>(
      managed_stream.get_bytes<4>());
}

BoxParserHelperBase::BoxParserHelperBase(ManagedStream &stream, uint64_t length,
    uint32_t t_box_value_in_stream, bool has_xl_field)
    : managed_stream(stream.get_sub_managed_stream(length)), length(length),
      t_box_value_in_stream(t_box_value_in_stream), has_xl_field(has_xl_field) {
  managed_stream.seek(get_non_data_length());
}

BoxParserHelperBase::BoxParserHelperBase(ManagedStream &stream)
    : managed_stream(stream), length(get_l_box_value_from_stream()),
      t_box_value_in_stream(get_t_box_value_from_stream()) {
  if (length == 1) {
    length = get_xl_box_value_from_stream();
  }
}

BoxParserHelperBase BoxParserHelperBase::get_helper_with_protected_range() {
  managed_stream.rewind();
  return BoxParserHelperBase(
      managed_stream, length, t_box_value_in_stream, has_xl_field);
}

uint64_t BoxParserHelperBase::get_length() const noexcept {
  return length;
}

uint32_t BoxParserHelperBase::get_t_box_value() const noexcept {
  return t_box_value_in_stream;
}

uint64_t BoxParserHelperBase::get_non_data_length() const noexcept {
  auto non_data_length = 8;  //4 from l_box, 4 from t_box
  if (has_xl_field) {
    non_data_length += 8;
  }
  return non_data_length;
}

uint64_t BoxParserHelperBase::get_data_length() const noexcept {
  return get_length() - get_non_data_length();
}

ManagedStream BoxParserHelperBase::get_data_stream() {
  managed_stream.seek(get_non_data_length());
  return managed_stream.get_sub_managed_stream(get_data_length());
}

ManagedStream BoxParserHelperBase::get_remaining_stream() {
  return managed_stream.get_remaining_sub_managed_stream();
}

bool BoxParserHelperBase::has_data_available() const noexcept {
  return managed_stream.is_valid();
}

uint64_t BoxParserHelperBase::tell() const noexcept {
  return managed_stream.tell();
}

bool BoxParserHelperBase::is_a_box_with_id(const uint32_t id) {
  if (this->get_t_box_value() == id)
    return true;
  return false;
}