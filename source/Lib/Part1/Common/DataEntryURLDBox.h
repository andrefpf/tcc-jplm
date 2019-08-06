#ifndef DATAENTRYURLDBOX_H__
#define DATAENTRYURLDBOX_H__

#include "DBox.h"
#include "DataEntryURLBoxContents.h"

class DataEntryURLDBox : public DBox {
 public:
  DataEntryURLDBox(const DataEntryURLBoxContents& contents)
      : DBox(std::make_unique<DataEntryURLBoxContents>(contents)) {
  }


  DataEntryURLDBox(const DataEntryURLDBox& other)
      : DBox(std::make_unique<DataEntryURLBoxContents>(
            other.get_ref_to_contents())) {
  }


  ~DataEntryURLDBox() = default;


  DataEntryURLDBox* clone() const override {
    return new DataEntryURLDBox(*this);
  }
};

#endif /* end of include guard: DATAENTRYURLDBOX_H__ */