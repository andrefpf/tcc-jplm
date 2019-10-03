#ifndef MULEDECODER_H__
#define MULEDECODER_H__

#include "App/TemporaryCommonTransformMode/MuleCodec.h"

#include "Lib/Part2/Decoder/TransformMode/Hierarchical4DDecoder.h"
#include "Lib/Part2/Decoder/TransformMode/PartitionDecoder.h"
#include "Lib/Utils/Stream/BinaryTools.h"

// void read_int_from_file(int* dest, FILE* fp);

class MuleDecoder : public MuleCodec
{
private:
	const ContiguousCodestreamCode& codestream_code;
	std::unique_ptr<LightFieldTransformMode<>> decoded_lightfield;
	Hierarchical4DDecoder hierarchical_4d_decoder;
	void open_decoded_lightfield();
public:
	MuleDecoder(ParameterHandler handler, const ContiguousCodestreamCode& codestream_code);
	~MuleDecoder();
	
	void setup_header_data_into_decoded_lightfield();
	void read_initial_data_from_compressed_file();	

	void decode();

};

#endif /* end of include guard: MULEDECODER_H__ */