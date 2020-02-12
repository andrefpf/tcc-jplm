set(UTILS_IMAGE
    ColorModelUtils.cpp
    ColorSpaces.cpp
    Generic2DIterator.cpp
    Generic2DStructure.cpp
    ImageChannel.cpp
    ImageChannelUtils.cpp
    ImageColorSpacesConversor.cpp
    Image.cpp
    ImageExceptions.cpp
    ImageFile.cpp
    ImageIO.cpp
    ImageMetrics.cpp
    ImageUtils.cpp
    Metrics.cpp
    PGXFile.cpp
    PGXFileIO.cpp
    PixelMapFileBinary.cpp
    PixelMapFile.cpp
    PixelMapFileIO.cpp
    PPMBinaryFile.cpp
    Raster2DIterator.cpp
    RGBImage.cpp
    Snake2DIterator.cpp
    ThreeChannelImage.cpp
    UndefinedImage.cpp
    YCbCrImage.cpp
    YCoCgImage.cpp)

add_library(image ${UTILS_IMAGE})
target_link_libraries(image stream)