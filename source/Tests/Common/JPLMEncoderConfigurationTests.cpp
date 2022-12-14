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

/** \file     JPLMEncoderConfigurationTests.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-27
 */

#include <exception>
#include <filesystem>
#include "Lib/Common/JPLMEncoderConfiguration.h"
#include "Lib/Common/JPLMEncoderConfigurationLightField4DTransformMode.h"
#include "gtest/gtest.h"


using namespace std;

std::string resources_path = "../resources/";

std::string root_path = "..";


TEST(JPLMEncoderConfiguration, SimpleTestWithInput) {
  const char* argv[] = {"", "-i", "../cfg/part2/mule/"};
  int argc = 3;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("../cfg/part2/mule/", config.get_input_filename().c_str());
}


TEST(JPLMEncoderConfiguration, SimpleTestWithOutput) {
  const char* argv[] = {"", "-o", "./output.jpl"};
  int argc = 3;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("./output.jpl", config.get_output_filename().c_str());
}


TEST(JPLMEncoderConfiguration, SimpleTestWIthInputAndOutput) {
  const char* argv[] = {"", "-i", "/home/PLENO/DATASETS/LENSLETS/I01_Bikes/",
      "-o", "/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl"};
  int argc = 5;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("/home/PLENO/DATASETS/LENSLETS/I01_Bikes/",
      config.get_input_filename().c_str());
  EXPECT_STREQ("/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl",
      config.get_output_filename().c_str());
}


TEST(JPLMEncoderConfiguration, SimpleTestWIthInputAndOutputMixedStyles) {
  const char* argv[] = {"", "--input",
      "/home/PLENO/DATASETS/LENSLETS/I01_Bikes/", "-o",
      "/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl"};
  int argc = 5;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("/home/PLENO/DATASETS/LENSLETS/I01_Bikes/",
      config.get_input_filename().c_str());
  EXPECT_STREQ("/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl",
      config.get_output_filename().c_str());
}


TEST(JPLMEncoderConfiguration, SimpleTestWIthInputAndOutputMixedStyles2) {
  const char* argv[] = {"", "-i", "/home/PLENO/DATASETS/LENSLETS/I01_Bikes/",
      "--output", "/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl"};
  int argc = 5;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("/home/PLENO/DATASETS/LENSLETS/I01_Bikes/",
      config.get_input_filename().c_str());
  EXPECT_STREQ("/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl",
      config.get_output_filename().c_str());
}


//changed behavior, the second parameter must be used
TEST(JPLMEncoderConfiguration, TestRepeatedOptionWithSameParam) {
  const char* argv[] = {"", "-i", "/home/PLENO/DATASETS/LENSLETS/I01_Bikes/",
      "--output", "/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl", "-i",
      "/home/PLENO/DATASETS/LENSLETS/OUTRO"};
  int argc = 7;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("/home/PLENO/DATASETS/LENSLETS/OUTRO",
      config.get_input_filename().c_str());
  EXPECT_STREQ("/home/JPEG_PLENO/RESULTS/I01_Bikes.jpl",
      config.get_output_filename().c_str());
}


TEST(JPLMEncoderConfiguration, SimpleCLITest1) {
  string a(root_path + "/cfg/part2/4DTransformMode/Bikes/I01_Bikes_22016.json");
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-c", a.c_str(),
      "-p", "2", "-t", "13", "-s", "13", "-v", "434", "-u", "626"};
  int argc = 15;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ(
      "../resources/small_greek/", config.get_input_filename().c_str());
  EXPECT_EQ(JpegPlenoPart::LightField, config.get_jpeg_pleno_part());
}


TEST(JPLMEncoderConfigurationLightField, SimpleCLITest2) {
  string a(root_path + "/cfg/part2/4DTransformMode/Bikes/I01_Bikes_22016.json");
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-c", a.c_str(),
      "-p", "2", "-t", "13", "-s", "13", "-v", "434", "-u", "626"};
  int argc = 15;
  JPLMEncoderConfigurationLightField config(argc, const_cast<char**>(argv));
  EXPECT_STREQ(
      "../resources/small_greek/", config.get_input_filename().c_str());

  EXPECT_EQ(JpegPlenoPart::LightField, config.get_jpeg_pleno_part());
  EXPECT_EQ(13, config.get_number_of_rows_t());
  EXPECT_EQ(13, config.get_number_of_columns_s());
  EXPECT_EQ(434, config.get_view_height_v());
  EXPECT_EQ(626, config.get_view_width_u());
}


