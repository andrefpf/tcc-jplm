#ifndef VIEWIOPOLICIES_H__
#define VIEWIOPOLICIES_H__

#include <queue>
#include <unordered_set>
#include "Lib/Part2/Common/CommonExceptions.h"
#include "Lib/Part2/Common/View.h"
#include "Lib/Utils/Image/Image.h"

template<typename T>
class ViewIOPolicy {
 protected:
  virtual void load_image_if_necessary(View<T>& view) = 0;

 public:
  ViewIOPolicy() = default;
  ~ViewIOPolicy() = default;


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) {
    load_image_if_necessary(view);
    return view.get_value_at(channel, coordinate);
  }


  virtual const Image<T>& get_image_at(View<T>& view) {
    load_image_if_necessary(view);
    return *view.get_image_ptr();
  }
};


template<typename T>
class ViewIOPolicyLimitlessMemory : public ViewIOPolicy<T> {
 public:
  ViewIOPolicyLimitlessMemory() = default;


  ~ViewIOPolicyLimitlessMemory() = default;


  void load_image_if_necessary(View<T>& view) override {
    if (!view.has_image())
      view.load_image();
  }
};


template<typename T>
class ViewIOPolicyOneAtATime : public ViewIOPolicy<T> {
 protected:
  View<T>* last = nullptr;

 public:
  ViewIOPolicyOneAtATime() = default;


  ~ViewIOPolicyOneAtATime() = default;


  void load_image_if_necessary(View<T>& view) {
    if (last != nullptr && last != &view) {
      last->release_image();
    }
    view.load_image();
    last = &view;
  }
};


template<typename T>
class ViewIOPolicyQueue : public ViewIOPolicy<T> {
 protected:
  std::deque<View<T>*> queue;
  std::unordered_set<const View<T>*> set;


  bool is_loaded(const View<T>* view) const {
    return set.count(view) > 0;
  }


  void release_view_image() {
    if (queue.size() > 0) {
      auto ref_to_view = queue.front();
      ref_to_view->release_image();
      queue.pop_front();
      set.erase(ref_to_view);
    }
  }
};


template<typename T>
class ViewIOPolicyLimitedNumberOfViews : public ViewIOPolicyQueue<T> {
 protected:
  std::size_t max_views = 3;
  std::size_t current_views = 0;


  void load_image_if_necessary(View<T>& view) override {
    if (!this->is_loaded(&view)) {
      if (current_views + 1 > max_views) {
        this->release_view_image();
      } else {
        current_views++;
      }
      this->queue.push_back(&view);
      this->set.insert(&view);
      view.load_image();
    }
  }
};


template<typename T>
class ViewIOPolicyLimitedMemory : public ViewIOPolicyQueue<T> {
 protected:
  //value equivalent to 9 1920x1080 views with uint16_t
  std::size_t max_bytes = 111974400;  //about 14 MB
  std::size_t current_bytes = 0;

  virtual void load_image_if_necessary(View<T>& view) override {
    if (!this->is_loaded(&view)) {
      auto expected_number_of_bytes =
          view.get_number_of_pixels() * 3 * sizeof(T);
      if (current_bytes + expected_number_of_bytes > max_bytes) {
        this->release_view_image();
      } else {
        current_bytes += expected_number_of_bytes;
      }
      this->queue.push_back(&view);
      this->set.insert(&view);
      view.load_image();
    }
  }

 public:
  void set_max_bytes(std::size_t bytes) {
    max_bytes = bytes;
    while (current_bytes > max_bytes) {
      auto expected_number_of_bytes =
          this->queue.front()->get_number_of_pixels() * 3 * sizeof(T);
      this->release_view_image();
      current_bytes -= expected_number_of_bytes;
    }
  }


  auto get_max_bytes() const {
    return max_bytes;
  }


  auto get_current_bytes() const {
    return current_bytes;
  }
};

#endif /* end of include guard: VIEWIOPOLICIES_H__ */