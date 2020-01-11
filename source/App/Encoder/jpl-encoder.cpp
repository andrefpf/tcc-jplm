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

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/JPLMCodecFactory.h"
#include "Lib/Common/JPLMConfigurationFactory.h"
#ifdef __unix__
  #include <sys/resource.h>
#endif

int main(int argc, char const* argv[]) {
  auto start = std::chrono::steady_clock::now();
  auto configuration =
      JPLMConfigurationFactory::get_encoder_configuration(argc, argv);
  std::ofstream of_stream(configuration->get_output_filename(),
      std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
  if (!of_stream.is_open()) {
    std::cerr << "Error opening output file" << std::endl;
    exit(EXIT_FAILURE);
  }


  auto encoder = JPLMCodecFactory::get_encoder(
      std::move(std::unique_ptr<JPLMEncoderConfiguration>(
          static_cast<JPLMEncoderConfiguration*>(configuration.release()))));
  encoder->run();
  const auto& jpl_file = encoder->get_ref_to_jpl_file();


  auto initial_of_stream_position = of_stream.tellp();
  of_stream << jpl_file;
  auto final_of_stream_position = of_stream.tellp();

  of_stream.close();


  auto end = chrono::steady_clock::now();


  std::cout << "Elapsed time in seconds (wall time): "
            << chrono::duration_cast<chrono::seconds>(end - start).count()
            << " s" << std::endl;

#ifdef __unix__
  int who = RUSAGE_SELF;
  struct rusage usage;
  [[maybe_unused]] auto ret = getrusage(who, &usage);
  std::cout << "User time: " << usage.ru_utime.tv_sec << "s"
            << " " << usage.ru_utime.tv_usec / 1000 << "ms\n"
            << "Max memory usage: " << usage.ru_maxrss << " kbytes."
            << std::endl;
#endif

  std::cout << "Bytes written to file: "
            << final_of_stream_position - initial_of_stream_position
            << " bytes " << std::endl;

  exit(EXIT_SUCCESS);
}