#ifndef VIEWIOPOLICIES_H__
#define VIEWIOPOLICIES_H__

#include <queue>
#include "Lib/Part2/Common/CommonExceptions.h"
#include "Lib/Part2/Common/View.h"
#include "Lib/Utils/Image/Image.h"

template<typename T>
class ViewIOPolicy {
 public:
  ViewIOPolicy() = default;
  ~ViewIOPolicy() = default;


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) = 0;
};


template<typename T>
class ViewIOPolicyLimitlessMemory : public ViewIOPolicy<T> {
 public:
  ViewIOPolicyLimitlessMemory() = default;
  ~ViewIOPolicyLimitlessMemory() = default;


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) override {
    view.load_image();
    return view.get_value_at(channel, coordinate);
  }
};


template<typename T>
class ViewIOPolicyOneAtATime : public ViewIOPolicy<T> {
 protected:
  View<T>* last = nullptr;


 public:
  ViewIOPolicyOneAtATime() = default;
  ~ViewIOPolicyOneAtATime() = default;


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) override {
    if (last != nullptr && last != &view) {
      last->release_image();
    }
    view.load_image();
    last = &view;
    return view.get_value_at(channel, coordinate);
  }
};


template<typename T>
class ViewIOPolicyQueue : public ViewIOPolicy<T> {
 protected:
  std::deque<View<T>*> queue;


  bool is_loaded(const View<T>* view) const {
    if (std::find(queue.begin(), queue.end(), view) == queue.end()) {
      return false;
    }
    return true;
  }


  void release_view_image() {
    if (queue.size() > 0) {
      auto ref_to_view = queue.front();
      ref_to_view->release_image();
      queue.pop_front();
    }
  }
};


template<typename T>
class ViewIOPolicyLimitedNumberOfViews : public ViewIOPolicyQueue<T> {
 protected:
  std::size_t max_views = 3;
  std::size_t current_views = 0;


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) override {
    if (!this->is_loaded(&view)) {
      if (current_views + 1 > max_views) {
        this->release_view_image();
      } else {
        current_views++;
      }
      this->queue.push_back(&view);
      view.load_image();
    }
    return view.get_value_at(channel, coordinate);
  }
};


template<typename T>
class ViewIOPolicyLimitedMemory : public ViewIOPolicyQueue<T> {
 protected:
  //value equivalent to 9 1920x1080 views with uint16_t
  std::size_t max_bytes = 111974400;  //about 14 MB
  std::size_t current_bytes = 0;


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) override {
    if (!this->is_loaded(&view)) {
      auto expected_number_of_bytes =
          view.get_number_of_pixels() * 3 * sizeof(T);
      if (current_bytes + expected_number_of_bytes > max_bytes) {
        this->release_view_image();
      } else {
        current_bytes += expected_number_of_bytes;
      }
      this->queue.push_back(&view);
      view.load_image();
    }
    return view.get_value_at(channel, coordinate);
  }
};


// //this policy seems to make sense only for MuLELf
// template<typename T>
// class LevelDCacheViewIOPolicy : public ViewIOPolicy<T> {
//  protected:
//  	std::size_t begin = 0;
//  	std::size_t end = 0;

//  	bool is_loaded(const std::size_t i) const {
//  		if( begin <=i && i < end) {
//  			return true;
//  		}
//  		return false;
//  	}

//  	std::pair<std::size_t, std::size_t> get_ajusted_coordinate(const std::pair<std::size_t, std::size_t>& coordinate) const {
//  		const auto& [i, j] = coordinate;
//  		return std::make_pair(i+begin, j);
//  	}

//  public:
//   LevelDCacheViewIOPolicy() = default;
//   ~LevelDCacheViewIOPolicy() = default;


//   virtual T get_value_at(const Image<T>* image,
//       const std::size_t channel,
//       const std::pair<std::size_t, std::size_t>& coordinate) const override {
//     if (!image) {
//       throw ViewExceptions::ImageWasNotInitialyzedException();
//     }
//     if (channel >= image->get_number_of_channels()) {
//       throw ViewExceptions::InvalidNumberOfChannelsException();
//     }

//     if (!is_loaded(std::get<0>(coordinate))) {
//     	//must load the image
//     }

//     return image->get_pixel_at(channel, get_ajusted_coordinate(coordinate));
//   };


// };

#endif /* end of include guard: VIEWIOPOLICIES_H__ */