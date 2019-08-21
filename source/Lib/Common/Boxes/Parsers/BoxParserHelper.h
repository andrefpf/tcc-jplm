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
  bool has_xl_field = false;

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
    has_xl_field = true;
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


  uint64_t get_non_data_lenght() const noexcept {
    auto non_data_length = 8;  //4 from l_box, 4 from t_box
    if (has_xl_field) {
      non_data_length += 8;
    }
    return non_data_length;
  }

  uint64_t get_data_lenght() const noexcept {
    return get_length() - get_non_data_lenght();
  }


  ManagedStream get_data_stream() {
    managed_stream.seek(get_non_data_lenght());
    return managed_stream.get_sub_managed_stream(get_data_lenght());
  }


  template<typename T>
  T get_next() {
    using namespace BinaryTools;
    return get_value_from_big_endian_byte_vector<T>(
        managed_stream.get_bytes<sizeof(T)>());
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
    check_t_box_value();
  }


  virtual ~BoxParserHelper() = default;
};


#endif /* end of include guard: BOXPARSERHELPER_H__ */