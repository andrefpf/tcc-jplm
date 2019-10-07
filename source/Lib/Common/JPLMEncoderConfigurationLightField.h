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

/** \file     JPLMEncoderConfigurationLightField.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-11
 */

#ifndef JPLMENCODERCONFIGURATIONLIGHTFIELD_H__
#define JPLMENCODERCONFIGURATIONLIGHTFIELD_H__

#include "Lib/Common/JPLMEncoderConfiguration.h"
#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Part2/Common/Lightfield.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"

//stub
class JPLMEncoderConfigurationLightField : public JPLMEncoderConfiguration {
 public:
  JPLMEncoderConfigurationLightField(int argc, char **argv);
  LightfieldIOConfiguration get_lightfield_io_configurations() const;
  const Type &get_type() const;
  const Type &get_compression_type() const;

 protected:
  void parse_json(string path);

 private:
  void parse_mode_type(const json &conf);
  void check_inconsistencies(void);
};

JPLMEncoderConfigurationLightField::JPLMEncoderConfigurationLightField(
    int argc, char **argv)
    : JPLMEncoderConfiguration(argc, argv) {
  if (!config.empty())
    parse_json(config);
  check_inconsistencies();
}

LightfieldIOConfiguration
JPLMEncoderConfigurationLightField::get_lightfield_io_configurations() const {
  // \todo check this constants
  LightfieldDimension<std::size_t> size(3, 3, 32, 32);
  LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);
  return LightfieldIOConfiguration(input, initial, size);
}

const Type &JPLMEncoderConfigurationLightField::get_type() const {
  return type;
}

void JPLMEncoderConfigurationLightField::parse_mode_type(const json &conf) {
  if (conf.contains("type")) {
    string t = conf["type"].get<string>();
    std::transform(t.begin(), t.end(), t.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (t == "transform mode" || t == "transform_mode" || t == "mule")
      type = Type::transform_mode;
    else if (t == "prediction mode" || t == "prediction_mode" || t == "wasp")
      type = Type::prediction_mode;
    else
      throw NotImplementedYetInputTypeParseException(t);
  }
}

void JPLMEncoderConfigurationLightField::parse_json(string path) {
  JPLMEncoderConfiguration::parse_json(path);
  ifstream ifs(path);
  json conf = json::parse(ifs);
  parse_mode_type(conf);
}

void JPLMEncoderConfigurationLightField::check_inconsistencies(void) {
  vector<bool> rules({
      !(part != JpegPlenoPart::LightField) && (type != Type::transform_mode),
      !(part != JpegPlenoPart::LightField) && (type != Type::prediction_mode),
  });
  if (std::any_of(rules.begin(), rules.end(), [](bool t) { return t; }))
    throw InconsistentOptionsException();
}

const Type &JPLMEncoderConfigurationLightField::get_compression_type() const {
  return type;
}

#endif /* end of include guard: JPLMENCODERCONFIGURATIONLIGHTFIELD_H__ */