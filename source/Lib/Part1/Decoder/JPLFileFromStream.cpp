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
#include "Lib/Part1/Common/JPLFile.h"


void JPLFileFromStream::check_boxes_constraints() {
  if (auto it = temp_decoded_boxes.find(FileTypeBox::id);
      it != temp_decoded_boxes.end()) {
    throw JPLFileFromStreamExceptions::MoreThanOneFileTypeBoxException();
  }
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
  //   //! \todo Implement for Point Cloud Boxes...
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

  // this->managed_stream.seek(12 + 20);
  number_of_decoded_boxes += decode_boxes();
  check_boxes_constraints();
  populate_jpl_fields();
  // std::move(*(this->parser.parse<XMLBoxWithCatalog>()));
}


uint64_t JPLFileFromStream::get_number_of_decoded_boxes() {
  return number_of_decoded_boxes;
}
