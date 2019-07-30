#ifndef UNDEFINEDBOX_H__
#define UNDEFINEDBOX_H__

#include "Box.h"
#include "UndefinedDBoxContents.h"

class UndefinedBox : public Box {
 public:
  UndefinedBox(const TBox& type, const UndefinedDBoxContents& contents)
      : Box(type, UndefinedDBox(contents)) {
  }


  ~UndefinedBox() = default;
};

#endif /* end of include guard: UNDEFINEDBOX_H__ */