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
  bool throw_if_missing_bytes = false;

  bool is_different_endianess() const;


 public:
  PGXFile(const std::string& file_name, const std::streampos raster_begin,
      std::size_t width, std::size_t height, std::size_t depth, bool is_signed,
      PGXEndianess endianess)
      : ImageFile(ImageFileType::PGX, file_name, raster_begin, width, height),
        depth(depth), _is_signed(is_signed), endianess(endianess) {
  }


  /**
   * @brief      Determines if signed.
   *
   * @return     True if signed, False otherwise.
   */
  bool is_signed() const noexcept;


  /**
   * @brief      Gets the depth.
   *
   * @return     The depth.
   */
  std::size_t get_depth() const noexcept;


  /**
   * @brief      Gets the endianess.
   *
   * @return     The endianess.
   */
  PGXEndianess get_endianess() const noexcept;


  /**
   * @brief      Destroys the object.
   */
  virtual ~PGXFile() = default;


  /**
   * @brief      Changes the endianess if needed
   *
   * @param[in]  value  The value
   *
   * @tparam     T      Type of the value
   *
   * @return     Value with changed endianess if required
   */
  template<typename T>
  T change_endianess(T value);


  /**
 * @brief      Writes an image to file.
 *
 * @param[in]  image  The image
 *
 * @tparam     T      Type of the data in image
 */
  template<typename T>
  void write_image_to_file(const UndefinedImage<T>& image);


  /**
   * @brief      Reads a full image.
   *
   * @tparam     T     Type of the pixel in image
   *
   * @return     An undefined image containing data in file (or zero if file is empty)
   */
  template<typename T>
  std::unique_ptr<UndefinedImage<T>> read_full_image() {
    if (!file.is_open()) {
      file.open(this->filename.c_str());
      if (file.fail()) {
        auto error_code = errno;
        throw PGXFileExceptions::FailedOppeningPGXFileException(
            this->filename, error_code);
      }
      //<! \todo check if this next file open checking is necessary...
      // if (!file.is_open()) {
      // }
    }

    file.seekg(raster_begin);

    auto image = std::make_unique<UndefinedImage<T>>(
        this->width, this->height, this->depth, 1);
    auto number_of_samples = this->width * this->height;
    std::vector<T> sample_vector(number_of_samples);
    auto number_of_bytes_required = number_of_samples * sizeof(T);
    file.read(reinterpret_cast<char*>(sample_vector.data()),
        number_of_bytes_required);


    if (throw_if_missing_bytes && (!file)) {
      throw PGXFileExceptions::ReadLessPixelThenRequired(
          this->filename, file.gcount(), number_of_bytes_required);
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

    file.close();

    return image;
  }


  /**
   * @brief      Reads a full image.
   *
   * @return     A variant containing a image with the size (type) obtained from the file header.
   */
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
      //<! \todo check if should throw..
    } else {  // (not signed)
      if (depth <= 8) {
        return read_full_image<uint8_t>();
      }
      if (depth <= 16) {
        return read_full_image<uint16_t>();
      }
      if (depth <= 32) {
        return read_full_image<uint32_t>();
      }
      //<! \todo check if should throw..
    }
    throw PGXFileIOExceptions::InvalidPGXException(depth, this->is_signed());
  }
};


/**
 * @brief      Changes the endianess of value if the machine endianess is different from the file endianess
 *
 * @param[in]  value  The value
 *
 * @tparam     T      Type of the value
 *
 * @return     Value with changed endianess when required
 */
template<typename T>
T PGXFile::change_endianess(T value) {
  if constexpr (std::is_signed<T>::value) {
    // typedef std::make_unsigned<T>::type unsiged_T_type;
    return (T) BinaryTools::swap_endianess(
        static_cast<typename std::make_unsigned<T>::type>(value));
  } else {
    return BinaryTools::swap_endianess(value);
  }
}


template<typename T>
void PGXFile::write_image_to_file(const UndefinedImage<T>& image) {
  if (auto n_channels = image.get_number_of_channels(); n_channels != 1) {
    throw PGXFileExceptions::ImageHasMoreThanOneChannelException(n_channels);
  }

  if (!file.is_open()) {
    //first check if directory exists
    auto filename_path = std::filesystem::path(filename);
    if (!std::filesystem::exists(filename_path.parent_path())) {
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
    // std::cout << "for some reason the file was not open..." << std::endl;
    // std::cout << "Filename is " << filename << std::endl;
  }

  auto bytes_per_pixel = std::ceil(image.get_bpp() / (double) 8.0);

  if (sizeof(T) != bytes_per_pixel) {
    //what to do?
    // std::cout << "Bytes per pixel " << bytes_per_pixel << std::endl;
    // std::cout << "sizeof(T) " << sizeof(T) << std::endl;
    // std::cout << "I should do something here..." << std::endl;
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
      // std::cout << "considered endianess" << std::endl;
      // std::cout << "values " << values.size() << std::endl;
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

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PGXFILE_H__ */