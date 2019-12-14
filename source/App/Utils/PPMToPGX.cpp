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
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include <sstream>
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

  std::cout << image_from_file->get_type() << std::endl;


  auto split_images = ImageUtils::get_splitting_of(*image_from_file);

  std::cout << split_images.size() << std::endl;


  ImageIO::imwrite(*split_images[0], "teste_0.pgx");
  ImageIO::imwrite(*split_images[1], "teste_1.pgx");
  ImageIO::imwrite(*split_images[2], "teste_2.pgx");


  auto im_0 = ImageIO::open("teste_0.pgx");
  auto imag_0 = ImageIO::read<UndefinedImage, uint16_t>(*im_0);
  std::cout << "readed image 0" << std::endl;
  auto im_1 = ImageIO::open("teste_1.pgx");
  auto imag_1 = ImageIO::read<UndefinedImage, uint16_t>(*im_1);
  std::cout << "readed image 1" << std::endl;
  auto im_2 = ImageIO::open("teste_2.pgx");
  auto imag_2 = ImageIO::read<UndefinedImage, uint16_t>(*im_2);
  std::cout << "readed image 2" << std::endl;
  
  std::cout << imag_0->get_width() << "x" << imag_0->get_height() << std::endl;
  std::cout << imag_1->get_width() << "x" << imag_1->get_height() << std::endl;
  std::cout << imag_2->get_width() << "x" << imag_2->get_height() << std::endl;

  auto image_sum = ImageUtils::get_undefined_images_as<BT601Image>(*imag_0, *imag_1, *imag_2);


  ImageIO::imwrite(*image_sum, "output.ppm");


  return 0;
}