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
#include <algorithm>
#include <any>
#include <iostream>
#include <string>
#include "CLI/CLI.hpp"
#include "JPLMConfigurationExceptions.h"
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
  const bool &is_help_mode() const;

 protected:
  struct CLIArgument {
   public:
    CLIArgument(const std::string &longOption, const std::string &short_option,
        const std::string &description,
        const std::function<void(std::any)> &action)
        : long_option(longOption), short_option(short_option),
          description(description), action(action) {
      this->parsed = false;
    }

    void parse(std::string key, std::any value) {
      if (!this->parsed && (key == long_option || key == short_option)) {
        action(value);
        this->parsed = true;
      }
    }

   public:
    const std::string &getLongOption() const {
      return long_option;
    }

    const std::string &getShortOption() const {
      return short_option;
    }

    const std::string &getDescription() const {
      return description;
    }

   private:
    std::string long_option;
    std::string short_option;
    std::string description;
    bool parsed;
    std::function<void(std::any)> action;
  };

 protected:
  std::vector<CLIArgument> arguments;
  std::string input;
  std::string output;
  void run_help();
  void parse_cli(int argc, char **argv);
  bool validate_param(std::string param);
  bool validate_value(unsigned int size, unsigned int pos, char **argv);

private:
  bool help_mode_flag = false;
  std::string executable_name;
};

const std::string &JPLMConfiguration::get_input_filename() const {
  return input;
}

const std::string &JPLMConfiguration::get_output_filename() const {
  return output;
}

JPLMConfiguration::JPLMConfiguration(int argc, char **argv) {
  add_option_to_holder();
  parse_cli(argc, argv);
}

void JPLMConfiguration::add_option_to_holder(void) {
  holder.add_option({{"input"}, {[this](auto v) { this->input = v; }}, 'i'})
      .set_synopsys(
          "Input (If Part II, it is a directory containing a set of "
          "uncompressed "
          "light-field images xxx_yyy.ppm).");
  holder.add_option({{"output"}, {[this](auto v) { this->output = v; }}, 'o'})
      .set_synopsys("Output compressed bitstream");
}

void JPLMConfiguration::parse_cli(int argc, char **argv) {
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
