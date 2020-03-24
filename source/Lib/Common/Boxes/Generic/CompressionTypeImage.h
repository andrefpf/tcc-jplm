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

/** \file     CompressionTypeImage.h
 *  \brief    This file keeps the declaration of CompressionTypeImage enumeration
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-22
 */


#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_COMPRESSIONTYPEIMAGE_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_COMPRESSIONTYPEIMAGE_H__

#include "CompressionType.h"


/**
 * \brief      Enum class for representing the compression type of images.
 * \note       The values that are not defined in the present enum are reserved for ISO use.
 */
enum class CompressionTypeImage : compression_type_data {
  // ISO/IEC 15444-2:2004 defines the following values:
  Uncompressed = 0,
  Modified_Huffman = 1,
  Modified_READ = 2,
  Modified_Modified_READ = 3,
  JBIG_bilevel = 4,
  JPEG =
      5,  //!< Indicates that <a href="https://jpeg.org/jpeg/index.html">JPEG</a> is used to encode the image. It is coded as \link compression_type_data \endlink value 5
  JPEG_LS =
      6,  //!< Indicates that <a href="https://jpeg.org/jpegls/index.html">JPEG LS</a> is used to encode the image. It is coded as \link compression_type_data \endlink value 6
  JPEG_2000 =
      7,  //!< Indicates that <a href="https://jpeg.org/jpeg2000/">JPEG 2000</a> is used to encode the image. It is coded as \link compression_type_data \endlink value 0
  JBIG2 = 8,
  JBIG_any = 9,
  // ISO/IEC 15444-2:2004/AMD 3 defines the following values:
  RLE = 10,
  JPEG_XR =
      11,  //!< Indicates that <a href="https://jpeg.org/jpegxr/index.html">JPEG XR</a> is used to encode the image. It is coded as \link compression_type_data \endlink value 11
};

//\todo find and include the values for JPEG_XT and JPEG_XS
//XS comment: Indicates that <a href="https://jpeg.org/jpegxs/index.html">JPEG XS</a> is used to encode the image. It is coded as \link compression_type_data \endlink value 5
//XT comment: Indicates that <a href="https://jpeg.org/jpegxt/index.html">JPEG XT</a> is used to encode the image. It is coded as \link compression_type_data \endlink value 3

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_COMPRESSIONTYPEIMAGE_H__ */