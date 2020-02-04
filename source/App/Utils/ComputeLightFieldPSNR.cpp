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

/** \file     ComputeLightFieldPSNR.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-11-05
 */


#include <algorithm>
#include <filesystem>
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "Lib/Part2/Common/LightfieldDimension.h"
#include "Lib/Part2/Common/LightfieldFromFile.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Part2/Common/ViewIOPolicyOneAtATime.h"
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageMetrics.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"


std::map<std::string, ImageType> string_to_image_type_map;


void fill_map() {
  string_to_image_type_map["rgb"] = ImageType::RGB;
  string_to_image_type_map["bt601"] = ImageType::BT601;
  string_to_image_type_map["bt709"] = ImageType::BT709;
  string_to_image_type_map["bt2020"] = ImageType::BT2020;
}


void show_psnr(const std::string& filename_original,
    const std::string& filename_decoded, const ImageType type = ImageType::RGB,
    std::size_t t_max = 13, std::size_t s_max = 13) {
  LightfieldDimension<std::size_t> size(t_max, s_max, 32, 32);
  LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);
  LightfieldIOConfiguration original_configuration(
      filename_original, initial, size);
  LightfieldIOConfiguration decoded_configuration(
      filename_decoded, initial, size);

  auto original_lightfield =
      std::make_unique<LightfieldFromFile<uint16_t>>(original_configuration);


  auto decoded_lightfield =
      std::make_unique<LightfieldFromFile<uint16_t>>(decoded_configuration);


  /*  auto original_policy = std::make_unique<ViewIOPolicyOneAtATime<uint16_t>>();
  original_lightfield->set_view_io_policy(std::move(original_policy));
  auto decoded_policy = std::make_unique<ViewIOPolicyOneAtATime<uint16_t>>();
  decoded_lightfield->set_view_io_policy(std::move(decoded_policy));*/

  auto printer = ImageMetrics::visitors::PSNRPrinter();
  for (auto t = decltype(t_max){0}; t < t_max; ++t) {
    for (auto s = decltype(s_max){0}; s < s_max; ++s) {
      std::cout << "T: " << t << "   S: " << s << std::endl;
      auto original_image = std::make_unique<RGBImage<uint16_t>>(
          original_lightfield->get_image_at<RGBImage>({t, s}));
      auto decoded_image = std::make_unique<RGBImage<uint16_t>>(
          decoded_lightfield->get_image_at<RGBImage>({t, s}));
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
  }
}


int main(int argc, char const* argv[]) {
  if (argc < 4) {
    std::cout << "Expecting 5 params\n";
    std::cout << "\tUsage: " << argv[0] << " color_space"
              << " /path/to/input/directory/"
                 " /path/to/output/directory/"
              << " t"
              << " s" << std::endl;
    exit(1);
  }
  fill_map();


  std::string color_space(argv[1]);
  std::transform(
      color_space.begin(), color_space.end(), color_space.begin(), ::tolower);

  auto type = ImageType::RGB;  //default
  auto it = string_to_image_type_map.find(color_space);
  if (it == string_to_image_type_map.end()) {
    std::cerr << "Unable to find color space " << color_space
              << ". The available ones are: \n";
    for (const auto& map_iterator : string_to_image_type_map) {
      std::cerr << '\t' << std::get<0>(map_iterator) << '\n';
    }
    std::cerr << "Using RGB as default:\n";
  } else {
    type = it->second;
  }

  std::string filename_original(argv[2]);
  std::string filename_decoded(argv[3]);


  int t = 13;
  int s = 13;

  if (argc > 4) {
    t = s = atoi(argv[4]);
  }

  if (argc > 5) {
    s = atoi(argv[5]);
  }

  show_psnr(filename_original, filename_decoded, type, t, s);

  return 0;
}