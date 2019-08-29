#ifndef JPLFILEFROMSTREAM_H__
#define JPLFILEFROMSTREAM_H__

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part1/Common/JPLFile.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "source/Lib/Common/Boxes/Parsers/BoxParserRegistry.h"


class JPLFileParser {
 protected:
  const BoxParserRegistry& parser = BoxParserRegistry::get_instance();
  const std::string filename;
  const uint64_t file_size;
  std::ifstream if_stream;
  ManagedStream managed_stream;
  std::unique_ptr<JpegPlenoSignatureBox> temp_signature;
  std::unique_ptr<FileTypeBox> temp_file_type;
  std::map<uint32_t, std::vector<std::unique_ptr<Box>>> temp_decoded_boxes;

  auto decode_boxes() {
    uint64_t decoded_boxes = 0;
    while (this->managed_stream.is_valid()) {
      std::cout << "have " << this->managed_stream.get_length() << " bytes to decode" << std::endl;
      // auto managed_substream = managed_stream.get_sub_managed_stream(
      //     file_size - managed_stream.tell());
      // auto decoded_box = parser.parse(std::move(managed_substream));
      auto decoded_box =
          parser.parse(managed_stream.get_remaining_sub_managed_stream());
      decoded_boxes++;
      auto id = decoded_box->get_tbox().get_value();
      if (auto it = temp_decoded_boxes.find(id);
          it == temp_decoded_boxes.end()) {
        temp_decoded_boxes[id] =
            std::vector<std::unique_ptr<Box>>();  //std::move(decoded_box)
      }
      temp_decoded_boxes[id].emplace_back(std::move(decoded_box));
      std::cout << "decoded box with id: " << id << std::endl;
    }
    return decoded_boxes;
  }


  auto get_signature_box() {
    return std::move(temp_signature);
  }


  auto get_file_type_box() {
    return std::move(temp_file_type);
  }


 public:
  JPLFileParser(const std::string& filename)
      : filename(filename), file_size(std::filesystem::file_size(filename)),
        if_stream(filename, std::ifstream::binary),
        managed_stream(if_stream, static_cast<uint64_t>(file_size)) {
    if (file_size < 20) {
      throw JPLFileFromStreamExceptions::InvalidTooSmallFileException(
          file_size);
    }
    temp_signature = parser.parse<JpegPlenoSignatureBox>(
        managed_stream.get_remaining_sub_managed_stream());
    temp_file_type = parser.parse<FileTypeBox>(
        managed_stream.get_remaining_sub_managed_stream());
  }


  virtual ~JPLFileParser() {
    //! \todo check why it is needed to release the remaining ptrs in the map
    for (auto& something : temp_decoded_boxes) {
      for (auto& ptr : something.second) {
        ptr.release();
      }
    }
  }
};


class JPLFileFromStream : public JPLFileParser, public JPLFile {
 protected:
  uint64_t decoded_boxes =
      2;  //it has at least decoded the signature and file type...

  void check_boxes_constraints() {
    if (auto it = temp_decoded_boxes.find(FileTypeBox::id);
        it != temp_decoded_boxes.end()) {
      throw JPLFileFromStreamExceptions::MoreThanOneFileTypeBoxException();
    }
  }


  void populate_light_field_codestreams() {
    if (auto it = temp_decoded_boxes.find(JpegPlenoLightFieldBox::id);
        it != temp_decoded_boxes.end()) {
      auto& jpeg_pleno_light_field_boxes = it->second;
      for (auto& jpeg_pleno_light_field_box : jpeg_pleno_light_field_boxes) {
        jpeg_pleno_codestreams.emplace_back(
            std::unique_ptr<JpegPlenoCodestreamBox>(
                static_cast<JpegPlenoCodestreamBox*>(
                    jpeg_pleno_light_field_box.release())));
        std::cout << "Found!" << std::endl;
      }
      jpeg_pleno_light_field_boxes.clear();
      jpeg_pleno_light_field_boxes.shrink_to_fit();
    }    
  }

  //! \todo Implement for Point Cloud Boxes...
  void populate_point_cloud_codestreams() {
    // if (auto it = temp_decoded_boxes.find(JpegPlenoPointCloudBox::id);
    //     it != temp_decoded_boxes.end()) {
    //     ...
    // }
  }


  void populate_hologram_codestreams() {
    // if (auto it = temp_decoded_boxes.find(JpegPlenoHologramBox::id);
    //     it != temp_decoded_boxes.end()) {
    //   //! \todo Implement for Point Cloud Boxes...
    // }
  }




  void populate_codestreams_list() {
    // jpeg_pleno_codestreams
    populate_light_field_codestreams();
    populate_point_cloud_codestreams();
    populate_hologram_codestreams();
  }


  void populate_jpl_fields() {
    // populate_xml_box_with_catalog();
    // populate_thumbnail();
    populate_codestreams_list();
  }

 public:
  JPLFileFromStream(const std::string& filename)
      : JPLFileParser(filename), JPLFile(JPLFileParser::get_signature_box(),
                                     JPLFileParser::get_file_type_box()) {
    if (!this->file_type_box->is_compatible_with<JpegPlenoSignatureBox>()) {
      throw JPLFileFromStreamExceptions::
          JpegPlenoNotInCompatibilityListException();
    }

    this->managed_stream.seek(12 + 20);
    decoded_boxes += decode_boxes();
    check_boxes_constraints();
    populate_jpl_fields();
    // std::move(*(this->parser.parse<XMLBoxWithCatalog>()));
  }


  auto get_number_of_decoded_boxes() {
    return decoded_boxes;
  }


  virtual ~JPLFileFromStream() = default;
};

#endif /* end of include guard: JPLFILEFROMSTREAM_H__ */