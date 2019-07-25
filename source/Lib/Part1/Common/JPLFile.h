#ifndef JPLFILE_H__
#define JPLFILE_H__

#include "FileTypeBox.h"
#include "JpegPlenoCodestreamBox.h"
#include "JpegPlenoSignatureBox.h"
#include "JpegPlenoThumbnailBox.h"
#include "IntellectualPropertyBox.h"


class JPLFile {
 protected:
  JpegPlenoSignatureBox jpeg_pleno_signature_box;
  JpegPlenoFileTypeContents file_type_box;  //this is the FileTypeBox
  // std::optional<XMLBoxWithCatalog> xml_box_with_catalog;
  std::optional<JpegPlenoThumbnailBox> jpeg_pleno_thumbnail_box;
  std::optional<std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>>
      jpeg_pleno_codestreams;
  std::optional<IntellectualPropertyBox> ipr_box;
  // std::optional<std::vector<XMLBox>> xml_boxes; //boxes??
  // std::optional<std::vector<UUIDBox>> uuid_boxes;
  // std::optional<std::vector<UUIDInfoBox>> uuid_info_boxes;
 public:
  JPLFile();
  ~JPLFile();

  JpegPlenoSignatureBox get_jpeg_pleno_signature_box() const noexcept {
    return jpeg_pleno_signature_box;
  }

  JpegPlenoFileTypeContents get_file_type_box() const noexcept {
    return file_type_box;
  }
};

#endif /* end of include guard: JPLFILE_H__ */