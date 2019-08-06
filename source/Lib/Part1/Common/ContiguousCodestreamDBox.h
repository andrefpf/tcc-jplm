#ifndef CONTIGUOUSCODESTREAMDBOX_H__
#define CONTIGUOUSCODESTREAMDBOX_H__

#include "DBox.h"
#include "ContiguousCodestreamContents.h"

class ContiguousCodestreamDBox : public DBox {
 public:
  ContiguousCodestreamDBox(const ContiguousCodestreamContents& contents)
      : DBox(std::make_unique<ContiguousCodestreamContents>(contents)) {
  }


  ContiguousCodestreamDBox(const ContiguousCodestreamDBox& other)
      : DBox(std::make_unique<ContiguousCodestreamContents>(other.get_ref_to_contents())) {
  }


  ~ContiguousCodestreamDBox() = default;


  ContiguousCodestreamDBox* clone() const override {
    return new ContiguousCodestreamDBox(*this);
  }
};

#endif /* end of include guard: CONTIGUOUSCODESTREAMDBOX_H__ */