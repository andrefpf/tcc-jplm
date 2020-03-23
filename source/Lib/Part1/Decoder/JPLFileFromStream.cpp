/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */

#include "Lib/Part1/Decoder/JPLFileFromStream.h"


// thumbnail_box_position
// fist_plenoptic_box_position
// xml_box_position
// file_type_box_position
// last_plenoptic_box_position


bool is_plenoptic(const int id) {
  if ((id != static_cast<t_box_id_type>(
                 JpegPlenoCodestreamBoxTypes::LightField)) &&
      (id != static_cast<t_box_id_type>(
                 JpegPlenoCodestreamBoxTypes::PointCloud)) &&
      (id !=
          static_cast<t_box_id_type>(JpegPlenoCodestreamBoxTypes::Hologram))) {
    return false;
  }
  return true;
}


bool xml_box_has_catalog(const XMLBox& xml_box) {
  const auto& contents =
      xml_box.get_ref_to_contents().get_string_with_contents();
  if (contents.find("<pleno-elements>") == std::string::npos) {
    return false;
  }
  return true;
}


std::pair<uint64_t, uint64_t> get_min_max_positions(
    const std::vector<std::pair<uint64_t, std::unique_ptr<Box>>>& boxes) {
  auto compare = [](const auto& pair_a, const auto& pair_b) {
    return (std::get<0>(pair_a) < std::get<0>(pair_b));
  };

  auto min_pair = std::min_element(boxes.begin(), boxes.end(), compare);
  auto max_pair = std::max_element(boxes.begin(), boxes.end(), compare);

  return std::make_pair(std::get<0>(*min_pair), std::get<0>(*max_pair));
}


JPLFileFromStream::ConstrainedBoxIndex
JPLFileFromStream::get_constrained_box_index() const {
  auto index = ConstrainedBoxIndex();
  for (const auto& [id, boxes] : temp_decoded_boxes) {
    const auto& [min_position, max_position] = get_min_max_positions(boxes);

    if (is_plenoptic(id)) {
      std::cout << "Found plenoptic id" << std::endl;
      std::cout << "boxes size: " << boxes.size() << std::endl;
      std::cout << "min position " << min_position;
      std::cout << "max position " << max_position;
      index.number_of_plenoptic_elements += boxes.size();

      for (const auto& [position, xml_box] : boxes) {
        std::cout << "Position: " << position << std::endl;
      }

      if (min_position < index.fist_plenoptic_box_position) {
        index.fist_plenoptic_box_position = min_position;
      }
      if (max_position > index.last_plenoptic_box_position) {
        index.last_plenoptic_box_position = max_position;
      }
      continue;
    }

    if (id == JpegPlenoThumbnailBox::id) {
      if (max_position > index.thumbnail_box_position) {
        index.thumbnail_box_position = max_position;
      }
      continue;
    }

    if (id == XMLBox::id) {
      for (const auto& [position, xml_box] : boxes) {
        if (xml_box_has_catalog(static_cast<const XMLBox&>(*xml_box))) {
          if (index.xml_box_with_catalog_position) {
            //already have a xml box with catalog detected... what to do in this case?
          }
          index.xml_box_with_catalog_position = position;
        }
      }
    }
  }
  // if (id ==)

  index.file_type_box_position = this->file_type_box_index;

  return index;
}

