#ifndef JPLM_LIB_UTILS_IMAGE_GENERIC2DITERATOR_H__
#define JPLM_LIB_UTILS_IMAGE_GENERIC2DITERATOR_H__

#include <cstddef>  //for std::ptrdiff_t
#include <iterator>


template<typename T>
class Generic2DIterator {
 protected:
  T* data_ptr;
  std::size_t stride;

 public:
  Generic2DIterator(T* ptr = nullptr) : data_ptr(ptr){};


  Generic2DIterator(const Generic2DIterator& other) = default;


  virtual ~Generic2DIterator() = default;


  virtual Generic2DIterator<T>& operator=(
      const Generic2DIterator<T>& rawIterator) = default;


  virtual operator bool() const = 0;


  virtual T& operator*() = 0;


  virtual const T& operator*() const = 0;


  virtual T* operator->() = 0;


  virtual T* get_data_ptr() const = 0;


  virtual const T* get_const_data_ptr() const = 0;
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_GENERIC2DITERATOR_H__ */