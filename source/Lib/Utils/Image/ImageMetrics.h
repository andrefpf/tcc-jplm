#ifndef IMAGEMETRICS_H__
#define IMAGEMETRICS_H__

#include <iomanip>  //for setprecision
#include "Image.h"
#include "ImageChannelUtils.h"
#include "Metrics.h"

namespace ImageMetrics {
enum class Available { PSNR, MSE };

template<typename T>
std::vector<double> get_peak_signal_to_noise_ratio(
    const Image<T>& original_image, const Image<T>& encoded_image) {
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
//PSNR Visitor
struct PSNRPrinter {
  template<template<typename> class ImageT0, template<typename> class ImageT1,
      typename T0, typename T1>
  void operator()(const std::unique_ptr<ImageT0<T0>>& original,
      const std::unique_ptr<ImageT1<T1>>& encoded) {
    static_assert(std::is_base_of<Image<T0>, ImageT0<T0>>(),
        "Original image must be an image!");
    static_assert(std::is_base_of<Image<T1>, ImageT1<T1>>(),
        "Encoded image must be an image!");
    if constexpr (std::is_same<T0, T1>::value) {
      if (original->get_type() == encoded->get_type()) {
        return print_psnrs(*(static_cast<Image<T0>*>(original.get())),
            *(static_cast<Image<T1>*>(encoded.get())));
      }
      std::cerr << "Images are from different types..." << std::endl;
    }
    different_representations_message<T0, T1>();
  }
};


struct PSNRGetter {
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
  void operator()(const std::unique_ptr<Image<T1>>& original,
      const std::unique_ptr<Image<T2>>& encoded) {
    if constexpr (std::is_same<T1, T2>::value) {
      return get_sum_of_squared_errors(*(original.get()), *(encoded.get()));
    }
    different_representations_message<T1, T2>();
  }
};

}  // namespace visitors


}  // namespace ImageMetrics

#endif /* end of include guard: IMAGEMETRICS_H__ */