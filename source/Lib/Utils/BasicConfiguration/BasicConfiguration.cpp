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


/**
 * @brief      Adds the basic options.
 */
void BasicConfiguration::add_options() {
  this->add_cli_option({"--help", "-h", "Print this help message and exit",
      [this]([[maybe_unused]] std::any v) {
        this->help_mode_flag = true;
        this->executable_name = std::string(this->arg_vector[0]);
      },
      this->current_hierarchy_level});

  this->add_cli_option({"--config", "-c", "Path to configuration file in JSON.",
      [this](std::string arg) {
        if (!arg.empty()) {
          if (std::filesystem::exists(arg)) {
            parse_json(arg);
          } else {
            throw BasicConfigurationExceptions::ConfigFileDoesNotExistException(
                arg);
          }
        }
      },
      this->current_hierarchy_level});
}


/**
 * @brief      Gets the console table already initialized (formated).
 *
 * @return     The console table.
 */
ConsoleTable get_console_table() {
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
  return table;
}


/**
 * @brief      Shows the help table.
 *
 * @param[in]  message  The message
 * @param      table    The table
 */
void show_help(const std::string &message, ConsoleTable &table) {
  std::cout << message << std::endl;
  std::cout << table << std::endl;
}


/**
 * @brief      Runs the help if help flag is set
 */
void BasicConfiguration::run_help() const {
  if (this->help_mode_flag) {
    auto table = get_console_table();
    unsigned int count = 0;
    for (const auto &option : this->cli_options) {
      if (option.get_level() == this->hierarchy_level) {
        table[count][1](samilton::Alignment::right) =
            option.get_short_option() + "," + option.get_long_option();
        table[count][2](samilton::Alignment::left) = option.get_description();
        ++count;
      }
    }
    for (const auto &option : this->cli_json_options) {
      if (option.get_level() == this->hierarchy_level) {
        table[count][1](samilton::Alignment::right) =
            option.get_short_option() + "," + option.get_long_option();
        table[count][2](samilton::Alignment::left) = option.get_description();
        ++count;
      }
    }
    if (count != 0) {
      show_help(message, table);
    }
  }
}


/**
 * @brief      Validades a parameter (option)
 *
 * @param[in]  param  The parameter
 *
 * @return     { description_of_the_return_value }
 */
bool BasicConfiguration::validate_param(std::string param) {
  const std::string prefix = "-";
  return !param.compare(0, prefix.size(), prefix);
}


/**
 * @brief      Validades a value (argument)
 *
 * @param[in]  size  The size
 * @param[in]  pos   The position
 * @param      argv  The arguments array
 *
 *  \todo check if this function is still needed
 *
 * @return     True if a value is valid, false otherwise
 */
bool BasicConfiguration::validate_value(
    unsigned int size, unsigned int pos, [[maybe_unused]] char **argv) {
  return pos < size - 1;
}


/**
 * @brief      Parses the arguments in cli
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 */
void BasicConfiguration::parse_cli(int argc, char **argv) {
  for (int n = 1; n < argc; n++) {
    std::string key(reinterpret_cast<char *>(argv[n]));
    if (validate_param(key)) {
      std::string value("");
      if (validate_value(argc, n, argv)) {
        value = std::string(reinterpret_cast<char *>(argv[n + 1]));
      }
      for (auto &option : cli_options) {
        option.parse(key, value);
      }
      for (auto &option : cli_json_options) {
        option.CLIOption::parse(key, value);
      }
    }
  }
  for (auto &option : cli_options) {
    if (!option.is_parsed()) {
      option.run_default_action();
    }
  }
  for (auto &option : cli_json_options) {
    if (!option.is_parsed()) {
      option.run_default_action();
    }
  }
}


/**
 * @brief      Determines if help mode.
 *
 * @return     True if help mode, False otherwise.
 */
bool BasicConfiguration::is_help_mode() const {
  return help_mode_flag;
}


/**
 * @brief      Constructs a new instance. (protected)
 *
 * @param[in]  argc   The count of arguments
 * @param      argv   The arguments array
 * @param[in]  level  The level
 * 
 * \todo remove argc from this protected constructor
 */
BasicConfiguration::BasicConfiguration(
    [[maybe_unused]] int argc, char **argv, std::size_t level)
    : arg_vector(argv), hierarchy_level(level) {
}


/**
 * @brief      Initializes the object by adding all options and parsing cli
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 */
void BasicConfiguration::init(int argc, char **argv) {
  this->add_options();
  this->parse_cli(argc, argv);
  this->run_help();
}


/**
 * @brief      Constructs a new instance.
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 * 
 * \todo Add a tutorial to show that it is necessary to call all init in the public ctr
 */
BasicConfiguration::BasicConfiguration(int argc, char **argv)
    : BasicConfiguration(
          argc, argv, BasicConfiguration::current_hierarchy_level) {
  this->init(argc, argv);
}


/**
 * @brief      Parses the options present in the json on path
 *
 * @param[in]  path  The path
 */
void BasicConfiguration::parse_json(const std::string &path) {
  std::ifstream input_file_stream(path);
  nlohmann::json conf = nlohmann::json::parse(input_file_stream);

  for (auto &option : this->json_options) {
    option.parse(conf);
  }

  for (auto &option : this->cli_json_options) {
    option.JSONOption::parse(conf);
  }
}


/**
 * @brief      Adds a cli option to the list of options.
 *
 * @param[in]  option  The option
 */
void BasicConfiguration::add_cli_option(const CLIOption &option) {
  cli_options.push_back(option);
}


/**
 * @brief      Adds a json option to the list of options.
 *
 * @param[in]  option  The option
 */
void BasicConfiguration::add_json_option(const JSONOption &option) {
  json_options.push_back(option);
}


/**
 * @brief      Adds a cli/json option to the list of options.
 *
 * @param[in]  option  The option
 */
void BasicConfiguration::add_cli_json_option(const CLIAndJSONOption &option) {
  cli_json_options.push_back(option);
}