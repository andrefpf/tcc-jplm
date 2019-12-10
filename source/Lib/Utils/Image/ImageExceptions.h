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
#include <string>

namespace ImageChannelExceptions {
class InvalidSizeException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "A image channel size (width, height, bpp) cannot be zero";
  }
};


class InvalidIndexWriteException : public std::exception {
 private:
  std::string message_;

 public:
  explicit InvalidIndexWriteException(
      const std::size_t i, const std::size_t j) {
    message_ = "Trying to write in invalid index of image channel at (" +
               std::to_string(i) + ", " + std::to_string(j) + ")";
  }

  explicit InvalidIndexWriteException() {
    message_ = "Trying to write in invalid index of image channel";
  }
  virtual const char* what() const throw() {
    return message_.c_str();
  }
};


class InvalidIndexReadException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Trying to read from invalid index of image channel";
  }
};


class InvalidBppException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The image channel internal representation does not support a value "
           "with the required bpp.";
  }
};


class InvalidValueException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "The image channel internal representation does not support the "
           "required value.";
  }
};

}  // namespace ImageChannelExceptions

namespace ImageIOExceptions {
class InexistentPathException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Inexistent path.";
  }
};


class FileAlreadyExistsException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "File already exists and I will not overwrite it.";
  }
};


class InexistentFileException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Inexistent file.";
  }
};


class NotARegularFileException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Not a regular file.";
  }
};


class UnknownImageTypeException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Unknown image type";
  }
};


class PermisionToReadDeniedException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Permission to read from file was denied.";
  }
};

class UnableToOpenFileException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Unable to open file.";
  }
};
}  // namespace ImageIOExceptions


namespace PixelMapFileIOExceptions {
class InvalidPixelMapIndexException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Permission to read from file was denied.";
  }
};


class IncompletePixelMapFileException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Permission to read from file was denied.";
  }
};


class NoImplementedYetPixelMapFileException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Permission to read from file was denied.";
  }
};


class BppMustBeLargerThanZeroException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Pix Map file (PPM, PGM or PNM) bpp must be larger than zero.";
  }
};


class BppMustBeSmallerOrEqualToSixteenException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Pix Map file (PPM, PGM or PNM) bpp must be <= 16.";
  }
};


class WidthMustBeLargerThanZeroException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Pix Map file (PPM, PGM or PNM) width must be larger than zero.";
  }
};


class HeightMustBeLargerThanZeroException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Pix Map file (PPM, PGM or PNM) height must be larger than zero.";
  }
};


}  // namespace PixelMapFileIOExceptions


namespace MetricsExceptions {

class DifferentBppImagesException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Both images must have the same bpp for computing the metric.";
  }
};


class DifferentHeightImagesException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Both images must have the same height for computing the metric.";
  }
};


class DifferentWidthImagesException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Both images must have the same width for computing the metric.";
  }
};


class DifferentSizeImagesException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Both images must have the same size for computing the metric.";
  }
};


class DifferentColorSpaceImagesException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Both images must be at the same color space to enable computing "
           "the metric.";
  }
};


}  // namespace MetricsExceptions

namespace ImageUtilsExceptions {
class ContainerHasFewerBitsThanNeededException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Trying to fit image in container with fewer bits than the images "
           "original bit depth";
  }
};


class NegativeValueInUnsignedVariableException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Trying to fit image with negative values in image with unsigned "
           "variable representation";
  }
};


class OverflowException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Trying to insert value in variable type with fewer bits than the "
           "needed to keep such value. (Value > Max)";
  }
};

}  // namespace ImageUtilsExceptions


namespace PGXFileExceptions {
class InvalidIdException : public std::exception {
 private:
  std::string message_;

 public:
  explicit InvalidIdException(const std::string obtained_id) {
    message_ = "Obtained an invalid ID: " + obtained_id + ". Expecting PG";
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }
};


class InvalidEndianessException : public std::exception {
 private:
  std::string message_;

 public:
  explicit InvalidEndianessException(const std::string obtained_endianess) {
    message_ = "Obtained an invalid Endianess: " + obtained_endianess +
               ". Expecting ML (Big endian) or LM (Little endian)";
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }
};


}  // namespace PGXFileExceptions

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGEEXCEPTIONS_H__ */