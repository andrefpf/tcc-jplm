
set(IMAGE_SOURCES Metrics.cpp ImageChannel.cpp Image.cpp ColorSpaces.cpp ImageColorSpacesConversor.h)

set(PIXMAP_SOURCES ImageFile.cpp PixelMapFileBinary.h PPMBinaryFile.cpp PixelMapFileIO.cpp)

add_library(image ${IMAGE_SOURCES} ${PIXMAP_SOURCES})