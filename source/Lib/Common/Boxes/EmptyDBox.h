#ifndef EMPTYDBOX_H__
#define EMPTYDBOX_H__

#include "DBox.h"
#include "EmptyDBoxContents.h"


class EmptyDBox : public DBox {
 public:
  EmptyDBox() : DBox(std::unique_ptr<DBoxContents>(new EmptyDBoxContents())) {
  }


  EmptyDBox(const EmptyDBox&) : DBox(std::unique_ptr<DBoxContents>(new EmptyDBoxContents())) {};


  EmptyDBox(EmptyDBox&& other) : DBox(std::move(other)) {};


  virtual const DBoxContents& get_ref_to_contents() const override {
    return static_cast<const DBoxContents&>(*contents);
  }
  

  ~EmptyDBox() = default;


  EmptyDBox* clone() const override {
    return new EmptyDBox(*this);
  }
};

#endif /* end of include guard: EMPTYDBOX_H__ */