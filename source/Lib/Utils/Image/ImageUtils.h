#ifndef IMAGEUTILS_H__
#define IMAGEUTILS_H__

#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageColorSpacesConversor.h"

namespace ImageUtils {

	template<typename Tout, typename Tin>
	constexpr void check_image_container_requisites(const std::unique_ptr<Image<Tin>>& image_in) {
		if constexpr(std::is_same<Tin, Tout>::value){
			//copy image from image_in and return
			return; //if the types are the same, there is no need for further checking
		}
		if (image_in->get_bpp() > sizeof(Tout)*8) {
			//throw ImageUtilsExceptions::ContainerHasFewerBitsThanNeededException();
		}
		if constexpr (std::is_signed<Tin>::value && std::is_unsigned<Tout>::value) {
			//check in to garantee all values >= 0;
		}
		if constexpr (std::numeric_limits<Tin>::max() > std::numeric_limits<Tout>::max()) {
			//check for maximum value in Tin?
		}
	}

 
	// template<class ImageClass, typename Tout, typename Tin>
	 template<template<typename> class ImageTout, template<typename> class ImageTin,
      typename Tout, typename Tin>
	std::unique_ptr<ImageTout<Tout>> get_image_with_new_container_type(const std::unique_ptr<ImageTin<Tin>>& image_in) {
		check_image_container_requisites<Tout, Tin>(image_in);
		// auto image_out = std::make_unique<ImageTout<Tout>>(image_in->get_width(), image_in->get_height(), image_in->get_bpp());
		auto intermediary_image = ImageColorSpaceConversion::convert::to<ImageTout>(image_in);
		if constexpr(std::is_same<Tin, Tout>::value) {
			return intermediary_image;
		}
		auto output_image = std::make_unique<ImageTout<Tout>>(image_in->get_width(), image_in->get_height(), image_in->get_bpp());
		auto output_image_iterator = output_image->begin();

		for(const auto& channel: intermediary_image) {
			auto output_image_channel_iterator = output_image_iterator->begin();
			for(const auto value: channel) {
				*output_image_channel_iterator = static_cast<Tout>(value);
				output_image_channel_iterator++;
			}
			output_image_iterator++;
		}

		return std::move(output_image);

	}


}

#endif /* end of include guard: IMAGEUTILS_H__ */