//
TEST(JPLMEncoderConfiguration, RaiseErrorWhetherConfigNotExists) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-c",
      "/tmp/donotcreateme/I01Bikes.cfg"};
  int argc = 5;

  EXPECT_THROW(
      { JPLMEncoderConfiguration config(argc, const_cast<char**>(argv)); },
      BasicConfigurationExceptions::ConfigFileDoesNotExistException);
}


TEST(JPLMEncoderConfigurationLightField, SimpleTestOnlyTypeShortParam) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-T", "0"};
  int argc = 7;
  JPLMEncoderConfigurationLightField config(argc, const_cast<char**>(argv));
  EXPECT_EQ(CompressionTypeLightField::transform_mode, config.get_type());
}


TEST(JPLMEncoderConfigurationLightField, SimpleTestOnlyTypeShortParam2) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-T", "1"};
  int argc = 7;
  JPLMEncoderConfigurationLightField config(argc, const_cast<char**>(argv));
  EXPECT_NE(CompressionTypeLightField::transform_mode, config.get_type());
}


TEST(JPLMEncoderConfigurationLightField, SimpleTestOnlyTypeShortParam3) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-T", "1"};
  int argc = 7;
  JPLMEncoderConfigurationLightField config(argc, const_cast<char**>(argv));
  EXPECT_EQ(CompressionTypeLightField::prediction_mode, config.get_type());
}


TEST(JPLMEncoderConfigurationLightField, SimpleTestOnlyTypeLongParam) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "--type", "0"};
  int argc = 7;
  JPLMEncoderConfigurationLightField config(argc, const_cast<char**>(argv));
  EXPECT_EQ(CompressionTypeLightField::transform_mode, config.get_type());
}


TEST(JPLMEncoderConfigurationLightField, SimpleTestOnlyTypeLongParam2) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "--type", "1"};
  int argc = 7;
  JPLMEncoderConfigurationLightField config(argc, const_cast<char**>(argv));
  EXPECT_EQ(CompressionTypeLightField::prediction_mode, config.get_type());
}


