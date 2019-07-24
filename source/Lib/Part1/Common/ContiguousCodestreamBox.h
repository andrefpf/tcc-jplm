#ifndef JPLM_LIB_PART1_COMMON_CONTIGUOUSCODESTREAMBOX_H__
#define JPLM_LIB_PART1_COMMON_CONTIGUOUSCODESTREAMBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"

class ContiguousCodestreamContents {
	//not implemented yet...
public:
	uint64_t get_size() const noexcept {
		return 0;
	}
};


class ContiguousCodestreamDBox : public DBox {
 public:
  ContiguousCodestreamDBox(const ContiguousCodestreamContents& contents)
      : DBox(std::make_any<ContiguousCodestreamContents>(contents)) {
  }


  ContiguousCodestreamDBox(const ContiguousCodestreamDBox& other)
      : DBox(std::make_any<ContiguousCodestreamContents>(
            std::any_cast<ContiguousCodestreamContents>(other.contents))) {
  }


  ~ContiguousCodestreamDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<ContiguousCodestreamContents>(this->contents).get_size();
  }


  ContiguousCodestreamDBox* clone() const override {
    return new ContiguousCodestreamDBox(*this);
  }
};


class ContiguousCodestreamBox : public Box {
 public:
  ContiguousCodestreamBox(const ContiguousCodestreamContents& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::ContiguousCodestreamBoxType)),
            ContiguousCodestreamDBox(contents)){};
  ~ContiguousCodestreamBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_CONTIGUOUSCODESTREAMBOX_H__ */