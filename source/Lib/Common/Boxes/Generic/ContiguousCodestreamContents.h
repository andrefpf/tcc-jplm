#ifndef CONTIGUOUSCODESTREAMCONTENTS_H__
#define CONTIGUOUSCODESTREAMCONTENTS_H__

#include "Lib/Common/Boxes/InMemoryDBoxContents.h"

class ContiguousCodestreamContents : public InMemoryDBoxContents {
  //not implemented yet...
 public:
  virtual uint64_t size() const noexcept override {
    return 0;
  }


  virtual ContiguousCodestreamContents* clone() const override {
    return new ContiguousCodestreamContents(*this);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const ContiguousCodestreamContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const ContiguousCodestreamContents&) const {
    //! \todo Implement this class :)
    return true;
  }


  bool operator!=(const ContiguousCodestreamContents& other) const {
    return !this->operator==(other);
  }
};

#endif /* end of include guard: CONTIGUOUSCODESTREAMCONTENTS_H__ */