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

/** \file     JPLFileParser.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */

#include "JPLFileParser.h"
#include <filesystem>


std::unique_ptr<FileTypeBox>
JPLFileParser::decode_boxes_until_a_file_type_box_is_found() {
  while (this->managed_stream.is_valid()) {
    auto decoded_box =
        parser.parse(managed_stream.get_remaining_sub_managed_stream());
    if (decoded_box->get_tbox().get_value() == FileTypeBox::id) {
      file_type_box_index = decoded_boxes;
      ++decoded_boxes;
      return std::unique_ptr<FileTypeBox>(
          static_cast<FileTypeBox*>(decoded_box.release()));
    }
    ++decoded_boxes;
  }
  return nullptr;
}


uint64_t JPLFileParser::decode_boxes() {
  while (this->managed_stream.is_valid()) {
    // std::cout << "have " << this->managed_stream.get_length()
    //           << " bytes to decode" << std::endl;
    // auto managed_substream = managed_stream.get_sub_managed_stream(
    //     file_size - managed_stream.tell());
    // auto decoded_box = parser.parse(std::move(managed_substream));
    auto decoded_box =
        parser.parse(managed_stream.get_remaining_sub_managed_stream());
    auto id = decoded_box->get_tbox().get_value();
    if (auto it = temp_decoded_boxes.find(id); it == temp_decoded_boxes.end()) {
      //temp dececoded boxes map does not contain a box with the decoded box id yet
      temp_decoded_boxes[id] = std::vector<std::pair<uint64_t,
          std::unique_ptr<Box>>>();  //std::move(decoded_box)
      //created an empty vector of boxes
    }
    temp_decoded_boxes[id].emplace_back(
        std::make_pair(decoded_boxes++, std::move(decoded_box)));
    // std::cout << "decoded box with id: " << id << std::endl;
  }
  return decoded_boxes;
}


std::unique_ptr<JpegPlenoSignatureBox> JPLFileParser::get_signature_box() {
  return std::move(temp_signature);
}


std::unique_ptr<FileTypeBox> JPLFileParser::get_file_type_box() {
  return std::move(temp_file_type);
}


JPLFileParser::JPLFileParser(const std::string& filename)
    : filename(filename), file_size(std::filesystem::file_size(filename)),
      if_stream(filename, std::ifstream::binary),
      managed_stream(if_stream, static_cast<uint64_t>(file_size)) {
  if (file_size < 20) {
    //the file should have at least the file type box
    throw JPLFileFromStreamExceptions::InvalidTooSmallFileException(file_size);
  }
  // temp_signature = parser.parse<JpegPlenoSignatureBox>(
  //     managed_stream.get_remaining_sub_managed_stream());

  temp_signature = parser.parse<JpegPlenoSignatureBox, false>(
      managed_stream
          .get_remaining_sub_managed_stream());  //optional, may be nullptr
  if (temp_signature) {
    //if a JPEG Pleno Signature box is found it counts...
    ++decoded_boxes;
  }

  //parser parse until

  temp_file_type = decode_boxes_until_a_file_type_box_is_found();
  // parser.parse<FileTypeBox>(
  //     managed_stream.get_remaining_sub_managed_stream());
}


JPLFileParser::~JPLFileParser() {
  //! \todo check why it is needed to release the remaining ptrs in the map
  for (auto& something : temp_decoded_boxes) {
    for (auto& pair : something.second) {
      auto& ptr = std::get<1>(pair);
      ptr.release();
    }
  }
}
