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
public:
    JPLFileParser(const std::string& filename)
      : filename(filename), file_size(std::filesystem::file_size(filename)),
        if_stream(filename, std::ifstream::binary),
        managed_stream(if_stream, static_cast<uint64_t>(file_size)){}


    auto get_signature_box() {
    	auto managed_substream = managed_stream.get_sub_managed_stream(0,12);
		return parser.parse<JpegPlenoSignatureBox>(managed_substream);
    }

    auto get_file_type_box() {
    	auto managed_substream = managed_stream.get_sub_managed_stream(12,12+20);
		return parser.parse<FileTypeBox>(managed_substream);
    }

    virtual ~JPLFileParser() = default;
};


class JPLFileFromStream : public JPLFileParser, public JPLFile {
 
 public:
  JPLFileFromStream(const std::string& filename)
      : JPLFileParser(filename),
        JPLFile(JPLFileParser::get_signature_box(),
            JPLFileParser::get_file_type_box()) {
        this->managed_stream.seek(12+20);
  }

// .get_sub_managed_stream(
//                     file_size - managed_stream.tell())

  //   auto managed_stream = ManagedStream(if_stream, 100);
  // BoxParserRegistry::get_instance().parse<JpegPlenoSignatureBox>(
  //       managed_stream);

  virtual ~JPLFileFromStream() = default;
};

#endif /* end of include guard: JPLFILEFROMSTREAM_H__ */