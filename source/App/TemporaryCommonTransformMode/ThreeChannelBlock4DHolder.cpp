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

/** \file     ThreeChannelBlock4DHolder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

/*
 * @Author: Ismael Seidel
 * @Date:   2019-03-26 12:15:04
 * @Last Modified by:   Ismael Seidel
 * @Last Modified time: 2019-05-14 14:18:49
 */

#include "ThreeChannelBlock4DHolder.h"

void ThreeChannelBlock4DHolder::shift_uv_planes(int shift, int position_t,
                                                int position_s) const {
  for (auto block : channels_ptr_array) {
    block->shift_data_from_uv_plane_at(shift, position_t, position_s);
  }
}


void ThreeChannelBlock4DHolder::clip(int min, int max) const {
  for (auto block : channels_ptr_array) {
    block->clip(min, max);
  }
}


void ThreeChannelBlock4DHolder::set_block_dimensions(int length_t, int length_s,
                                                     int length_v,
                                                     int length_u) const {
  for (auto block : channels_ptr_array) {
    block->set_dimension({length_t, length_s, length_v, length_u});
  }
}


void ThreeChannelBlock4DHolder::set_blocks_to_zero() const {
  for (auto block : channels_ptr_array) {
    block->fill_with_zeros();
  }
}


void ThreeChannelBlock4DHolder::add_constant_to_pels(int value) const {
  for (auto block : channels_ptr_array) {
    (*block) += value;
  }
}


void ThreeChannelBlock4DHolder::get_data_from_lightfield(LightField& lightfield,
                                                         int verticalView,
                                                         int horizontalView,
                                                         int viewLine,
                                                         int viewColumn) {
  

  lightfield.ReadBlock4DfromLightField(&std::get<0>(channels), verticalView,
                                       horizontalView, viewLine, viewColumn, 0);
  lightfield.ReadBlock4DfromLightField(&std::get<1>(channels), verticalView,
                                       horizontalView, viewLine, viewColumn, 1);
  lightfield.ReadBlock4DfromLightField(&std::get<2>(channels), verticalView,
                                       horizontalView, viewLine, viewColumn, 2);
}


void ThreeChannelBlock4DHolder::set_data_into_lightfield(LightField& lightfield,
                                                         int verticalView,
                                                         int horizontalView,
                                                         int viewLine,
                                                         int viewColumn) {
  lightfield.WriteBlock4DtoLightField(&(std::get<2>(channels)), verticalView,
                                      horizontalView, viewLine, viewColumn, 2);
  lightfield.WriteBlock4DtoLightField(&(std::get<1>(channels)), verticalView,
                                      horizontalView, viewLine, viewColumn, 1);
  lightfield.WriteBlock4DtoLightField(&(std::get<0>(channels)), verticalView,
                                      horizontalView, viewLine, viewColumn, 0);
}


void ThreeChannelBlock4DHolder::copy_to(ThreeChannelBlock4DHolder& dest) const {
  if (&dest == this) return;
  fprintf(stderr, "Not implemented yet...\n");
  // I need to check the copy assignment in the blocks..
}


void ThreeChannelBlock4DHolder::resize_blocks(int length_t, int length_s,
                                              int length_v,
                                              int length_u) const {
  // for (auto block : channels_ptr_array) {
  //   block->resize_avoiding_free(length_t, length_s, length_v, length_u);
  // }
}


