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

#include "Lib/Utils/Image/PGXFileIO.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"


inline std::string get_header_id_string() noexcept {
  return "PG";
}


inline std::string get_big_endian_string() noexcept {
  return "ML";
}


inline std::string get_little_endian_string() noexcept {
  return "LM";
}


inline char get_unsigned_char() noexcept {
  return '+';
}


inline char get_signed_char() noexcept {
  return '-';
}


void check_header(std::ifstream& ifstream) {
  std::string id;
  ifstream >> id;
  if (id != get_header_id_string()) {
    throw PGXFileExceptions::InvalidIdException(id);
  }
}


auto get_endianess(std::ifstream& ifstream) {
  std::string endianess;
  ifstream >> endianess;
  if (endianess == get_big_endian_string()) {
    return PGXEndianess::PGX_ML_BIG_ENDIAN;
  }
  if (endianess != get_little_endian_string()) {
    throw PGXFileExceptions::InvalidEndianessException(endianess);
  }
  return PGXEndianess::PGX_LM_LITTLE_ENDIAN;
}


auto get_is_signed(std::ifstream& ifstream) {
  auto sign = ifstream.get();
  if (sign == ' ') {
    sign = ifstream.get();
  }
  if (sign == get_signed_char()) {
    return true;
  }
  if (sign != get_unsigned_char()) {
    throw PGXFileExceptions::InvalidSignFieldException(std::string(1, sign));
  }
  return false;
}


auto get_value(std::ifstream& ifstream) {
  std::size_t value;
  ifstream >> value;
  return value;
}


void advance_to_raster_begin(std::ifstream& ifstream) {
  auto new_line_char = ifstream.get();
  if (new_line_char == ' ') {
    new_line_char = ifstream.get();
  }
  if (new_line_char == 0x0d) {
    new_line_char = ifstream.get();
  }
  if (new_line_char != 0x0a) {
    throw PGXFileExceptions::InvalidNewLine(std::string(1, new_line_char));
  }
}


std::unique_ptr<PGXFile> PGXFileIO::open(const std::string& filename) {
  std::ifstream file(filename, std::ios::in);

  check_header(file);
  const auto endianess = get_endianess(file);
  const auto is_signed = get_is_signed(file);
  const auto depth = get_value(file);
  const auto width = get_value(file);
  const auto height = get_value(file);

  advance_to_raster_begin(file);

#ifdef _WIN32
  std::streamoff end_of_header = file.tellg();
  std::uint16_t line_breaks =
      PixelMapFileIO::count_line_breaks(filename, end_of_header);
  std::streamoff raster_begin = end_of_header - line_breaks;
#else
  std::streamoff raster_begin = file.tellg();
#endif

  return std::make_unique<PGXFile>(
      filename, raster_begin, width, height, depth, is_signed, endianess);
}


std::unique_ptr<PGXFile> PGXFileIO::open(const std::string& filename,
    std::size_t width, std::size_t height, std::size_t depth, bool is_signed) {
  std::fstream file(filename, std::ios::out);

  file << get_header_id_string() << " ";

  //assuming that we are always going to write in big endian
  file << get_big_endian_string() << " ";

  if (is_signed) {
    file << get_signed_char();
  } else {
    file << get_unsigned_char();
  }

  file << std::to_string(depth) << " ";
  file << std::to_string(width) << " ";
  file << std::to_string(height) << " ";

  // The same as "file << std::endl", but in Windows
  file.close();
  file.open(filename, std::ios::out | std::ios::app | std::ios::binary);
  file << static_cast<std::uint8_t>(10) << std::flush;
  file.close();

  // The same as "file << std::endl", but in Windows
  file.open(filename, std::ios::in | std::ios::out);
  file.seekg(0, std::ios::end);
  auto raster_begin = file.tellg();

  return std::make_unique<PGXFile>(filename, raster_begin, width, height, depth,
      is_signed, PGXEndianess::PGX_ML_BIG_ENDIAN);
}