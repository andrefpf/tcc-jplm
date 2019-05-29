
set(IMAGE_SOURCES Metrics.cpp ImageChannel.cpp Image.cpp ColorSpaces.cpp ImageColorSpacesConversor.h ImageFile.cpp)
set(PIXMAP_SOURCES PixelMapFile.cpp PixelMapFileBinary.h PPMBinaryFile.cpp PixelMapFileIO.cpp)
add_library(image ${IMAGE_SOURCES} ${PIXMAP_SOURCES})