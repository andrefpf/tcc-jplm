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

/** \file     ImageFile.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-13
 */

#include "ImageFile.h"


ImageFile::~ImageFile() {
  if (file.is_open())
    file.close();
}


void ImageFile::open() {
  if (!file.is_open())
    file.open(filename);
  if (!file.is_open())
    std::cerr << "Unable to open file " << filename << "." << std::endl;
}


void ImageFile::check(const std::string& filename) {
  auto path = std::filesystem::path(filename).parent_path();
  if (!std::filesystem::exists(path)) {
    std::cerr << "Path " << path.c_str() << " does not exist." << std::endl;
    throw ImageIOExceptions::InexistentPathException();
  }
  if (!std::filesystem::exists(filename)) {
    std::cerr << "File " << filename
              << " does not exist... Need more information to create a new file"
              << std::endl;
    throw ImageIOExceptions::InexistentFileException();
  }
  if (!std::filesystem::is_regular_file(filename)) {
    std::cerr << "File " << filename
              << " is not a regular file (maybe it is a directory?)"
              << std::endl;
    throw ImageIOExceptions::NotARegularFileException();
  }
  // if(std::filesystem::permission)
}