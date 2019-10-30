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
#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "nlohmann/json.hpp"
#include "CppConsoleTable/CppConsoleTable.hpp"

using ConsoleTable = samilton::ConsoleTable;
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

   private:
    std::string long_option;
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
    std::string short_option;
    std::string description;
    bool parsed;
    std::function<void(std::any)> action;
  };

 protected:
  std::vector<CLIArgument> arguments;

 protected:
  std::string input;
  std::string output;
  void parse_cli(int argc, char **argv);
};

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
         std::cout << "Usage:" << std::any_cast<std::string>(v) << " [OPTIONS]" << std::endl;
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

void JPLMConfiguration::parse_cli(int argc, char **argv) {
  for (int n = 1; n < argc - 1; n++) {
    std::string key = argv[n];
    std::string value = argv[n + 1];
    std::for_each(arguments.begin(), arguments.end(),
        [key, value](CLIArgument &s) { std::cout << "IGUAL=" << key << ", " << value << std::endl; s.parse(key, value); });
  }
}

#endif  //JPLM_JPLMConfiguration_H
