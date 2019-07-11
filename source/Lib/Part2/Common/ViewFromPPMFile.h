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

/** \file     ViewFromPPMFile.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-06
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEWFROMPPMFILE_H__
#define JPLM_LIB_PART2_COMMON_VIEWFROMPPMFILE_H__

#include "Lib/Part2/Common/View.h"
#include "Lib/Part2/Common/ViewToFilenameTranslator.h"
#include "Lib/Utils/Image/ImageUtils.h"
#include "Lib/Utils/Image/PixelMapFile.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"

template<typename T>
class ViewFromPPMFile : public View<T> {
 protected:
  const std::string path;
  const std::pair<std::size_t, std::size_t> position;
  std::unique_ptr<ViewToFilenameTranslator> name_translator;
  std::unique_ptr<PixelMapFile> ppm_file;

 public:
  ViewFromPPMFile(const std::string& path,
      const std::pair<std::size_t, std::size_t>& position)
      //using the default view constructor because we cannot initialize first the derived class members
      : View<T>(), path(path), position(position),
        name_translator(std::make_unique<PPM3CharViewToFilename>()),
        ppm_file(PixelMapFileIO::open(
            {path + name_translator->view_position_to_filename(position)})) {
    this->view_size = {ppm_file->get_width(), ppm_file->get_height()};
    this->bpp = ppm_file->get_number_of_bits_per_pixel();
  };


  ViewFromPPMFile(const ViewFromPPMFile& other)
      : View<T>(other), path(other.path), position(other.position),
        name_translator(std::make_unique<PPM3CharViewToFilename>()),
        ppm_file(PixelMapFileIO::open(
            {path + name_translator->view_position_to_filename(position)})) {
  }


  virtual ViewFromPPMFile<T>* clone() const override {
    return new ViewFromPPMFile<T>(*this);
  }


  ViewFromPPMFile(ViewFromPPMFile&& other) noexcept
      : View<T>(std::move(other)) {
    std::swap(name_translator, other.name_translator);
    std::swap(ppm_file, other.ppm_file);
  }


  void load_image(const std::pair<std::size_t, std::size_t>& size,
      const std::pair<std::size_t, std::size_t>& initial = {0, 0}) override {
    const auto& [i, j] = initial;
    if ((i == 0) && (j == 0) && (size == this->view_size)) {
      //loads the entire image
      auto readed_variant = ppm_file->read_full_image();
      auto readed_image =
          PixelMapFileIO::extract_image_with_type_from_variant<RGBImage,
              uint16_t>(readed_variant);
      this->image_ = ImageUtils::get_image_with_new_container_type<RGBImage,
          RGBImage, T, uint16_t>(readed_image);
    } else {
      //loads image patch
    }
  }

  ~ViewFromPPMFile() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEWFROMPPMFILE_H__ */