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

/** \file     ParameterHandler.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef PARAMETERHANDLER_H__
#define PARAMETERHANDLER_H__

#include <getopt.h>
#include <string.h>
#include <algorithm>  //std::transform
#include <filesystem>
#include <fstream>  // std::ifstream
#include <functional>
#include <iostream>  // std::cin, std::cout
#include <locale>  //std::tolower
#include <map>
#include <memory>
#include "Lib/Utils/Image/ColorSpaces.h"
#include "Lib/Part2/Common/TransformMode/Block4D.h"

namespace fs = std::filesystem;

class Option {
 private:
  static int option_counter;

 public:
  std::vector<std::string> names;
  char short_name;
  int option_number;
  std::function<void(const char*)> handler_char = nullptr;
  std::function<void(void)> handler_void = nullptr;
  bool has_argument;
  bool available_to_encoder;
  bool available_to_decoder;
  std::string synopsys = "The synopsys for this option is not available yet";

  Option(std::vector<std::string> option_names, char short_option = 0,
      bool available_in_encoder = true, bool available_in_decoder = true)
      : names(option_names), short_name(short_option),
        available_to_encoder(available_in_encoder),
        available_to_decoder(available_in_decoder) {
    if (short_option == 0) {
      option_number = option_counter++;
    } else {
      option_number = static_cast<int>(short_option);
      names.push_back({char(short_option)});
    }
  };

  Option(std::vector<std::string> option_names,
      std::function<void(const char*)> option_handler, char short_option = 0,
      bool available_in_encoder = true, bool available_in_decoder = true)
      : Option(option_names, short_option, available_in_encoder,
            available_in_decoder) {
    handler_char = option_handler;
    has_argument = true;
  };


  Option(std::vector<std::string> option_names,
      std::function<void(void)> option_handler, char short_option = 0,
      bool available_in_encoder = true, bool available_in_decoder = true)
      : Option(option_names, short_option, available_in_encoder,
            available_in_decoder) {
    handler_void = option_handler;
    has_argument = false;
  };

  ~Option() = default;

  void print() const {
    if (short_name != 0) {
      std::cout << "\t-" << short_name << ", ";
    } else {
      std::cout << "\t";
    }
    for (auto option_name : names) {
      std::cout << "--" << option_name << ", \t";
    }
    std::cout << std::endl;
    std::cout << "\t\t" << synopsys << std::endl << std::endl;
  }

  void set_synopsys(std::string synopsys) {
    this->synopsys = synopsys;
  }

  std::vector<std::string> get_names() const {
    return names;
  }

  int get_number() const {
    return option_number;
  }

  void handle(const char* param) {
    if (handler_char != nullptr) {
      handler_char(param);
    } else {
      std::cerr << "There is no handler associated with the option."
                << std::endl;
    }
  }

  void handle() {
    if (handler_void != nullptr) {
      handler_void();
    } else {
      std::cerr << "There is no handler associated with the option."
                << std::endl;
    }
  }
};

class OptionsHolder {
 private:
  std::map<int, Option> options;
  std::map<std::string, int> options_name_map;
  Option unknown_option = Option({"Unknown Option"});
  // std::unique_ptr<struct option[]> options_struct = nullptr;
  struct option* options_struct = nullptr;

 public:
  OptionsHolder() = default;
  ~OptionsHolder(){};  //delete options struct

  Option& add_option(Option option) {
    auto list_of_names = option.get_names();
    auto result = options.insert(std::make_pair(option.get_number(), option));
    if (!result.second) {
      std::cerr << "The option " << option.names.at(0)
                << " was already in the list of options" << std::endl;
      exit(1);
    }
    for (auto name : list_of_names) {
      auto result =
          options_name_map.insert(std::make_pair(name, option.get_number()));
      if (!result.second) {
        std::cerr << "The name " << name
                  << " used for option already exists (i.e., there is a "
                     "duplicate name for options)."
                  << std::endl;
        exit(1);
      }
    }
    return options.find(option.get_number())->second;
  };

  void print_all_options() const {
    for (auto option : options) {
      option.second.print();
    }
  }

  Option& get_option_by_name(std::string name) {
    auto option_iterator = options_name_map.find(name);
    if (option_iterator != options_name_map.end()) {
      return options.find(option_iterator->second)->second;
    }
    return unknown_option;
  }

  Option& get_option_by_id(int option_id) {
    auto option_iterator = options.find(option_id);
    if (option_iterator != options.end()) {
      return option_iterator->second;
    }
    return unknown_option;
  }

  struct option* generate_options_struct(bool encoder) {
    auto number_of_options = options_name_map.size();
    // options_struct = std::unique_ptr<struct option[]>(new struct option[number_of_options+1]);
    options_struct = new struct option[number_of_options + 1];
    int counter = 0;
    for (auto option_idx : options_name_map) {
      auto option_iterator = options.find(option_idx.second);
      if (option_iterator == options.end()) {
        std::cerr << "Option not found..." << std::endl;
      } else {
        auto current_option = option_iterator->second;
        if ((encoder && current_option.available_to_encoder) ||
            (!encoder && current_option.available_to_decoder)) {
          auto name = new char[option_idx.first.size() + 1];
          strcpy(name, option_idx.first.c_str());
          options_struct[counter].name = name;
          options_struct[counter].has_arg =
              static_cast<int>(current_option.has_argument);
          options_struct[counter].flag = 0;
          options_struct[counter].val = current_option.get_number();
          counter++;
        }
      }
    }
    options_struct[counter] = {0, 0, 0, 0};
    return options_struct;
  }

  std::string generate_short_options(bool encoder) {
    std::string short_options(":");

    for (auto option : options) {
      auto current_option = option.second;
      if ((encoder && current_option.available_to_encoder) ||
          (!encoder && current_option.available_to_decoder)) {
        int option_number = current_option.get_number();
        if (option_number < 256) {
          short_options.push_back(char(option_number));
          if (current_option.has_argument) {
            short_options.push_back(':');
          }
        }
      }
    }

    return short_options;
  }
};

class ParameterHandler {
 private:
  OptionsHolder holder;

  void print_usage() const;
  void print_help(const char* program_name) const;

  void parse_configuration_file(const char* filename);
  void parse_color_transform(const char* name);
  void parse_extension_method(const char* arg);
  void parse_decoded_lightfield(const char* arg);
  void parse_encoded_lightfield(const char* arg);
  void parse_input(const char* arg);
  void parse_output(const char* arg);

  void instantiate_options();

 public:  //default parameters
  //lightfiled path
  fs::path encoded_lightfield;  //-lf //should be -i, as for input
  fs::path decoded_lightfield;  //-o
  //lightfield configuration
  bool is_lenslet_13x13 = false;  //-lenslet13x13
  int number_of_vertical_views = 13;  //-nv
  int number_of_horizontal_views = 13;  //-nh
  int first_horizontal_view_number = 0;  //-off_h
  int first_vertical_view_number = 0;  //-off_v

  //Color conversion
  ColorSpaces::ColorSpace color_transform = ColorSpaces::ColorSpace::BT601;
  // -bt601 -ycocg

  // Block extension method
  ExtensionMethod extension_method = ExtensionMethod::REPEAT_LAST;  // -e
  //-extension_repeat -extension_dct_ext -extension_dct_zero -extension_zero -extension_mean -extension_none -extension_cyclic

  //RD
  double lambda = 1024.0;  //-lambda

  //transform configuration:
  int transform_length_t =
      13;  //-t or -k for setting transform_length_t and transform_length_s
  int transform_length_s =
      13;  //-s or -k for setting transform_length_t and transform_length_s
  int transform_length_v =
      15;  //-v or -l for setting transform_length_v and transform_length_u
  int transform_length_u =
      15;  //-u or -l for setting transform_length_v and transform_length_u
  int min_transform_length_t = 4;  // -min_t
  int min_transform_length_s = 4;  // -min_s
  int min_transform_length_v = 4;  // -min_v
  int min_transform_length_u = 4;  // -min_u
  double transform_scale_t = 1.0;  //-t_scale
  double transform_scale_s = 1.0;  //-s_scale
  double transform_scale_v = 1.0;  //-v_scale
  double transform_scale_u = 1.0;  //-u_scale

  //other
  bool verbose = false;  //-v0

  //this is needed to decide about input and output, which depends on wheter we are in the encoder or decoder...
  bool is_encoder = true;

  ParameterHandler(bool handle_encoder) : is_encoder(handle_encoder) {
    instantiate_options();
  };
  ~ParameterHandler() = default;

  void parse(int argc, char* const argv[]);
  void print_configurations();
};

#endif /* end of include guard: PARAMETERHANDLER_H__ */