void RGBBlock4DHolder::convert_to(YCbCrBlock4DHolder& dest_color_space) const {
  // using namespace ColorSpaces;
  // const Block4D& R = std::get<RGB::R>(channels);
  // const Block4D& G = std::get<RGB::G>(channels);
  // const Block4D& B = std::get<RGB::B>(channels);
  // Block4D& Y = std::get<YCbCr::Y>(dest_color_space.channels);
  // Block4D& Cb = std::get<YCbCr::Cb>(dest_color_space.channels);
  // Block4D& Cr = std::get<YCbCr::Cr>(dest_color_space.channels);

  // constexpr auto keep_dynamic_range = true;
  // auto converter_provider =
  //     ConversorProvider<block4DElementType, BT601Coefficients,
  //                       keep_dynamic_range>();

  // auto bits_per_pel = static_cast<uint32_t>(std::ceil(std::log2(scale)));
  // std::unique_ptr<GenericColorSpacesConverter<
  //     block4DElementType, BT601Coefficients, keep_dynamic_range> >
  //     convert(converter_provider.getConverter(bits_per_pel));

  // auto number_of_elements = R.get_number_of_elements();
  // for (decltype(number_of_elements) i = 0; i < number_of_elements; ++i) {
  //   // auto ycbcr = convert->to_YCbCr(
  //   //     std::make_tuple(R.mPixelData[i], G.mPixelData[i], B.mPixelData[i]));
  //   // Y.mPixelData[i] = std::get<YCbCr::Y>(ycbcr);
  //   // Cb.mPixelData[i] = std::get<YCbCr::Cb>(ycbcr);
  //   // Cr.mPixelData[i] = std::get<YCbCr::Cr>(ycbcr);
  //   double pixel =  0.299 * R.mPixelData[i] + 0.587 * G.mPixelData[i] + 0.114
  //   * B.mPixelData[i]; Y.mPixelData[i] = round(pixel); pixel = -0.16875 *
  //   R.mPixelData[i] -0.33126 * G.mPixelData[i] + 0.5 * B.mPixelData[i];
  //   Cb.mPixelData[i] = round(pixel) + (scale + 1)/2;
  //   pixel = 0.5 * R.mPixelData[i] -0.41869 * G.mPixelData[i] -0.08131  *
  //   B.mPixelData[i]; Cr.mPixelData[i] = round(pixel) + (scale + 1)/2;
  // }
}


void RGBBlock4DHolder::convert_to(YCoCgBlock4DHolder& dest_color_space) const {
  // using namespace ColorSpaces;
  // const Block4D& R = std::get<RGB::R>(channels);
  // const Block4D& G = std::get<RGB::G>(channels);
  // const Block4D& B = std::get<RGB::B>(channels);
  // Block4D& Y = std::get<YCoCg::Yy>(dest_color_space.channels);
  // Block4D& Co = std::get<YCoCg::Co>(dest_color_space.channels);
  // Block4D& Cg = std::get<YCoCg::Cg>(dest_color_space.channels);

  // auto number_of_elements = R.get_number_of_elements();
  // for (decltype(number_of_elements) i = 0; i < number_of_elements; ++i) {
  //   Co.mPixelData[i] = R.mPixelData[i] - B.mPixelData[i];
  //   auto t = B.mPixelData[i] + (Co.mPixelData[i] >> 1);
  //   Cg.mPixelData[i] = G.mPixelData[i] - t;
  //   Y.mPixelData[i] = t + (Cg.mPixelData[i] >> 1);
  //   Co.mPixelData[i] += (scale + 1) / 2;
  //   Cg.mPixelData[i] += (scale + 1) / 2;
  // }
}


void ThreeChannelBlock4DHolder::convert_to(
    ThreeChannelBlock4DHolder& dest_color_space) const {
  using namespace ColorSpaces;
  switch (dest_color_space.get_my_type()) {
    case ColorSpace::RGB:
      return convert_to(static_cast<RGBBlock4DHolder&>(dest_color_space));
    case ColorSpace::YCoCg:
      return convert_to(static_cast<YCoCgBlock4DHolder&>(dest_color_space));
    default:
      return convert_to(static_cast<YCbCrBlock4DHolder&>(dest_color_space));
  }
}


