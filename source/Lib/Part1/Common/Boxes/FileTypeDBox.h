#ifndef FILETYPEDBOX_H__
#define FILETYPEDBOX_H__

#include "source/Lib/Common/Boxes/DBox.h"
#include "FileTypeContents.h"

class FileTypeDBox : public DBox {
 public:
  FileTypeDBox(const FileTypeContents& contents)
      : DBox(std::make_unique<FileTypeContents>(contents)) {
  }


  FileTypeDBox(const FileTypeDBox& other)
      : DBox(std::make_unique<FileTypeContents>(other.get_ref_to_contents())) {
  }


  virtual ~FileTypeDBox() = default;


  virtual const FileTypeContents& get_ref_to_contents() const override {
    return static_cast<const FileTypeContents&>(*contents);
  }


  FileTypeDBox* clone() const override {
    return new FileTypeDBox(*this);
  }
};


#endif /* end of include guard: FILETYPEDBOX_H__ */