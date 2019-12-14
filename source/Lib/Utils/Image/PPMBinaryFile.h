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

/** \file     PPMBinaryFile.h
 *  \brief    Test of image utils.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-28
 */

#ifndef JPLM_LIB_UTILS_IMAGE_PPMBINARYFILE_H__
#define JPLM_LIB_UTILS_IMAGE_PPMBINARYFILE_H__

#include "Lib/Utils/Image/ImageColorSpacesConversor.h"
#include "Lib/Utils/Image/PixelMapFileBinary.h"


class PPMBinaryFile : public PixelMapFileBinary {
 public:
  PPMBinaryFile(const std::string& file_name)
      : PixelMapFileBinary(file_name, PixelMapType::P6){};
  PPMBinaryFile(const PPMBinaryFile& other) = default;
  PPMBinaryFile(const std::string& file_name, std::size_t width,
      std::size_t height, std::size_t max_value)
      : PixelMapFileBinary(
            file_name, PixelMapType::P6, width, height, max_value){};
  PPMBinaryFile(const std::string& file_name, const std::streampos raster_begin,
      std::size_t width, std::size_t height, std::size_t max_value)
      : PixelMapFileBinary(file_name, raster_begin, PixelMapType::P6, width,
            height, max_value){};

  template<typename T>
  std::unique_ptr<RGBImage<T>> read_rgb_image_patch(
      std::pair<std::size_t, std::size_t> origin,
      std::pair<std::size_t, std::size_t> length);

  template<typename T>
  std::unique_ptr<RGBImage<T>> read_full_rgb_image();

  std::variant<std::unique_ptr<Image<uint8_t>>,
      std::unique_ptr<Image<uint16_t>>>
  read_image_patch(std::pair<std::size_t, std::size_t> origin,
      std::pair<std::size_t, std::size_t> length) override;
  std::variant<std::unique_ptr<Image<uint8_t>>,
      std::unique_ptr<Image<uint16_t>>>
  read_full_image() override;

  template<typename T>
  void write_image_to_file(const Image<T>& image) {
    // std::cout << "raster_begin " << raster_begin << std::endl;
    if (!file.is_open()) {
      if (!std::filesystem::exists(filename)) {
        file.open(filename, std::ios::out | std::ios::binary | std::ios::in);
        if (file.is_open()) {
          file << 'P' << type << std::endl;
          file << width << " " << height << std::endl;
          file << max_value << std::endl;
          raster_begin = file.tellg();
        }
      } else {
        file.open(filename, std::ios::out | std::ios::binary | std::ios::in);
      }
    }
    if (file.is_open()) {
      if (image.get_type() == ImageType::RGB) {
        std::vector<std::tuple<T, T, T>> rgb_vector(
            image.get_number_of_pixels_per_channel());
        const T* r_ptr = image[0].data();
        const T* g_ptr = image[1].data();
        const T* b_ptr = image[2].data();
        for (auto& pixel : rgb_vector) {
          std::get<2>(pixel) = *r_ptr;
          std::get<1>(pixel) = *g_ptr;
          std::get<0>(pixel) = *b_ptr;
          r_ptr++;
          g_ptr++;
          b_ptr++;
        }

        if constexpr (sizeof(T) == 2 && using_little_endian()) {
          for (auto& pixel : rgb_vector) {
            pixel = std::apply(
                [](auto... x) {
                  return std::make_tuple((x << 8) | (x >> 8)...);
                },
                pixel);
          }
        }
        // std::cout << file.tellg() << std::endl;
        auto raster_begin = get_raster_begin();
        if(raster_begin < 0) {
          file.seekg(0, std::ios::end);
        } else {
          file.seekp(get_raster_begin(), std::ios::beg);
        }
        // std::cout << rgb_vector.size() << std::endl;
        file.write(reinterpret_cast<char*>(rgb_vector.data()),
            rgb_vector.capacity() * sizeof(std::tuple<T, T, T>));
        // std::cout << file.tellg() << std::endl;
        file.flush();
        // std::cout << file.tellg() << std::endl;
      } else {
        // std::cout << "Image is not RGB..." << std::endl;
        auto rgb_image =
            ImageColorSpaceConversion::convert::to<RGBImage>(image);
        write_image_to_file(rgb_image);
      }
    } else {
      std::cout << "still not open... what is happening?" << std::endl;
      //! \todo 
      //throw ErrorOpeningImageToWrite
    }
  }

  template<typename T>
  void write_image_patch_to_file(
      const Image<T>& patch_image, std::pair<std::size_t, std::size_t> origin) {
    if (!file.is_open()) {
      if (!std::filesystem::exists(filename)) {
        file.open(filename, std::ios::out | std::ios::binary | std::ios::in);
        if (file.is_open()) {
          file << 'P' << type << std::endl;
          file << width << " " << height << std::endl;
          file << max_value << std::endl;
          raster_begin = file.tellg();
        }
      } else {
        file.open(filename, std::ios::out | std::ios::binary | std::ios::in);
      }
    }
    if (file.is_open()) {
      if (patch_image.get_type() == ImageType::RGB) {
        auto patch_width = patch_image.get_width();
        auto patch_height = patch_image.get_height();
        std::vector<std::tuple<T, T, T>> rgb_vector(patch_width);
        std::streamoff vertical_offset(
            std::get<0>(origin) * width * sizeof(std::tuple<T, T, T>));
        std::streamoff offset_in_image(
            vertical_offset +
            std::get<1>(origin) * sizeof(std::tuple<T, T, T>));

        file.seekp(raster_begin + offset_in_image);

        // std::vector<std::tuple<T, T, T>> rgb_vector(patch_image.get_number_of_pixels());
        const T* r_ptr = patch_image[0].data();
        const T* g_ptr = patch_image[1].data();
        const T* b_ptr = patch_image[2].data();

        auto number_of_bytes_to_write =
            rgb_vector.capacity() * sizeof(std::tuple<T, T, T>);
        auto file_stride = std::streamoff(
            sizeof(std::tuple<T, T, T>) * width - number_of_bytes_to_write);


        for (decltype(patch_height) i = 0; i < patch_height; ++i) {
          for (auto& pixel : rgb_vector) {
            std::get<2>(pixel) = *r_ptr;
            std::get<1>(pixel) = *g_ptr;
            std::get<0>(pixel) = *b_ptr;
            r_ptr++;
            g_ptr++;
            b_ptr++;
          }
          if constexpr (sizeof(T) == 2 && using_little_endian()) {
            for (auto& pixel : rgb_vector) {
              pixel = std::experimental::apply(
                  [](auto... x) {
                    return std::make_tuple((x << 8) | (x >> 8)...);
                  },
                  pixel);
            }
          }
          file.write(reinterpret_cast<char*>(rgb_vector.data()),
              number_of_bytes_to_write);
          file.seekp(file_stride, std::ios::cur);
        }


        file.flush();
      }
    } else {
      std::cout << "still not open... what is happening?" << std::endl;
    }
  }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PPMBINARYFILE_H__ */