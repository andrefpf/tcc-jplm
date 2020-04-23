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
#include <magic_enum.hpp>
#include "CppConsoleTable/CppConsoleTable.hpp"
#include "Lib/Utils/BasicConfiguration/CLIAndJSONOption.h"
#include "Lib/Utils/BasicConfiguration/CommonExceptions.h"


class BasicConfiguration {
 private:
  bool help_mode_flag = false;
  bool verbose_flag = false;
  static constexpr std::size_t current_hierarchy_level = 0;
  char **arg_vector;

 protected:
  // std::vector<Options> options;
  std::vector<CLIOption> cli_options;
  std::vector<JSONOption> json_options;
  std::vector<CLIAndJSONOption> cli_json_options;

  std::size_t hierarchy_level =
      0;  //<! the hierarchy level used for printing help
  std::string executable_name = "undefined";
  std::string message = std::string("");

  /**
  * @brief      Adds options.
  */
  virtual void add_options();

  template<class E>
  std::string get_valid_enumerated_options_str(std::vector<E> hidden_options = {}) {
    // constexpr auto values = magic_enum::enum_names<E>();
    constexpr auto all_values = magic_enum::enum_values<E>();
    auto not_hidden_values = std::vector<E>();

    for(const auto& value: all_values) {
      if (std::find(hidden_options.begin(), hidden_options.end(), value) == hidden_options.end()) {
        not_hidden_values.push_back(value);
      }
    }
    auto values = std::vector<std::string>();
    std::transform(not_hidden_values.begin(), not_hidden_values.end(), std::back_inserter(values),
                   [](auto value) -> std::string { return std::string(magic_enum::enum_name(value)); });


    std::stringstream available_values_string_stream;
    for (const auto &enum_value : values) {
      if((enum_value == values.back()) && (values.size() > 1)) {
        available_values_string_stream << "and ";
      }
      available_values_string_stream << enum_value;
      auto enum_from_str = magic_enum::enum_cast<E>(enum_value);
      if(enum_from_str) {
        auto enum_int = magic_enum::enum_integer(*enum_from_str);  
        //the static cast is necessary to print the value as a number when the underlying type
        //of the enum is uint8_t or char      
        available_values_string_stream << " (" << static_cast<int>(enum_int) << ")";
      }      
      if(enum_value != values.back()) {
        available_values_string_stream << ", ";
      } else {
        available_values_string_stream << ".";
      }
    }
    return available_values_string_stream.str();
  }


  template<class E>
  E parse_enum_option_as(const std::string& option) {
    //first, tries to cast considering as string...
    auto casted_enum = magic_enum::enum_cast<E>(option);
    if (casted_enum) {
      return *casted_enum;
    }
    //if it fails, tries to cast from integer value
    try {
      auto value = std::stoi(option);  
      casted_enum = magic_enum::enum_cast<E>(value);
      if (casted_enum) {
        return *casted_enum;
      }
    } catch (...) { //if failed, do nothing
    }    

    throw BasicConfigurationExceptions::
        InvalidEnumeratedOptionException(option, get_valid_enumerated_options_str<E>());
  }


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
  void init(int argc, char **argv);
  BasicConfiguration(int argc, char **argv, std::size_t level);

 public:
  BasicConfiguration(int argc, char **argv);

  virtual ~BasicConfiguration() = default;

  bool is_help_mode() const;

  bool is_verbose() const;
};

#endif  // JPLM_LIB_UTILS_BASIC_CONFIGURATION_BASIC_CONFIGURATION_H