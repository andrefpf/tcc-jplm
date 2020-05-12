/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
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

/** \file     Lenslet13x13Shifter.cpp
 *  \brief    Test of PPM Binary File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-30
 */

#include <filesystem>
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include "Lib/Utils/BasicConfiguration/BasicConfiguration.h"
#include "Lib/Utils/Image/ImageExceptions.h"
#include "Lib/Utils/Image/ImageIO.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"
#include "cppitertools/product.hpp"
#include "cppitertools/range.hpp"
#include "tqdm-cpp/tqdm.hpp"


class Lenslet13x13ShifterConfiguration : public BasicConfiguration {
 private:
  static constexpr std::size_t current_hierarchy_level = 0;

 protected:
  std::string input;
  std::string output;
  std::string direction;
  bool show_progress_bar_flag = false;

  Lenslet13x13ShifterConfiguration(int argc, char** argv, std::size_t level)
      : BasicConfiguration(argc, argv, level) {
  }


  virtual void add_options() override {
    BasicConfiguration::add_options();


    this->add_cli_json_option({"--input", "-i",
        "Input directory that contains at least three subdirectories with PGX "
        "files.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          if (conf.contains("input")) {
            return conf["input"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          this->input = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--output", "-o",
        "Output PPM (P6) filename. The name of the output file will be used to "
        "detect the input name in each channel directory. ",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          if (conf.contains("output")) {
            return conf["output"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          this->output = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--direction", "-d",
        "Direction of shift ('encoder' to be used in the encoder or"
        " 'decoder' to be used after the decoding process). ",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          if (conf.contains("direction")) {
            return conf["direction"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          this->direction = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--show-progress-bar", "-progress",
        "Enables the display of a progress bar showing the percentage of "
        "completion, run time and expected finishing time.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          if (conf.contains("show-progress-bar")) {
            return conf["show-progress-bar"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string arg) {
          if (arg == "false") {
            this->show_progress_bar_flag = false;
          } else {
            this->show_progress_bar_flag = true;
          }
        },
        this->current_hierarchy_level,
        {[this]() -> std::string { return "false"; }}});
  }

 public:
  Lenslet13x13ShifterConfiguration(int argc, char** argv)
      : Lenslet13x13ShifterConfiguration(argc, argv,
            Lenslet13x13ShifterConfiguration::current_hierarchy_level) {
    this->init(argc, argv);
  }


  virtual ~Lenslet13x13ShifterConfiguration() = default;


  const std::string& get_input_filename() const {
    return input;
  }


  const std::string& get_output_filename() const {
    return output;
  }


  const std::string& get_direction() const {
    return direction;
  }


  bool show_progress_bar() const {
    return show_progress_bar_flag;
  }
};


void shift_view(const std::string& filename_input,
    const std::string& filename_output, int8_t value) {
  auto view_file = PixelMapFileIO::open(filename_input);
  auto view_variant = view_file->read_full_image();
  std::visit(
      [filename_output, value](auto& view) {
        view->shift_pixels_by(value);
        ImageIO::imwrite(*(view.get()), filename_output);
      },
      view_variant);
}


void copy_view(
    const std::string& filename_input, const std::string& filename_output) {
  namespace fs = std::filesystem;
  if (!fs::exists(filename_input)) {
    throw ImageIOExceptions::InexistentFileException();
  }
  if (fs::exists(filename_output)) {
    throw ImageIOExceptions::FileAlreadyExistsException();
  }
  fs::copy(filename_input, filename_output);
}


std::string get_view_name(
    const std::pair<std::size_t, std::size_t>& coordinate) {
  std::ostringstream string_stream;
  string_stream << std::setw(3) << std::setfill('0') << std::get<0>(coordinate)
                << '_' << std::setw(3) << std::setfill('0')
                << std::get<1>(coordinate) << ".ppm";
  return string_stream.str();
}


bool are_input_and_output_paths_valid(
    const std::string& input_path, const std::string& output_path) {
  auto valid = true;
  if (input_path == output_path) {
    std::cerr << "Input and output paths must be different" << std::endl;
    valid = false;
  }

  if (!std::filesystem::is_directory(input_path)) {
    std::cerr << "Input path must be a directory" << std::endl;
    valid = false;
  }

  if (!std::filesystem::is_directory(output_path)) {
    std::cerr << "Output_path path must be a directory" << std::endl;
    valid = false;
  }
  return valid;
}


void shift_for_encoding(const std::string& input_path,
    const std::string& output_path, bool show_bar) {
  auto initial_t = 1;
  auto initial_s = 1;
  auto final_t = initial_t + 13;
  auto final_s = initial_s + 13;

  auto t_range = iter::range(initial_t, final_t);
  auto s_range = iter::range(initial_s, final_s);
  std::vector<std::tuple<int, int>> dimensions;

  for (auto&& coordinate : iter::product(t_range, s_range)) {
    dimensions.push_back(coordinate);
  }

  if (show_bar) {
    for (auto&& [t, s] : tq::tqdm(dimensions)) {
      auto input_view_name = get_view_name({t, s});
      auto output_view_name = get_view_name({t - 1, s - 1});
      if ((t == initial_t) || (t == final_t - 1)) {
        if ((s == initial_s) || (s == final_s - 1)) {
          shift_view({input_path + input_view_name},
              {output_path + output_view_name}, 2);
          continue;
        }
      }
      copy_view(
          {input_path + input_view_name}, {output_path + output_view_name});
    }
  } else {
    for (auto&& [t, s] : dimensions) {
      auto input_view_name = get_view_name({t, s});
      auto output_view_name = get_view_name({t - 1, s - 1});
      if ((t == initial_t) || (t == final_t - 1)) {
        if ((s == initial_s) || (s == final_s - 1)) {
          std::cout << "Shifting view " << input_view_name << '\n';
          shift_view({input_path + input_view_name},
              {output_path + output_view_name}, 2);
          continue;
        }
      }
      std::cout << "Copying view " << input_view_name << '\n';
      copy_view(
          {input_path + input_view_name}, {output_path + output_view_name});
    }
  }
}


void shift_for_decoding(const std::string& input_path,
    const std::string& output_path, bool show_bar) {
  auto initial_t = 0;
  auto initial_s = 0;
  auto final_t = initial_t + 13;
  auto final_s = initial_s + 13;


  auto t_range = iter::range(initial_t, final_t);
  auto s_range = iter::range(initial_s, final_s);
  std::vector<std::tuple<int, int>> dimensions;

  for (auto&& coordinate : iter::product(t_range, s_range)) {
    dimensions.push_back(coordinate);
  }


  if (show_bar) {
    for (auto&& [t, s] : tq::tqdm(dimensions)) {
      auto input_view_name = get_view_name({t, s});
      auto output_view_name = get_view_name({t + 1, s + 1});
      if ((t == initial_t) || (t == final_t - 1)) {
        if ((s == initial_s) || (s == final_s - 1)) {
          shift_view({input_path + input_view_name},
              {output_path + output_view_name}, -2);
          continue;
        }
      }
      copy_view(
          {input_path + input_view_name}, {output_path + output_view_name});
    }
  } else {
    for (auto&& [t, s] : dimensions) {
      auto input_view_name = get_view_name({t, s});
      auto output_view_name = get_view_name({t + 1, s + 1});
      if ((t == initial_t) || (t == final_t - 1)) {
        if ((s == initial_s) || (s == final_s - 1)) {
          std::cout << "Shifting view " << input_view_name << '\n';
          shift_view({input_path + input_view_name},
              {output_path + output_view_name}, -2);
          continue;
        }
      }
      std::cout << "Copying view " << input_view_name << '\n';
      copy_view(
          {input_path + input_view_name}, {output_path + output_view_name});
    }
  }
}


int main(int argc, char const* argv[]) {
  auto configuration =
      Lenslet13x13ShifterConfiguration(argc, const_cast<char**>(argv));
  if (configuration.is_help_mode()) {
    exit(0);
  }

  std::string input_path = configuration.get_input_filename();
  std::string output_path = configuration.get_output_filename();
  std::string direction = configuration.get_direction();
  bool show_bar = configuration.show_progress_bar();

  if (!are_input_and_output_paths_valid(input_path, output_path)) {
    exit(1);
  }

  if (direction == "decode" || direction == "d" || direction == "decoding") {
    shift_for_decoding(input_path, output_path, show_bar);
  } else {
    shift_for_encoding(input_path, output_path, show_bar);
  }

  return 0;
}