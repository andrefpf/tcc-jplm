#ifndef XLBOX_H__
#define XLBOX_H__

#include "BoxDataHolder.h"


class XLBox : public BoxDataHolder<uint64_t> {
 public:
  XLBox() = default;
  XLBox(const uint64_t& value) : BoxDataHolder<uint64_t>(value) {
  }
  ~XLBox() = default;

  bool operator==(const XLBox& other) const {
    return this->is_equal(other);
  }

  bool operator!=(const XLBox& other) const {
    return !operator==(other);
  }
};

#endif /* end of include guard: XLBOX_H__ */