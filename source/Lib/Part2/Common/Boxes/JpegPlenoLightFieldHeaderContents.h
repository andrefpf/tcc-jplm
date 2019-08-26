#ifndef JPLM_LIB_PART2_COMMON_JPEGPLENOLIGHTFIELDHEADERCONTENTS_H__
#define JPLM_LIB_PART2_COMMON_JPEGPLENOLIGHTFIELDHEADERCONTENTS_H__

#include <memory>
#include <tuple>  //std::tie
#include <vector>
#include "Lib/Common/Boxes/Generic/BitsPerComponentBox.h"
#include "Lib/Common/Boxes/Generic/ChannelDefinitionBox.h"
#include "Lib/Common/Boxes/Generic/ColourSpecificationBox.h"
#include "Lib/Common/Boxes/SuperBoxDBoxContents.h"
#include "LightFieldHeaderBox.h"

class JpegPlenoLightFieldHeaderContents : public SuperBoxDBoxContents {
 protected:
  std::unique_ptr<LightFieldHeaderBox> lhdr;  //light_field_header_box
  std::unique_ptr<BitsPerComponentBox>
      bpcc;  //bits per component box; ! optional !
  //! \todo check the condition: "there shall be at least one Colour Specification box within the JPEG Pleno Header Box"
  std::vector<std::unique_ptr<ColourSpecificationBox>>
      colr;  //colour specification boxes; ! optionals?? !
  std::unique_ptr<ChannelDefinitionBox>
      cdef;  //channel definition box ! optional !
 public:
  //! \todo use always the same constructor
  JpegPlenoLightFieldHeaderContents(const LightFieldHeaderBox& lhdr,
      const std::vector<std::unique_ptr<ColourSpecificationBox>>& colr)
      : lhdr(std::make_unique<LightFieldHeaderBox>(lhdr)) {
    auto number_of_colour_specification_boxes = colr.size();
    if (number_of_colour_specification_boxes == 0) {
      //throw
    }
    this->colr.reserve(number_of_colour_specification_boxes);
    for (const auto& colour_specification_box : colr) {
      this->colr.push_back(
          std::make_unique<ColourSpecificationBox>(*colour_specification_box));
    }
  }

  JpegPlenoLightFieldHeaderContents(const LightFieldHeaderBox& lhdr,
      const BitsPerComponentBox& bpcc,
      const std::vector<std::unique_ptr<ColourSpecificationBox>>& colr)
      : lhdr(std::make_unique<LightFieldHeaderBox>(lhdr)),
        bpcc(std::make_unique<BitsPerComponentBox>(bpcc)) {
    auto number_of_colour_specification_boxes = colr.size();
    if (number_of_colour_specification_boxes == 0) {
      //throw
    }
    this->colr.reserve(number_of_colour_specification_boxes);
    for (const auto& colour_specification_box : colr) {
      this->colr.push_back(
          std::make_unique<ColourSpecificationBox>(*colour_specification_box));
    }
  }

  uint64_t size() const noexcept override {
    auto size = lhdr->size();
    if (bpcc) {
      size += bpcc->size();
    }
    for (const auto& colour_specification_box : colr) {
      size += colour_specification_box->size();
    }
    if (cdef) {
      size += cdef->size();
    }
    return size;
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const JpegPlenoLightFieldHeaderContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const JpegPlenoLightFieldHeaderContents& other) const
      noexcept {
    if (*(this->lhdr) == *(other.lhdr) && (this->colr == other.colr)) {
      if ((this->bpcc && other.bpcc) && (*(this->bpcc) == *(other.bpcc))) {
        if ((this->cdef && other.cdef) && (*(this->cdef) == *(other.cdef))) {
          return true;
        }
      }
    }
    return false;
  }


  bool operator!=(const JpegPlenoLightFieldHeaderContents& other) const
      noexcept {
    return !this->operator==(other);
  }

  virtual ~JpegPlenoLightFieldHeaderContents() = default;


  std::ostream& write_to(std::ostream& stream) const final {
  	stream << *lhdr;
  	if(bpcc) {
  		stream << *bpcc;
  	}
	for (const auto& colour_specification_box : colr) {
      stream << *colour_specification_box;
    }
    if(cdef) {
  		stream << *cdef;
  	}
  	return stream;
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_JPEGPLENOLIGHTFIELDHEADERCONTENTS_H__ */