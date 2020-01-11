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

/** \file     PGXFile.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-05
 */


#ifndef JPLM_LIB_UTILS_IMAGE_PGXFILE_H__
#define JPLM_LIB_UTILS_IMAGE_PGXFILE_H__

#include <cstddef>
#include <type_traits>  //std::is_signed
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageFile.h"
#include "Lib/Utils/Image/UndefinedImage.h"
#include "Lib/Utils/Stream/BinaryTools.h"


enum class PGXEndianess { PGX_ML_BIG_ENDIAN = 0, PGX_LM_LITTLE_ENDIAN = 1 };


class PGXFile : public ImageFile {
 protected:
  std::size_t depth;
  bool _is_signed;
  const PGXEndianess endianess;

  bool is_different_endianess() const {
    if (this->endianess == PGXEndianess::PGX_ML_BIG_ENDIAN) {
      if constexpr (BinaryTools::using_little_endian()) {
        return true;
      }
    } else {
      if constexpr (!BinaryTools::using_little_endian()) {
        return true;
      }
    }
    return false;
  }


 public:
  PGXFile(const std::string& file_name, const std::streampos raster_begin,
      std::size_t width, std::size_t height, std::size_t depth, bool is_signed,
      PGXEndianess endianess)
      : ImageFile(ImageFileType::PGX, file_name, raster_begin, width, height),
        depth(depth), _is_signed(is_signed), endianess(endianess) {
  }


  bool is_signed() const noexcept {
    return _is_signed;
  }


  auto get_depth() const noexcept {
    return depth;
  }


  auto get_endianess() const noexcept {
    return endianess;
  }


  virtual ~PGXFile() = default;


  template<typename T>
  T change_endianess(T value) {
    if constexpr (std::is_signed<T>::value) {
      // typedef std::make_unsigned<T>::type unsiged_T_type;
      return (T) BinaryTools::swap_endianess(
          static_cast<typename std::make_unsigned<T>::type>(value));
    } else {
      return BinaryTools::swap_endianess(value);
    }
  }


  template<typename T>
  void write_image_to_file(const UndefinedImage<T>& image) {
    if (image.get_number_of_channels() != 1) {
      //! \todo throw error if number of channels of the current undefined image is not 1
      //throw error
      std::cout << "there is more than one channel..." << std::endl;
    }

    if (!file.is_open()) {
      //first check if directory exists
      auto filename_path = std::filesystem::path(filename);
      if(!std::filesystem::exists(filename_path.parent_path())) {
        // std::cout << "filename_path.parent_path " << filename_path.parent_path() << " does not exist" << std::endl;
        std::filesystem::create_directory(filename_path.parent_path());
      }

      if (!std::filesystem::exists(filename)) {
        file.open(filename, std::ios::out | std::ios::binary | std::ios::in);
        //should write
      } else {
        file.open(filename, std::ios::out | std::ios::binary | std::ios::in);
        //should read
      }
      std::cout << "for some reason the file was not open..." << std::endl;
      std::cout << "Filename is " << filename << std::endl;
    }

    auto bytes_per_pixel = std::ceil(image.get_bpp() / (double) 8.0);

    if (sizeof(T) != bytes_per_pixel) {
      //what to do?
      std::cout << "Bytes per pixel " << bytes_per_pixel << std::endl;
      std::cout << "sizeof(T) " << sizeof(T) << std::endl;
      std::cout << "I should do something here..." << std::endl;
    }

    file.seekp(this->raster_begin);

    if constexpr (sizeof(T) > 1) {
      if (is_different_endianess()) {
        std::vector<T> values;
        values.reserve(image.get_number_of_pixels());
        for (const auto& value : image.get_channel(0)) {
          values.push_back(change_endianess(value));
        }
        // auto size_of_vector = image.get_number_of_pixels();
        std::cout << "considered endianess" << std::endl;
        std::cout << "values " << values.size() << std::endl;
        file.write(reinterpret_cast<const char*>(values.data()),
            image.get_number_of_pixels_per_channel() * sizeof(T));
        file.flush();
        file.close();
        return;
      }
    }

    file.write(reinterpret_cast<const char*>(image.get_channel(0).data()),
        image.get_number_of_pixels_per_channel() * sizeof(T));
  }


  template<typename T>
  std::unique_ptr<UndefinedImage<T>> read_full_image() {
    if (!file.is_open()) {
      file.open(this->filename.c_str());
      if (!file.is_open()) {
        //error
      }
    }

    file.seekg(raster_begin);

    auto image = std::make_unique<UndefinedImage<T>>(
        this->width, this->height, this->depth, 1);
    auto number_of_samples = this->width * this->height;
    std::vector<T> sample_vector(number_of_samples);
    file.read(reinterpret_cast<char*>(sample_vector.data()),
        number_of_samples * sizeof(T));

    if (!file) {
      //error? less pixels than availabe in file file.gcount() (readed)
    }

    if constexpr (sizeof(T) > 1) {
      if (is_different_endianess()) {
        for (auto& value : sample_vector) {
          value = change_endianess(value);
        }
      }
    }

    auto image_channel_ptr = image->get_channel(0).data();
    auto vector_data = sample_vector.data();

    for (auto i = decltype(number_of_samples){0}; i < number_of_samples; ++i) {
      *image_channel_ptr = *(vector_data++);
      ++image_channel_ptr;
    }

    return image;
  }

  std::variant<std::unique_ptr<UndefinedImage<uint8_t>>,
      std::unique_ptr<UndefinedImage<int8_t>>,
      std::unique_ptr<UndefinedImage<uint16_t>>,
      std::unique_ptr<UndefinedImage<int16_t>>,
      std::unique_ptr<UndefinedImage<uint32_t>>,
      std::unique_ptr<UndefinedImage<int32_t>>>
  read_full_image() {
    if (this->is_signed()) {
      if (depth <= 8) {
        return read_full_image<int8_t>();
      }
      if (depth <= 16) {
        return read_full_image<int16_t>();
      }
      if (depth <= 32) {
        return read_full_image<int32_t>();
      }
      //throw..
    }  //else (not signed)
    if (depth <= 8) {
      return read_full_image<uint8_t>();
    }
    if (depth <= 16) {
      return read_full_image<uint16_t>();
    }
    if (depth <= 32) {
      return read_full_image<uint32_t>();
    }
    //throw
  }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PGXFILE_H__ */