#ifndef JPEGPLENOTHUMBNAILCONTENTS_H__
#define JPEGPLENOTHUMBNAILCONTENTS_H__

#include <algorithm>
#include <numeric>
#include "source/Lib/Common/Boxes/Generic/BitsPerComponentBox.h"
#include "source/Lib/Common/Boxes/Generic/ChannelDefinitionBox.h"
#include "source/Lib/Common/Boxes/Generic/ColourSpecificationBox.h"
#include "source/Lib/Common/Boxes/Generic/ContiguousCodestreamBox.h"
#include "source/Lib/Common/Boxes/Generic/ImageHeaderBox.h"
#include "source/Lib/Common/Boxes/InMemoryDBoxContents.h"

class JpegPlenoThumbnailContents : public InMemoryDBoxContents {
 protected:
  ImageHeaderBox ihdr;  //image header box
  BitsPerComponentBox bpcc;
  std::vector<ColourSpecificationBox> colr;
  ChannelDefinitionBox cdef;
  ContiguousCodestreamBox jpc2;

 public:
  JpegPlenoThumbnailContents() = default;

  
  JpegPlenoThumbnailContents(const JpegPlenoThumbnailContents& other)
      : ihdr(other.ihdr), bpcc(other.bpcc), colr(other.colr), cdef(other.cdef),
        jpc2(other.jpc2) {
  }


  JpegPlenoThumbnailContents(JpegPlenoThumbnailContents&& other)
      : ihdr(std::move(other.ihdr)), bpcc(std::move(other.bpcc)),
        colr(std::move(other.colr)), cdef(std::move(other.cdef)),
        jpc2(std::move(other.jpc2)) {
  }


  virtual JpegPlenoThumbnailContents* clone() const override {
    return new JpegPlenoThumbnailContents(*this);
  }


  virtual uint64_t size() const noexcept override {
    auto sum_colr_sizes = [](uint64_t sum, const ColourSpecificationBox& val) {
      return sum + val.size();
    };

    return ihdr.size() + bpcc.size() +
           std::accumulate(colr.begin(), colr.end(), 0, sum_colr_sizes) +
           cdef.size() + jpc2.size();
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const JpegPlenoThumbnailContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const JpegPlenoThumbnailContents& other) const {
    return std::tie(
               this->ihdr, this->bpcc, this->colr, this->cdef, this->jpc2) ==
           std::tie(other.ihdr, other.bpcc, other.colr, other.cdef, other.jpc2);
  }


  bool operator!=(const JpegPlenoThumbnailContents& other) const {
    return !this->operator==(other);
  }
};

#endif /* end of include guard: JPEGPLENOTHUMBNAILCONTENTS_H__ */