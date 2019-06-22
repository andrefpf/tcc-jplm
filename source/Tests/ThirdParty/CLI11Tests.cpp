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

/** \file     CLI11Tests.cpp
 *  \brief    Tests for the external library CLI11
 *  \details  See more details about it at
 *            https://github.com/CLIUtils/CLI11
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-06-21
 */

#include <algorithm>
#include <string>
#include <vector>
#include "CLI/CLI.hpp"
#include "gtest/gtest.h"

using CLIArguments = std::vector<std::string>;

struct SimpleCLI11Test : public testing::Test {
 protected:
  CLI::App app{"SimpleCLI11Test app"};
  CLIArguments arguments;

  SimpleCLI11Test() : arguments({""}) {
  }

  void run() {
    std::sort(std::begin(arguments), std::end(arguments));
    app.parse(arguments);
  }
};

TEST_F(SimpleCLI11Test, OneFlag) {
  app.add_flag("-c,--count");
  arguments = {"-c"};
  run();
  EXPECT_EQ(1, app.count("-c"));
  EXPECT_EQ(1, app.count("--count"));
}

TEST_F(SimpleCLI11Test, OneFlagLong) {
  app.add_flag("-c,--count");
  arguments = {"--count"};
  run();
  EXPECT_EQ(1, app.count("-c"));
  EXPECT_EQ(1, app.count("--count"));
}


TEST_F(SimpleCLI11Test, OneFlagWindows) {
  app.add_flag("-c,--count");
  arguments = {"/c"};
  app.allow_windows_style_options();
  run();
  EXPECT_EQ(1, app.count("-c"));
  EXPECT_EQ(1, app.count("--count"));
}

TEST_F(SimpleCLI11Test, CountNonExist) {
  app.add_flag("-c,--count");
  arguments = {"-c"};
  run();
  EXPECT_THROW(app.count("--nonexist"), CLI::OptionNotFound);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}