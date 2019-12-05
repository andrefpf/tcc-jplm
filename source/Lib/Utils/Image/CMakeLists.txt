
set(IMAGE_SOURCES Metrics.cpp ImageIO.cpp ImageChannel.cpp Image.cpp ColorModelUtils.cpp ColorSpaces.cpp ImageColorSpacesConversor.h)

set(IMAGE_FILE_SOURCES ImageFile.cpp)

set(PIXMAP_SOURCES PixelMapFile.cpp PixelMapFileBinary.h PPMBinaryFile.cpp PixelMapFileIO.cpp)

set(PGX_SOURCES PGXFile.cpp PGXFileIO.cpp)

add_library(image ${IMAGE_SOURCES} ${IMAGE_FILE_SOURCES} ${PIXMAP_SOURCES} ${PGX_SOURCES})