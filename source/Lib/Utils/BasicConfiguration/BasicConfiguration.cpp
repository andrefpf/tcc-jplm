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

/** \file     BasicConfiguration.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-02-11
 */

#include "BasicConfiguration.h"

using ConsoleTable = samilton::ConsoleTable;
using sAlign = samilton::Alignment;

void BasicConfiguration::add_options_to_cli(char **argv) {
  cli_options.push_back({"--help", "-h", "Print this help message and exit",
      [this, argv]([[maybe_unused]] std::any v) {
        this->help_mode_flag = true;
        this->executable_name = std::string(argv[0]);
      },
      this->current_hierarchy_level});
}


void BasicConfiguration::run_help() const {
  if (this->help_mode_flag) {
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
    for (const auto &cli_option : this->cli_options) {
      if (cli_option.get_level() == this->hierarchy_level) {
        table[count][1](sAlign::right) =
            cli_option.get_short_option() + "," + cli_option.get_long_option();
        table[count][2](sAlign::left) = cli_option.get_description();
        ++count;
      }
    }
    if (count != 0) {
      std::cout << message << std::endl;
      std::cout << table << std::endl;
    }
  }
}


bool BasicConfiguration::validate_param(std::string param) {
  const std::string prefix = "-";
  return !param.compare(0, prefix.size(), prefix);
}


bool BasicConfiguration::validate_value(
    unsigned int size, unsigned int pos, [[maybe_unused]] char **argv) {
  return pos < size - 1;
}


void BasicConfiguration::parse_cli(int argc, char **argv) {
  for (int n = 1; n < argc; n++) {
    std::string key(reinterpret_cast<char *>(argv[n]));
    if (validate_param(key)) {
      if (validate_value(argc, n, argv)) {
        std::string value(reinterpret_cast<char *>(argv[n + 1]));
        std::for_each(cli_options.begin(), cli_options.end(),
            [key, value](auto &s) { s.parse(key, value); });
      } else {
        std::string value("");
        std::for_each(cli_options.begin(), cli_options.end(),
            [key, value](auto &s) { s.parse(key, value); });
      }
    }
  }
}


bool BasicConfiguration::is_help_mode() const {
  return help_mode_flag;
}


BasicConfiguration::BasicConfiguration(int argc, char **argv, std::size_t level)
    : hierarchy_level(level) {
  add_options_to_cli(argv);
  this->parse_cli(argc, argv);
}