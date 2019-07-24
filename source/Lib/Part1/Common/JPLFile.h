#ifndef JPLFILE_H__
#define JPLFILE_H__

#include "JpegPlenoSignatureBox.h"
#include "FileTypeBox.h"
#include "JpegPlenoThumbnailBox.h"

class JPLFile
{
protected:
	JpegPlenoSignatureBox jpeg_pleno_signature_box;
	JpegPlenoFileTypeContents file_type_box; //this is the FileTypeBox
	// std::optional<XMLBoxWithCatalog> xml_box_with_catalog;
	std::optional<JpegPlenoThumbnailBox> jpeg_pleno_thumbnail_box;
	// std::optional<JpegPlenoLightFieldBox> jpeg_pleno_light_field_box;
	// std::optional<JpegPlenoPointCloudBox> jpeg_pleno_point_cloud_box;
	// std::optional<JpegPlenoHologramBox> jpeg_pleno_hologram_box;
	// std::optional<IPRBox> ipr_box;
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