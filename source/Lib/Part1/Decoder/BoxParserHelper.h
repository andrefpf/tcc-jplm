#ifndef BOXPARSERHELPER_H__
#define BOXPARSERHELPER_H__

#include "CommonExceptions.h"
#include "Lib/Part1/Common/BinaryTools.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class BoxParserHelperBase {
 protected:
  ManagedStream& managed_stream;
  uint64_t length;
  uint32_t t_box_value_in_stream;

  uint32_t get_l_box_value_from_stream() {
    //ensuring the stream is at the expected position. i.e., the next get_bytes<sizeof(l_box_t)> will get l_box data..
    managed_stream.rewind();
    using namespace BinaryTools;
    return get_value_from_big_endian_byte_vector<uint32_t>(
        managed_stream.get_bytes<4>());
  }


  uint64_t get_xl_box_value_from_stream() {
    //ensuring the stream is at the expected position. i.e., the next get_bytes<sizeof(l_box_t)> will get l_box data..
    using namespace BinaryTools;
    return get_value_from_big_endian_byte_vector<uint64_t>(
        managed_stream.get_bytes<8>());
  }


  uint32_t get_t_box_value_from_stream() {
    using namespace BinaryTools;
    return get_value_from_big_endian_byte_vector<uint32_t>(
        managed_stream.get_bytes<4>());
  }


 public:
  BoxParserHelperBase(ManagedStream& stream)
      : managed_stream(stream), length(get_l_box_value_from_stream()),
        t_box_value_in_stream(get_t_box_value_from_stream()) {
    if (length == 1) {
      length = get_xl_box_value_from_stream();
    }
  }


  uint64_t get_length() const noexcept {
    return length;
  }


  uint32_t get_t_box_value() const noexcept {
    return t_box_value_in_stream;
  }


  virtual ~BoxParserHelperBase() = default;
};


template<class ParsingBox>
class BoxParserHelper : public BoxParserHelperBase {
 protected:
  void check_t_box_value() {
    if (this->t_box_value_in_stream != ParsingBox::id) {
      throw BoxParserExceptions::WrongTBoxValueException(
          this->t_box_value_in_stream, ParsingBox::id);
    }
  }


 public:
  BoxParserHelper(ManagedStream& stream) : BoxParserHelperBase(stream) {
  }

  virtual ~BoxParserHelper() = default;
};


#endif /* end of include guard: BOXPARSERHELPER_H__ */