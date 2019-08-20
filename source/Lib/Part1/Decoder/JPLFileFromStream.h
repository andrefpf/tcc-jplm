#ifndef JPLFILEFROMSTREAM_H__
#define JPLFILEFROMSTREAM_H__

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/BoxParserRegistry.h"
#include "Lib/Part1/Common/JPLFile.h"
#include "Lib/Utils/Stream/ManagedStream.h"


class JPLFileParser {
 protected:
  const BoxParserRegistry& parser = BoxParserRegistry::get_instance();
  const std::string filename;
  const uint64_t file_size;
  std::ifstream if_stream;
  ManagedStream managed_stream;
  std::unique_ptr<JpegPlenoSignatureBox> temp_signature;
  std::unique_ptr<FileTypeBox> temp_file_type;

 public:
  JPLFileParser(const std::string& filename)
      : filename(filename), file_size(std::filesystem::file_size(filename)),
        if_stream(filename, std::ifstream::binary),
        managed_stream(if_stream, static_cast<uint64_t>(file_size)) {
    if (file_size < 20) {
      throw JPLFileFromStreamExceptions::InvalidTooSmallFileException(
          file_size);
    }
    temp_signature =
        std::move(parser.parse<JpegPlenoSignatureBox>(managed_stream));
    auto managed_substream = managed_stream.get_sub_managed_stream(
        file_size - managed_stream.tell());
    temp_file_type = std::move(parser.parse<FileTypeBox>(managed_substream));
  }


  auto get_signature_box() {
    return std::move(temp_signature);
  }


  auto get_file_type_box() {
    return std::move(temp_file_type);
  }


  virtual ~JPLFileParser() = default;
};


class JPLFileFromStream : public JPLFileParser, public JPLFile {
 protected:
  uint64_t decoded_boxes = 0;

 public:
  JPLFileFromStream(const std::string& filename)
      : JPLFileParser(filename), JPLFile(JPLFileParser::get_signature_box(),
                                     JPLFileParser::get_file_type_box()) {
    if(!this->file_type_box->is_compatible_with<JpegPlenoSignatureBox>()) {
    	throw JPLFileFromStreamExceptions::JpegPlenoNotInCompatibilityListException();
    }

    this->managed_stream.seek(12 + 20);
    decoded_boxes += 2;
    // std::move(*(this->parser.parse<XMLBoxWithCatalog>()));

    while (this->managed_stream.is_valid()) {
      std::cout << "Got another box..." << std::endl;
      decoded_boxes++;
    }
    std::cout << "No more boxes to decode" << std::endl;
  }


  auto get_number_of_decoded_boxes() {
    return decoded_boxes;
  }


  virtual ~JPLFileFromStream() = default;
};

#endif /* end of include guard: JPLFILEFROMSTREAM_H__ */