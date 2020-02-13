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
 *  \date     2020-02-03
 */


#ifndef JPLM_LIB_COMMON_COMMON_EXCEPTIONS_H
#define JPLM_LIB_COMMON_COMMON_EXCEPTIONS_H

#include <exception>
#include <stdexcept>
#include <string>


namespace JPLMCommonExceptions {

/**
 * @brief      Exception for signaling not implemented errors.
 */
class NotImplementedException : public std::logic_error {
 public:
  NotImplementedException(const std::string& what)
      : std::logic_error("Not implemented yet: " + what) {
  }
};

}  // namespace JPLMCommonExceptions


namespace JPLMConfigurationExceptions {


class UndefinedPartException : public std::exception {
 private:
  std::string msg =
      "Part was not specified. The encoder has no way to know which part of "
      "the standard is to be used.";

 public:
  UndefinedPartException() = default;

  const char* what() const throw() {
    return msg.c_str();
  }
};


class InvalidPartException : public std::exception {
 private:
  std::string msg = "The informed part is not a valid part for running JPLM.";

 public:
  InvalidPartException() = default;

  const char* what() const throw() {
    return msg.c_str();
  }
};


class NotImplementedYetInputTypeParseException : public std::exception {
 private:
  std::string msg;

 public:
  NotImplementedYetInputTypeParseException(std::string m)
      : msg("Option " + m + " is not implemented yet.") {
  }

  const char* what() const throw() {
    return msg.c_str();
  }
};


class InconsistentOptionsException : public std::exception {
  const char* what() const throw() {
    return "Inconsistent Options.";
  }
};


class NotImplementedYetPartException : public std::exception {
  const char* what() const throw() {
    return "Chosen part not implemented yet.";
  }
};


class NotImplementedYetModeException : public std::exception {
  const char* what() const throw() {
    return "Mode not implemented yet.";
  }
};


class UnsuportedPredictionMode : public std::exception {
  const char* what() const throw() {
    return "Prediction mode is currently not supported in this reference "
           "software.";
  }
};


class UnknownCLIParameterException : public std::exception {
 private:
  std::string msg;

 public:
  UnknownCLIParameterException(std::string m)
      : msg("CLI parameter " + m + " is not known.") {
  }

  const char* what() const throw() {
    return msg.c_str();
  }
};

}  // namespace JPLMConfigurationExceptions

#endif  // JPLM_LIB_COMMON_COMMON_EXCEPTIONS_H