TEST(JPLMEncoderConfigurationLightField, SimpleTestOnlyTypeLongParam3) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "--type", "1"};
  int argc = 7;
  JPLMEncoderConfigurationLightField config(argc, const_cast<char**>(argv));
  EXPECT_NE(CompressionTypeLightField::transform_mode, config.get_type());
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest, LambdaFromCLI) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-l", "12"};
  int argc = 7;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_DOUBLE_EQ(12, config.get_lambda());
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest,
    BorderPolicyPadding) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-B", "padding"};
  int argc = 7;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_EQ(BorderBlocksPolicy::padding, config.get_border_blocks_policy());
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest,
    TransformParametersFromCLI_Basic) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-l", "0.5",
      "--transform_size_maximum_intra_view_vertical", "31",
      "--transform_size_minimum_intra_view_vertical", "4",
      "--transform_size_maximum_intra_view_horizontal", "31",
      "--transform_size_minimum_intra_view_horizontal", "4",
      "--transform_size_maximum_inter_view_vertical", "13",
      "--transform_size_minimum_inter_view_vertical", "13",
      "--transform_size_maximum_inter_view_horizontal", "13",
      "--transform_size_minimum_inter_view_horizontal", "13"};
  int argc = 23;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_DOUBLE_EQ(0.5, config.get_lambda());
  EXPECT_EQ(31, config.get_maximal_transform_size_intra_view_vertical());
  EXPECT_EQ(4, config.get_minimal_transform_size_intra_view_vertical());
  EXPECT_EQ(31, config.get_maximal_transform_size_intra_view_horizontal());
  EXPECT_EQ(4, config.get_minimal_transform_size_intra_view_horizontal());
  EXPECT_EQ(13, config.get_maximal_transform_size_inter_view_vertical());
  EXPECT_EQ(13, config.get_minimal_transform_size_inter_view_vertical());
  EXPECT_EQ(13, config.get_maximal_transform_size_inter_view_horizontal());
  EXPECT_EQ(13, config.get_minimal_transform_size_inter_view_horizontal());
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest,
    TransformParametersFromCLI_BasicWithPropertiesBinding) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-l", "0.5",
      "--transform_size_maximum_intra_view_vertical", "31",
      "--transform_size_minimum_intra_view_vertical", "4",
      "--transform_size_maximum_intra_view_horizontal", "31",
      "--transform_size_minimum_intra_view_horizontal", "4",
      "--transform_size_maximum_inter_view_vertical", "13",
      "--transform_size_minimum_inter_view_vertical", "13",
      "--transform_size_maximum_inter_view_horizontal", "13",
      "--transform_size_minimum_inter_view_horizontal", "13"};
  int argc = 23;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_DOUBLE_EQ(0.5, config.get_lambda());
  EXPECT_EQ(31, config.transform_size.maximum.intra_view.vertical);
  EXPECT_EQ(4, config.transform_size.minimum.intra_view.vertical);
  EXPECT_EQ(31, config.transform_size.maximum.intra_view.horizontal);
  EXPECT_EQ(4, config.transform_size.minimum.intra_view.horizontal);
  EXPECT_EQ(13, config.transform_size.maximum.inter_view.vertical);
  EXPECT_EQ(13, config.transform_size.minimum.inter_view.vertical);
  EXPECT_EQ(13, config.transform_size.maximum.inter_view.horizontal);
  EXPECT_EQ(13, config.transform_size.minimum.inter_view.horizontal);
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest,
    TransformParametersFromCLI_DifferentWithPropertiesBinding) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-l", "0.5",
      "--transform_size_maximum_intra_view_vertical", "31",
      "--transform_size_minimum_intra_view_vertical", "4",
      "--transform_size_maximum_intra_view_horizontal", "30",
      "--transform_size_minimum_intra_view_horizontal", "8",
      "--transform_size_maximum_inter_view_vertical", "13",
      "--transform_size_minimum_inter_view_vertical", "1",
      "--transform_size_maximum_inter_view_horizontal", "7",
      "--transform_size_minimum_inter_view_horizontal", "3"};
  int argc = 23;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_DOUBLE_EQ(0.5, config.get_lambda());
  EXPECT_EQ(31, config.transform_size.maximum.intra_view.vertical);
  EXPECT_EQ(4, config.transform_size.minimum.intra_view.vertical);
  EXPECT_EQ(30, config.transform_size.maximum.intra_view.horizontal);
  EXPECT_EQ(8, config.transform_size.minimum.intra_view.horizontal);
  EXPECT_EQ(13, config.transform_size.maximum.inter_view.vertical);
  EXPECT_EQ(1, config.transform_size.minimum.inter_view.vertical);
  EXPECT_EQ(7, config.transform_size.maximum.inter_view.horizontal);
  EXPECT_EQ(3, config.transform_size.minimum.inter_view.horizontal);
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest,
    TransformParametersFromJSON_DifferentWithPropertiesBinding) {
  string a(root_path + "/cfg/part2/4DTransformMode/Bikes/I01_Bikes_22016.json");
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-c", a.c_str()};
  int argc = 7;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_EQ(31, config.transform_size.maximum.intra_view.vertical);
  EXPECT_EQ(4, config.transform_size.minimum.intra_view.vertical);
  EXPECT_EQ(25, config.transform_size.maximum.intra_view.horizontal);
  EXPECT_EQ(4, config.transform_size.minimum.intra_view.horizontal);

  EXPECT_EQ(13, config.transform_size.maximum.inter_view.vertical);
  EXPECT_EQ(13, config.transform_size.minimum.inter_view.vertical);
  EXPECT_EQ(13, config.transform_size.maximum.inter_view.horizontal);
  EXPECT_EQ(13, config.transform_size.minimum.inter_view.horizontal);
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest,
    TransformParametersFromJSON_DifferentWithPropertiesGet) {
  string a(root_path + "/cfg/part2/4DTransformMode/Bikes/I01_Bikes_22016.json");
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-c", a.c_str()};
  int argc = 7;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_EQ(31, config.get_maximal_transform_size_intra_view_vertical());
  EXPECT_EQ(4, config.get_minimal_transform_size_intra_view_vertical());
  EXPECT_EQ(25, config.get_maximal_transform_size_intra_view_horizontal());
  EXPECT_EQ(4, config.get_minimal_transform_size_intra_view_horizontal());

  EXPECT_EQ(13, config.get_maximal_transform_size_inter_view_vertical());
  EXPECT_EQ(13, config.get_minimal_transform_size_inter_view_vertical());
  EXPECT_EQ(13, config.get_maximal_transform_size_inter_view_horizontal());
  EXPECT_EQ(13, config.get_minimal_transform_size_inter_view_horizontal());
}


