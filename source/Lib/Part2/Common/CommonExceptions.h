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
 *  \brief    Class that contains all exceptions that may be thrown by Image lib
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-04
 */

#ifndef JPLM_LIB_PART2_COMMON_COMMONEXCEPTIONS_H__
#define JPLM_LIB_PART2_COMMON_COMMONEXCEPTIONS_H__

#include <exception>

namespace LightfieldCoordinateExceptions {


class UnsignedUnderflowException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "This operation generated an underflow";
  }
};


class OverflowException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "This operation generated an overflow";
  }
};


}  // namespace LightfieldCoordinateExceptions


namespace LightfieldDimensionExceptions {


class InvalidZeroDimensionException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "A dimension must be a positive number different than zero.";
  }
};

}  // namespace LightfieldDimensionExceptions


namespace ViewExceptions {

class InvalidZeroWidthException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The view width was set to zero but it cannot be zero";
  }
};


class InvalidZeroHeightException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The view height was set to zero but it cannot be zero";
  }
};


class InvalidZeroBppException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The bpp was set to zero but it cannot be zero";
  }
};


class ImageWasNotInitialyzedException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The image contained in this view was not initialized.";
  }
};


class InvalidNumberOfChannelsException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The image contained in this view has a invalid number of channels.";
  }
};

}  // namespace ViewExceptions


namespace LightfieldIOConfigurationExceptions {
class InvalidLightfieldPath : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The lightfield must be a path";
  }
};
}  // namespace LightfieldIOConfigurationExceptions


namespace ViewToFilenameTranslatorExceptions {
class Char3OverflowException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The required view position is larger than 999, which is the "
           "maximum supported in this representation";
  }
};
}  // namespace ViewToFilenameTranslatorExceptions

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_COMMONEXCEPTIONS_H__ */