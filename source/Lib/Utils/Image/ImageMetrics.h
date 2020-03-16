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

/** \file     ImageMetrics.h
 *  \brief    Color Space conversion class
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-07
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGEMETRICS_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGEMETRICS_H__

#include <iomanip>  //for setprecision
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageChannelUtils.h"
#include "Lib/Utils/Image/ImageColorSpacesConversor.h"
#include "Lib/Utils/Image/Metrics.h"


namespace ImageMetrics {
enum class Available { PSNR, MSE, SSE, MAX_ABS_ERROR };

template<typename T>
void check_image_properties(
    const Image<T>& original_image, const Image<T>& encoded_image) {
  if (original_image.get_type() != encoded_image.get_type()) {
    throw MetricsExceptions::DifferentColorSpaceImagesException();
  }
  auto same_width = true;
  auto same_height = true;
  if (original_image.get_width() != encoded_image.get_width()) {
    same_width = false;
  }
  if (original_image.get_height() != encoded_image.get_height()) {
    same_height = false;
  }
  if (!same_width && !same_height) {
    throw MetricsExceptions::DifferentSizeImagesException();
  }
  if (!same_width) {
    throw MetricsExceptions::DifferentWidthImagesException();
  }
  if (!same_height) {
    throw MetricsExceptions::DifferentHeightImagesException();
  }
  if (original_image.get_bpp() != encoded_image.get_bpp()) {
    throw MetricsExceptions::DifferentBppImagesException();
  }
}


template<typename T>
std::vector<std::size_t> get_maximum_absolute_error(
    const Image<T>& original_image, const Image<T>& encoded_image) {
  check_image_properties(original_image, encoded_image);
  auto number_of_channels = original_image.get_number_of_channels();
  auto rect_vector = std::vector<std::size_t>();
  rect_vector.reserve(number_of_channels);

  for (auto i = decltype(number_of_channels){0}; i < number_of_channels; ++i) {
    rect_vector.emplace_back(ImageChannelUtils::get_maximum_absolute_error(
        original_image.get_channel(i), encoded_image.get_channel(i)));
  }

  return rect_vector;
}


template<typename T>
std::vector<double> get_peak_signal_to_noise_ratio(
    const Image<T>& original_image, const Image<T>& encoded_image) {
  check_image_properties(original_image, encoded_image);
  auto number_of_channels = original_image.get_number_of_channels();
  auto rect_vector = std::vector<double>();
  rect_vector.reserve(number_of_channels);

  for (decltype(number_of_channels) i = 0; i < number_of_channels; ++i) {
    rect_vector.emplace_back(ImageChannelUtils::get_peak_signal_to_noise_ratio(
        original_image.get_channel(i), encoded_image.get_channel(i)));
  }

  return rect_vector;
}


template<typename T>
std::vector<double> get_mse(
    const Image<T>& original_image, const Image<T>& encoded_image) {
  check_image_properties(original_image, encoded_image);
  auto number_of_channels = original_image.get_number_of_channels();
  auto rect_vector = std::vector<double>();
  rect_vector.reserve(number_of_channels);

  for (decltype(number_of_channels) i = 0; i < number_of_channels; ++i) {
    rect_vector.emplace_back(ImageChannelUtils::get_mse(
        original_image.get_channel(i), encoded_image.get_channel(i)));
  }

  return rect_vector;
}


template<typename T>
std::vector<double> get_sum_of_squared_errors(
    const Image<T>& original_image, const Image<T>& encoded_image) {
  check_image_properties(original_image, encoded_image);
  auto number_of_channels = original_image.get_number_of_channels();
  auto rect_vector = std::vector<double>();
  rect_vector.reserve(number_of_channels);

  for (decltype(number_of_channels) i = 0; i < number_of_channels; ++i) {
    rect_vector.emplace_back(ImageChannelUtils::get_sum_of_squared_errors(
        original_image.get_channel(i), encoded_image.get_channel(i)));
  }

  return rect_vector;
}


template<typename T1, typename T2>
void different_representations_message() {
  std::cerr << "Images have different representations (" << typeid(T1).name()
            << " vs. " << typeid(T2).name() << ")." << std::endl;
}


template<typename T>
void print_psnrs(const Image<T>& original, const Image<T>& encoded) {
  auto psnrs = get_peak_signal_to_noise_ratio(original, encoded);
  auto channel_names = original.get_channel_names();
  auto c = 0;
  auto show_names = false;
  if (channel_names.size() == psnrs.size())
    show_names = true;

  for (const auto psnr : psnrs) {
    std::cout << "PSNR";
    if (show_names)
      std::cout << " " << channel_names[c++];
    std::cout << ": " << psnr << " dB" << std::endl;
  }
}


template<typename T>
void print_maximum_absolute_errors(
    const Image<T>& original, const Image<T>& encoded) {
  auto maximum_absolute_errors = get_maximum_absolute_error(original, encoded);
  for (const auto maximum_absolute_error : maximum_absolute_errors) {
    std::cout << "Max Abs Error: " << maximum_absolute_error << std::endl;
  }
}


template<typename T>
void print_mses(const Image<T>& original, const Image<T>& encoded) {
  auto mses = get_mse(original, encoded);
  for (const auto mse : mses) {
    std::cout << "MSE: " << mse << std::endl;
  }
}


template<typename T>
void print_sses(const Image<T>& original, const Image<T>& encoded) {
  auto sses = get_sum_of_squared_errors(original, encoded);
  for (const auto sse : sses) {
    std::cout << "SSE: " << sse << std::endl;
  }
}


namespace visitors {

template<template<typename> class ImageT0, template<typename> class ImageT1,
    typename T0, typename T1>
void image_parameter_check() {
  static_assert(std::is_base_of<Image<T0>, ImageT0<T0>>(),
      "Original image must be an image!");
  static_assert(std::is_base_of<Image<T1>, ImageT1<T1>>(),
      "Encoded image must be an image!");
}


// struct ColorSpaceVisitor {
//     template<template<typename> class ImageT0, template<typename> class ImageT1,
//       typename T0, typename T1>
//   void operator()(const std::unique_ptr<ImageT0<T0>>& original,
//       const std::unique_ptr<ImageT1<T1>>& encoded) {
//     image_parameter_check<ImageT0, ImageT1, T0, T1>();
//     if constexpr (std::is_same<T0, T1>::value) {
//       if (original->get_type() == encoded->get_type()) {
//         return print_psnrs(*(static_cast<Image<T0>*>(original.get())),
//             *(static_cast<Image<T1>*>(encoded.get())));
//       }
//       std::cerr << "Images are from different types..." << std::endl;
//     }
//     different_representations_message<T0, T1>();
//   }

//   template<template<typename> class ImageT0, template<typename> class ImageT1,
//       typename T0, typename T1>
//   auto operator()(const std::unique_ptr<ImageT0<T0>>& original,
//       const std::unique_ptr<ImageT1<T1>>& encoded, const ImageType type) {
//     using namespace ImageColorSpaceConversion;
//     switch (type) {
//       case ImageType::RGB:
//         operator()(
//             convert::to<RGBImage>(original), convert::to<RGBImage>(encoded));
//         break;
//       case ImageType::BT601:
//         operator()(convert::to<BT601Image>(original),
//             convert::to<BT601Image>(encoded));
//         break;
//       case ImageType::BT709:
//         operator()(convert::to<BT709Image>(original),
//             convert::to<BT709Image>(encoded));
//         break;
//       case ImageType::BT2020:
//         operator()(convert::to<BT2020Image>(original),
//             convert::to<BT2020Image>(encoded));
//         break;
//       default:
//         std::cerr << "Error default" << std::endl;
//     }
//   }
//  protected:
//   ColorSpaceVisitor() = default;
// };

//PSNR Visitor
struct PSNRPrinter {
  // using ColorSpaceVisitor::operator();

  template<template<typename> class ImageT0, template<typename> class ImageT1,
      typename T0, typename T1>
  void operator()(const std::unique_ptr<ImageT0<T0>>& original,
      const std::unique_ptr<ImageT1<T1>>& encoded) {
    image_parameter_check<ImageT0, ImageT1, T0, T1>();
    if constexpr (std::is_same<T0, T1>::value) {
      if (original->get_type() == encoded->get_type()) {
        return print_psnrs(*(static_cast<Image<T0>*>(original.get())),
            *(static_cast<Image<T1>*>(encoded.get())));
      }
      std::cerr << "Images are from different types..." << std::endl;
    }
    different_representations_message<T0, T1>();
  }

  template<template<typename> class type_class,
      template<typename> class ImageT0, template<typename> class ImageT1,
      typename T0, typename T1>
  void operator()(const std::unique_ptr<ImageT0<T0>>& original,
      const std::unique_ptr<ImageT1<T1>>& encoded) {
    using namespace ImageColorSpaceConversion;
    operator()(
        convert::to<type_class>(encoded), convert::to<type_class>(original));
  }
};


struct MaximumErrorPrinter {
  template<template<typename> class ImageT0, template<typename> class ImageT1,
      typename T0, typename T1>
  void operator()(const std::unique_ptr<ImageT0<T0>>& original,
      const std::unique_ptr<ImageT1<T1>>& encoded) {
    // image_parameter_check<ImageT0, ImageT1, T0, T1>();
    if constexpr (std::is_same<T0, T1>::value) {
      if (original->get_type() == encoded->get_type()) {
        return print_maximum_absolute_errors(
            *(static_cast<Image<T0>*>(original.get())),
            *(static_cast<Image<T1>*>(encoded.get())));
      }
      std::cerr << "Images are from different types..." << std::endl;
    }
    different_representations_message<T0, T1>();
  }


  template<template<typename> class type_class,
      template<typename> class ImageT0, template<typename> class ImageT1,
      typename T0, typename T1>
  void operator()(const std::unique_ptr<ImageT0<T0>>& original,
      const std::unique_ptr<ImageT1<T1>>& encoded) {
    using namespace ImageColorSpaceConversion;
    operator()(
        convert::to<type_class>(encoded), convert::to<type_class>(original));
  }
};


struct PSNRGetter {
  // using ColorSpaceVisitor::operator();

  template<template<typename> class ImageT0, template<typename> class ImageT1,
      typename T0, typename T1>
  std::vector<double> operator()(const std::unique_ptr<ImageT0<T0>>& original,
      const std::unique_ptr<ImageT1<T1>>& encoded) {
    static_assert(std::is_base_of<Image<T0>, ImageT0<T0>>(),
        "Original image must be an image!");
    static_assert(std::is_base_of<Image<T1>, ImageT1<T1>>(),
        "Encoded image must be an image!");
    if constexpr (std::is_same<T0, T1>::value) {
      return get_peak_signal_to_noise_ratio(
          *(original.get()), *(encoded.get()));
    }
    return std::vector<double>();
  }
};


//MSE Visitor
struct MSEPrinter {
  template<typename T1, typename T2>
  void operator()(const std::unique_ptr<Image<T1>>& original,
      const std::unique_ptr<Image<T2>>& encoded) {
    if constexpr (std::is_same<T1, T2>::value) {
      return print_mses(*(original.get()), *(encoded.get()));
    }
    different_representations_message<T1, T2>();
  }
};


struct MSEGetter {
  template<template<typename> class ImageT0, template<typename> class ImageT1,
      typename T0, typename T1>
  std::vector<double> operator()(const std::unique_ptr<ImageT0<T0>>& original,
      const std::unique_ptr<ImageT1<T1>>& encoded) {
    static_assert(std::is_base_of<Image<T0>, ImageT0<T0>>(),
        "Original image must be an image!");
    static_assert(std::is_base_of<Image<T1>, ImageT1<T1>>(),
        "Encoded image must be an image!");
    if constexpr (std::is_same<T0, T1>::value) {
      return get_mse(*(original.get()), *(encoded.get()));
    }
    return std::vector<double>();
  }
};


//SSE Visitor
struct SSEPrinter {
  template<typename T1, typename T2>
  void operator()(const std::unique_ptr<Image<T1>>& original,
      const std::unique_ptr<Image<T2>>& encoded) {
    if constexpr (std::is_same<T1, T2>::value) {
      return print_sses(*(original.get()), *(encoded.get()));
    }
    different_representations_message<T1, T2>();
  }
};

struct SSEGetter {
  template<typename T1, typename T2>
  std::vector<double> operator()(const std::unique_ptr<Image<T1>>& original,
      const std::unique_ptr<Image<T2>>& encoded) {
    if constexpr (std::is_same<T1, T2>::value) {
      return get_sum_of_squared_errors(*(original.get()), *(encoded.get()));
    }
    different_representations_message<T1, T2>();
  }
};

}  // namespace visitors


}  // namespace ImageMetrics

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGEMETRICS_H__ */
