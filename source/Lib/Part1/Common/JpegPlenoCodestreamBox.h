#ifndef JPEGPLENOCODESTREAMBOX_H__
#define JPEGPLENOCODESTREAMBOX_H__

//the ideia is to be the base for 
// JpegPlenoLightFieldBox
// JpegPlenoPointCloudBox
// JpegPlenoHologramBox
// 

#include "Box.h"

class JpegPlenoCodestreamBox : public Box
{
public:
	JpegPlenoCodestreamBox(const TBox& type_box, const DBox& data_box) : Box(type_box, data_box) {}
	virtual ~JpegPlenoCodestreamBox() = default;
};

#endif /* end of include guard: JPEGPLENOCODESTREAMBOX_H__ */