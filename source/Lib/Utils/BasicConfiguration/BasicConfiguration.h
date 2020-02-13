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

/** \file     BasicConfiguration.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-02-11
 */

#ifndef JPLM_LIB_UTILS_BASIC_CONFIGURATION_BASIC_CONFIGURATION_H
#define JPLM_LIB_UTILS_BASIC_CONFIGURATION_BASIC_CONFIGURATION_H

#include <algorithm>
#include <any>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "CppConsoleTable/CppConsoleTable.hpp"
#include "Lib/Utils/BasicConfiguration/CLIAndJSONOption.h"


class BasicConfiguration {
 private:
  bool help_mode_flag = false;
  static constexpr std::size_t current_hierarchy_level = 0;
  void add_options_to_cli(char **argv);


 protected:
  // std::vector<Options> options;
  std::vector<CLIOption> cli_options;
  std::vector<JSONOption> json_options;
  std::vector<CLIAndJSONOption> cli_json_options;

  std::size_t hierarchy_level =
      0;  //<! the hierarchy level used for printing help
  std::string executable_name = "undefined";
  std::string message = std::string("");

  void run_help() const;
  void parse_cli(int argc, char **argv);
  void parse_json(const std::string &path);
  //<! \todo check if "validate_param" method name could be "is_param_valid"
  bool validate_param(std::string param);
  //<! \todo check if "validate_value" method name could be "is_value_valid"
  bool validate_value(unsigned int size, unsigned int pos, char **argv);
  void add_cli_option(const CLIOption &option);
  void add_json_option(const JSONOption &option);
  void add_cli_json_option(const CLIAndJSONOption &option);
  BasicConfiguration(int argc, char **argv, std::size_t level);

 public:
  BasicConfiguration(int argc, char **argv);

  virtual ~BasicConfiguration() = default;

  bool is_help_mode() const;
};

#endif  // JPLM_LIB_UTILS_BASIC_CONFIGURATION_BASIC_CONFIGURATION_H