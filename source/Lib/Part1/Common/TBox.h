#ifndef TBOX_H__
#define TBOX_H__

#include "BoxDataHolder.h"

class TBox : public BoxDataHolder<uint32_t> {
 public:
  TBox() = default;
  TBox(const uint32_t& value) : BoxDataHolder<uint32_t>(value) {
  }
  ~TBox() = default;

  bool operator==(const TBox& other) const {
    return this->is_equal(other);
  }

  bool operator!=(const TBox& other) const {
    return !operator==(other);
  }
};

#endif /* end of include guard: TBOX_H__ */