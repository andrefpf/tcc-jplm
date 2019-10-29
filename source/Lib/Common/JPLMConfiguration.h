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

/** \file     JPLMConfiguration.h
 *  \brief    General configuration data object
 *  \details  JPLMConfiguration is the most basic abstraction of configuration
 *            data object for being used by the JPLM Encoder and Decoders. It
 *            contains only the input and output paths extracted from the
 *            command line interface. All configuration objects are derived
 *            from JPLMConfiguration.
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-09-26
 */
#ifndef JPLM_JPLMConfiguration_H
#define JPLM_JPLMConfiguration_H

#include <getopt.h>
#include <iostream>
#include <string>
#include "CLI/CLI.hpp"
#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

enum class JpegPlenoPart {
  LightField = 2,
};

class JPLMConfiguration {
 public:
  JPLMConfiguration();

  JPLMConfiguration(int argc, char **argv);

  const std::string &get_input_filename() const;

  const std::string &get_output_filename() const;

 protected:
  class Option {
   private:
    static int option_counter;

   public:
    std::vector<std::string> names;
    char short_name;
    int option_number;
    std::function<void(const char *)> handler_char = nullptr;
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
        std::function<void(const char *)> option_handler, char short_option = 0,
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

    void handle(const char *param) {
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
    struct option *options_struct = nullptr;

   public:
    OptionsHolder() = default;

    ~OptionsHolder(){};  //delete options struct

    Option &add_option(Option option) {
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

    Option &get_option_by_name(std::string name) {
      auto option_iterator = options_name_map.find(name);
      if (option_iterator != options_name_map.end()) {
        return options.find(option_iterator->second)->second;
      }
      return unknown_option;
    }

    Option &get_option_by_id(int option_id) {
      auto option_iterator = options.find(option_id);
      if (option_iterator != options.end()) {
        return option_iterator->second;
      }
      return unknown_option;
    }

    struct option *generate_options_struct(bool encoder) {
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

 protected:
  std::string input;
  std::string output;
  OptionsHolder holder;
  void parse_cli(int argc, char **argv);
};

const std::string &JPLMConfiguration::get_input_filename() const {
  return input;
}

const std::string &JPLMConfiguration::get_output_filename() const {
  return output;
}

JPLMConfiguration::JPLMConfiguration(int argc, char **argv) {
  parse_cli(argc, argv);
}

void JPLMConfiguration::parse_cli(int argc, char **argv) {
  holder.add_option({{"input"}, {[this](auto v) { this->input = v; }}, 'i'})
      .set_synopsys(
          "Input (If Part II, it is a directory containing a set of "
          "uncompressed "
          "light-field images xxx_yyy.ppm).");
  holder.add_option({{"output"}, {[this](auto v) { this->output = v; }}, 'o'})
      .set_synopsys("Output compressed bitstream");

  bool is_encoder = true;
  auto short_options = holder.generate_short_options(is_encoder);
  const struct option *optionas = holder.generate_options_struct(is_encoder);

  int opt;
  while ((opt = getopt_long_only(
              argc, argv, short_options.c_str(), optionas, NULL)) > 0) {
    if (opt == ':') {
      std::cerr << "option " << holder.get_option_by_id(optopt).get_names()[0]
                << " expected argument" << std::endl;

      continue;
    }
    if (opt == '?') {
      std::cerr << "Unknown option: " << std::string(argv[optind - 1])
                << std::endl;
      continue;
    }

    auto obtained_option = holder.get_option_by_id(opt);
    if (obtained_option.has_argument) {
      obtained_option.handle(optarg);
    } else {
      obtained_option.handle();
    }
  }
}

// PRIVATE CLASSES
int JPLMConfiguration::Option::option_counter = 256;

#endif  //JPLM_JPLMConfiguration_H
