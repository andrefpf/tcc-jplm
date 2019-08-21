#ifndef EMPTYDBOXCONTENTS_H__
#define EMPTYDBOXCONTENTS_H__

#include "InMemoryDBoxContents.h"

class EmptyDBoxContents : public InMemoryDBoxContents {
 public:
  EmptyDBoxContents() = default;


  ~EmptyDBoxContents() = default;


  virtual std::vector<std::byte> get_bytes() const noexcept override {
    return std::vector<std::byte>();
  }


  virtual uint64_t size() const noexcept override {
    return 0;
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return true;
  }


  virtual EmptyDBoxContents* clone() const override {
    return new EmptyDBoxContents(*this);
  }
};

#endif /* end of include guard: EMPTYDBOXCONTENTS_H__ */