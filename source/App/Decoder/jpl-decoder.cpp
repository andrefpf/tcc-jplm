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

/** \file     jpl-decoder.cpp
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
#include "Lib/Part1/Decoder/JPLFileFromStream.h"
#ifdef __unix__
  #include <sys/resource.h>
#endif

int main(int argc, char const* argv[]) {
  auto start = std::chrono::steady_clock::now();
  auto configuration =
      JPLMConfigurationFactory::get_decoder_configuration(argc, argv);

  std::cout << configuration->get_input_filename() << std::endl;
    //checking the file extension for showing a warning message

  std::string input_filename_extension(std::filesystem::path(configuration->get_input_filename()).extension());
  std::transform(input_filename_extension.begin(), input_filename_extension.end(), input_filename_extension.begin(), ::tolower);
  if(std::filesystem::path(configuration->get_input_filename()).extension() != std::filesystem::path(".jpl")) {
      std::cerr << "Warning: the recommended extension is .jpl: " << std::endl;
      std::cerr << "\tWhen stored in traditional computer file systems, JPL files should be given the file extension \".jpl\" (readers should allow mixed case for the alphabetic characters)." << std::endl;
      std::cerr << "\tThe used extension was: " << input_filename_extension << std::endl;
      std::cerr << "\tThe file may still contain a valid JPEG Pleno stream and thus it will be parsed anyway." << std::endl;
  }

  std::cout << configuration->get_output_filename() << std::endl;

  auto jpl_file =
      std::make_shared<JPLFileFromStream>(configuration->get_input_filename());

  auto decoders = JPLMCodecFactory::get_decoders(
      jpl_file, configuration->get_output_filename());
  for (auto& decoder : decoders) {
    decoder->run();
  }

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

  exit(EXIT_SUCCESS);
}