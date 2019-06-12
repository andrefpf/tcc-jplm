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
  }
  ~LightfieldFromPPMFile() = default;

};

#endif /* end of include guard: LIGHTFIELDFROMPPMFILE_H__ */