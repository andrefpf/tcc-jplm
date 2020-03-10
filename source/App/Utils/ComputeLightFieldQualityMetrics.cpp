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

/** \file     ComputeLightFieldQualityMetrics.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-11-05
 */

#include <algorithm>
#include <filesystem>
#include <iomanip>  //std::setprecision, std::setw, and std::setfill
#include <iostream>
#include <magic_enum.hpp>
#include <map>
#include <sstream>
#include <string>
#include "CppConsoleTable/CppConsoleTable.hpp"
#include "Lib/Part2/Common/LightfieldDimension.h"
#include "Lib/Part2/Common/LightfieldFromFile.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Part2/Common/ViewIOPolicyOneAtATime.h"
#include "Lib/Utils/BasicConfiguration/BasicConfiguration.h"
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageMetrics.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"

// namespace ImageMetrics {
// enum class Available { PSNR, MSE, SSE, MAX_ABS_ERROR };

using Metric = ImageMetrics::Available;

enum class ReportType : uint16_t {
  NONE = 0,
  VIEW_CHANNELS = 1,
  VIEW_AVERAGE = 2,
  CHANNEL_AVERAGE = 4,
  AVERAGE = 8,
};


class ReportShowFlags {
 protected:
  uint16_t report_mask = magic_enum::enum_integer(ReportType::NONE);

 public:
  ReportShowFlags() = default;
  virtual ~ReportShowFlags() = default;


  template<ReportType type>
  void include_report() {
    constexpr uint16_t type_as_int = magic_enum::enum_integer(type);
    report_mask |= type_as_int;
  }


  void set_all_reports() {
    for (auto report : magic_enum::enum_values<ReportType>()) {
      report_mask |= magic_enum::enum_integer(report);
    }
  }


  template<ReportType type>
  bool should_report() const {
    constexpr uint16_t type_as_int = magic_enum::enum_integer(type);
    return type_as_int & report_mask;
  }
};


class ComputeLightfieldQualityMetricsConfiguration : public BasicConfiguration {
 private:
  static constexpr std::size_t current_hierarchy_level = 0;

 protected:
  std::string input_baseline;
  std::string input_test;
  Metric metric;

  std::map<Metric, ReportShowFlags> reports;


  uint32_t number_of_rows_t;
  uint32_t number_of_columns_s;
  ComputeLightfieldQualityMetricsConfiguration(
      int argc, char **argv, std::size_t level)
      : BasicConfiguration(argc, argv, level) {
    //       ReportShowFlags<Metric::PSNR> psnr_report;
    // ReportShowFlags<Metric::MSE> mse_report;
    // ReportShowFlags<Metric::SSE> sse_report;
    // ReportShowFlags<Metric::MAX_ABS_ERROR> max_report;
    for (auto metric : magic_enum::enum_values<Metric>()) {
      std::cout << "Added metric: " << magic_enum::enum_name(metric)
                << std::endl;
      reports.emplace(std::make_pair(metric, ReportShowFlags()));
    }
  }


