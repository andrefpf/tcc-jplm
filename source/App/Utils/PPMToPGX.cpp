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

/** \file     PPMToPGX.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-14
 */

#include <filesystem>
#include <iostream>
#include "Lib/Utils/Image/ImageIO.h"


bool are_input_and_output_paths_valid(
    const std::string& input_path, const std::string& output_path) {
  auto valid = true;
  if (input_path == output_path) {
    std::cerr << "Input and output paths must be different" << std::endl;
    valid = false;
  }

  if (std::filesystem::is_directory(input_path)) {
    std::cerr << "Input must be a ppm file" << std::endl;
    valid = false;
  }

  if (!std::filesystem::is_directory(output_path)) {
    std::cerr << "Output_path path must be a directory" << std::endl;
    valid = false;
  }
  return valid;
}


int main(int argc, char const* argv[]) {
  if (argc < 3) {
    std::cout << "Expecting 2 params\n";
    std::cout << "\tUsage: " << argv[0]
              << " /path/to/input/file.ppm /path/to/output/directory"
              << std::endl;
    exit(1);
  }

  std::string input_path(argv[1]);
  std::string output_path(argv[2]);

  if (!are_input_and_output_paths_valid(input_path, output_path)) {
    exit(1);
  }


  auto image_file = ImageIO::open(input_path);

  auto image_from_file = ImageIO::read<BT601Image, uint16_t>(*image_file);


  auto split_images = ImageUtils::get_splitting_of(*image_from_file);

  auto output = std::filesystem::path(output_path);
  auto input = std::filesystem::path(input_path);

  auto number_of_channels = image_from_file->get_number_of_channels();

  for (auto i = decltype(number_of_channels){0}; i < number_of_channels; ++i) {
    auto path_for_output = output;
    path_for_output /=
        std::to_string(i) / input.filename().replace_extension(".pgx");


    if (!std::filesystem::exists(path_for_output.parent_path())) {
      std::filesystem::create_directory(path_for_output.parent_path());
    }
    ImageIO::imwrite(*split_images[i], path_for_output.string());
  }

  return 0;
}