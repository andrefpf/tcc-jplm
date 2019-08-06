#ifndef COLOURSPECIFICATIONDBOX_H__
#define COLOURSPECIFICATIONDBOX_H__

#include "DBox.h"
#include "ColourSpecificationContents.h"

class ColourSpecificationDBox : public DBox {
 public:
  ColourSpecificationDBox(const ColourSpecificationContents& contents)
      : DBox(std::make_unique<ColourSpecificationContents>(contents)) {
  }


  ColourSpecificationDBox(const ColourSpecificationDBox& other)
      : DBox(std::make_unique<ColourSpecificationContents>(other.get_ref_to_contents())) {
  }

  ~ColourSpecificationDBox() = default;


  ColourSpecificationDBox* clone() const override {
    return new ColourSpecificationDBox(*this);
  }

};


#endif /* end of include guard: COLOURSPECIFICATIONDBOX_H__ */