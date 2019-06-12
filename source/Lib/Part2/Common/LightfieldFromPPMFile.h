#ifndef LIGHTFIELDFROMPPMFILE_H__
#define LIGHTFIELDFROMPPMFILE_H__

#include "Lib/Part2/Common/Lightfield.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Part2/Common/ViewFromPPMFile.h"

template<typename T>
class LightfieldFromPPMFile : public Lightfield<T> {
 public:
  LightfieldFromPPMFile(const LightfieldIOConfiguration& configuration)
      : Lightfield<T>(configuration.get_size().get_t_and_s(), true) {
    //TODO: check configuration.get_path()
    for (const auto& coordinate : configuration.get_raster_view_coordinates()) {
      this->set_view_at(
          std::move(ViewFromPPMFile<T>(configuration.get_path(), coordinate)),
          coordinate);
    }
    std::cout << "Finished constructing Lightfield from PPM files.."
              << std::endl;
  }
  ~LightfieldFromPPMFile() = default;

  // virtual void set_view_at(
  //     View<T>&& view, const std::pair<std::size_t, std::size_t>& coordinate) override {
  //   this->set_element_at(
  //       std::move(std::make_unique<ViewFromPPMFile<T>>(std::move(static_cast<ViewFromPPMFile<T>&&>(view)))), coordinate);
  // }
};

#endif /* end of include guard: LIGHTFIELDFROMPPMFILE_H__ */