#ifndef MULEDECODER_H__
#define MULEDECODER_H__

#include "App/TemporaryCommonTransformMode/MuleCodec.h"

#include "Lib/Part2/Decoder/TransformMode/Hierarchical4DDecoder.h"
#include "Lib/Part2/Decoder/TransformMode/PartitionDecoder.h"

void read_int_from_file(int* dest, FILE* fp);

class MuleDecoder : public MuleCodec
{
private:
	Hierarchical4DDecoder hierarchical_4d_decoder;
public:
	MuleDecoder(ParameterHandler handler);
	~MuleDecoder();
	
	void setup_header_data_into_decoded_lightfield();
	void read_initial_data_from_compressed_file();	

	void decode();
	template <typename encodedColorHolder>
	void decode();

};

#endif /* end of include guard: MULEDECODER_H__ */