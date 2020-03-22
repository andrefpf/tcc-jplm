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

/** \file     JPLFile.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */

#include "Lib/Part1/Common/JPLFile.h"


std::ostream& operator<<(std::ostream& os, const JPLFile& jpl_file) {
  os << *(jpl_file.jpeg_pleno_signature_box) << *(jpl_file.file_type_box);
  if (jpl_file.xml_box_with_catalog) {
    jpl_file.xml_box_with_catalog =
        CatalogGenerator::get_xml_box_with_updated_catalog(
            jpl_file.jpeg_pleno_codestreams);
    os << (*jpl_file.xml_box_with_catalog);
  }
  if (jpl_file.jpeg_pleno_thumbnail_box) {
    os << (*jpl_file.jpeg_pleno_thumbnail_box);
  }
  for (const auto& codestream : jpl_file.jpeg_pleno_codestreams) {
    os << *codestream;
  }
  if (jpl_file.ipr_box) {
    os << (*jpl_file.ipr_box);
  }
  if (jpl_file.uuid_boxes) {
    const auto& uuid_boxes = *(jpl_file.uuid_boxes);
    for (const auto& uuid_box : uuid_boxes) {
      os << *uuid_box;
    }
  }
  if (jpl_file.uuid_info_boxes) {
    const auto& uuid_info_boxes = *(jpl_file.uuid_info_boxes);
    for (const auto& uuid_info_boxe : uuid_info_boxes) {
      os << *uuid_info_boxe;
    }
  }
  return os;
}


std::size_t JPLFile::size() const noexcept {
  std::size_t size = 0;
  size += this->jpeg_pleno_signature_box->size();
  size += this->file_type_box->size();
  if (this->xml_box_with_catalog) {
    //first updates the xml box with catalog if it already exists
    this->xml_box_with_catalog =
        CatalogGenerator::get_xml_box_with_updated_catalog(
            this->jpeg_pleno_codestreams);
    //only then gets the size of the xml box with catalog
    size += this->xml_box_with_catalog->size();
  }
  if (this->jpeg_pleno_thumbnail_box) {
    size += this->jpeg_pleno_thumbnail_box->size();
  }
  for (const auto& codestream : this->jpeg_pleno_codestreams) {
    size += codestream->size();
  }
  if (this->ipr_box) {
    size += this->ipr_box->size();
  }
  if (this->uuid_boxes) {
    const auto& uuid_boxes = *(this->uuid_boxes);
    for (const auto& uuid_box : uuid_boxes) {
      size += uuid_box->size();
    }
  }
  if (this->uuid_info_boxes) {
    const auto& uuid_info_boxes = *(this->uuid_info_boxes);
    for (const auto& uuid_info_boxe : uuid_info_boxes) {
      size += uuid_info_boxe->size();
    }
  }
  return size;
}

JPLFile::JPLFile(const FileTypeBox& file_type_box)
    : jpeg_pleno_signature_box(std::make_unique<JpegPlenoSignatureBox>()),
      file_type_box(std::make_unique<FileTypeBox>(file_type_box)) {
}


JPLFile::JPLFile(const JpegPlenoSignatureBox& jpeg_pleno_signature_box,
    const FileTypeBox& file_type_box)
    : jpeg_pleno_signature_box(
          std::make_unique<JpegPlenoSignatureBox>(jpeg_pleno_signature_box)),
      file_type_box(std::make_unique<FileTypeBox>(file_type_box)) {
}


FileTypeBox JPLFile::get_file_type_box() const noexcept {
  return *file_type_box;
}


JpegPlenoSignatureBox JPLFile::get_jpeg_pleno_signature_box() const noexcept {
  return *jpeg_pleno_signature_box;
}


std::size_t JPLFile::number_of_codestreams() const noexcept {
  return jpeg_pleno_codestreams.size();
}


bool JPLFile::has_codestream() const noexcept {
  return jpeg_pleno_codestreams.size() > 0;
}


bool JPLFile::has_thumbnail() const noexcept {
  return jpeg_pleno_thumbnail_box ? true : false;
}


JPLFile& JPLFile::add_thumbnail_box(
    const JpegPlenoThumbnailBox& thumbnail_box) {  //thumbnail_box
  jpeg_pleno_thumbnail_box =
      std::make_unique<JpegPlenoThumbnailBox>(thumbnail_box);
  return *this;
}


JPLFile& JPLFile::add_codestream_box(
    std::unique_ptr<JpegPlenoCodestreamBox>&& codestream_box) {
  jpeg_pleno_codestreams.emplace_back(std::move(codestream_box));
  return *this;
}


JPLFile::JPLFile(
    std::unique_ptr<JpegPlenoSignatureBox>&& jpeg_pleno_signature_box,
    std::unique_ptr<FileTypeBox>&& file_type_box)
    : jpeg_pleno_signature_box(std::move(jpeg_pleno_signature_box)),
      file_type_box(std::move(file_type_box)) {
}


JPLFile::JPLFile(JpegPlenoSignatureBox&& jpeg_pleno_signature_box,
    FileTypeBox&& file_type_box)
    : jpeg_pleno_signature_box(std::make_unique<JpegPlenoSignatureBox>(
          std::move(jpeg_pleno_signature_box))),
      file_type_box(std::make_unique<FileTypeBox>(std::move(file_type_box))) {
}


std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>&
JPLFile::get_reference_to_codestreams() noexcept {
  return jpeg_pleno_codestreams;
}


const std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>&
JPLFile::get_reference_to_codestreams() const noexcept {
  return jpeg_pleno_codestreams;
}