void YCbCrBlock4DHolder::convert_to(RGBBlock4DHolder& dest_color_space) const {
  // using namespace ColorSpaces;
  // const Block4D& Y = std::get<YCbCr::Y>(channels);
  // const Block4D& Cb = std::get<YCbCr::Cb>(channels);
  // const Block4D& Cr = std::get<YCbCr::Cr>(channels);
  // Block4D& R = std::get<RGB::R>(dest_color_space.channels);
  // Block4D& G = std::get<RGB::G>(dest_color_space.channels);
  // Block4D& B = std::get<RGB::B>(dest_color_space.channels);

  // constexpr auto keep_dynamic_range = true;
  // auto converter_provider =
  //     ConversorProvider<block4DElementType, BT601Coefficients,
  //                       keep_dynamic_range>();

  // auto bits_per_pel = static_cast<uint32_t>(std::ceil(std::log2(scale)));
  // std::unique_ptr<GenericColorSpacesConverter<
  //     block4DElementType, BT601Coefficients, keep_dynamic_range> >
  //     convert(converter_provider.getConverter(bits_per_pel));

  // auto number_of_elements = Y.get_number_of_elements();
  // for (decltype(number_of_elements) i = 0; i < number_of_elements; ++i) {
  //   // auto rgb = convert->to_RGB(
  //   //     std::make_tuple(Y.mPixelData[i], Cb.mPixelData[i], Cr.mPixelData[i]));
  //   // R.mPixelData[i] = std::get<0>(rgb);
  //   // G.mPixelData[i] = std::get<1>(rgb);
  //   // B.mPixelData[i] = std::get<2>(rgb);
  //   double pixel_Y = Y.mPixelData[i];
  //   //double pixel_Cb = Cb.mPixelData[n]-512;
  //   //double pixel_Cr = Cr.mPixelData[n]-512;
  //   double pixel_Cb = Cb.mPixelData[i]-(scale+1)/2;
  //   double pixel_Cr = Cr.mPixelData[i]-(scale+1)/2;
  //   R.mPixelData[i] = round(pixel_Y - 0.0000071525 * pixel_Cb + 1.4020 * pixel_Cr);
  //   G.mPixelData[i] = round(pixel_Y -0.34413 * pixel_Cb - 0.71414 * pixel_Cr);
  //   B.mPixelData[i] = round(pixel_Y + 1.7720 * pixel_Cb - 0.000040249 * pixel_Cr);

  // }
}


void YCbCrBlock4DHolder::convert_to(
    YCoCgBlock4DHolder& dest_color_space) const {
  fprintf(stderr, "Not implemented yet...\n");
  fprintf(stderr,
          "Conversion from YCbCr to YCoCg(%d) is not implemented yet...\n",
          dest_color_space.get_my_type());
}


void YCoCgBlock4DHolder::convert_to(RGBBlock4DHolder& dest_color_space) const {
  // using namespace ColorSpaces;
  // const Block4D& Y = std::get<YCoCg::Yy>(channels);
  // const Block4D& Co = std::get<YCoCg::Co>(channels);
  // const Block4D& Cg = std::get<YCoCg::Cg>(channels);
  // Block4D& R = std::get<RGB::R>(dest_color_space.channels);
  // Block4D& G = std::get<RGB::G>(dest_color_space.channels);
  // Block4D& B = std::get<RGB::B>(dest_color_space.channels);

  // auto number_of_elements = Y.get_number_of_elements();
  // for (decltype(number_of_elements) i = 0; i < number_of_elements; ++i) {
  //   Co.mPixelData[i] = Co.mPixelData[i] - (scale + 1) / 2;
  //   Cg.mPixelData[i] = Cg.mPixelData[i] - (scale + 1) / 2;
  //   auto t = Y.mPixelData[i] - (Cg.mPixelData[i] >> 1);
  //   G.mPixelData[i] = Cg.mPixelData[i] + t;
  //   B.mPixelData[i] = t - (Co.mPixelData[i] >> 1);
  //   R.mPixelData[i] = B.mPixelData[i] + Co.mPixelData[i];
  // }
}


void YCoCgBlock4DHolder::convert_to(
    YCbCrBlock4DHolder& dest_color_space) const {
  fprintf(stderr,
          "Conversion from YCoCg to YCbCr(%d) is not implemented yet...\n",
          dest_color_space.get_my_type());
}