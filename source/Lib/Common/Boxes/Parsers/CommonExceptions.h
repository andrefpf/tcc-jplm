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
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-15
 */

#ifndef JPLM_LIB_COMMON_BOXES_PARSERS_COMMONEXCEPTIONS_H__
#define JPLM_LIB_COMMON_BOXES_PARSERS_COMMONEXCEPTIONS_H__

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace BoxParserExceptions {


class WrongTBoxValueException : public std::exception {
 protected:
  std::string message;

 public:
  WrongTBoxValueException(
      const uint32_t readed_type, const uint32_t expected_type) {
    auto string_stream = std::stringstream();
    string_stream << "Error parsing box. Expected T_BOX value: 0x" << std::hex
                  << std::setfill('0') << std::setw(8) << expected_type
                  << ". Obtained T_BOX value: 0x" << readed_type << std::dec
                  << std::endl;
    message = string_stream.str();
  }

  const char* what() const noexcept override {
    return message.c_str();
  }
};


class ParserOfRequiredBoxIsNotImplemented : public std::exception {
 protected:
  std::string message;

 public:
  ParserOfRequiredBoxIsNotImplemented(const uint32_t expected_type) {
    auto string_stream = std::stringstream();
    string_stream
        << "Error parsing required box. Expected T_BOX to be parsed: 0x"
        << std::hex << std::setfill('0') << std::setw(8) << std::dec
        << " but there is no parser implemented (registered) for such a box "
           "type."
        << std::endl;
    message = string_stream.str();
  }

  const char* what() const noexcept override {
    return message.c_str();
  }
};

}  // namespace BoxParserExceptions


namespace JpegPlenoSignatureBoxParserExceptions {
class InvalidJpegPlenoSignatureBox : public std::exception {
 public:
  InvalidJpegPlenoSignatureBox() = default;

  const char* what() const noexcept override {
    return "Invalid content (D_BOX) in Jpeg Pleno signature box";
  }
};
}  // namespace JpegPlenoSignatureBoxParserExceptions


namespace JPLFileFromStreamExceptions {
class InvalidTooSmallFileException : public std::exception {
 protected:
  std::string message;

 public:
  InvalidTooSmallFileException(const uint64_t filesize)
      : message(std::string("Error, jpl file is too small (only ") +
                std::to_string(filesize) +
                std::string(" bytes). Expecting at least 20 bytes.")) {
  }


  const char* what() const noexcept override {
    return message.c_str();
  }
};


class JpegPlenoNotInCompatibilityListException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Jpeg Pleno was not found in compatibility list.";
  }
};


class MoreThanOneFileTypeBoxException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "This JPL file has more than one File Type Box. The standard "
           "defines that only one must be present in the bitstream.";
  }
};


}  // namespace JPLFileFromStreamExceptions

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_PARSERS_COMMONEXCEPTIONS_H__ */