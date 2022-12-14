/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     LightfieldIOConfiguration.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-12
 */
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"


void LightfieldIOConfiguration::check_configurations() {
  namespace fs = std::filesystem;
  if (!fs::is_directory(lightfield_path)) {
    throw LightfieldIOConfigurationExceptions::InvalidLightfieldPath();
  }
}


LightfieldIOConfiguration::LightfieldIOConfiguration(
    const std::string &path_to_lightfield,
    const LightfieldDimension<std::size_t> &lightfield_size,
    const LightfieldCoordinate<std::size_t> &initial_coordinate,
    uint16_t number_of_colour_channels)
    : lightfield_path(path_to_lightfield), lightfield_size(lightfield_size),
      lightfield_initial_coordinate(initial_coordinate),
      number_of_colour_channels(number_of_colour_channels) {
  check_configurations();
}


LightfieldIOConfiguration::LightfieldIOConfiguration(
    const std::string &path_to_lightfield,
    const LightfieldCoordinate<std::size_t> &initial_coordinate,
    const LightfieldCoordinate<std::size_t> &final_coordinate,
    uint16_t number_of_colour_channels)
    : LightfieldIOConfiguration(path_to_lightfield,
          LightfieldDimension(final_coordinate - initial_coordinate),
          initial_coordinate, number_of_colour_channels) {
}


uint16_t LightfieldIOConfiguration::get_number_of_colour_channels() const
    noexcept {
  return number_of_colour_channels;
}


LightfieldCoordinate<std::size_t>
LightfieldIOConfiguration::get_initial_coordinate() const noexcept {
  return lightfield_initial_coordinate;
}


LightfieldDimension<std::size_t> LightfieldIOConfiguration::get_size() const
    noexcept {
  return lightfield_size;
}


std::string LightfieldIOConfiguration::get_path() const noexcept {
  return lightfield_path;
}


std::size_t LightfieldIOConfiguration::get_number_of_pixels_per_view() const
    noexcept {
  return lightfield_size.get_number_of_pixels_per_view();
}


std::size_t LightfieldIOConfiguration::get_number_of_views_per_lightfield()
    const noexcept {
  return lightfield_size.get_number_of_views_per_lightfield();
}


std::size_t LightfieldIOConfiguration::get_number_of_pixels_per_lightfield()
    const noexcept {
  return get_number_of_views_per_lightfield() * get_number_of_pixels_per_view();
}


std::vector<std::pair<std::size_t, std::size_t>>
LightfieldIOConfiguration::get_raster_view_coordinates() const noexcept {
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