TEST(JPLMEncoderConfigurationLightField4DTransformModeTest,
    WholeConfigurationValidFile) {
  string conf(
      root_path + "/cfg/part2/4DTransformMode/Bikes/I01_Bikes_22016.json");
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-o",
      "../resources/out_small_greek/", "-c", conf.c_str()};
  int argc = 7;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_EQ(31, config.get_maximal_transform_size_intra_view_vertical());
  EXPECT_EQ(4, config.get_minimal_transform_size_intra_view_vertical());
  EXPECT_EQ(25, config.get_maximal_transform_size_intra_view_horizontal());
  EXPECT_EQ(4, config.get_minimal_transform_size_intra_view_horizontal());

  EXPECT_EQ(13, config.get_maximal_transform_size_inter_view_vertical());
  EXPECT_EQ(13, config.get_minimal_transform_size_inter_view_vertical());
  EXPECT_EQ(13, config.get_maximal_transform_size_inter_view_horizontal());
  EXPECT_EQ(13, config.get_minimal_transform_size_inter_view_horizontal());

  EXPECT_EQ(13, config.get_number_of_columns_s());
  EXPECT_EQ(13, config.get_number_of_rows_t());
  EXPECT_EQ(434, config.get_view_height_v());
  EXPECT_EQ(625, config.get_view_width_u());

  EXPECT_DOUBLE_EQ(22016, config.get_lambda());
}


TEST(
    JPLMEncoderConfigurationLightField4DTransformModeTest, EncodeBikesDataset) {
  const char* argv[] = {"", "--part", "2", "--type", "0", "--input",
      "~/DATASETS/I01_Bikes/", "--output", "./test_bikes.lf",
      "--transform_size_maximum_inter_view_vertical", "13",
      "--transform_size_maximum_inter_view_horizontal", "13",
      "--transform_size_maximum_intra_view_vertical", "31",
      "--transform_size_maximum_intra_view_horizontal", "31",
      "--transform_size_minimum_inter_view_vertical", "13",
      "--transform_size_minimum_inter_view_horizontal", "13",
      "--transform_size_minimum_intra_view_vertical", "4",
      "--transform_size_minimum_intra_view_horizontal", "4", "--lambda",
      "10000", "-t", "13", "-s", "9", "-v", "434", "-u", "626"};
  int argc = 35;
  JPLMEncoderConfigurationLightField4DTransformMode config(
      argc, const_cast<char**>(argv));
  EXPECT_EQ(JpegPlenoPart::LightField, config.get_jpeg_pleno_part());
  EXPECT_EQ("~/DATASETS/I01_Bikes/", config.get_input_filename());
  EXPECT_EQ("./test_bikes.lf", config.get_output_filename());
  EXPECT_EQ(13, config.transform_size.maximum.inter_view.vertical);
  EXPECT_EQ(13, config.transform_size.maximum.inter_view.horizontal);

  EXPECT_EQ(31, config.transform_size.maximum.intra_view.vertical);
  EXPECT_EQ(31, config.transform_size.maximum.intra_view.horizontal);

  EXPECT_EQ(13, config.transform_size.minimum.inter_view.vertical);
  EXPECT_EQ(13, config.transform_size.minimum.inter_view.horizontal);

  EXPECT_EQ(4, config.transform_size.minimum.intra_view.vertical);
  EXPECT_EQ(4, config.transform_size.minimum.intra_view.horizontal);

  EXPECT_DOUBLE_EQ(10000, config.get_lambda());
  EXPECT_EQ(13, config.get_number_of_rows_t());
  EXPECT_EQ(9, config.get_number_of_columns_s());
  EXPECT_EQ(434, config.get_view_height_v());
  EXPECT_EQ(626, config.get_view_width_u());
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  if (argc > 1) {
    resources_path = std::string(argv[1]);
    auto resources_fs_path = std::filesystem::path(resources_path.c_str());
    root_path = resources_fs_path.parent_path().string();
  }
  return RUN_ALL_TESTS();
}
