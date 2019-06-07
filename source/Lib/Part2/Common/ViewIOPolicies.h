#ifndef VIEWIOPOLICIES_H__
#define VIEWIOPOLICIES_H__

#include "Lib/Part2/Common/CommonExceptions.h"
#include "Lib/Utils/Image/Image.h"

template<typename T>
class ViewIOPolicy {
 public:
  ViewIOPolicy() = default;
  ~ViewIOPolicy() = default;

  virtual T get_value_at(const Image<T>* image,
      const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    if (!image) {
      throw ViewExceptions::ImageWasNotInitialyzedException();
    }
    if (channel >= image->get_number_of_channels()) {
      throw ViewExceptions::InvalidNumberOfChannelsException();
    }
    return image->get_pixel_at(channel, coordinate);
  };
};


//this policy seems to make sense only for MuLELf
template<typename T>
class LevelDCacheViewIOPolicy : public ViewIOPolicy<T> {
 protected: 
 	std::size_t begin = 0;
 	std::size_t end = 0;

 	bool is_loaded(const std::size_t i) const {
 		if( begin <=i && i < end) {
 			return true;
 		}
 		return false;
 	}

 	std::pair<std::size_t, std::size_t> get_ajusted_coordinate(const std::pair<std::size_t, std::size_t>& coordinate) const {
 		const auto& [i, j] = coordinate;
 		return std::make_pair(i+begin, j);
 	}

 public:
  LevelDCacheViewIOPolicy() = default;
  ~LevelDCacheViewIOPolicy() = default;



  virtual T get_value_at(const Image<T>* image,
      const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) const override {
    if (!image) {
      throw ViewExceptions::ImageWasNotInitialyzedException();
    }
    if (channel >= image->get_number_of_channels()) {
      throw ViewExceptions::InvalidNumberOfChannelsException();
    }

    if (!is_loaded(std::get<0>(coordinate))) {
    	//must load the image
    }

    return image->get_pixel_at(channel, get_ajusted_coordinate(coordinate));
  };


  
};

#endif /* end of include guard: VIEWIOPOLICIES_H__ */