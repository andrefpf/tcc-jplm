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

/** \file     jpl-encoder.cpp
 *  \brief
 *  \details
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */


#include <cstdlib>
#include <string>
#include "Lib/Common/JPLMCodecFactory.h"
#include "Lib/Common/JPLMConfigurationFactory.h"
#include "Lib/Utils/Stats/EncoderRunTimeStatistics.h"


int main(int argc, char const* argv[]) {
  auto configuration =
      JPLMConfigurationFactory::get_encoder_configuration(argc, argv);
  std::ofstream of_stream(configuration->get_output_filename(),
      std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);

  if (configuration->is_verbose()) {
    std::cerr << "JPEG Pleno Model (JPLM) Encoder. \nVerbose mode \"on\"\n";
  }

  //<! needed to check here because configuration is unique ptr that will be moved...
  auto show_statistics = configuration->show_runtime_statistics();
  auto run_time_statistics = EncoderRunTimeStatistics(of_stream);

  //<! \todo avoid the static cast as the get_encoder_configuration function
  // should return an specialized encoder configuration
  auto encoder = JPLMCodecFactory::get_encoder(configuration);

  encoder->run();

  auto& jpl_file = encoder->get_ref_to_jpl_file();

  if (configuration->must_generate_xml_box_with_catalog()) {
    jpl_file.enable_catalog();
  }

  of_stream << jpl_file;

  if (show_statistics) {
    run_time_statistics.show_statistics();
  }

  of_stream.close();

  exit(EXIT_SUCCESS);
}