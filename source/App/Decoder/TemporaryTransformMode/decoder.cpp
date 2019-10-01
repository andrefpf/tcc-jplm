#include "App/TemporaryCommonTransformMode/MuleCodec.h"
#include "MuleDecoder.h"

int main(int argc, char **argv) {
    constexpr auto is_encoder=false;
    ParameterHandler handler(is_encoder);//is decoder
    handler.parse(argc, argv);
    handler.print_configurations();

    MuleDecoder decoder(handler);
    decoder.decode();
}