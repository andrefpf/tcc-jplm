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

/** \file     LightFieldViewCoordinateChange.cpp
 *  \brief    This util is intended to change the filenames of light-field views given an offset
 *  \details  The reference software only reads lightfields starting at position 0,0 (t, s) and 
 *  expects the displacements between views to be one. 
 *  
 *  For instance, the <a href="https://jpeg.org/plenodb/lf/pleno_lf/set2.zip">Set2 2K sub</a> dataset must be updated
 *  before being used as input in the reference software. To update it to the correct coordinates, 
 *  it is possible to use the following command:
 *  
 *  
 *  ```bash 
 *  ./bin/lightfield_coordinate_shift --input ~/RAW/set2/ --output ~/RAW/set2_corrected_coordinates/ --initial_input_t 0 --initial_input_s 2 --initial_output_t 0 --initial_output_s 0 --step_input_t 2 --step_input_s 3 
 *  ```
 *  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-01-29
 */

#ifdef _WIN32
#include "getopt.h"
#else
#include <getopt.h>
#endif

#include <filesystem>
#include <iostream>
#include <vector>
#include "Lib/Part2/Common/PPM3CharViewToFilename.h"
#include "Lib/Utils/BasicConfiguration/BasicConfiguration.h"
#include "Lib/Utils/Image/ImageExceptions.h"


class LightfieldCoordinateChangeConfiguration : public BasicConfiguration {
 private:
  static constexpr std::size_t current_hierarchy_level = 0;

 protected:
  std::string input;
  std::string output;

  bool need_to_compute_final_coordinate = true;

  uint32_t input_initial_t;
  uint32_t input_initial_s;
  uint32_t input_step_t;
  uint32_t input_step_s;
  uint32_t input_final_t;
  uint32_t input_final_s;

  uint32_t output_initial_t;
  uint32_t output_initial_s;
  uint32_t output_step_t;
  uint32_t output_step_s;

  /**
   * @brief      Constructs a new instance.
   *
   * @param[in]  argc   The count of arguments
   * @param      argv   The arguments array
   * @param[in]  level  The level
   */
  LightfieldCoordinateChangeConfiguration(
      int argc, char** argv, std::size_t level)
      : BasicConfiguration(argc, argv, level) {
  }


