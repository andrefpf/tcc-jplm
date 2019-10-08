#ifndef MULEDECODER_H__
#define MULEDECODER_H__

#include "App/TemporaryCommonTransformMode/MuleCodec.h"

#include "Lib/Part2/Decoder/TransformMode/Hierarchical4DDecoder.h"
#include "Lib/Part2/Decoder/TransformMode/PartitionDecoder.h"
#include "Lib/Utils/Stream/BinaryTools.h"
#include <tuple>

class MuleDecoder : public MuleCodec
{
private:
	PartitionDecoder partition_decoder;
	const ContiguousCodestreamCode& codestream_code;
	Hierarchical4DDecoder hierarchical_4d_decoder;
	void open_decoded_lightfield();
public:
	MuleDecoder(ParameterHandler handler, const ContiguousCodestreamCode& codestream_code);
	~MuleDecoder();
	
	void setup_header_data_into_decoded_lightfield();
	void read_initial_data_from_compressed_file();	

	void decode();

	virtual void run_for_block_4d(const uint32_t channel,
      const int32_t level_shift, const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& size) override;
};

#endif /* end of include guard: MULEDECODER_H__ */