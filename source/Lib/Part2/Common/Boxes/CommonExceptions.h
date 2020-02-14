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
 *  \date     2019-09-17
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_COMMONEXCEPTIONS_H__
#define JPLM_LIB_PART2_COMMON_BOXES_COMMONEXCEPTIONS_H__

#include <exception>
#include <string>
#include <tuple>


namespace CameraParameterBoxExceptions {
class InvalidCameraParameterArrayVectorSizeException : public std::exception {
 protected:
  std::string message;

 public:
  InvalidCameraParameterArrayVectorSizeException(
      const std::size_t expected_size, const std::size_t obtained_size)
      : message(std::string("Camera Parameter array was expecting") +
                std::to_string(expected_size) +
                std::string(" camera parameters. However, only ") +
                std::to_string(obtained_size) +
                std::string(" were available. ")) {
  }


  const char* what() const noexcept override {
    return message.c_str();
  }
};


class MissingCompleteInitializationException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Trying to access data from a partially initialized Camera "
           "Parameter Box. To get the desired data it is necessary to provide "
           "light field data throught initialize_missing_row_and_column "
           "method.";
  }
};


class InvalidLazzyInitializationException : public std::exception {
 protected:
  std::string message;

 public:
  InvalidLazzyInitializationException(
      const uint32_t rows, const uint32_t columns, const std::size_t n_views)
      : message(std::string("Camera Parameters array was partially initialized "
                            "considering a total of ") +
                std::to_string(n_views) +
                std::string(" views. However, the missing rows and columns "
                            "were initialized as ") +
                std::to_string(rows) + std::string(" (rows) and ") +
                std::to_string(columns) +
                std::string(" (columns). This gives a total of ") +
                std::to_string(rows * columns) + std::string(" views.")) {
  }


  const char* what() const noexcept override {
    return message.c_str();
  }
};


class InvalidCoordinateException : public std::exception {
 protected:
  std::string message;

 public:
  InvalidCoordinateException(const uint32_t rows, const uint32_t columns,
      const std::tuple<uint32_t, uint16_t>& position)
      : message(std::string("Invalid position exception.") +
                std::string("Trying to access position (") +
                std::to_string(std::get<0>(position)) +
                std::to_string(std::get<1>(position)) +
                std::string("). However, there are only ") +
                std::to_string(rows) + std::string(" rows and ") +
                std::to_string(columns) + std::string(" columns available.")) {
  }


  const char* what() const noexcept override {
    return message.c_str();
  }
};
}  // namespace CameraParameterBoxExceptions


namespace JpegPlenoLightFieldBoxExceptions {
class UninitializedContigousCodestreamException : public std::exception {
 protected:
  std::string message;

 public:
  UninitializedContigousCodestreamException()
      : message("Trying to access an uninitialized contigous codestream") {
  }

  const char* what() const noexcept override {
    return message.c_str();
  }
};

}  // namespace JpegPlenoLightFieldBoxExceptions

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_COMMONEXCEPTIONS_H__ */