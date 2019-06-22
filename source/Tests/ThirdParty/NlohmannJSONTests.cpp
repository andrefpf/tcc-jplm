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

/** \file     NlohmannJSONTests.cpp
 *  \brief    Tests for the external library Nlohmann::JSON
 *  \details  See more details about it at
 *            https://github.com/nlohmann/json
 *            https://nlohmann.github.io/json/
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-06-21
 */

#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;


struct SimpleEmptyNlohmannJSONTest : public testing::Test {
 protected:
  json j;
  SimpleEmptyNlohmannJSONTest() : j({""}) {
  }

  void run() {
    const json j_const(j);
    EXPECT_EQ(false, j.empty());
    EXPECT_EQ(false, j_const.empty());
    EXPECT_TRUE(j.empty() == (j.begin() == j.end()));
    EXPECT_TRUE(j_const.empty() == (j_const.begin() == j_const.end()));
  }
};

struct SimpleSizeOneNlohmannJSONTest : public testing::Test {
 protected:
  json j;
  SimpleSizeOneNlohmannJSONTest() : j({""}) {
  }

  void run() {
    const json j_const(j);
    EXPECT_EQ(1, j.size());
    EXPECT_EQ(1, j_const.size());
    EXPECT_EQ(std::distance(j.begin(), j.end()), j.size());
    EXPECT_EQ(std::distance(j_const.begin(), j_const.end()), j_const.size());
    EXPECT_EQ(std::distance(j.rbegin(), j.rend()), j.size());
    EXPECT_EQ(
        std::distance(j_const.crbegin(), j_const.crend()), j_const.size());
  }
};

struct SimpleMaxSizeNlohmannJSONTest : public testing::Test {
 protected:
  json j;
  SimpleMaxSizeNlohmannJSONTest() : j({""}) {
  }

  void run() {
    const json j_const(j);
    EXPECT_EQ(j.max_size(), j_const.max_size());
  }
};


TEST_F(SimpleEmptyNlohmannJSONTest, TestBooleanEmpty) {
  json j = true;
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestStringEmpty) {
  json j = "Test";
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestArrayEmpty) {
  json j = json::array();
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestFilledArrayEmpty) {
  json j = {"A", "B", "C"};
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestObjectEmpty) {
  json j = json::object();
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestFilledObjectEmpty) {
  json j = {{"one", 1}, {"two", 2}, {"three", 3}};
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestIntEmpty) {
  json j = -15;
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestUIntEmpty) {
  json j = 22u;
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestFloatEmpty) {
  json j = 3.1415;
  run();
}

TEST_F(SimpleEmptyNlohmannJSONTest, TestNullEmpty) {
  json j = nullptr;
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestBoolSize) {
  json j = true;
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestStringSize) {
  json j = "Test";
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestArraySize) {
  json j = json::array();
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestFilledArraySize) {
  json j = {"A", "B", "C"};
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestObjectSize) {
  json j = json::object();
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestFilledObjectSize) {
  json j = {{"one", 1}, {"two", 2}, {"three", 3}};
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestIntSize) {
  json j = -15;
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestUIntSize) {
  json j = 22u;
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestFloatSize) {
  json j = 3.1415;
  run();
}

TEST_F(SimpleSizeOneNlohmannJSONTest, TestNullSize) {
  json j = nullptr;
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestBooleanMaxsize) {
  json j = true;
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestStringMaxsize) {
  json j = "Test";
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestArrayMaxsize) {
  json j = json::array();
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestFilledArrayMaxsize) {
  json j = {"A", "B", "C"};
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestObjectMaxsize) {
  json j = {"A", "B", "C"};
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestFilledObjectMaxsize) {
  json j = {{"one", 1}, {"two", 2}, {"three", 3}};
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestIntMaxsize) {
  json j = -15;
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestUIntMaxsize) {
  json j = 22u;
  run();
}

TEST_F(SimpleMaxSizeNlohmannJSONTest, TestFloatMaxsize) {
  json j = 3.1415;
  run();
}

TEST(SimpleTest, Iterator) {
  json j = {{"A", 1}, {"B", 2}};
  int count = 1;
  for (auto& i : j.items()) i.value() = ++count;
  EXPECT_EQ(j, json({{"A", 2}, {"B", 3}}));
}

TEST(SimpleTest, JSONAsFirstClassDataType) {
  const json expected = {{"pi", 3.141}, {"happy", true}, {"name", "Niels"},
      {"nothing", nullptr}, {"answer", {{"everything", 42}}},
      {"list", {1, 0, 2}}, {"object", {{"currency", "USD"}, {"value", 42.99}}}};
  json j;
  j["pi"] = 3.141;
  j["happy"] = true;
  j["name"] = "Niels";
  j["nothing"] = nullptr;
  j["answer"]["everything"] = 42;
  j["list"] = {1, 0, 2};
  j["object"] = {{"currency", "USD"}, {"value", 42.99}};
  EXPECT_EQ(j, expected);
}

TEST(SimpleTest, SerializeDeserialize) {
  const json expected = "{ \"happy\": true, \"pi\": 3.141 }"_json;
  const json raw_literal = R"(
  {
    "happy": true,
    "pi": 3.141
  }
  )"_json;
  auto explicitely_parsed = json::parse("{ \"happy\": true, \"pi\": 3.141 }");
  EXPECT_EQ(raw_literal, expected);
  EXPECT_EQ(explicitely_parsed, expected);
}

TEST(SimpleTest, SerializeDumpDeserialize) {
  const json expected = "{ \"happy\": true, \"pi\": 3.141 }"_json;
  const std::string dumped = expected.dump();  // {\"happy\":true,\"pi\":3.141}
  const json restored = json::parse(dumped);
  const json restored_notparsed = json(dumped);
  EXPECT_EQ(restored, expected);
  EXPECT_NE(restored, restored_notparsed);
}

TEST(SimpleTest, SerializationVersusAssigment) {
  const json j_string = "this is a JSON string";
  std::string cpp_string = j_string.get<std::string>();
  std::string cpp_string2;
  j_string.get_to(cpp_string2);
  EXPECT_EQ(cpp_string, cpp_string2);
  EXPECT_EQ(cpp_string2, j_string.get<std::string>());
  std::string serialized_string = j_string.dump();
  EXPECT_NE(cpp_string, serialized_string);
  EXPECT_NE(j_string, serialized_string);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
