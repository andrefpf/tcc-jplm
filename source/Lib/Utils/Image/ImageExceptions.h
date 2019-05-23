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

/** \file     ImageExceptions.h
 *  \brief    Class that contains all exceptions that may be thrown by Image lib
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-20
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGEEXCEPTIONS_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGEEXCEPTIONS_H__

#include <exception>

namespace ImageChannelExceptions {
class InvalidSizeException : public std::exception {
  const char* what() const noexcept override {
    return "A image channel size (width, height, bpp) cannot be zero";
  }
};
class InvalidIndexWriteException : public std::exception {
  const char* what() const noexcept override {
    return "Trying to write in invalid index of image channel";
  }
};
class InvalidIndexReadException : public std::exception {
  const char* what() const noexcept override {
    return "Trying to read from invalid index of image channel";
  }
};
class InvalidBppException : public std::exception {
  const char* what() const noexcept override {
    return "The image channel internal representation does not support a value with the required bpp.";
  }
};
class InvalidValueException : public std::exception {
  const char* what() const noexcept override {
    return "The image channel internal representation does not support the required value.";
  }
};
}  // namespace ImageChannelExceptions

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGEEXCEPTIONS_H__ */