  virtual void add_options() override {
    BasicConfiguration::add_options();


    this->add_cli_json_option({"--input_baseline", "-ib",
        "Input directory that contains at least three subdirectories with PGX "
        "files of the baseline (e.g., original) lightfield.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("input_baseline")) {
            return conf["input_baseline"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) { this->input_baseline = v; },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--input_test", "-it",
        "Input directory that contains at least three subdirectories with PGX "
        "files of the test (e.g., decoded) lightfield.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("input_test")) {
            return conf["input_test"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) { this->input_test = v; },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--number_of_rows", "-t",
        "Number of light-field view rows. Mandatory.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("number_of_rows")) {
            return std::to_string(conf["number_of_rows"].get<uint32_t>());
          }
          return std::nullopt;
        },
        [this](std::string arg) { this->number_of_rows_t = std::stoi(arg); },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--number_of_columns", "-s",
        "Number of light-field view columns. Mandatory.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("number_of_columns")) {
            return std::to_string(conf["number_of_columns"].get<uint32_t>());
          }
          return std::nullopt;
        },
        [this](std::string arg) { this->number_of_columns_s = std::stoi(arg); },
        this->current_hierarchy_level});

    constexpr auto metrics = magic_enum::enum_names<Metric>();
    std::stringstream available_metrics_string_stream;
    for (const auto &metric_name : metrics) {
      available_metrics_string_stream << metric_name << ", ";
    }

    this->add_cli_json_option({"--metric", "-m",
        "Metric to be computed. "
        "Available metrics: " +
            available_metrics_string_stream.str(),
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("metric")) {
            return conf["metric"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) {
          auto metric = magic_enum::enum_cast<Metric>(v);
          if (metric) {
            this->metric = *metric;
          } else {
            //<! \todo throw error
          }
        },
        this->current_hierarchy_level});


    this->add_cli_json_option({"--show-report-view-channels", "-srvc",
        "Shows a view-by-view report for each channel. "
        "Metric parameter is optional. If not set, all metrics will be shown. "
        "Available metrics: " +
            available_metrics_string_stream.str(),
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("show-report-view-channels")) {
            return conf["show-report-view-channels"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) {
          if (v.empty()) {
            set_report<ReportType::VIEW_CHANNELS>();
          } else {
            auto metric = magic_enum::enum_cast<Metric>(v);
            if (metric) {
              set_report<ReportType::VIEW_CHANNELS>(*metric);
            } else {
              //<! \todo throw error
            }
          }
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--show-report-view-average", "-srva",
        "Shows a view-by-view report with the average of all channels. "
        "Metric parameter is optional. If not set, all metrics will be shown. "
        "Available metrics: " +
            available_metrics_string_stream.str() +
            "The average PSNR is obtained after the average MSE.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("show-report-view-average")) {
            return conf["show-report-view-average"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) {
          if (v.empty()) {
            set_report<ReportType::VIEW_AVERAGE>();
          } else {
            auto metric = magic_enum::enum_cast<Metric>(v);
            if (metric) {
              set_report<ReportType::VIEW_AVERAGE>(*metric);
            } else {
              //<! \todo throw error
            }
          }
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--show-report-channel-average", "-srca",
        "Shows a channel-by-channel report with the average of all views. "
        "Metric parameter is optional. If not set, all metrics will be shown. "
        "Available metrics: " +
            available_metrics_string_stream.str() +
            "The average PSNR is obtained after the average MSE.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("show-report-channel-average")) {
            return conf["show-report-channel-average"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) {
          if (v.empty()) {
            set_report<ReportType::CHANNEL_AVERAGE>();
          } else {
            auto metric = magic_enum::enum_cast<Metric>(v);
            if (metric) {
              set_report<ReportType::CHANNEL_AVERAGE>(*metric);
            } else {
              //<! \todo throw error
            }
          }
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--show-report-average", "-avg",
        "Summary. Shows a report with the average of all channels of all "
        "views. "
        "Metric parameter is optional. If not set, all metrics will be shown. "
        "Available metrics: " +
            available_metrics_string_stream.str() +
            "The average PSNR is obtained after the average MSE.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("show-report-average")) {
            return conf["show-report-average"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) {
          if (v.empty()) {
            set_report<ReportType::AVERAGE>();
          } else {
            auto metric = magic_enum::enum_cast<Metric>(v);
            if (metric) {
              set_report<ReportType::AVERAGE>(*metric);
            } else {
              //<! \todo throw error
            }
          }
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--show-report-all", "-all",
        "Show all reports for the metric passed as parameter. "
        "Metric parameter is optional. If not set, all metrics will be shown. "
        "Available metrics: " +
            available_metrics_string_stream.str() +
            "The average PSNR is obtained after the average MSE.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("show-report-average")) {
            return conf["show-report-average"].get<std::string>();
          }
          return std::nullopt;
        },
        [this](std::string v) {
          if (v.empty()) {
            set_all_reports();
          } else {
            auto metric = magic_enum::enum_cast<Metric>(v);
            if (metric) {
              set_all_reports(*metric);
            } else {
              //<! \todo throw error
            }
          }
        },
        this->current_hierarchy_level});
  }

 public:
  ComputeLightfieldQualityMetricsConfiguration(int argc, char **argv)
      : ComputeLightfieldQualityMetricsConfiguration(argc, argv,
            ComputeLightfieldQualityMetricsConfiguration::
                current_hierarchy_level) {
    this->init(argc, argv);
  }


  const ReportShowFlags &get_report(Metric metric) const {
    auto iter = reports.find(metric);
    if (iter != reports.end()) {
      return iter->second;
    } else {
      throw std::runtime_error("Metric not found getting report");
    }
  }

  template<ReportType type>
  void set_report(Metric metric) {
    auto iter = reports.find(metric);
    if (iter != reports.end()) {
      iter->second.include_report<type>();
    } else {
      throw std::runtime_error("Metric not found setting report");
    }
  }

  template<ReportType type>
  void set_report() {
    for (auto metric : magic_enum::enum_values<Metric>()) {
      set_report<type>(metric);
    }
  }


  void set_all_reports() {
    for (auto &it : reports) {
      it.second.set_all_reports();
    }
  }


  void set_all_reports(Metric metric) {
    auto iter = reports.find(metric);
    if (iter != reports.end()) {
      iter->second.set_all_reports();
    } else {
      throw std::runtime_error("Metric not found setting all reports");
    }
  }


  virtual ~ComputeLightfieldQualityMetricsConfiguration() = default;


  const std::string &get_input_baseline_filename() const {
    return input_baseline;
  }


  const std::string &get_input_test_filename() const {
    return input_test;
  }


  auto get_t() const {
    return this->number_of_rows_t;
  }


  auto get_s() const {
    return this->number_of_columns_s;
  }


  samilton::ConsoleTable get_console_table() const {
    samilton::ConsoleTable table(1, 1, samilton::Alignment::centre);
    samilton::ConsoleTable::TableChars chars;
    chars.topDownSimple = '\0';
    chars.leftSeparation = '\0';
    chars.centreSeparation = '\0';
    chars.downLeft = '\0';
    chars.downRight = '\0';
    chars.leftRightSimple = '\0';
    chars.rightSeparation = '\0';
    chars.topLeft = '\0';
    chars.topRight = '\0';
    chars.topSeparation = '\0';
    chars.downSeparation = '\0';
    table.setTableChars(chars);
    return table;
  }
};


