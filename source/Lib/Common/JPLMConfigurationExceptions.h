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

/** \file     ParseExceptions.h
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-09-26
 */
#ifndef JPLM_PARSEEXCEPTIONS_H
#define JPLM_PARSEEXCEPTIONS_H

#include <exception>
#include <string>
#include <type_traits>
#include "magic_enum.hpp"

using namespace std;

class NotImplementedYetInputTypeParseException : public std::exception {
 private:
  string msg;

 public:
  NotImplementedYetInputTypeParseException(string m)
      : msg("Option " + m + " is not implemented yet.") {
  }

  const char* what() const throw() {
    return msg.c_str();
  }
};

class InconsistentOptionsException : public exception {
  const char* what() const throw() {
    return "Inconsistent Options.";
  }
};


class NotImplementedYetPartException : public exception {
  const char* what() const throw() {
    return "Chosen part not implemented yet.";
  }
};

class NotImplementedYetModeException : public exception {
  const char* what() const throw() {
    return "Mode not implemented yet.";
  }
};

class ConfigFileDoesNotExistException : public std::exception {
private:
  string msg;

public:
  ConfigFileDoesNotExistException(string m)
      : msg("Config file " + m + " does not exists.") {
  }

  const char* what() const throw() {
    return msg.c_str();
  }
};

#endif  //JPLM_PARSEEXCEPTIONS_H
