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

/** \file     PPMBinaryFile.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-07
 */

#include "PPMBinaryFile.h"


void correct_endianess(
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>& pixel_vector) {
  if constexpr (using_little_endian()) {
    for (auto& pixel : pixel_vector) {
      pixel = std::apply(
          [](auto... x) { return std::make_tuple((x << 8) | (x >> 8)...); },
          pixel);
    }
  }
}


template<typename T>
std::unique_ptr<RGBImage<T>> PPMBinaryFile::read_rgb_image_patch(
    std::pair<std::size_t, std::size_t> origin,
    std::pair<std::size_t, std::size_t> length) {
  if (file.is_open()) {
    auto patch_width = std::get<1>(length);
    auto patch_height = std::get<0>(length);
    auto patch_image = std::make_unique<RGBImage<T>>(
        patch_width, patch_height, get_number_of_bits_per_pixel());
    auto r_ptr = patch_image->get_channel(0).data();
    auto g_ptr = patch_image->get_channel(1).data();
    auto b_ptr = patch_image->get_channel(2).data();

    std::vector<std::tuple<T, T, T>> rgb_vector(patch_width);
    std::streamoff vertical_offset(
        std::get<0>(origin) * width * sizeof(std::tuple<T, T, T>));
    std::streamoff offset_in_image(
        vertical_offset + std::get<1>(origin) * sizeof(std::tuple<T, T, T>));

    std::streamoff offset = raster_begin + offset_in_image;
    file.seekg(offset, std::ios::beg);

    auto number_of_bytes_to_read =
        rgb_vector.capacity() * sizeof(std::tuple<T, T, T>);
    auto file_stride = std::streamoff(
        sizeof(std::tuple<T, T, T>) * width - number_of_bytes_to_read);

    for (decltype(patch_height) i = 0; i < patch_height; ++i) {
      file.read(
          reinterpret_cast<char*>(rgb_vector.data()), number_of_bytes_to_read);
      file.seekg(file_stride, std::ios::cur);

      if constexpr (std::is_same<T, uint16_t>()) {
        correct_endianess(rgb_vector);
      }

      for (const auto& pixel : rgb_vector) {
        *r_ptr++ = std::get<2>(pixel);
        *g_ptr++ = std::get<1>(pixel);
        *b_ptr++ = std::get<0>(pixel);
      }
    }
    return patch_image;
  }
  std::cerr << "Unable to open file..." << std::endl;
  exit(2);
}


std::variant<std::unique_ptr<Image<uint8_t>>, std::unique_ptr<Image<uint16_t>>>
PPMBinaryFile::read_image_patch(std::pair<std::size_t, std::size_t> origin,
    std::pair<std::size_t, std::size_t> length) {
  if (!file.is_open()) {
    file.open(filename, std::ios::binary | std::ios::in);
  }
  if (max_value < 256) {
    return read_rgb_image_patch<uint8_t>(origin, length);
  }
  return read_rgb_image_patch<uint16_t>(origin, length);
}


template<typename T>
std::unique_ptr<RGBImage<T>> PPMBinaryFile::read_full_rgb_image() {
  static_assert(std::is_same<T, uint8_t>() || std::is_same<T, uint16_t>(),
      "Binary PPM only supports uint8_t and uint16_t types");
  if (file.is_open()) {
    auto image = std::make_unique<RGBImage<T>>(
        width, height, this->get_number_of_bits_per_pixel());
    std::vector<std::tuple<T, T, T>> rgb_vector(
        image->get_number_of_pixels_per_channel());
    file.seekg(raster_begin);
    file.read(reinterpret_cast<char*>(rgb_vector.data()),
        rgb_vector.capacity() * sizeof(std::tuple<T, T, T>));
    if (!file) {
      std::cerr << "Expecting " << image->get_number_of_pixels() << " pixels."
                << std::endl;
      std::cerr << "Read only " << file.gcount() << std::endl;
    }

    if constexpr (std::is_same<T, uint16_t>()) {
      correct_endianess(rgb_vector);
    }

    auto r_ptr = image->get_channel(0).data();
    auto g_ptr = image->get_channel(1).data();
    auto b_ptr = image->get_channel(2).data();

    for (const auto& pixel : rgb_vector) {
      *r_ptr++ = std::get<2>(pixel);
      *g_ptr++ = std::get<1>(pixel);
      *b_ptr++ = std::get<0>(pixel);
    }
    return image;
  }
  std::cerr << "Unable to open file..." << std::endl;
  exit(2);
}


std::variant<std::unique_ptr<Image<uint8_t>>, std::unique_ptr<Image<uint16_t>>>
PPMBinaryFile::read_full_image() {
  if (!file.is_open()) {
    file.open(filename, std::ios::binary | std::ios::in);
  }
  if (max_value < 256) {
    return read_full_rgb_image<uint8_t>();
  }
  return read_full_rgb_image<uint16_t>();
}