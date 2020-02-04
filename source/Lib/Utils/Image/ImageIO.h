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

/** \file     ImageIO.h
 *  \brief    Test of PPM Binary File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-29
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGEIO_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGEIO_H__

#include "Lib/Utils/Image/ImageColorSpacesConversor.h"
#include "Lib/Utils/Image/PGXFileIO.h"
#include "Lib/Utils/Image/PPMBinaryFile.h"
#include "Lib/Utils/Image/PixelMapFile.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"


namespace ImageIO {


void write_image_to_file(
    const std::variant<Image<uint8_t>, Image<uint16_t>>& image,
    const std::string& filename);

template<typename T>
void write_image_to_file(const Image<T>& image, const std::string& filename);

void write_image_to_file(
    const Image<uint16_t>& image, const std::string& filename);
void write_image_to_file(
    const Image<uint8_t>& image, const std::string& filename);


/**
 * \brief      Function to write a full image to file
 *
 * \param[in]  image      The image
 * \param[in]  filename   The filename
 * \param[in]  overwrite  The overwrite flag. If set, the file will be overwritten if it already exists
 *
 * \tparam     T            The value container of the image (template resolution makes this hidden)
 */
template<typename T>
void imwrite(const Image<T>& image, const std::string& filename,
    const bool overwrite = false) {
  namespace fs = std::filesystem;
  using fpath = fs::path;
  auto name = fpath(filename);

  if (!overwrite && fs::exists(filename)) {
    throw ImageIOExceptions::FileAlreadyExistsException();
  }

  if (name.extension() == fpath(".ppm")) {
    auto max_value =
        static_cast<std::size_t>(std::pow(2.0, image.get_bpp())) - 1;
    auto oppened_image = PixelMapFileIO::open(filename, PixelMapType::P6,
        image.get_width(), image.get_height(), max_value);
    auto oppened_image_as_ppm = std::unique_ptr<PPMBinaryFile>(
        static_cast<PPMBinaryFile*>(oppened_image.release()));
    oppened_image_as_ppm->write_image_to_file(image);
  } else {
    if (name.extension() == fpath(".pgx")) {
      if(image.get_type() == ImageType::Undefined) {
        auto oppened_image = PGXFileIO::open(
            filename, image.get_width(), image.get_height(), image.get_bpp(), image.is_signed());
        oppened_image->write_image_to_file(dynamic_cast<const UndefinedImage<T>&>(image));
      } else {
        //! \todo throw error if trying to write a pgx image that is not an undefined image type
        //throw
      }

    }
  }
}


/**
 * \brief      Function to write image patch to file
 *
 * \param[in]  patch_image  The image patch to be written
 * \param[in]  filename     The filename
 * \param[in]  origin       The origin (the initial point from which the patch will be placed)
 *
 * \tparam     T            The value container of the image (template resolution makes this hidden)
 */
template<typename T>
void imwrite(const Image<T>& patch_image, const std::string& filename,
    std::pair<std::size_t, std::size_t> origin) {
  using fpath = std::filesystem::path;
  auto name = fpath(filename);

  if (name.extension() == fpath(".ppm")) {
    auto oppened_image =
        PixelMapFileIO::open(filename);  //should test if exists...

    auto oppened_image_as_rgb = std::unique_ptr<PPMBinaryFile>(
        static_cast<PPMBinaryFile*>(oppened_image.release()));
    oppened_image_as_rgb->write_image_patch_to_file(patch_image, origin);
  }

  // if (name.extension() == fpath(".pgx")) {
  //   auto oppened_image =
  //       PGXFileIO::open(filename);  //should test if exists...
}


/**
 * \brief      matlab like image reading function
 *
 * \param[in]  filename  The filename
 *
 * \tparam     ImageT    The expected return image class
 * \tparam     T         The image value container in the ImageT
 *
 * \return     returns a std::unique_ptr to ImageT<T> read from file
 */
