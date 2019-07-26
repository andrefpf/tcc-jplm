#ifndef JPEGPLENOFILETYPECONTENTS_H__
#define JPEGPLENOFILETYPECONTENTS_H__

#include "FileTypeBox.h"

class JpegPlenoFileTypeContents : public FileTypeContents {
 public:
  JpegPlenoFileTypeContents(
      uint32_t minor_version = 0, std::vector<uint32_t> compatibility_list = {})
      : FileTypeContents(static_cast<DefinedBoxesTypesUnderlyingType>(
                             DefinedBoxesTypes::JPEGPlenoSignatureBoxType),
            minor_version, compatibility_list) {
    //A file that conforms to this international standard shall have at least one CLi
    //filed and shall contain the value 'jpl\040' in one of the CLiFields
    if (!this->is_the_file_compatible_with(this->get_brand())) {
      this->add_compatible_standard_to_list(this->get_brand());
    }
  }
};


#endif /* end of include guard: JPEGPLENOFILETYPECONTENTS_H__ */