  /**
   * @brief      Adds options.
   */
  virtual void add_options() override {
    BasicConfiguration::add_options();


    this->add_cli_json_option(
        {"--input", "-i", "Input path that contains the light field.",
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
        "Output path to contain the light field with shifted coordinates.",
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


    this->add_cli_json_option({"--input_initial_t", "-Iit",
        "Initial T value for the input lightfield.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          try {
            return std::to_string(
                conf.at("input_shift").at("initial").at("t").get<uint32_t>());
          } catch (nlohmann::json::out_of_range& e) {
          }
          return std::nullopt;
        },
        [this](std::string arg) {
          this->input_initial_t = static_cast<uint32_t>(std::stoul(arg));
        },
        this->current_hierarchy_level,
        {[this]() -> std::string { return "0"; }}});

    this->add_cli_json_option({"--input_initial_s", "-Iis",
        "Initial S value for the input lightfield.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          try {
            return std::to_string(
                conf.at("input_shift").at("initial").at("s").get<uint32_t>());
          } catch (nlohmann::json::out_of_range& e) {
          }
          return std::nullopt;
        },
        [this](std::string arg) {
          this->input_initial_s = static_cast<uint32_t>(std::stoul(arg));
        },
        this->current_hierarchy_level,
        {[this]() -> std::string { return "0"; }}});

    this->add_cli_json_option(
        {"--input_step_t", "-Ist", "Step to sample in T direction.",
            [this](const nlohmann::json& conf) -> std::optional<std::string> {
              try {
                return std::to_string(
                    conf.at("input_shift").at("step").at("t").get<uint32_t>());
              } catch (nlohmann::json::out_of_range& e) {
              }
              return std::nullopt;
            },
            [this](std::string arg) {
              this->input_step_t = static_cast<uint32_t>(std::stoul(arg));
            },
            this->current_hierarchy_level,
            {[this]() -> std::string { return "1"; }}});

    this->add_cli_json_option(
        {"--input_step_s", "-Iss", "Step to sample in S direction.",
            [this](const nlohmann::json& conf) -> std::optional<std::string> {
              try {
                return std::to_string(
                    conf.at("input_shift").at("step").at("s").get<uint32_t>());
              } catch (nlohmann::json::out_of_range& e) {
              }
              return std::nullopt;
            },
            [this](std::string arg) {
              this->input_step_s = static_cast<uint32_t>(std::stoul(arg));
            },
            this->current_hierarchy_level,
            {[this]() -> std::string { return "1"; }}});

    this->add_cli_json_option({"--input_final_t", "-Ift",
        "Final T value. If neither final T nor final S are set, all files that "
        "meet the naming requirements will be considered as to be shifted. ",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          try {
            auto ret_val = std::to_string(
                conf.at("input_shift").at("final").at("t").get<uint32_t>());
            this->need_to_compute_final_coordinate = false;
            return ret_val;
          } catch (nlohmann::json::out_of_range& e) {
          }
          return std::nullopt;
        },
        [this](std::string arg) {
          this->input_final_t = static_cast<uint32_t>(std::stoul(arg));
        },
        this->current_hierarchy_level,
        {[this]() -> std::string { return "0"; }}});


    this->add_cli_json_option({"--input_final_s", "-Ifs",
        "Final S value. If neither final T nor final S are set, all files that "
        "meet the naming requirements will be considered as to be shifted. ",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          try {
            auto ret_val = std::to_string(
                conf.at("input_shift").at("final").at("s").get<uint32_t>());
            this->need_to_compute_final_coordinate = false;
            return ret_val;
          } catch (nlohmann::json::out_of_range& e) {
          }
          return std::nullopt;
        },
        [this](std::string arg) {
          this->input_final_s = static_cast<uint32_t>(std::stoul(arg));
        },
        this->current_hierarchy_level,
        {[this]() -> std::string { return "0"; }}});


    this->add_cli_json_option({"--output_initial_t", "-Oit",
        "Initial T value for the output lightfield.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          try {
            return std::to_string(
                conf.at("output_shift").at("initial").at("t").get<uint32_t>());
          } catch (nlohmann::json::out_of_range& e) {
          }
          return std::nullopt;
        },
        [this](std::string arg) {
          this->output_initial_t = static_cast<uint32_t>(std::stoul(arg));
        },
        this->current_hierarchy_level,
        {[this]() -> std::string { return "0"; }}});

    this->add_cli_json_option({"--output_initial_s", "-Ois",
        "Initial S value for the output lightfield.",
        [this](const nlohmann::json& conf) -> std::optional<std::string> {
          try {
            return std::to_string(
                conf.at("output_shift").at("initial").at("s").get<uint32_t>());
          } catch (nlohmann::json::out_of_range& e) {
          }
          return std::nullopt;
        },
        [this](std::string arg) {
          this->output_initial_s = static_cast<uint32_t>(std::stoul(arg));
        },
        this->current_hierarchy_level,
        {[this]() -> std::string { return "0"; }}});

    this->add_cli_json_option(
        {"--output_step_t", "-Ost", "Step to sample in T direction.",
            [this](const nlohmann::json& conf) -> std::optional<std::string> {
              try {
                return std::to_string(
                    conf.at("output_shift").at("step").at("t").get<uint32_t>());
              } catch (nlohmann::json::out_of_range& e) {
              }
              return std::nullopt;
            },
            [this](std::string arg) {
              this->output_step_t = static_cast<uint32_t>(std::stoul(arg));
            },
            this->current_hierarchy_level,
            {[this]() -> std::string { return "1"; }}});

    this->add_cli_json_option(
        {"--output_step_s", "-Oss", "Step to sample in S direction.",
            [this](const nlohmann::json& conf) -> std::optional<std::string> {
              try {
                return std::to_string(
                    conf.at("output_shift").at("step").at("s").get<uint32_t>());
              } catch (nlohmann::json::out_of_range& e) {
              }
              return std::nullopt;
            },
            [this](std::string arg) {
              this->output_step_s = static_cast<uint32_t>(std::stoul(arg));
            },
            this->current_hierarchy_level,
            {[this]() -> std::string { return "1"; }}});
  }


  std::pair<std::size_t, std::size_t> find_final_coordinates() {
    const auto& [initial_t, initial_s] = this->get_initial_input_coordinate();
    const auto& [step_t, step_s] = this->get_step_input();
    auto name_translator = PPM3CharViewToFilename();

    //find max_t
    auto t = initial_t;
    while (std::filesystem::is_regular_file(
        input + name_translator.view_position_to_filename({t, initial_s}))) {
      t += step_t;
    }

    //find max_s
    auto s = initial_s;
    while (std::filesystem::is_regular_file(
        input + name_translator.view_position_to_filename({initial_t, s}))) {
      s += step_s;
    }

    return {t, s};
  }

 public:
  /**
   * @brief      Constructs a new instance.
   *
   * @param[in]  argc  The count of arguments
   * @param      argv  The arguments array
   */
  LightfieldCoordinateChangeConfiguration(int argc, char** argv)
      : LightfieldCoordinateChangeConfiguration(argc, argv,
            LightfieldCoordinateChangeConfiguration::current_hierarchy_level) {
    this->init(argc, argv);
    if (need_to_compute_final_coordinate) {
      std::tie(input_final_t, input_final_s) = find_final_coordinates();
      need_to_compute_final_coordinate = false;
    }
  }


  /**
   * @brief      Destroys the object.
   */
  virtual ~LightfieldCoordinateChangeConfiguration() = default;


  /**
   * @brief      Gets the input filename.
   *
   * @return     The input filename.
   */
  const std::string& get_input_filename() const {
    return input;
  }


  /**
   * @brief      Gets the output filename.
   *
   * @return     The output filename.
   */
  const std::string& get_output_filename() const {
    return output;
  }


  /**
   * @brief      Determines if required to compute the final coordidate.
   *
   * @return     True if required to compute the final coordidate, False otherwise.
   */
  bool is_required_to_compute_the_final_coordidate() const {
    return need_to_compute_final_coordinate;
  }


  /**
   * @brief      Gets the initial input coordinate.
   *
   * @return     The initial input coordinate.
   */
  std::pair<std::size_t, std::size_t> get_initial_input_coordinate() const {
    return {input_initial_t, input_initial_s};
  }


  /**
   * @brief      Gets the initial output coordinate.
   *
   * @return     The initial output coordinate.
   */
  std::pair<std::size_t, std::size_t> get_initial_output_coordinate() const {
    return {output_initial_t, output_initial_s};
  }


  /**
   * @brief      Gets the final input coordinate.
   *
   * @return     The final input coordinate.
   */
  std::pair<std::size_t, std::size_t> get_final_input_coordinate() const {
    return {input_final_t, input_final_s};
  }


  /**
   * @brief      Gets the step input.
   *
   * @return     The step input.
   */
  std::pair<std::size_t, std::size_t> get_step_input() const {
    return {input_step_t, input_step_s};
  }


  /**
   * @brief      Gets the step output.
   *
   * @return     The step output.
   */
  std::pair<std::size_t, std::size_t> get_step_output() const {
    return {output_step_t, output_step_s};
  }
};


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


