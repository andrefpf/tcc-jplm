#ifndef VIEWFROMPPMFILE_H__
#define VIEWFROMPPMFILE_H__

#include "Lib/Part2/Common/View.h"
#include "Lib/Part2/Common/ViewToFilenameTranslator.h"
#include "Lib/Utils/Image/PixelMapFile.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"

template<typename T>
class ViewFromPPMFile : public View<T> {
 protected:
  std::unique_ptr<ViewToFilenameTranslator> name_translator;
  std::unique_ptr<PixelMapFile> ppm_file;

 public:
  ViewFromPPMFile(const std::string& path,
      const std::pair<std::size_t, std::size_t>& position)
      //using the default view constructor because we cannot initialize first the derived class members
      : View<T>(), name_translator(std::make_unique<PPM3CharViewToFilename>()),
        ppm_file(PixelMapFileIO::open(
            {path + name_translator->view_position_to_filename(position)})) {
          std::cout << "initialized view" << std::endl;
    this->view_size = {ppm_file->get_width(), ppm_file->get_height()};
    std::cout << "got size" << std::endl;
    this->bpp = ppm_file->get_number_of_bits_per_pixel();
    std::cout << "got bpp" << std::endl;
  };


  ViewFromPPMFile(ViewFromPPMFile&& other) {
    std::cout << "in move contructor" << std::endl;
    std::swap(name_translator, other.name_translator);
    std::swap(ppm_file, other.ppm_file);
  }


  //     auto oppened_image_as_rgb = std::unique_ptr<PPMBinaryFile>(
  // static_cast<PPMBinaryFile*>(oppened_image.release()));

  ~ViewFromPPMFile() = default;
};

#endif /* end of include guard: VIEWFROMPPMFILE_H__ */