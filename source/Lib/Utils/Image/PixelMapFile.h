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

/** \file     PixelMapFile.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-28
 */

#ifndef JPLM_LIB_UTILS_IMAGE_PIXELMAPFILE_H__
#define JPLM_LIB_UTILS_IMAGE_PIXELMAPFILE_H__

#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageFile.h"


//P1 and P4 are bitmaps, PBM (portable bitmap), or pbm - Netpbm bi-level image format according to http://netpbm.sourceforge.net/doc/pbm.html
//P2 and P5 are grayscale images, PGM (portable graymap)
//P3 and P6 are fullcolor images, PPM (portable pixmap)
enum PixelMapType { Unknown = 0, P1, P2, P3, P4, P5, P6 };


class PixelMapFile : public ImageFile {
 protected:
  std::size_t max_value = 0;
  PixelMapType type = PixelMapType::Unknown;

 public:
  PixelMapFile(const std::string& file_name, PixelMapType type)
      : ImageFile(ImageFileType::PixelMap, file_name), type(type){};


  PixelMapFile(std::filesystem::path& file_name, PixelMapType type)
      : ImageFile(ImageFileType::PixelMap, file_name), type(type){};


  PixelMapFile(const PixelMapFile& other) = delete;


  PixelMapFile(PixelMapFile&& other) : ImageFile(std::move(other)) {
    std::swap(type, other.type);
    std::swap(max_value, other.max_value);
  }


  PixelMapFile(const std::string& file_name, PixelMapType type,
      std::size_t width, std::size_t height, std::size_t max_value)
      : ImageFile(ImageFileType::PixelMap, file_name, width, height),
        max_value(max_value), type(type){};


  PixelMapFile(const std::string& file_name, const std::streampos raster_begin,
      PixelMapType type, std::size_t width, std::size_t height,
      std::size_t max_value)
      : ImageFile(
            ImageFileType::PixelMap, file_name, raster_begin, width, height),
        max_value(max_value), type(type){};


  virtual ~PixelMapFile() = default;

  /**
   * \brief      Gets the type.
   *
   * \return     The type, from the enum PixelMapType.
   */
  auto get_type() const noexcept {
    return type;
  }


  /**
   * \brief      Determines if this PixelMapFile has properties (type, width, height, and max_value).
   *
   * \param[in]  type       The type
   * \param[in]  width      The width
   * \param[in]  height     The height
   * \param[in]  max_value  The maximum value
   *
   * \return     True if has properties, False otherwise.
   */
  bool has_properties(PixelMapType type, std::size_t width, std::size_t height,
      std::size_t max_value) const noexcept;


  /**
   * \brief      Determines if this PixelMapFile is equivalent to other one.
   *
   * \param[in]  other  The other
   *
   * \return     True if equivalent to, False otherwise.
   */
  bool is_equivalent_to(const PixelMapFile& other) const noexcept;


  /**
   * \brief      Gets the number of bits per pixel.
   * \details    Computes the number of bpp from the max_value
   *
   * \return     The number of bits per pixel.
   */
  std::size_t get_number_of_bits_per_pixel() const;


  /**
   * \brief      Reads a full image.
   *
   * \return     A variant of unique_ptrs of either Image<uint8_t> or Image<uint16_t>
   */
  virtual std::variant<std::unique_ptr<Image<uint8_t>>,
      std::unique_ptr<Image<uint16_t>>>
  read_full_image() = 0;


  /**
   * \brief      Reads an image patch, i.e., a rectangular area of the complete image.
   *
   * \param[in]  origin  The origin
   * \param[in]  length  The length
   *
   * \return     { description_of_the_return_value }
   */
  virtual std::variant<std::unique_ptr<Image<uint8_t>>,
      std::unique_ptr<Image<uint16_t>>>
  read_image_patch(std::pair<std::size_t, std::size_t> origin,
      std::pair<std::size_t, std::size_t> length) = 0;
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PIXELMAPFILE_H__ */