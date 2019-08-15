#ifndef BOXPARSERHELPER_H__
#define BOXPARSERHELPER_H__

#include "CommonExceptions.h"
#include "Lib/Part1/Common/BinaryTools.h"
#include "Lib/Utils/Stream/ManagedStream.h"

template<class ParsingBox>
class BoxParserHelper {
 private:
  ManagedStream& managed_stream;
  uint64_t length;

 protected:
  uint32_t get_l_box_value() {
    //ensuring the stream is at the expected position. i.e., the next get_bytes<sizeof(l_box_t)> will get l_box data..
    managed_stream.rewind();
    using namespace BinaryTools;
    return get_value_from_big_endian_byte_vector<uint32_t>(
        managed_stream.get_bytes<4>());
  }


  uint64_t get_xl_box_value() {
    //ensuring the stream is at the expected position. i.e., the next get_bytes<sizeof(l_box_t)> will get l_box data..
    using namespace BinaryTools;
    return get_value_from_big_endian_byte_vector<uint64_t>(
        managed_stream.get_bytes<8>());
  }


  void check_t_box_value() {
    using namespace BinaryTools;
    if (auto id = get_value_from_big_endian_byte_vector<uint32_t>(
            managed_stream.get_bytes<4>());
        id != ParsingBox::id) {
      throw BoxParserExceptions::WrongTBoxValueException(id, ParsingBox::id);
    }
  }


 public:
  BoxParserHelper(ManagedStream& stream)
      : managed_stream(stream), length(get_l_box_value()) {
    check_t_box_value();
    if (length == 1) {
      length = get_xl_box_value();
    }
  }


  virtual ~BoxParserHelper() = default;


  uint64_t get_length() const noexcept {
    return length;
  }
};


#endif /* end of include guard: BOXPARSERHELPER_H__ */