#ifndef JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__
#define JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace BoxParserExceptions {

class WrongTBoxValueException : public std::exception {
 protected:
  // const uint32_t readed_type;
  // const uint32_t expected_type;
  // const char* xuxu = "xuxu";
  std::string message;

 public:
  WrongTBoxValueException(
      const uint32_t readed_type, const uint32_t expected_type) {
    auto string_stream = std::stringstream();
    string_stream << "Error parsing box. Expected T_BOX value: 0x" << std::hex
                  << std::setfill('0') << std::setw(8) << expected_type
                  << ". Obtained T_BOX value: 0x" << readed_type << std::dec
                  << std::endl;
    message = string_stream.str();
  }

  const char* what() const noexcept override {
    return message.c_str();
  }
};
}  // namespace BoxParserExceptions


namespace JpegPlenoSignatureBoxParserExceptions {
class InvalidJpegPlenoSignatureBox : public std::exception {
 public:
  InvalidJpegPlenoSignatureBox() = default;

  const char* what() const noexcept override {
    return "Invalid content (D_BOX) in Jpeg Pleno signature box";
  }
};
}  // namespace JpegPlenoSignatureBoxParserExceptions


namespace JPLFileFromStreamExceptions {
class InvalidTooSmallFileException : public std::exception {
 protected:
  std::string message;

 public:
  InvalidTooSmallFileException(const uint64_t filesize)
      : message(std::string("Error, jpl file is too small (only ") +
                std::to_string(filesize) +
                std::string(" bytes). Expecting at least 20 bytes.")) {
  }


  const char* what() const noexcept override {
    return message.c_str();
  }
};

class JpegPlenoNotInCompatibilityListException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Jpeg Pleno was not found in compatibility list.";
  }
};

}  // namespace JPLFileFromStreamExceptions

#endif /* end of include guard: JPLM_LIB_PART1_DECODER_COMMONEXCEPTIONS_H__ */