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

/** \file     JSONOption.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-02-12
 */

#ifndef JPLM_LIB_UTILS_BASIC_CONFIGURATION_JSON_OPTION_H
#define JPLM_LIB_UTILS_BASIC_CONFIGURATION_JSON_OPTION_H

#include "Lib/Utils/BasicConfiguration/Option.h"
#include "nlohmann/json.hpp"

class JSONOption : public virtual Option {
 protected:
  std::function<std::optional<std::any>(const nlohmann::json &json)>
      parse_action;

 public:
  JSONOption(const std::string &description,
      const std::function<std::optional<std::any>(const nlohmann::json &json)>
          &parse_action,
      const std::function<void(std::any)> &action, std::size_t level,
      const DefaultParameter &default_parameter = DefaultParameter())
      : Option(description, action, level, default_parameter),
        parse_action(parse_action) {
  }


  void parse(const nlohmann::json &json) {
    auto parsed = parse_action(json);
    if (parsed) {
      this->run_action(*parsed);
    }
  }


  virtual ~JSONOption() = default;
};

#endif  // JPLM_LIB_UTILS_BASIC_CONFIGURATION_JSON_OPTION_H