void JPLFileFromStream::check_boxes_constraints() {
  //restriction 0, no more than one file type box shall exist
  if (auto it = temp_decoded_boxes.find(FileTypeBox::id);
      it != temp_decoded_boxes.end()) {
    throw JPLFileFromStreamExceptions::MoreThanOneFileTypeBoxException();
  }

  // From A.2.3 File organization

  // restriction 1: The JPEG Pleno Signature box shall be the
  // first box in a JPL file and the File Type box
  //(defined in ISO/IEC 15444-1) shall immediately follow the
  // JPEG Pleno Signature box.
  //
  // This restriction will not be tested so as the decoder can decode
  // any file that contains a file type box with the jpl signature in
  // its compatility list

  auto constrained_box_index = get_constrained_box_index();
  if (constrained_box_index.thumbnail_box_position) {
    std::cout << "thumbnail_box_position "
              << *(constrained_box_index.thumbnail_box_position) << std::endl;
  }
  if (constrained_box_index.xml_box_with_catalog_position) {
    std::cout << "xml_box_with_catalog_position "
              << *(constrained_box_index.xml_box_with_catalog_position)
              << std::endl;
  }
  std::cout << "file_type_box_position "
            << constrained_box_index.file_type_box_position << std::endl;
  std::cout << "fist_plenoptic_box_position "
            << constrained_box_index.fist_plenoptic_box_position << std::endl;
  std::cout << "last_plenoptic_box_position "
            << constrained_box_index.last_plenoptic_box_position << std::endl;
  std::cout << "number_of_plenoptic_elements "
            << constrained_box_index.number_of_plenoptic_elements << std::endl;

  // restriction 2: The JPEG Pleno Thumbnail box shall be signalled
  // before the JPEG Pleno Light Field, JPEG Pleno
  // Point Cloud, and JPEG Pleno Hologram superboxes.
  if (constrained_box_index.thumbnail_box_position) {
    //a thumbnail box was detected in the codestream
    if (*(constrained_box_index.thumbnail_box_position) >
        constrained_box_index.fist_plenoptic_box_position) {
      throw FileOrganizationExceptions::
          ThumbnailShallBeSignalledBeforePlenopticDataException(
              *(constrained_box_index.thumbnail_box_position),
              constrained_box_index.fist_plenoptic_box_position);
    }
  }


  //throw

  // restriction 3: A JPL file can contain an optional XML box
  // signalling catalog information (see A.5.8). An XML box
  // containing catalog information should be signalled after
  // the File Type box and before the first superbox containing plenoptic data.
  if (constrained_box_index.xml_box_with_catalog_position) {
    //a xml box with catalog was detected in the codestream
    if ((*(constrained_box_index.xml_box_with_catalog_position) <
            constrained_box_index.file_type_box_position) ||
        (*(constrained_box_index.xml_box_with_catalog_position) >
            constrained_box_index.fist_plenoptic_box_position)) {
      throw FileOrganizationExceptions::
          ACatalogingXLMBoxShallBeSignalledAfterFileTypeBoxAndBeforePlenopticDataException(
              *(constrained_box_index.xml_box_with_catalog_position),
              constrained_box_index.file_type_box_position,
              constrained_box_index.fist_plenoptic_box_position);
    }
  }


  // restriction 4: The JPEG Pleno Light Field, JPEG Pleno Point Cloud,
  // and JPEG Pleno Hologram superboxes signalling plenoptic data, shall
  // be signalled as one monolithic block with no preferred ordering,
  // and no other boxes shall be signalled in between.
  //
  // The comparison with max is to check if a plenoptic box was detected in the file
  if ((constrained_box_index.fist_plenoptic_box_position !=
          std::numeric_limits<uint64_t>::max()) &&
      ((constrained_box_index.last_plenoptic_box_position -
           constrained_box_index.fist_plenoptic_box_position + 1) !=
          constrained_box_index.number_of_plenoptic_elements)) {
    throw FileOrganizationExceptions::InvalidBoxBetweenPlenopticBoxesException(
        constrained_box_index.fist_plenoptic_box_position,
        constrained_box_index.last_plenoptic_box_position);
  }

  // other restrictions may be tested here
}


void JPLFileFromStream::populate_light_field_codestreams() {
  if (auto it = temp_decoded_boxes.find(JpegPlenoLightFieldBox::id);
      it != temp_decoded_boxes.end()) {
    auto& jpeg_pleno_light_field_boxes_pairs = it->second;
    for (auto& jpeg_pleno_light_field_box_pair :
        jpeg_pleno_light_field_boxes_pairs) {
      auto& jpeg_pleno_light_field_box =
          std::get<1>(jpeg_pleno_light_field_box_pair);
      jpeg_pleno_codestreams.emplace_back(
          std::unique_ptr<JpegPlenoCodestreamBox>(
              static_cast<JpegPlenoCodestreamBox*>(
                  jpeg_pleno_light_field_box.release())));
    }
    jpeg_pleno_light_field_boxes_pairs.clear();
    jpeg_pleno_light_field_boxes_pairs.shrink_to_fit();
  }
}


void JPLFileFromStream::populate_point_cloud_codestreams() {
  // if (auto it = temp_decoded_boxes.find(JpegPlenoPointCloudBox::id);
  //     it != temp_decoded_boxes.end()) {
  //     ...
  // }
}


void JPLFileFromStream::populate_hologram_codestreams() {
  // if (auto it = temp_decoded_boxes.find(JpegPlenoHologramBox::id);
  //     it != temp_decoded_boxes.end()) {
  //   //! \todo Implement for Hologram Boxes...
  // }
}


void JPLFileFromStream::populate_codestreams_list() {
  // jpeg_pleno_codestreams
  populate_light_field_codestreams();
  populate_point_cloud_codestreams();
  populate_hologram_codestreams();
}


void JPLFileFromStream::populate_jpl_fields() {
  //! \todo populate_xml_box_with_catalog();
  //! \todo populate_thumbnail();
  populate_codestreams_list();
}


JPLFileFromStream::JPLFileFromStream(const std::string& filename)
    : JPLFileParser(filename), JPLFile(JPLFileParser::get_signature_box(),
                                   JPLFileParser::get_file_type_box()) {
  if (!this->file_type_box->get_ref_to_contents().is_the_file_compatible_with(
          JpegPlenoSignatureBox::id)) {
    throw JPLFileFromStreamExceptions::
        JpegPlenoNotInCompatibilityListException();
  }
  number_of_decoded_boxes += decode_boxes();
  check_boxes_constraints();
  populate_jpl_fields();
}


uint64_t JPLFileFromStream::get_number_of_decoded_boxes() {
  return number_of_decoded_boxes;
}
