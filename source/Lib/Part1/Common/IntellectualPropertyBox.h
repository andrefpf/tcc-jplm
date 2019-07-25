#ifndef INTELLECTUALPROPERTYBOX_H__
#define INTELLECTUALPROPERTYBOX_H__


#include "Box.h"
#include "DefinedBoxes.h"
#include "UndefinedDBoxContents.h"

class IntellectualPropertyBox : public Box {
 public:
  IntellectualPropertyBox()
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::IntellectualPropertyBoxType)),
            UndefinedDBox(UndefinedDBoxContents())){};
  IntellectualPropertyBox(const UndefinedDBoxContents& content)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::IntellectualPropertyBoxType)),
            UndefinedDBox(content)){};
  ~IntellectualPropertyBox();
};

#endif /* end of include guard: INTELLECTUALPROPERTYBOX_H__ */