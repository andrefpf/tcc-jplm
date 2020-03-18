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

/** \file     CommonExceptions.h
 *  \brief    Class that contains all exceptions that may be thrown by Part1/Decoder lib
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-03-18
 */


#ifndef JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__
#define JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__

#include <exception>
#include <iostream>
#include <string>

namespace FileOrganizationExceptions {


class ValueNotAllowedException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "This value is not allowed by the expecification";
  }
};


class ThumbnailShallBeSignalledBeforePlenopticDataException
    : public std::exception {
 protected:
  std::string message_;

 public:
  ThumbnailShallBeSignalledBeforePlenopticDataException(
      const uint64_t thumbnail_box_position,
      const uint64_t fist_plenoptic_box_position)
      : message_(std::string("The JPEG Pleno Thumbnail box shall be signalled "
                             "before the JPEG Pleno Light Field, JPEG Pleno "
                             "Point Cloud, and JPEG Pleno Hologram superboxes. "
                             "Thumbnail box position in the codestream is ") +
                 std::to_string(thumbnail_box_position) +
                 std::string(". The first plenoptic box position is ") +
                 std::to_string(fist_plenoptic_box_position)) {
  }
  const char* what() const noexcept override {
    return message_.c_str();
  }
};


class
    ACatalogingXLMBoxShallBeSignalledAfterFileTypeBoxAndBeforePlenopticDataException
    : public std::exception {
 protected:
  std::string message_;

 public:
  ACatalogingXLMBoxShallBeSignalledAfterFileTypeBoxAndBeforePlenopticDataException(
      const uint64_t xml_box_position, const uint64_t file_type_box_position,
      const uint64_t fist_plenoptic_box_position)
      : message_(std::string("An XML box containing catalog information should "
                             "be signalled after the File Type box and before "
                             "the first superbox containing plenoptic data."
                             "XML box position in the codestream is ") +
                 std::to_string(xml_box_position) +
                 ". File type box position in the codestream is " +
                 std::to_string(file_type_box_position) +
                 std::string(". The first plenoptic box position is ") +
                 std::to_string(fist_plenoptic_box_position)) {
  }
  const char* what() const noexcept override {
    return message_.c_str();
  }
};

}  // namespace FileOrganizationExceptions


#endif /* end of include guard: JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__ */