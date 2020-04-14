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
 *  \date     2020-02-13
 */

#ifndef UTILS_STREAM_COMMONEXCEPTIONS_H__
#define UTILS_STREAM_COMMONEXCEPTIONS_H__

#include <exception>
#include <limits>
#include <string>

namespace ManagedStreamExceptions {
class OverflowOfInitialPositionException : public std::exception {
 protected:
  std::string message;

 public:
  OverflowOfInitialPositionException(uint64_t initial_position)
      : message(
            std::string("Overflow of initial position with value ") +
            std::to_string(initial_position) +
            std::string(". However, this class only supports a offset of ") +
            std::to_string(std::numeric_limits<int64_t>::max())) {
  }
  const char* what() const noexcept override {
    return message.c_str();
  }
};

class FinalPositioSmallerThanInitialException : public std::exception {
 protected:
  std::string message;

 public:
  FinalPositioSmallerThanInitialException(
      uint64_t initial_position, uint64_t final_position)
      : message(
            std::string("Final position is smaller than initial position ") +
            std::to_string(initial_position) + std::string(" > ") +
            std::to_string(final_position) +
            std::string(
                ". However, final position must be larger than initial.")) {
  }
  const char* what() const noexcept override {
    return message.c_str();
  }
};


class OutOfBoundsException : public std::exception {
 protected:
  std::string message;

 public:
  OutOfBoundsException(size_t n_bytes, uint64_t initial_position,
      uint64_t final_position, uint64_t current_position)
      : message(
            std::string("Out of bounfs exception. Tried to access ") +
            std::to_string(n_bytes) + std::string("byte(s) from position ") +
            std::to_string(current_position) +
            std::string(
                ". However, this object only is permited to access between  ") +
            std::to_string(initial_position) + std::string(" to ") +
            std::to_string(final_position)) {
  }
  const char* what() const noexcept override {
    return message.c_str();
  }
};


class InvalidIndexForSubManagedStreamException : public std::exception {
 protected:
  std::string message;

 public:
  InvalidIndexForSubManagedStreamException(uint64_t initial_position_sub,
      uint64_t final_position_sub, uint64_t initial_position_current,
      uint64_t final_position_current)
      : message(std::string("Invalid index for sub managed stream. Position "
                            "range available: [") +
                std::to_string(initial_position_current) + std::string(", ") +
                std::to_string(final_position_current) +
                std::string("[ . Tried: [") +
                std::to_string(initial_position_sub) + std::string(", ") +
                std::to_string(final_position_sub) + std::string("[")) {
  }
  const char* what() const noexcept override {
    return message.c_str();
  }
};


class SeekBeforeInitialPositionException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Trying to seek to a point before the initial position.";
  }
};


class SeekAfterFinalPositionException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Trying to seek to a point after the final position.";
  }
};


class UnknownSeekDirectionException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Unknown seek direction";
  }
};


class InvalidEqualInitialAndFinalPositionException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "ManagedStream Initial and Final position must be different in "
           "constructor";
  }
};


class TryingToAccessBeyondEOFException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "ManagedStream tried to access data after EOF";
  }
};


class ClosedStreamException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "ManagedStream needs an open stream in constructor";
  }
};


}  // namespace ManagedStreamExceptions

#endif /* end of include guard: UTILS_STREAM_COMMONEXCEPTIONS_H__ */