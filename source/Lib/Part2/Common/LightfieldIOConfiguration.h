#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDIOCONFIGURATION_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDIOCONFIGURATION_H__

#include <filesystem>
#include <tuple>
#include <vector>
#include "LightfieldDimension.h"

class LightfieldIOConfiguration {
 protected:
  const std::string lightfield_path;
  const LightfieldCoordinate<std::size_t> lightfield_initial_coordinate;
  const LightfieldDimension<std::size_t> lightfield_size;


  void check_configurations() {
    namespace fs = std::filesystem;
    if (!fs::is_directory(lightfield_path)) {
      throw LightfieldIOConfigurationExceptions::InvalidLightfieldPath();
    }
  }


 public:
  LightfieldIOConfiguration(const std::string& path_to_lightfield,
      const LightfieldCoordinate<std::size_t>& initial_coordinate,
      const LightfieldDimension<std::size_t>& lightfield_size)
      : lightfield_path(path_to_lightfield),
        lightfield_initial_coordinate(initial_coordinate),
        lightfield_size(lightfield_size) {
    check_configurations();
  };


  LightfieldIOConfiguration(const std::string& path_to_lightfield,
      const LightfieldCoordinate<std::size_t>& initial_coordinate,
      const LightfieldCoordinate<std::size_t>& final_coordinate)
      : LightfieldIOConfiguration(path_to_lightfield, initial_coordinate,
            LightfieldDimension(final_coordinate - initial_coordinate)){};


  ~LightfieldIOConfiguration() = default;


  LightfieldCoordinate<std::size_t> get_initial_coordinate() const noexcept {
    return lightfield_initial_coordinate;
  }


  LightfieldDimension<std::size_t> get_size() const noexcept {
    return lightfield_size;
  }


  std::string get_path() const noexcept {
    return lightfield_path;
  }


  std::size_t get_number_of_pixels_per_view() const noexcept {
    return lightfield_size.get_number_of_pixels_per_view();
  }


  std::size_t get_number_of_views_per_lightfield() const noexcept {
    return lightfield_size.get_number_of_views_per_lightfield();
  }


  std::size_t get_number_of_pixels_per_lightfield() const noexcept {
    return get_number_of_views_per_lightfield() *
           get_number_of_pixels_per_view();
  }


  std::vector<std::pair<std::size_t, std::size_t>> get_raster_view_coordinates()
      const noexcept {
    auto initial_t = lightfield_initial_coordinate.get_t();
    auto initial_s = lightfield_initial_coordinate.get_s();
    auto final_t = initial_t + lightfield_size.get_t();
    auto final_s = initial_s + lightfield_size.get_s();
    auto number_of_views = get_number_of_views_per_lightfield();

    std::vector<std::pair<std::size_t, std::size_t>> ret_val;
    ret_val.reserve(number_of_views);
    for (auto t = initial_t; t < final_t; ++t) {
      for (auto s = initial_s; s < final_s; ++s) {
        ret_val.emplace_back(std::make_pair(t, s));
      }
    }
    return ret_val;
  }
};


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDIOCONFIGURATION_H__ */