template<template<typename> class ImageT, typename T>
std::unique_ptr<ImageT<T>> imread(const std::string& filename) {
  using fpath = std::filesystem::path;
  auto name = fpath(filename);
  if (name.extension() == fpath(".ppm")) {
    auto ppm_file = PixelMapFileIO::open(filename);
    auto image = ppm_file->read_full_image();
    auto converted_image =
        PixelMapFileIO::extract_image_with_type_from_variant<ImageT, T>(image);
    return converted_image;
  }
  if (name.extension() == fpath(".pgx")) {
    [[maybe_unused]] auto pgx_file = PGXFileIO::open(filename);
  }
  throw std::logic_error(
      "Not fully implemented (for types other than ppm and pgx): "
      "ImageIO::imread");
}

// template<typename T>
// std::unique_ptr<UndefinedImage<T>> imread(const std::string& filename) {
//   return nullptr;
// }

/**
 * \brief      opens a image file pointer
 *
 * \return     A unique_ptr to the image file that is oppened
 */
std::unique_ptr<ImageFile> open(const std::string& filename);


template<typename T>
std::unique_ptr<Image<T>> read(ImageFile& image_file) {
  switch (image_file.get_type()) {
    case ImageFileType::PixelMap: {
      auto image = dynamic_cast<PixelMapFile&>(image_file).read_full_image();
      return PixelMapFileIO::extract_image_with_type_from_variant<RGBImage, T>(
          image);
    }
    case ImageFileType::PGX: {
      auto variant_image = dynamic_cast<PGXFile&>(image_file).read_full_image();
      auto image =
          std::visit(PGXFileIO::UndefinedImageVisitor<T>(), variant_image);
      return image;
      // return nullptr;
    }
  }
}

/**
 * \brief      Function to read a image from a file
 *
 * \param      image_file  A oppened image file (using ImageIO::open(filename))
 *
 * \tparam     ImageT    The expected return image class
 * \tparam     T         The image value container in the ImageT
 *
 * \return     returns a std::unique_ptr to ImageT<T> read from file
 */
template<template<typename> class ImageT, typename T>
std::unique_ptr<ImageT<T>> read(ImageFile& image_file) {
  // switch (image_file.get_type()) {
  //   case ImageFileType::PixelMap: {
  //     auto image = dynamic_cast<PixelMapFile&>(image_file).read_full_image();
  //     auto converted_image =
  //         PixelMapFileIO::extract_image_with_type_from_variant<ImageT, T>(
  //             image);
  //     return converted_image;
  //   }
  //   case ImageFileType::PGX: {
  //     // if constexpr (std::is_same_v<ImageT<T>, UndefinedImage<T>>) {
  //     //   return
  //     // }
  //     return nullptr;
  //   }
  // }

  using namespace ImageColorSpaceConversion;
  auto image = read<T>(image_file);

  if constexpr (std::is_same_v<ImageT<T>, UndefinedImage<T>>) {
    return std::unique_ptr<ImageT<T>>(static_cast<UndefinedImage<T>*>(image.release()));
  } else {
    if (image->get_type() == ImageT<T>::image_type) {
      return std::unique_ptr<ImageT<T>>(static_cast<ImageT<T>*>(image.release()));
    }

    switch (image->get_type()) {
      case ImageType::RGB: {
        return std::make_unique<ImageT<T>>(
            std::move(convert::to<ImageT>(dynamic_cast<RGBImage<T>&>(*image))));
      }
      case ImageType::BT601: {
        return std::make_unique<ImageT<T>>(
            std::move(convert::to<ImageT>(dynamic_cast<BT601Image<T>&>(*image))));
      }
      case ImageType::Undefined: {
        //
      }    
  }

  }
  // return convert::to<ImageT<T>>(image);

  // throw std::logic_error("Not fully implemented: ImageIO::read");
}


}  // namespace ImageIO

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGEIO_H__ */