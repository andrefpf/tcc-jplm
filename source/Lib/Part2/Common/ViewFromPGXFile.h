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

/** \file     ViewFromPGXFile.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-16
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEWFROMPGXFILE_H__
#define JPLM_LIB_PART2_COMMON_VIEWFROMPGXFILE_H__

#include "Lib/Part2/Common/View.h"
#include "Lib/Part2/Common/ViewToFilenameTranslator.h"
#include "Lib/Utils/Image/ImageIO.h"
#include "Lib/Utils/Image/ImageUtils.h"
#include "Lib/Utils/Image/PGXFile.h"
#include "Lib/Utils/Image/PGXFileIO.h"

template<typename T>
class ViewFromPGXFile : public View<T> {
 protected:
  const std::string path;
  const std::pair<std::size_t, std::size_t> position;
  std::size_t number_of_channels;
  std::unique_ptr<ViewToFilenameTranslator> name_translator;
  std::vector<std::unique_ptr<PGXFile>> pgx_files;
  // std::unique_ptr<PGXFile> pgx_file;
  bool overwrite_pgx_file_in_destructor = false;

  void open_pgx_files() {
    auto base_path = std::filesystem::path(path);
    for (auto i = decltype(number_of_channels){0}; i < number_of_channels;
         ++i) {
      auto current_path = base_path / std::to_string(i) /
                          name_translator->view_position_to_filename(position);
      pgx_files.push_back(PGXFileIO::open(current_path));
    }
  }


  void open_new_pgx_files(
      std::size_t width, std::size_t height, std::size_t depth) {
    auto base_path = std::filesystem::path(path);
    for (auto i = decltype(number_of_channels){0}; i < number_of_channels;
         ++i) {
      auto current_path = base_path / std::to_string(i) /
                          name_translator->view_position_to_filename(position);
      pgx_files.push_back(
          PGXFileIO::open(current_path, width, height, depth, false));
    }
  }

 public:
  ViewFromPGXFile(const std::string& path,
      const std::pair<std::size_t, std::size_t>& position,
      std::size_t number_of_channels)
      //using the default view constructor because we cannot initialize first the derived class members
      : View<T>(), path(path), position(position),
        number_of_channels(number_of_channels),
        name_translator(std::make_unique<PGX3CharViewToFilename>()) {
    // ,
    //     pgx_file(PGXFileIO::open(
    //         {path + name_translator->view_position_to_filename(position)}))
    open_pgx_files();

    this->view_size = {pgx_files[0]->get_width(), pgx_files[0]->get_height()};
    this->bpp = pgx_files[0]->get_depth();
  };


  ViewFromPGXFile(const std::string& path,
      const std::pair<std::size_t, std::size_t>& position,
      const std::pair<std::size_t, std::size_t>& dimension_v_u,
      std::size_t depth, std::size_t number_of_channels)
      : View<T>(), path(path), position(position),
        number_of_channels(number_of_channels),
        name_translator(std::make_unique<PGX3CharViewToFilename>()),
        overwrite_pgx_file_in_destructor(true) {
    // pgx_file(PGXFileIO::open(
    //    {path + name_translator->view_position_to_filename(position)}, type,
    //    std::get<1>(dimension_v_u), std::get<0>(dimension_v_u),
    //    max_value));
    open_new_pgx_files(
        std::get<1>(dimension_v_u), std::get<0>(dimension_v_u), depth);
  }


  ViewFromPGXFile(const ViewFromPGXFile& other)
      : View<T>(other), path(other.path), position(other.position),
        number_of_channels(other.number_of_channels),
        name_translator(std::make_unique<PGX3CharViewToFilename>()) {
    open_pgx_files();
  }


  virtual ViewFromPGXFile<T>* clone() const override {
    return new ViewFromPGXFile<T>(*this);
  }


  ViewFromPGXFile(ViewFromPGXFile&& other) noexcept
      : View<T>(std::move(other)), path(std::move(other.path)),
        position(other.position) {
    std::swap(name_translator, other.name_translator);
    std::swap(pgx_files, other.pgx_files);
    overwrite_pgx_file_in_destructor = other.overwrite_pgx_file_in_destructor;
  }


  void load_image(const std::pair<std::size_t, std::size_t>& size,
      const std::pair<std::size_t, std::size_t>& initial = {
          0, 0}) const override {
    const auto& [i, j] = initial;
    if ((i == 0) && (j == 0) && (size == this->view_size)) {
      //needs to read all channels... std::vector<std::unique_ptr<PGXFile>> pgx_files;
      // std::unique_ptr<PGXFile> pgx_file;
      std::vector<std::unique_ptr<UndefinedImage<T>>> images_from_file;
      for (auto& pgx_file : pgx_files) {
        images_from_file.push_back(
            ImageIO::read<UndefinedImage, uint16_t>(*pgx_file));
      }
      this->image_ =
          ImageUtils::get_undefined_images_as_undefined_image(images_from_file);
    } else {
      //loads image patch
    }
  }


  virtual void write_image(const bool overwrite_file = false) override {
    if (this->image_) {
      auto images = ImageUtils::get_splitting_of(*(this->image_));
      auto number_of_channels = images.size();
      if (number_of_channels != pgx_files.size()) {
        std::cerr << "Errorrrr" << std::endl;
        //throws
      } else {
        for (auto c = decltype(number_of_channels){0}; c < number_of_channels;
             ++c) {
          ImageIO::imwrite(
              *(images[c]), pgx_files[c]->get_filename(), overwrite_file);
        }
      }

      // for(auto& pgx_file: pgx_files) {
      //   // images_from_file.push_back(ImageIO::read<UndefinedImage, uint16_t>(*pgx_file));
      // ImageIO::imwrite(
      // *this->image_, pgx_file->get_filename(), overwrite_file);
      // }
    }
  }

  virtual ~ViewFromPGXFile() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEWFROMPGXFILE_H__ */