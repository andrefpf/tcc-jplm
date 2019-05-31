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
#include "Image.h"
#include "ImageMetrics.h"
#include "PixelMapFileIO.h"


void show_psnr(
    const std::string& filename_original, const std::string& filename_decoded) {
  auto original_file = PixelMapFileIO::open(filename_original);
  auto decoded_file = PixelMapFileIO::open(filename_decoded);

  auto original_image = original_file->read_full_image();
  auto decoded_image = decoded_file->read_full_image();

  namespace visitor = ImageMetrics::visitors;
  std::visit(visitor::PSNRPrinter(), original_image, decoded_image);
}


int main(int argc, char const* argv[]) {
  if (argc < 3) {
    std::cout << "Expecting 2 params\n";
    std::cout << "\tUsage: " << argv[0]
              << " /path/to/input/directory/original.ppm "
                 "/path/to/output/directory/decoded.ppm"
              << std::endl;
    exit(1);
  }

  std::string filename_original(argv[1]);
  std::string filename_decoded(argv[2]);

  show_psnr(filename_original, filename_decoded);

  return 0;
}