/**
 * @brief      Verifies if the input and output paths are valid.
 * 
 *
 * @param[in]  input_path   The input path
 * @param[in]  output_path  The output path
 *
 * @return     True if input and output paths are valid, false otherwise.
 */
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
    std::filesystem::create_directory(output_path);
    if (!std::filesystem::is_directory(output_path)) {
      std::cerr << "Output_path path must be a directory" << std::endl;
      valid = false;
    }
  }
  return valid;
}


/**
 * @brief      Shifts the light field coordinates (filenames)
 *
 * @param[in]  configuration  The configuration
 */
void shift_coordinate_in_filenames(
    const LightfieldCoordinateChangeConfiguration& configuration) {
  const auto& [initial_input_t, initial_input_s] =
      configuration.get_initial_input_coordinate();
  const auto& [initial_output_t, initial_output_s] =
      configuration.get_initial_output_coordinate();
  const auto& [final_t, final_s] = configuration.get_final_input_coordinate();
  const auto& [step_input_t, step_input_s] = configuration.get_step_input();
  const auto& [step_output_t, step_output_s] = configuration.get_step_output();

  const std::string& input_path = configuration.get_input_filename();
  const std::string& output_path = configuration.get_output_filename();

  auto output_t = initial_output_t;
  auto output_s = initial_output_s;

  auto name_translator = PPM3CharViewToFilename();

  for (auto t = initial_input_t; t < final_t; t += step_input_t) {
    for (auto s = initial_input_s; s < final_s; s += step_input_s) {
      auto input_view_name = name_translator.view_position_to_filename({t, s});
      auto output_view_name =
          name_translator.view_position_to_filename({output_t, output_s});
      std::cout << "Copying view " << input_view_name << '\n';
      std::cout << "To view " << output_view_name << '\n';
      copy_view(
          {input_path + input_view_name}, {output_path + output_view_name});
      output_s += step_output_s;
    }
    output_t += step_output_t;
    output_s = initial_output_s;
  }
}


int main(int argc, char** argv) {
  auto configuration = LightfieldCoordinateChangeConfiguration(argc, argv);
  if (configuration.is_help_mode()) {
    exit(0);
  }

  //parameter checking
  if (!are_input_and_output_paths_valid(configuration.get_input_filename(),
          configuration.get_output_filename())) {
    exit(1);
  }

  shift_coordinate_in_filenames(configuration);

  return 0;
}
