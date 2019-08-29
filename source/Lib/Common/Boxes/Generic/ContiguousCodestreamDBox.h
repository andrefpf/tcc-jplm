#ifndef CONTIGUOUSCODESTREAMDBOX_H__
#define CONTIGUOUSCODESTREAMDBOX_H__

#include "ContiguousCodestreamContents.h"
#include "Lib/Common/Boxes/DBox.h"

class ContiguousCodestreamDBox : public DBox {
 public:
  ContiguousCodestreamDBox(const ContiguousCodestreamContents& contents)
      : DBox(std::make_unique<ContiguousCodestreamContents>(contents)) {
  }


  ContiguousCodestreamDBox(std::unique_ptr<ContiguousCodestreamCode>&& code)
      : DBox(std::make_unique<ContiguousCodestreamContents>(std::move(code))) {
  }


  ContiguousCodestreamDBox(const ContiguousCodestreamDBox& other)
      : DBox(std::make_unique<ContiguousCodestreamContents>(
            other.get_ref_to_contents())) {
  }


  virtual const ContiguousCodestreamContents& get_ref_to_contents()
      const override {
    return static_cast<const ContiguousCodestreamContents&>(*contents);
  }


  ~ContiguousCodestreamDBox() = default;


  ContiguousCodestreamDBox* clone() const override {
    return new ContiguousCodestreamDBox(*this);
  }
};

#endif /* end of include guard: CONTIGUOUSCODESTREAMDBOX_H__ */