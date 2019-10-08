#include "App/TemporaryCommonTransformMode/MuleCodec.h"
#include "MuleDecoder.h"
#include <filesystem>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCodeInMemory.h"

int main(int argc, char **argv) {
    constexpr auto is_encoder=false;
    ParameterHandler handler(is_encoder);//is decoder
    handler.parse(argc, argv);
    handler.print_configurations();

    auto file_size = std::filesystem::file_size(handler.encoded_lightfield);
    std::cout << file_size << std::endl;
    std::ifstream if_stream(handler.encoded_lightfield.string(), std::ifstream::binary);
    

    std::vector<std::byte> temp_vector(file_size);
    if_stream.read(reinterpret_cast<char*>(temp_vector.data()), file_size);

    ContiguousCodestreamCodeInMemory codestream_code(std::move(temp_vector));

    if_stream.close();

    MuleDecoder decoder(handler, codestream_code);
    decoder.run();
    // decoder.decode();

}