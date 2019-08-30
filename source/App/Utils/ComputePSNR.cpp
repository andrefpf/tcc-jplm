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

/** \file     Lenslet13x13Shifter.cpp
 *  \brief    Test of PPM Binary File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-31
 */

#include <filesystem>
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <string> 
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageMetrics.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"


std::map<std::string, ImageType> string_to_image_type_map;

void fill_map() {
  string_to_image_type_map["rgb"]=ImageType::RGB;
  string_to_image_type_map["bt601"]=ImageType::BT601;
  string_to_image_type_map["bt709"]=ImageType::BT709;
  string_to_image_type_map["bt2020"]=ImageType::BT2020;
}


void show_psnr(
    const std::string& filename_original, const std::string& filename_decoded, const ImageType type=ImageType::RGB) {
  auto original_file = PixelMapFileIO::open(filename_original);
  auto decoded_file = PixelMapFileIO::open(filename_decoded);

  auto original_image_variant = original_file->read_full_image();
  auto original_image = PixelMapFileIO::extract_image_with_type_from_variant<RGBImage,
              uint16_t>(original_image_variant);

  auto decoded_image_variant = decoded_file->read_full_image();
  auto decoded_image = PixelMapFileIO::extract_image_with_type_from_variant<RGBImage,
              uint16_t>(decoded_image_variant);

  auto printer = ImageMetrics::visitors::PSNRPrinter();

   switch (type) {
      case ImageType::RGB:
        printer(original_image, decoded_image);
        break;
      case ImageType::BT601:
        printer.operator()<BT601Image>(original_image, decoded_image);
        break;
      case ImageType::BT709:
        printer.operator()<BT709Image>(original_image, decoded_image);
        break;
      case ImageType::BT2020:
        printer.operator()<BT2020Image>(original_image, decoded_image);
        break;
      default:
        std::cerr << "Error default" << std::endl;
    }

}


int main(int argc, char const* argv[]) {
  if (argc < 4) {
    std::cout << "Expecting 2 params\n";
    std::cout << "\tUsage: " << argv[0]
              << " color_space"
              << " /path/to/input/directory/original.ppm"
                 " /path/to/output/directory/decoded.ppm"
              << std::endl;
    exit(1);
  }
  fill_map();

  std::string color_space(argv[1]);
  std::transform(color_space.begin(), color_space.end(), color_space.begin(), ::tolower);

  auto type = ImageType::RGB; //default
  auto it = string_to_image_type_map.find(color_space);
  if (it == string_to_image_type_map.end()) {
    std::cerr << "Unable to find color space " << color_space << ". The available ones are: \n";
    for(const auto& map_iterator: string_to_image_type_map) {
      std::cerr << '\t' << std::get<0>(map_iterator) << '\n';
    }
    std::cerr << "Using RGB as default:\n";
  } else {
    type = it->second;
  }


  std::string filename_original(argv[2]);
  std::string filename_decoded(argv[3]);

  show_psnr(filename_original, filename_decoded, type);

  return 0;
}
