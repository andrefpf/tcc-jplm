#ifndef INFILEDBOXCONTENTS_H__
#define INFILEDBOXCONTENTS_H__

#include <iostream>
#include <fstream> 
#include "DBoxContents.h"

class InFileDBoxContents : public DBoxContents {
 protected:
  std::string filename;
  std::ifstream file;

 public:
  InFileDBoxContents(const std::string& filename)
      : DBoxContents(), filename(filename),
        file(filename.c_str(), std::ios::in | std::ios::out) {
  }
  

  InFileDBoxContents(const InFileDBoxContents& other)
      : DBoxContents(), filename(other.filename),
        file(other.filename.c_str(), std::ios::in | std::ios::out) {
  }

  virtual std::ostream& write_to(std::ostream& stream) const final;
};

#endif /* end of include guard: INFILEDBOXCONTENTS_H__ */