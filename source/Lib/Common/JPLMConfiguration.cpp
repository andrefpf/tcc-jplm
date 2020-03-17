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

/** \file     JPLMConfiguration.cpp
 *  \brief    Implements JPLMCOnfiguration
 *  \details
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-11-26
 */

#include "Lib/Common/JPLMConfiguration.h"


void JPLMConfiguration::add_options() {
  BasicConfiguration::add_options();

  auto help_message = std::string(
      "Shows the total elapsed time in seconds (wall time) measured by using "
      "std::chronos. Notice that the actual execution time may be different.");
#ifdef __unix__
  help_message +=
      " If UNIX, also includes user time (ms) and max memory usage (kbytes).";
#endif
  help_message +=
      " This is the last report that will be shown after the execution.";

  this->add_cli_json_option({"--show-runtime-statistics", "-time", help_message,
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        if (conf.contains("show-runtime-statistics")) {
          return conf["show-runtime-statistics"].get<std::string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        if (arg == "false") {
          this->show_runtime_statistics_flag = false;
        } else {
          this->show_runtime_statistics_flag = true;
        }
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "false"; }}});

  this->add_cli_json_option({"--show-progress-bar", "-progress",
      "Enables the display of a progress bar showing the percentage of "
      "completion, run time and expected finishing time.",
      [this](const nlohmann::json &conf) -> std::optional<std::string> {
        if (conf.contains("show-progress-bar")) {
          return conf["show-progress-bar"].get<std::string>();
        }
        return std::nullopt;
      },
      [this](std::string arg) {
        if (arg == "false") {
          this->show_progress_bar_flag = false;
        } else {
          this->show_progress_bar_flag = true;
        }
      },
      this->current_hierarchy_level,
      {[this]() -> std::string { return "false"; }}});
}


const std::string &JPLMConfiguration::get_input_filename() const {
  return input;
}


const std::string &JPLMConfiguration::get_output_filename() const {
  return output;
}


JPLMConfiguration::JPLMConfiguration(int argc, char **argv)
    : JPLMConfiguration(
          argc, argv, JPLMConfiguration::current_hierarchy_level) {
  this->init(argc, argv);
}


JPLMConfiguration::JPLMConfiguration(int argc, char **argv, std::size_t level)
    : BasicConfiguration(argc, argv, level) {
}
