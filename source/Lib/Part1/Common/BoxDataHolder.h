#ifndef JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__
#define JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__

#include <ostream>

template<typename T>
class BoxDataHolder {
 protected:
  T value;

 public:
  BoxDataHolder() : value(0){};


  BoxDataHolder(const T& value) : value(value){};


  virtual ~BoxDataHolder() {
  }


  T get_value() const {
    return value;
  }


  void set_value(T value) {
    this->value = value;
  }
};


template<typename T>
std::ostream& operator<<(
    std::ostream& stream, const BoxDataHolder<T>& box_data_holder) {
  stream << box_data_holder.get_value();
  return stream;
}

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__ */