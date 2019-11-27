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

/** \file     JPLMConfiguration.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-11-26
 */
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

/** \file     JPLMConfiguration.cpp
 *  \brief    Implements JPLMCOnfiguration
 *  \details
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-11-26
 */

#include "Lib/Common/JPLMConfiguration.h"

const std::string &JPLMConfiguration::get_input_filename() const {
  return input;
}

const std::string &JPLMConfiguration::get_output_filename() const {
  return output;
}

JPLMConfiguration::JPLMConfiguration(int argc, char **argv) {
  arguments.push_back(
      {"--help", "-h", "Print this help message and exit", [this](std::any v) {
         std::cout << "JPLM Codec" << std::endl;
         std::cout << "Usage:" << std::any_cast<std::string>(v) << " [OPTIONS]"
                   << std::endl;
         std::cout << "Options:" << std::endl;
         ConsoleTable table(1, 1, samilton::Alignment::centre);
         unsigned int count = 0;
         for (auto o : this->arguments) {
           table[count][1] = o.getShortOption() + "," + o.getLongOption();
           table[count][2] = o.getDescription();
           count++;
         }
         std::cout << table << std::endl;
         //exit(0);
       }});

  arguments.push_back({"--input", "-i",
      "Input directory containing a set of uncompressed light-field images "
      "(xxx_yyy.ppm).",
      [this](std::any v) { this->input = std::any_cast<std::string>(v); }});
  arguments.push_back({"--output", "-o",
      "Output directory containing temporary light-field data and the "
      "compressed bitstream.",
      [this](std::any v) { this->output = std::any_cast<std::string>(v); }});
  this->parse_cli(argc, argv);
}

bool JPLMConfiguration::validate_param(std::string param) {
  auto opt = [param](CLIArgument &s) {
    return ((s.getShortOption() == param) || (s.getLongOption() == param));
  };

  auto starts_with = [param](std::string prefix) {
    if (prefix.length() > 0 && param.length() > prefix.length()) {
      for (int i = 0; i < prefix.length(); ++i)
        if (param[i] != prefix[i])
          return false;
      return true;
    }
    return false;
  };

  if (starts_with("-")) {
    //if (std::none_of(arguments.begin(), arguments.end(), opt))
    //  throw UnknownCLIParameterException(param);
    return true;
  } else {
    return false;
  }
}

void JPLMConfiguration::parse_cli(int argc, char **argv) {
  for (int n = 1; n < argc; n++) {
    std::string key(reinterpret_cast<char *>(argv[n]));
    if (validate_param(key)) {
      std::string value(reinterpret_cast<char *>(argv[n + 1]));
      std::for_each(arguments.begin(), arguments.end(),
          [key, value](CLIArgument &s) { s.parse(key, value); });
    }
  }
}