#ifndef JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__
#define JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace BoxParserExceptions {

class WrongTBoxValueException : public std::exception {
 protected:
  const uint32_t readed_type;
  const uint32_t expected_type;

 public:
  WrongTBoxValueException(
      const uint32_t readed_type, const uint32_t expected_type)
      : readed_type(readed_type), expected_type(expected_type) {
  }


  const char* what() const noexcept override {
    auto string_stream = std::stringstream();
    string_stream << "Error parsing box. Expected T_BOX value: 0x" << std::hex
                  << std::setfill('0') << std::setw(8) << expected_type
                  << ". Obtained T_BOX value: 0x" << readed_type << std::dec
                  << std::endl;
    return string_stream.str().c_str();
  }
};
}  // namespace BoxParserExceptions

#endif /* end of include guard: JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__ */