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

/** \file     JPLMDecoderConfiguration.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-11
 */

#ifndef JPLMDECODERCONFIGURATION_H__
#define JPLMDECODERCONFIGURATION_H__

#include "Lib/Common/JPLMConfiguration.h"

class JPLMDecoderConfiguration : public JPLMConfiguration {
 private:
  std::size_t current_hierarchy_level = 0;
  void add_interface() {
    arguments.push_back({"--input", "-i",
        "Input, i.e., a JPEG Pleno bitstream file (filename.jpl) to be "
        "decoded.",
        [this]([[maybe_unused]] std::any v) {
          this->input = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});
    arguments.push_back({"--output", "-o",
        "Output directory containing the decoded plenoptic data",
        [this]([[maybe_unused]] std::any v) {
          this->output = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});
  }

 protected:
  JPLMDecoderConfiguration(int argc, char **argv, std::size_t level)
      : JPLMConfiguration(argc, argv, level) {
    this->message =
        "JPLM Decoder\nUsage: jpl-decoder-bin"
        " [OPTIONS]\nOptions: ";
    add_interface();
  }

 public:
  JPLMDecoderConfiguration(int argc, char **argv)
      : JPLMDecoderConfiguration(argc, argv, 0) {
    run_help();
  }
  virtual ~JPLMDecoderConfiguration() = default;
};

#endif /* end of include guard: JPLMDECODERCONFIGURATION_H__ */