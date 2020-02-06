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
#include "CppConsoleTable/CppConsoleTable.hpp"


using ConsoleTable = samilton::ConsoleTable;
using sAlign = samilton::Alignment;
using json = nlohmann::json;


const std::string &JPLMConfiguration::get_input_filename() const {
  return input;
}

const std::string &JPLMConfiguration::get_output_filename() const {
  return output;
}

void JPLMConfiguration::run_help() {
  if (this->help_mode_flag) {
    std::cout << "JPLM Codec" << std::endl;
    std::cout << "Usage:" << this->executable_name << " [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    ConsoleTable table(1, 1, samilton::Alignment::centre);
    ConsoleTable::TableChars chars;
    chars.topDownSimple = '\0';
    chars.leftSeparation = '\0';
    chars.centreSeparation = '\0';
    chars.downLeft = '\0';
    chars.downRight = '\0';
    chars.leftRightSimple = '\0';
    chars.rightSeparation = '\0';
    chars.topLeft = '\0';
    chars.topRight = '\0';
    chars.topSeparation = '\0';
    chars.downSeparation = '\0';
    table.setTableChars(chars);
    unsigned int count = 0;
    for (auto o : this->arguments) {
      table[count][1](sAlign::right) =
          o.getShortOption() + "," + o.getLongOption();
      table[count][2](sAlign::left) = o.getDescription();
      count++;
    }
    std::cout << table << std::endl;
  }
}

JPLMConfiguration::JPLMConfiguration(int argc, char **argv) {
  arguments.push_back({"--help", "-h", "Print this help message and exit",
      [this, argv]([[maybe_unused]] std::any v) {
        this->help_mode_flag = true;
        this->executable_name = std::string(argv[0]);
      },
      this->hierarchy_level});

  arguments.push_back({"--input", "-i",
      "Input directory containing a set of uncompressed light-field images "
      "(xxx_yyy.ppm).",
      [this]([[maybe_unused]] std::any v) {
        this->input = std::any_cast<std::string>(v);
      },
      this->hierarchy_level});
  arguments.push_back({"--output", "-o",
      "Output directory containing temporary light-field data and the "
      "compressed bitstream.",
      [this]([[maybe_unused]] std::any v) {
        this->output = std::any_cast<std::string>(v);
      },
      this->hierarchy_level});
  this->parse_cli(argc, argv);
  run_help();
}

bool JPLMConfiguration::validate_param(std::string param) {
  const std::string prefix = "-";
  return !param.compare(0, prefix.size(), prefix);
}

bool JPLMConfiguration::validate_value(
    unsigned int size, unsigned int pos, [[maybe_unused]] char **argv) {
  return pos < size - 1;
}


void JPLMConfiguration::parse_cli(int argc, char **argv) {
  for (int n = 1; n < argc; n++) {
    std::string key(reinterpret_cast<char *>(argv[n]));
    if (validate_param(key)) {
      if (validate_value(argc, n, argv)) {
        std::string value(reinterpret_cast<char *>(argv[n + 1]));
        std::for_each(arguments.begin(), arguments.end(),
            [key, value](CLIArgument &s) { s.parse(key, value); });
      } else {
        std::string value("");
        std::for_each(arguments.begin(), arguments.end(),
            [key, value](CLIArgument &s) { s.parse(key, value); });
      }
    }
  }
}

const bool &JPLMConfiguration::is_help_mode() const {
  return help_mode_flag;
}
