#ifndef IMAGEIO_H__
#define IMAGEIO_H__

#include "PPMBinaryFile.h"
#include "PixelMapFile.h"
#include "PixelMapFileIO.h"

namespace ImageIO {

void write_image_to_file(
    const std::variant<Image<uint8_t>, Image<uint16_t>>& image,
    const std::string& filename);

template<typename T>
void write_image_to_file(const Image<T>& image, const std::string& filename);

void write_image_to_file(
    const Image<uint16_t>& image, const std::string& filename);
void write_image_to_file(
    const Image<uint8_t>& image, const std::string& filename);


template<typename T>
void write(const Image<T>& image, const std::string& filename,
    const bool overwrite = false) {
  namespace fs = std::filesystem;
  using fpath = fs::path;
  auto name = fpath(filename);

  if (!overwrite && fs::exists(filename)) {
    throw ImageIOExceptions::FileAlreadyExistsException();
  }

  if (name.extension() == fpath(".ppm")) {
    auto max_value =
        static_cast<std::size_t>(std::pow(2.0, image.get_bpp())) - 1;
    auto oppened_image = PixelMapFileIO::open(filename, PixelMapType::P6,
        image.get_width(), image.get_height(), max_value);
    auto oppened_image_as_rgb = std::unique_ptr<PPMBinaryFile>(
        static_cast<PPMBinaryFile*>(oppened_image.release()));
    oppened_image_as_rgb->write_image_to_file(image);
  }
}

template<typename T>
void write(const Image<T>& patch_image, const std::string& filename,
    std::pair<std::size_t, std::size_t> origin) {
  using fpath = std::filesystem::path;
  auto name = fpath(filename);

  if (name.extension() == fpath(".ppm")) {
    auto oppened_image =
        PixelMapFileIO::open(filename);  //should test if exists...

    auto oppened_image_as_rgb = std::unique_ptr<PPMBinaryFile>(
        static_cast<PPMBinaryFile*>(oppened_image.release()));
    oppened_image_as_rgb->write_image_patch_to_file(patch_image, origin);
  }
}


}  // namespace ImageIO

#endif /* end of include guard: IMAGEIO_H__ */