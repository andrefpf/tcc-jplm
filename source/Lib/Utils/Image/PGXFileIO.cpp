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

/** \file     PGXFileIO.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-05
 */

#include "PGXFileIO.h"


void check_header(std::ifstream& ifstream) {
  std::string id;
  ifstream >> id;
  if (id != "PG") {
    throw PGXFileExceptions::InvalidIdException(id);
  }
}


auto get_endianess(std::ifstream& ifstream) {
  std::string endianess;
  ifstream >> endianess;
  if (endianess == "ML") {
    return PGXEndianess::PGX_ML_BIG_ENDIAN;
  }
  if (endianess != "LM") {
    throw PGXFileExceptions::InvalidEndianessException(endianess);
  }
  return PGXEndianess::PGX_LM_LITTLE_ENDIAN;
}


auto get_is_signed(std::ifstream& ifstream) {
  auto sign = ifstream.get();
  if (sign == ' ') {
  	sign = ifstream.get();
  }
  if (sign == '-') {
    return true;
  }
  if (sign != '+') {
    throw PGXFileExceptions::InvalidSignFieldException(std::string(1, sign));
  }
  return false;
}


auto get_value(std::ifstream& ifstream) {
	std::size_t value;
	ifstream >> value;
	return value;
}


std::unique_ptr<PGXFile> PGXFileIO::open(const std::string& filename) {
  std::ifstream file(filename, std::ios::in);
  check_header(file);
  const auto endianess = get_endianess(file);
  const auto is_signed = get_is_signed(file);
  const auto depth = get_value(file);
  const auto width = get_value(file);
  const auto height = get_value(file);

  return std::make_unique<PGXFile>(filename, width, height, depth, is_signed, endianess);
}