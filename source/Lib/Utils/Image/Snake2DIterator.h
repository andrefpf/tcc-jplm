#ifndef JPLM_LIB_UTILS_IMAGE_SNAKE2DITERATOR_H__
#define JPLM_LIB_UTILS_IMAGE_SNAKE2DITERATOR_H__

#include <cstddef>  //for std::ptrdiff_t
#include <iterator>

template<typename T>
class Snake2DIterator {
 protected:
  T* data_ptr;

 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = T;
  using pointer = T*;
  using reference = T&;


  Snake2DIterator(T* ptr = nullptr) : data_ptr(ptr){};


  Snake2DIterator(const Snake2DIterator& other) = default;


  ~Snake2DIterator() = default;

  Snake2DIterator<T>& operator=(
      const Snake2DIterator<T>& rawIterator) = default;


  Snake2DIterator<T>& operator=(T* ptr) {
    data_ptr = ptr;
    return (*this);
  }


  operator bool() const {
    if (data_ptr != nullptr) {
      return true;
    }
    return false;
  }


  bool operator==(const Snake2DIterator<T>& other) const {
    return (data_ptr == other.get_const_data_ptr());
  }


  bool operator!=(const Snake2DIterator<T>& other) const {
    return (data_ptr != other.get_const_data_ptr());
  }


  Snake2DIterator<T>& operator+=(const std::ptrdiff_t& delta) {
    data_ptr += delta;
    return (*this);
  }


  Snake2DIterator<T>& operator-=(const std::ptrdiff_t& delta) {
    data_ptr -= delta;
    return (*this);
  }


  Snake2DIterator<T>& operator++() {
    ++data_ptr;
    return (*this);
  }


  Snake2DIterator<T>& operator--() {
    --data_ptr;
    return (*this);
  }


  Snake2DIterator<T> operator++(int) { //needs dummy int to indicate postfix
    auto temp_iterator(*this);
    ++data_ptr;
    return temp_iterator;
  }


  Snake2DIterator<T> operator--(int) { //needs dummy int to indicate postfix
    auto temp_iterator(*this);
    --data_ptr;
    return temp_iterator;
  }


  Snake2DIterator<T> operator+(const std::ptrdiff_t& delta) {
    auto oldPtr = data_ptr;
    data_ptr += delta;
    auto temp_iterator(*this);
    data_ptr = oldPtr;
    return temp_iterator;
  }


  Snake2DIterator<T> operator-(const std::ptrdiff_t& delta) {
    auto oldPtr = data_ptr;
    data_ptr -= delta;
    auto temp_iterator(*this);
    data_ptr = oldPtr;
    return temp_iterator;
  }


  std::ptrdiff_t operator-(const Raster2DIterator<T>& rawIterator) {
    return std::distance(rawIterator.get_data_ptr(), this->get_data_ptr());
  }


  T& operator*() {
    return *data_ptr;
  }


  const T& operator*() const {
    return *data_ptr;
  }


  T* operator->() {
    return data_ptr;
  }


  T* get_data_ptr() const {
    return data_ptr;
  }


  const T* get_const_data_ptr() const {
    return data_ptr;
  }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_SNAKE2DITERATOR_H__ */