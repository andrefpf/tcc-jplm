#ifndef UNDEFINEDDBOXCONTENTS_H__
#define UNDEFINEDDBOXCONTENTS_H__

#include "Box.h"

class UndefinedDBoxContents
{
protected:
	std::vector<uint8_t> byte_array;
public:
	UndefinedDBoxContents();
	~UndefinedDBoxContents();

	uint64_t get_size() const noexcept {
		return byte_array.size()*sizeof(uint8_t);
	}
};


class UndefinedDBox : public DBox {
 public:
  UndefinedDBox(const UndefinedDBoxContents& contents)
      : DBox(std::make_any<UndefinedDBoxContents>(contents)) {
  }


  UndefinedDBox(const UndefinedDBox& other)
      : DBox(std::make_any<UndefinedDBoxContents>(
            std::any_cast<UndefinedDBoxContents>(other.contents))) {
  }

  ~UndefinedDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<UndefinedDBoxContents>(this->contents).get_size();
  }


  UndefinedDBox* clone() const override {
    return new UndefinedDBox(*this);
  }
};



#endif /* end of include guard: UNDEFINEDDBOXCONTENTS_H__ */