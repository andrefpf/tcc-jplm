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

/** \file     BinaryTools.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-30
 */

#include "BinaryTools.h"

uint8_t BinaryTools::swap_endianess(const uint8_t in) {
  return in;
}


uint16_t BinaryTools::swap_endianess(const uint16_t in) {
  return ((in >> 8) & 0x00FF) | ((in << 8) & 0xFF00);
}


uint32_t BinaryTools::swap_endianess(const uint32_t in) {
  return ((in >> 24) & 0x000000FF) | ((in >> 8)  & 0x0000FF00) |
         ((in << 8)  & 0x00FF0000) | ((in << 24) & 0xFF000000);
}


uint64_t BinaryTools::swap_endianess(const uint64_t in) {
  return ((in >> 56) & 0x00000000000000FF) | ((in >> 40) & 0x000000000000FF00) |
         ((in >> 24) & 0x0000000000FF0000) | ((in >> 8)  & 0x00000000FF000000) |
         ((in << 8)  & 0x000000FF00000000) | ((in << 24) & 0x0000FF0000000000) |
         ((in << 40) & 0x00FF000000000000) | ((in << 56) & 0xFF00000000000000);
}

std::vector<std::byte>& BinaryTools::byte_vector_cat(
    std::vector<std::byte>& vec_a, const std::vector<std::byte>& vec_b) {
  vec_a.insert(vec_a.end(), vec_b.begin(), vec_b.end());
  return vec_a;
}