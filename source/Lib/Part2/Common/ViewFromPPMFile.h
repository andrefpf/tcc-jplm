#ifndef VIEWFROMPPMFILE_H__
#define VIEWFROMPPMFILE_H__

#include "Lib/Part2/Common/View.h"
#include "Lib/Part2/Common/ViewToFilenameTranslator.h"
#include "Lib/Utils/Image/ImageUtils.h"
#include "Lib/Utils/Image/PixelMapFile.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"

template<typename T>
class ViewFromPPMFile : public View<T> {
 protected:
  const std::string path;
  const std::pair<std::size_t, std::size_t> position;
  std::unique_ptr<ViewToFilenameTranslator> name_translator;
  std::unique_ptr<PixelMapFile> ppm_file;

 public:
  ViewFromPPMFile(const std::string& path,
      const std::pair<std::size_t, std::size_t>& position)
      //using the default view constructor because we cannot initialize first the derived class members
      : View<T>(), path(path), position(position),
        name_translator(std::make_unique<PPM3CharViewToFilename>()),
        ppm_file(PixelMapFileIO::open(
            {path + name_translator->view_position_to_filename(position)})) {
    this->view_size = {ppm_file->get_width(), ppm_file->get_height()};
    this->bpp = ppm_file->get_number_of_bits_per_pixel();
  };


  ViewFromPPMFile(const ViewFromPPMFile& other)
      : View<T>(other), path(other.path), position(other.position),
        name_translator(std::make_unique<PPM3CharViewToFilename>()),
        ppm_file(PixelMapFileIO::open(
            {path + name_translator->view_position_to_filename(position)})) {
  }


  ViewFromPPMFile(ViewFromPPMFile&& other) noexcept
      : View<T>(std::move(other)) {
    std::swap(name_translator, other.name_translator);
    std::swap(ppm_file, other.ppm_file);
  }


  void load_image(const std::pair<std::size_t, std::size_t>& size,
      const std::pair<std::size_t, std::size_t>& initial = {0, 0}) override {
    const auto& [i, j] = initial;
    if ((i == 0) && (j == 0) && (size == this->view_size)) {
      //loads the entire image
      auto readed_variant = ppm_file->read_full_image();
      auto readed_image =
          PixelMapFileIO::extract_image_with_type_from_variant<RGBImage,
              uint16_t>(readed_variant);
      this->image_ = ImageUtils::get_image_with_new_container_type<RGBImage,
          RGBImage, T, uint16_t>(readed_image);
    } else {
      //loads image patch
    }
  }

  ~ViewFromPPMFile() = default;
};

#endif /* end of include guard: VIEWFROMPPMFILE_H__ */