#ifndef JPEGPLENOFILETYPECONTENTS_H__
#define JPEGPLENOFILETYPECONTENTS_H__

#include "FileTypeBox.h"

class JpegPlenoFileTypeContents : public FileTypeContents {
 public:
  JpegPlenoFileTypeContents(uint32_t minor_version = 0,
      const std::vector<uint32_t>& compatibility_list = {})
      : FileTypeContents(static_cast<DefinedBoxesTypesUnderlyingType>(
                             DefinedBoxesTypes::JPEGPlenoSignatureBoxType),
            minor_version, compatibility_list) {
    //A file that conforms to this international standard shall have at least one CLi
    //filed and shall contain the value 'jpl\040' in one of the CLiFields
    if (!this->is_the_file_compatible_with(this->get_brand())) {
      this->add_compatible_standard_to_list(this->get_brand());
    }
  }


  JpegPlenoFileTypeContents(
      uint32_t minor_version, std::vector<uint32_t>&& compatibility_list)
      : FileTypeContents(static_cast<DefinedBoxesTypesUnderlyingType>(
                             DefinedBoxesTypes::JPEGPlenoSignatureBoxType),
            minor_version, std::move(compatibility_list)) {
    //A file that conforms to this international standard shall have at least one CLi
    //filed and shall contain the value 'jpl\040' in one of the CLiFields
    if (!this->is_the_file_compatible_with(this->get_brand())) {
      this->add_compatible_standard_to_list(this->get_brand());
    }
  }


  JpegPlenoFileTypeContents(const JpegPlenoFileTypeContents& other)
      : FileTypeContents(other) {
  }


  JpegPlenoFileTypeContents(JpegPlenoFileTypeContents&& other)
      : FileTypeContents(std::move(other)) {
  }
};


#endif /* end of include guard: JPEGPLENOFILETYPECONTENTS_H__ */