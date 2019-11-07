/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     JPLFileFromStream.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_PART1_JPLFILEFROMSTREAM_H__
#define JPLM_LIB_PART1_JPLFILEFROMSTREAM_H__

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
      // std::cout << "have " << this->managed_stream.get_length() << " bytes to decode" << std::endl;
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
      // std::cout << "decoded box with id: " << id << std::endl;
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
    //! \todo populate_xml_box_with_catalog();
    //! \todo populate_thumbnail();
    populate_codestreams_list();
  }

 public:
  JPLFileFromStream(const std::string& filename)
      : JPLFileParser(filename), JPLFile(JPLFileParser::get_signature_box(),
                                     JPLFileParser::get_file_type_box()) {
    if (!this->file_type_box->get_ref_to_contents().is_the_file_compatible_with(
            JpegPlenoSignatureBox::id)) {
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

#endif /* end of include guard: JPLM_LIB_PART1_JPLFILEFROMSTREAM_H__ */