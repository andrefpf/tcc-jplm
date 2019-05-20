#include "ImageChannel.h"
#include "Metrics.h"

namespace ImageChannelUtils {

template<typename T>
ImageChannel<typename std::make_unsigned<T>::type> absolute(
    const ImageChannel<T>& image_channel) {
  if constexpr (std::is_unsigned<T>())
    return image_channel;

  auto absolute_image = ImageChannel<typename std::make_unsigned<T>::type>(
      image_channel.width, image_channel.height, image_channel.max_bpp - 1);
  auto image_channel_ptr = image_channel.data();
  auto absolute_image_ptr = absolute_image.data();

  for (std::size_t i = 0; i < image_channel.getnumber_of_pixels; ++i) {
    *absolute_image_ptr = std::abs(*image_channel_ptr);
    absolute_image_ptr++;
    image_channel_ptr++;
  }
}

template<typename T>
double get_sum_of_squared_errors(const ImageChannel<T>& original_channel,
    const ImageChannel<T>& encoded_channel) {
  auto differences_image = original_channel - encoded_channel;
  auto error_vector = differences_image.as_raster_vector();
  return Metrics::get_sum_of_squared_errors<typename std::make_signed<T>::type>(
      error_vector);
}

template<typename T>
double get_mse(const ImageChannel<T>& original_channel,
    const ImageChannel<T>& encoded_channel) {
  auto sse = get_sum_of_squared_errors<T>(original_channel, encoded_channel);
  auto size = original_channel.get_number_of_pixels();
  return sse / static_cast<double>(size);
}

template<typename T>
double get_peak_signal_to_noise_ratio(const ImageChannel<T>& original_channel,
    const ImageChannel<T>& encoded_channel) {
  auto max_value =
      std::pow(2.0, static_cast<double>(original_channel.get_bpp())) - 1;
  return 10.0 * std::log10((max_value * max_value) /
                           get_mse(original_channel, encoded_channel));
}
}  // namespace ImageChannelUtils