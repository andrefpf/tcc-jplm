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

/** \file     JPLMDecoderConfiguration.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-02-10
 */

#include "JPLMDecoderConfiguration.h"

/**
 * @brief      Constructs a new instance. (protected)
 *
 * @param[in]  argc   The count of arguments
 * @param      argv   The arguments array
 * @param[in]  level  The level
 */
JPLMDecoderConfiguration::JPLMDecoderConfiguration(
    int argc, char **argv, std::size_t level)
    : JPLMConfiguration(argc, argv, level) {
  this->message =
      "JPLM Decoder\nUsage: " + this->executable_name + " [OPTIONS]\nOptions: ";
}

/**
 * @brief      Constructs a new instance. (public)
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 */
JPLMDecoderConfiguration::JPLMDecoderConfiguration(int argc, char **argv)
    : JPLMDecoderConfiguration(
          argc, argv, JPLMDecoderConfiguration::current_hierarchy_level) {
  this->init(argc, argv);
}

void JPLMDecoderConfiguration::add_options() {
  JPLMConfiguration::add_options();
  cli_options.push_back({"--input", "-i",
      "Input, i.e., a JPEG Pleno bitstream file (filename.jpl) to be "
      "decoded.",
      [this]([[maybe_unused]] std::any v) {
        this->input = std::any_cast<std::string>(v);
        //the hiearchy level must be tested if derivations of JPLMDecoderConfigurations are created.
        std::string input_filename_extension(
            std::filesystem::path(this->input).extension().string());
        std::transform(input_filename_extension.begin(),
            input_filename_extension.end(), input_filename_extension.begin(),
            ::tolower);
        if (std::filesystem::path(input_filename_extension) !=
            std::filesystem::path(".jpl")) {
          std::cerr << "Warning: the recommended extension is .jpl: "
                    << std::endl;
          std::cerr
              << "\tWhen stored in traditional computer file systems, JPL "
                 "files should be given the file extension \".jpl\" (readers "
                 "should allow mixed case for the alphabetic characters)."
              << std::endl;
          std::cerr << "\tThe used extension was: " << input_filename_extension
                    << std::endl;
          std::cerr << "\tThe file may still contain a valid JPEG Pleno stream "
                       "and thus it will be parsed anyway."
                    << std::endl;
        }
        std::cout << "Input: " << this->input << std::endl;
      },
      this->current_hierarchy_level});
  cli_options.push_back({"--output", "-o",
      "Output directory containing the decoded plenoptic data",
      [this]([[maybe_unused]] std::any v) {
        this->output = std::any_cast<std::string>(v);
        std::cout << "Output: " << this->output << std::endl;
      },
      this->current_hierarchy_level});
}

//checking the file extension for showing a warning message
