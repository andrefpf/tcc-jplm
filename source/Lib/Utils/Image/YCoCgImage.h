#ifndef YCOCGIMAGE_H__
#define YCOCGIMAGE_H__

#include "Lib/Utils/Image/ThreeChannelImage.h"

template<typename T>
class YCoCgImage : public ThreeChannelImage<T> {
 public:
  YCoCgImage(
      std::size_t width, std::size_t height, std::size_t bpp, ImageType type)
      : ThreeChannelImage<T>(width, height, bpp, type){};


  YCoCgImage(YCoCgImage<T>&& other) noexcept
      : ThreeChannelImage<T>(std::move(other)) {
  }


  YCoCgImage(const YCoCgImage<T>& other) : ThreeChannelImage<T>(other){};


  YCoCgImage& operator=(YCoCgImage<T>&& other) {
    ThreeChannelImage<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const YCoCgImage<T>& other) const {
    return this->is_equal(other);
  }


  ~YCoCgImage() = default;


  std::vector<std::string> get_channel_names() const final {
    return {"Y", "Co", "Cg"};
  }
};

#endif /* end of include guard: YCOCGIMAGE_H__ */