void compute_metric(
    const ComputeLightfieldQualityMetricsConfiguration &configuration) {
  auto t_max = configuration.get_t();
  auto s_max = configuration.get_s();
  LightfieldDimension<std::size_t> size(t_max, s_max, 32, 32);
  LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);
  LightfieldIOConfiguration baseline_lf_configuration(
      configuration.get_input_baseline_filename(), initial, size);
  LightfieldIOConfiguration test_lf_configuration(
      configuration.get_input_test_filename(), initial, size);

  auto baseline_lightfield =
      std::make_unique<LightfieldFromFile<uint16_t>>(baseline_lf_configuration);

  auto test_lightfield =
      std::make_unique<LightfieldFromFile<uint16_t>>(test_lf_configuration);

  // ImageMetrics auto psnrs = get_peak_signal_to_noise_ratio(original, encoded);

  auto bpp = baseline_lightfield->get_views_bpp();
  auto n_channels = baseline_lightfield->get_number_of_channels_in_view();
  auto n_views =
      static_cast<double>(baseline_lightfield->get_number_of_views());
  std::vector<double> mse_sum(n_channels, 0.0);
  std::vector<double> sse_sum(n_channels, 0.0);
  std::vector<std::size_t> max_error(n_channels, 0);
  double psnr_sum = 0.0;


  // auto channel_mse_table = configuration.get_console_table();
  // auto channel_psnr_table = configuration.get_console_table();

  std::vector<samilton::ConsoleTable> channel_mse_table;
  std::vector<samilton::ConsoleTable> channel_psnr_table;
  std::vector<samilton::ConsoleTable> channel_max_abs_error_table;
  std::vector<samilton::ConsoleTable> channel_sse_table;

  for (auto i = decltype(n_channels){0}; i < n_channels; ++i) {
    channel_mse_table.push_back(configuration.get_console_table());
    channel_psnr_table.push_back(configuration.get_console_table());
    channel_max_abs_error_table.push_back(configuration.get_console_table());
    channel_sse_table.push_back(configuration.get_console_table());
  }

  auto average_psnr_table = configuration.get_console_table();
  auto average_mse_table = configuration.get_console_table();
  auto max_max_abs_error_table = configuration.get_console_table();
  auto average_sse_table = configuration.get_console_table();

  auto precision = 2;

  const auto &report_psnr = configuration.get_report(Metric::PSNR);
  const auto &report_mse = configuration.get_report(Metric::MSE);
  const auto &report_sse = configuration.get_report(Metric::SSE);
  const auto &report_max = configuration.get_report(Metric::MAX_ABS_ERROR);

  // auto printer = ImageMetrics::visitors::PSNRPrinter();
  for (auto t = decltype(t_max){0}; t < t_max; ++t) {
    for (auto s = decltype(s_max){0}; s < s_max; ++s) {
      auto mses =
          ImageMetrics::get_mse(baseline_lightfield->get_image_at({t, s}),
              test_lightfield->get_image_at({t, s}));
      auto sum_of_squared_errors = ImageMetrics::get_sum_of_squared_errors(
          baseline_lightfield->get_image_at({t, s}),
          test_lightfield->get_image_at({t, s}));
      auto max_abs_errors = ImageMetrics::get_maximum_absolute_error(
          baseline_lightfield->get_image_at({t, s}),
          test_lightfield->get_image_at({t, s}));

      double view_average_mse = 0.0;
      double view_average_sse = 0.0;
      std::size_t view_max_abs_error = 0;
      for (auto i = decltype(n_channels){0}; i < n_channels; ++i) {
        auto mse = mses.at(i);
        auto max_abs_error = max_abs_errors.at(i);
        auto sse = sum_of_squared_errors.at(i);
        mse_sum.at(i) += mse;
        sse_sum.at(i) += sse;
        view_average_mse += mse;
        view_average_sse += sse;

        if (max_abs_error > view_max_abs_error) {
          view_max_abs_error = max_abs_error;
          if (max_abs_error > max_error.at(i)) {
            max_error.at(i) = max_abs_error;
          }
        }


        if (report_psnr.should_report<ReportType::VIEW_CHANNELS>()) {
          auto psnr =
              ImageChannelUtils::get_peak_signal_to_noise_ratio(bpp, mse);
          {
            std::ostringstream str;
            str << std::fixed << std::setprecision(precision) << psnr;
            channel_psnr_table.at(i)[t][s] = str.str();
          }
        }
        if (report_mse.should_report<ReportType::VIEW_CHANNELS>()) {
          std::ostringstream str;
          str << std::fixed << std::setprecision(precision) << mse;
          channel_mse_table.at(i)[t][s] = str.str();
        }
        if (report_sse.should_report<ReportType::VIEW_CHANNELS>()) {
          //this is set scientific as the number are too large
          std::ostringstream str;
          str << std::scientific << std::setprecision(precision) << sse;
          channel_sse_table.at(i)[t][s] = str.str();
        }
        if (report_max.should_report<ReportType::VIEW_CHANNELS>()) {
          channel_max_abs_error_table.at(i)[t][s] = max_abs_error;
        }
      }


      view_average_mse /= static_cast<double>(n_channels);
      if (report_psnr.should_report<ReportType::VIEW_AVERAGE>()) {
        auto view_average_psnr =
            ImageChannelUtils::get_peak_signal_to_noise_ratio(
                bpp, view_average_mse);
        std::ostringstream str;
        str << std::fixed << std::setprecision(precision) << view_average_psnr;
        average_psnr_table[t][s] = str.str();
      }
      if (report_mse.should_report<ReportType::VIEW_AVERAGE>()) {
        std::ostringstream str;
        str << std::fixed << std::setprecision(precision) << view_average_mse;
        average_mse_table[t][s] = str.str();
      }
      if (report_sse.should_report<ReportType::VIEW_AVERAGE>()) {
        view_average_sse /= static_cast<double>(n_channels);
        std::ostringstream str;
        str << std::scientific << std::setprecision(precision)
            << view_average_sse;
        average_sse_table[t][s] = str.str();
      }
      if (report_max.should_report<ReportType::VIEW_AVERAGE>()) {
        max_max_abs_error_table[t][s] = view_max_abs_error;
      }
    }
  }

  if (report_max.should_report<ReportType::VIEW_CHANNELS>()) {
    std::cout << "\n############### Max Absolute Error views ###############\n";

    for (auto i = decltype(n_channels){0}; i < n_channels; ++i) {
      std::cout << "Channel " << i << ": \n"
                << channel_max_abs_error_table.at(i) << "\n";
    }
  }

  if (report_max.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "Max all channels: \n" << max_max_abs_error_table;
  }
  if (report_max.should_report<ReportType::VIEW_CHANNELS>() ||
      report_max.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "\n################################################\n\n";
  }

  if (report_sse.should_report<ReportType::VIEW_CHANNELS>()) {
    std::cout << "\n############### SSE views ###############\n";

    for (auto i = decltype(n_channels){0}; i < n_channels; ++i) {
      std::cout << "Channel " << i << ": \n" << channel_sse_table.at(i) << "\n";
    }
  }
  if (report_sse.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "Average: \n" << average_sse_table;
  }

  if (report_sse.should_report<ReportType::VIEW_CHANNELS>() ||
      report_sse.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "\n################################################\n\n";
  }

  if (report_mse.should_report<ReportType::VIEW_CHANNELS>()) {
    std::cout << "\n############### MSE views ###############\n";

    for (auto i = decltype(n_channels){0}; i < n_channels; ++i) {
      std::cout << "Channel " << i << ": \n" << channel_mse_table.at(i) << "\n";
    }
  }
  if (report_mse.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "Average: \n" << average_mse_table;
  }
  if (report_mse.should_report<ReportType::VIEW_CHANNELS>() ||
      report_mse.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "\n################################################\n\n";
  }
  if (report_psnr.should_report<ReportType::VIEW_CHANNELS>()) {
    std::cout << "\n############### PSNR views ###############\n";

    for (auto i = decltype(n_channels){0}; i < n_channels; ++i) {
      std::cout << "Channel " << i << " (dB): \n"
                << channel_psnr_table.at(i) << "\n";
    }
  }
  if (report_psnr.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "Average: \n" << average_psnr_table;
  }
  if (report_psnr.should_report<ReportType::VIEW_CHANNELS>() ||
      report_psnr.should_report<ReportType::VIEW_AVERAGE>()) {
    std::cout << "\n################################################\n\n";
  }


  double sum_of_mses = 0.0;
  double sum_of_sses = 0.0;
  std::size_t max_error_all = 0;

  if ((report_max.should_report<ReportType::CHANNEL_AVERAGE>()) ||
      (report_sse.should_report<ReportType::CHANNEL_AVERAGE>()) ||
      (report_mse.should_report<ReportType::CHANNEL_AVERAGE>()) ||
      (report_psnr.should_report<ReportType::CHANNEL_AVERAGE>())) {
    std::cout << "\n############### Channel averages ###############\n";

    auto channel_estimates_table = configuration.get_console_table();
    auto line = 0;
    channel_estimates_table[line++][0](samilton::Alignment::right) =
        "Channel: ";

    if (report_max.should_report<ReportType::CHANNEL_AVERAGE>()) {
      channel_estimates_table[line++][0](samilton::Alignment::right) =
          "Max Abs Error: ";
    }
    if (report_sse.should_report<ReportType::CHANNEL_AVERAGE>()) {
      channel_estimates_table[line++][0](samilton::Alignment::right) = "SSE: ";
    }
    if (report_mse.should_report<ReportType::CHANNEL_AVERAGE>()) {
      channel_estimates_table[line++][0](samilton::Alignment::right) = "MSE: ";
    }
    if (report_psnr.should_report<ReportType::CHANNEL_AVERAGE>()) {
      channel_estimates_table[line++][0](samilton::Alignment::right) =
          "PSNR (dB): ";
    }

    for (auto i = 0; i < n_channels; ++i) {
      double mse = mse_sum.at(i) / n_views;
      double sse = sse_sum.at(i) / n_views;
      auto max_error_channel = max_error.at(i);
      if (max_error_channel > max_error_all) {
        max_error_all = max_error_channel;
      }

      sum_of_mses += mse;
      sum_of_sses += sse;
      double psnr = ImageChannelUtils::get_peak_signal_to_noise_ratio(bpp, mse);

      line = 0;
      channel_estimates_table[line++][i + 1] = i;


      if (report_max.should_report<ReportType::CHANNEL_AVERAGE>()) {
        channel_estimates_table[line++][i + 1] = max_error_channel;
      }
      if (report_sse.should_report<ReportType::CHANNEL_AVERAGE>()) {
        channel_estimates_table[line++][i + 1] = sse;
      }
      if (report_mse.should_report<ReportType::CHANNEL_AVERAGE>()) {
        channel_estimates_table[line++][i + 1] = mse;
      }
      if (report_psnr.should_report<ReportType::CHANNEL_AVERAGE>()) {
        channel_estimates_table[line++][i + 1] = psnr;
      }
    }

    std::cout << channel_estimates_table;

    std::cout << "\n################################################\n\n";
  }


  auto average_mse = sum_of_mses / static_cast<double>(n_channels);
  auto average_sse = sum_of_sses / static_cast<double>(n_channels);


  auto final_report_table = configuration.get_console_table();
  auto line = 0;

  if (report_max.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][0](samilton::Alignment::right) =
        "Max Abs Error: ";
  }
  if (report_sse.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][0](samilton::Alignment::right) = "SSE: ";
  }
  if (report_mse.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][0](samilton::Alignment::right) = "MSE: ";
  }
  if (report_psnr.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][0](samilton::Alignment::right) =
        "Average PSNR (dB): ";
  }


  line = 0;
  if (report_max.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][1](samilton::Alignment::right) = max_error_all;
  }
  if (report_sse.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][1](samilton::Alignment::right) = average_sse;
  }
  if (report_mse.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][1](samilton::Alignment::right) = average_mse;
  }
  if (report_psnr.should_report<ReportType::AVERAGE>()) {
    final_report_table[line++][1](samilton::Alignment::right) =
        ImageChannelUtils::get_peak_signal_to_noise_ratio(bpp, average_mse);
  }

  std::cout << final_report_table << std::endl;
}


int main(int argc, char const *argv[]) {
  auto configuration = ComputeLightfieldQualityMetricsConfiguration(
      argc, const_cast<char **>(argv));
  if (configuration.is_help_mode()) {
    exit(0);
  }
  compute_metric(configuration);
  return 0;
}