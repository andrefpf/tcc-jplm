#ifndef JPLM_LIB_UTILS_IMAGE_RASTER2DITERATOR_H__
#define JPLM_LIB_UTILS_IMAGE_RASTER2DITERATOR_H__

#include <cstddef>  //for std::ptrdiff_t
#include <iterator>
#include "Lib/Utils/Image/Generic2DIterator.h"

template<typename T>
class Raster2DIterator : public Generic2DIterator<T> {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;


  Raster2DIterator(T* ptr = nullptr) : Generic2DIterator<T>(ptr){};


  Raster2DIterator(const Raster2DIterator& other) = default;


  ~Raster2DIterator() = default;


  Raster2DIterator<T>& operator=(
      const Raster2DIterator<T>& rawIterator) = default;


  Raster2DIterator<T>& operator=(T* ptr) {
    this->data_ptr = ptr;
    return (*this);
  }


  operator bool() const override {
    if (this->data_ptr != nullptr) {
      return true;
    }
    return false;
  }


  bool operator==(const Raster2DIterator<T>& other) const {
    return (this->data_ptr == other.get_const_data_ptr());
  }


  bool operator!=(const Raster2DIterator<T>& other) const {
    return (this->data_ptr != other.get_const_data_ptr());
  }


  Raster2DIterator<T>& operator+=(const difference_type& delta) {
    this->data_ptr += delta;
    return (*this);
  }


  Raster2DIterator<T>& operator-=(const difference_type& delta) {
    this->data_ptr -= delta;
    return (*this);
  }


  Raster2DIterator<T>& operator++() {
    ++this->data_ptr;
    return (*this);
  }


  Raster2DIterator<T>& operator--() {
    --this->data_ptr;
    return (*this);
  }


  Raster2DIterator<T> operator++(int) {  //needs dummy int to indicate postfix
    auto temp_iterator(*this);
    ++this->data_ptr;
    return temp_iterator;
  }


  Raster2DIterator<T> operator--(int) {  //needs dummy int to indicate postfix
    auto temp_iterator(*this);
    --this->data_ptr;
    return temp_iterator;
  }


  Raster2DIterator<T> operator+(const difference_type& delta) {
    auto oldPtr = this->data_ptr;
    this->data_ptr += delta;
    auto temp_iterator(*this);
    this->data_ptr = oldPtr;
    return temp_iterator;
  }


  Raster2DIterator<T> operator-(const difference_type& delta) {
    auto oldPtr = this->data_ptr;
    this->data_ptr -= delta;
    auto temp_iterator(*this);
    this->data_ptr = oldPtr;
    return temp_iterator;
  }


  difference_type operator-(const Raster2DIterator<T>& rawIterator) {
    return std::distance(rawIterator.get_data_ptr(), this->get_data_ptr());
  }


  T& operator*() override {
    return *this->data_ptr;
  }


  const T& operator*() const override {
    return *this->data_ptr;
  }


  T* operator->() override {
    return this->data_ptr;
  }


  T* get_data_ptr() const override {
    return this->data_ptr;
  }


  const T* get_const_data_ptr() const override {
    return this->data_ptr;
  }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_RASTER2DITERATOR_H__ */