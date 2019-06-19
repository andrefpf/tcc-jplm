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

/** \file     CppIterTools.cpp
 *  \brief    Tests for the external library CppIterTools
 *  \details  See more details about it at
 *            https://github.com/ryanhaining/cppitertools
 *            https://docs.python.org/3/library/itertools.html
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-06-18
 */

#include <algorithm>
#include <iostream>
#include "cppitertools/accumulate.hpp"
#include "cppitertools/chain.hpp"
#include "cppitertools/combinations.hpp"
#include "cppitertools/combinations_with_replacement.hpp"
#include "cppitertools/compress.hpp"
#include "cppitertools/count.hpp"
#include "cppitertools/cycle.hpp"
#include "cppitertools/dropwhile.hpp"
#include "cppitertools/enumerate.hpp"
#include "cppitertools/filter.hpp"
#include "cppitertools/filterfalse.hpp"
#include "cppitertools/range.hpp"
#include "cppitertools/repeat.hpp"
#include "gtest/gtest.h"

using iter::accumulate;
using iter::chain;
using iter::combinations;
using iter::combinations_with_replacement;
using iter::compress;
using iter::count;
using iter::cycle;
using iter::dropwhile;
using iter::enumerate;
using iter::filter;
using iter::filterfalse;
using iter::range;
using iter::repeat;

TEST(CppIterTools, TestRangeOneArgument) {
  auto r = range(3);
  EXPECT_EQ(r.start(), 0);
  EXPECT_EQ(r.stop(), 3);
  EXPECT_EQ(r.step(), 1);

  // make sure iterators aren't changing the value
  auto it = r.begin();
  ++it;

  EXPECT_EQ(r.start(), 0);
  EXPECT_EQ(r.stop(), 3);
  EXPECT_EQ(r.step(), 1);
}

TEST(CppIterTools, TestRangeTwoArguments) {
  auto r = range(2, 10);
  EXPECT_EQ(r.start(), 2);
  EXPECT_EQ(r.stop(), 10);
  EXPECT_EQ(r.step(), 1);
}

TEST(CppIterTools, TestRangeThreeArguments) {
  auto r = range(-6, 20, 3);
  EXPECT_EQ(r.start(), -6);
  EXPECT_EQ(r.stop(), 20);
  EXPECT_EQ(r.step(), 3);
}

TEST(CppIterTools, TestRangeOneArgumentDouble) {
  auto r = range(3.5);
  EXPECT_DOUBLE_EQ(r.start(), 0);
  EXPECT_DOUBLE_EQ(r.stop(), 3.5);
  EXPECT_DOUBLE_EQ(r.step(), 1.0);
}

TEST(CppIterTools, TestRangeTwoArgumentDouble) {
  auto r = range(20.1, 31.7);
  EXPECT_DOUBLE_EQ(r.start(), 20.1);
  EXPECT_DOUBLE_EQ(r.stop(), 31.7);
  EXPECT_DOUBLE_EQ(r.step(), 1.0);
}

TEST(CppIterTools, TestRangeThreeArgumentDouble) {
  auto r = range(-6.3, 5.7, 0.1);
  EXPECT_DOUBLE_EQ(r.start(), -6.3);
  EXPECT_DOUBLE_EQ(r.stop(), 5.7);
  EXPECT_DOUBLE_EQ(r.step(), 0.1);
}

TEST(CppIterTools, TestRangeWithOnlyStop) {
  auto r = range(5);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{0, 1, 2, 3, 4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWithStartAndStop) {
  auto r = range(1, 5);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{1, 2, 3, 4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWithPositiveSteps) {
  auto r = range(1, 10, 3);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{1, 4, 7};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWithEmptyRange) {
  auto r = iter::range(0);
  EXPECT_TRUE(std::begin(r) == std::end(r));
}

TEST(CppIterTools, TestRangeWithPostfixIncrement) {
  auto r = iter::range(3);
  auto it = std::begin(r);
  it++;
  EXPECT_TRUE(*it == 1);
}

TEST(CppIterTools, TestRangeStartBiggerThanEnd) {
  auto r = range(5, 0);
  const std::vector<int> v(std::begin(r), std::end(r));
  EXPECT_TRUE(v.empty());
}

TEST(CppIterTools, TestRangeWithNegativeSteps) {
  auto r = range(5, -5, -3);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{5, 2, -1, -4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWhenStepNotDivideRangeEqually) {
  auto r = range(0, 5, 2);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{0, 2, 4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWhenStepIsBiggerThanRange) {
  auto r = range(0, 10, 20);
  const std::vector<int> v(std::begin(r), std::end(r));
  EXPECT_TRUE(v.size() == 1);
}

TEST(CppIterTools, TestRangeWhenStepIsZero) {
  auto r = range(0, 10, 0);
  EXPECT_TRUE(std::begin(r) == std::end(r));
  auto r2 = range(0, -10, 0);
  EXPECT_TRUE(std::begin(r2) == std::end(r2));
}

TEST(CppIterTools, TestConstExprRange) {
  constexpr auto r = range(10);
  (void) r;
  constexpr auto r2 = range(4, 10);
  (void) r2;
  constexpr auto r3 = range(4, 10, 2);
  (void) r3;

  constexpr auto it = r2.begin();  // std::begin isn't constexpr
  constexpr auto i = *it;
  EXPECT_EQ(i, 4);

  constexpr auto rf = range(10.0);
  constexpr auto itf = rf.begin();
  constexpr auto f = *itf;
  EXPECT_EQ(f, 0.0);
}

TEST(CppIterTools, TestIndexedRange) {
  auto r = range(4);
  EXPECT_EQ(r[0], 0);
  EXPECT_EQ(r[1], 1);
  EXPECT_EQ(r[2], 2);
  EXPECT_EQ(r[3], 3);
}

TEST(CppIterTools, TestIndexedRangeWithTwoArguments) {
  auto r = range(10, 14);
  EXPECT_EQ(r[0], 10);
  EXPECT_EQ(r[1], 11);
  EXPECT_EQ(r[2], 12);
  EXPECT_EQ(r[3], 13);
}

TEST(CppIterTools, TestIndexedRangeWithThreeArguments) {
  auto r = range(10, 20, 3);
  EXPECT_EQ(r[0], 10);
  EXPECT_EQ(r[1], 13);
  EXPECT_EQ(r[2], 16);
  EXPECT_EQ(r[3], 19);
}

TEST(CppIterTools, TestIndexedRangeWithThreeDoubleArguments) {
  auto r = range(0.0, 0.99, 0.2);
  EXPECT_DOUBLE_EQ(r[0], 0.0);
  EXPECT_DOUBLE_EQ(r[1], 0.2);
  EXPECT_DOUBLE_EQ(r[2], 0.4);
  EXPECT_DOUBLE_EQ(r[3], 0.6);
  EXPECT_DOUBLE_EQ(r[4], 0.8);
}

TEST(CppIterTools, TestAccumulateSimple) {
  const std::vector<int> expected{1, 3, 6, 10, 15};
  const std::vector<int> data{1, 2, 3, 4, 5};
  auto acc = accumulate(data);
  const std::vector<int> accumulated(std::begin(acc), std::end(acc));
  EXPECT_TRUE(accumulated == expected);
}

TEST(CppIterTools, TestAccumulateSimpleMuliplication) {
  const std::vector<int> expected{1, 2, 6, 24, 120};
  const std::vector<int> data{1, 2, 3, 4, 5};
  auto acc = accumulate(data, [](int a, int b) { return a * b; });
  const std::vector<int> accumulated(std::begin(acc), std::end(acc));
  EXPECT_TRUE(accumulated == expected);
}

TEST(CppIterTools, TestAccumulateZeroInMiddleMuliplication) {
  const std::vector<int> expected{3, 12, 72, 144, 144, 1296, 0, 0, 0, 0};
  const std::vector<int> data{3, 4, 6, 2, 1, 9, 0, 7, 5, 8};
  auto acc = accumulate(data, [](int a, int b) { return a * b; });
  const std::vector<int> accumulated(std::begin(acc), std::end(acc));
  EXPECT_TRUE(accumulated == expected);
}

TEST(CppIterTools, TestAccumulateWithMaximumFunction) {
  const std::vector<int> expected{3, 4, 6, 6, 6, 9, 9, 9, 9, 9};
  const std::vector<int> data{3, 4, 6, 2, 1, 9, 0, 7, 5, 8};
  auto acc = accumulate(data, [](int a, int b) { return std::max(a, b); });
  const std::vector<int> accumulated(std::begin(acc), std::end(acc));
  EXPECT_TRUE(accumulated == expected);
}

TEST(CppIterTools, TestAccumulateSimulateLoadAmortizationOfAnnualPayments) {
  // Amortize a 5% loan of 1000 with 4 annual payments of 90
  const std::vector<double> expected{1000, 960.0, 918.0, 873.9, 827.595};
  const std::vector<int> cashflows{1000, -90, -90, -90, -90};
  auto acc = accumulate(cashflows,
      [](auto balance, auto payment) { return balance * 1.05 + payment; });
  const std::vector<double> accumulated(std::begin(acc), std::end(acc));
  for (int i : range(expected.size()))
    EXPECT_NEAR(accumulated[i], expected[i], 0.1);
}

TEST(CppIterTools, TestAccumulateandRepeatSimulateChaoticRecurrenceRelation) {
  // See: https://en.wikipedia.org/wiki/Logistic_map
  const std::vector<double> expected{0.40, 0.91, 0.30, 0.81, 0.60, 0.92, 0.29,
      0.79, 0.63, 0.88, 0.39, 0.90, 0.33, 0.84, 0.52, 0.95, 0.18, 0.57, 0.93,
      0.25, 0.71, 0.79, 0.63, 0.88, 0.39, 0.91, 0.32, 0.83, 0.54, 0.95, 0.20,
      0.60, 0.91, 0.30, 0.80, 0.60};
  constexpr double r = 3.8;
  const auto logistic_map = [&r](auto x, [[maybe_unused]] auto _) {
    return r * x * (1 - x);
  };
  auto inputs = repeat(0.4, expected.size());
  auto acc = accumulate(inputs, logistic_map);
  const std::vector<double> accumulated(std::begin(acc), std::end(acc));
  for (int i : range(expected.size()))
    EXPECT_NEAR(accumulated[i], expected[i], 0.1);
}


TEST(CppIterTools, TestChainBasicTwoStrings) {
  const std::vector<char> expected{'A', 'B', 'C', 'D', 'E', 'F'};
  const std::string s1{"ABC"};
  const std::string s2{"DEF"};
  auto ch = chain(s1, s2);
  const std::vector<char> chained(std::begin(ch), std::end(ch));
  EXPECT_TRUE(chained == expected);
}

TEST(CppIterTools, TestChainBasicThreeStrings) {
  const std::vector<char> expected{'A', 'B', 'C', 'D', 'E', 'F', 'X', 'Y', 'Z'};
  const std::string s1{"ABC"};
  const std::string s2{"DEF"};
  const std::string s3{"XYZ"};
  auto ch = chain(s1, s2, s3);
  const std::vector<char> chained(std::begin(ch), std::end(ch));
  EXPECT_TRUE(chained == expected);
}


TEST(CppIterTools, TestChainBasicThreeStringsInplaceDefined) {
  const std::vector<char> expected{'A', 'B', 'C', 'D', 'E', 'F', 'X', 'Y', 'Z'};
  auto ch = chain(std::string{"ABC"}, std::string{"DEF"}, std::string{"XYZ"});
  const std::vector<char> chained(std::begin(ch), std::end(ch));
  EXPECT_TRUE(chained == expected);
}

TEST(CppIterTools, TestChainFromInterable) {
  const std::vector<char> expected{'A', 'B', 'C', 'D', 'E', 'F', 'X', 'Y', 'Z'};
  const std::vector<std::string> iterable{"ABC", "DEF", "XYZ"};
  auto ch = chain.from_iterable(iterable);
  const std::vector<char> chained(std::begin(ch), std::end(ch));
  EXPECT_TRUE(chained == expected);
}

TEST(CppIterTools, TestChainAsFlattener) {
  const std::vector<int> expected{2, 4, 6, 8, 10, 12, 14, 16, 18};
  std::vector<std::vector<int>> vector_of_vectors{
      {}, {2, 4, 6}, {}, {8, 10, 12}, {14, 16, 18}, {}};
  auto ch = chain.from_iterable(vector_of_vectors);
  const std::vector<int> chained(std::begin(ch), std::end(ch));
  EXPECT_TRUE(chained == expected);
}

TEST(CppIterTools, TestCombinationsSimpleStringTwoByTwo) {
  const std::vector<std::vector<char>> expected{
      {'A', 'B'}, {'A', 'C'}, {'A', 'D'}, {'B', 'C'}, {'B', 'D'}, {'C', 'D'}};
  std::string iterable{"ABCD"};
  std::vector<std::vector<char>> combined;
  for (auto&& v : combinations(iterable, 2))
    combined.emplace_back(std::begin(v), std::end(v));
  EXPECT_EQ(combined, expected);
}

TEST(CppIterTools, TestCombinationsSimpleVectorThreeByThree) {
  const std::vector<std::vector<int>> expected{
      {0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 3}};
  const std::vector<int> iterable{0, 1, 2, 3};
  std::vector<std::vector<int>> combined;
  for (auto&& v : combinations(iterable, 3))
    combined.emplace_back(std::begin(v), std::end(v));
  EXPECT_EQ(combined, expected);
}


TEST(CppIterTools, TestCombinationsWithReplacementSimpleStringTwoByTwo) {
  const std::vector<std::vector<char>> expected{
      {'A', 'A'}, {'A', 'B'}, {'A', 'C'}, {'B', 'B'}, {'B', 'C'}, {'C', 'C'}};
  std::string iterable{"ABC"};
  std::vector<std::vector<char>> combined;
  for (auto&& v : combinations_with_replacement(iterable, 2))
    combined.emplace_back(std::begin(v), std::end(v));
  EXPECT_EQ(combined, expected);
}


TEST(CppIterTools, TestCompressSimpleVector) {
  const std::vector<char> expected{'A', 'C', 'E', 'F'};
  const std::vector<char> iterable{'A', 'B', 'C', 'D', 'E', 'F'};
  const std::vector<bool> selector{true, false, true, false, true, true};
  auto c = compress(iterable, selector);
  std::vector<char> compressed(std::begin(c), std::end(c));
  EXPECT_EQ(compressed, expected);
}

TEST(CppIterTools, TestCompressSimpleString) {
  const std::vector<char> expected{'A', 'C', 'E', 'F'};
  std::string iterable{"ABCDEF"};
  const std::vector<bool> selector{true, false, true, false, true, true};
  auto c = compress(iterable, selector);
  std::vector<char> compressed(std::begin(c), std::end(c));
  EXPECT_EQ(compressed, expected);
}

TEST(CppIterTools, TestCompressStringWithIntSelector) {
  const std::vector<char> expected{'A', 'C', 'E', 'F'};
  std::string iterable{"ABCDEF"};
  const std::vector<int> selector{1, 0, 1, 0, 1, 1};
  auto c = compress(iterable, selector);
  std::vector<char> compressed(std::begin(c), std::end(c));
  EXPECT_EQ(compressed, expected);
}


TEST(CppIterTools, TestCompressVectorWithTransformedSelector) {
  const std::vector<unsigned int> expected{11, 13, 15, 17, 19};
  std::vector<bool> selector;
  auto iterable = range(20);
  std::transform(std::begin(iterable), std::end(iterable),
      std::back_inserter(selector),
      [](unsigned int it) { return ((it > 10) && (it % 2 != 0)); });
  auto c = compress(iterable, selector);
  std::vector<unsigned int> compressed(std::begin(c), std::end(c));
  EXPECT_EQ(compressed, expected);
}


TEST(CppIterTools, TestCountSimple) {
  const std::vector<char> expected{10, 11, 12, 13, 14};
  std::vector<char> counted;
  for (auto c : count(10)) {
    counted.push_back(c);
    if (c == 14)
      break;
  }
  EXPECT_EQ(expected, counted);
}


TEST(CppIterTools, TestCountParameterless) {
  const std::vector<char> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<char> counted;
  for (auto c : count()) {
    counted.push_back(c);
    if (c == 9)
      break;
  }
  EXPECT_EQ(expected, counted);
}

TEST(CppIterTools, TestCountNoDefaultStep) {
  const std::vector<int> expected{0, 2, 4, 6, 8, 10};
  std::vector<int> counted;
  for (auto c : count(0, 2)) {
    counted.push_back(c);
    if (c > 9)
      break;
  }
  EXPECT_EQ(expected, counted);
}

TEST(CppIterTools, TestCountNoDefaultStepWithFloatRange) {
  const std::vector<double> expected{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
  std::vector<double> counted;
  for (auto c : count(0.0, 0.2)) {
    counted.push_back(c);
    if (c >= 1.0)
      break;
  }
  for (int i : range(expected.size()))
    EXPECT_DOUBLE_EQ(counted[i], expected[i]);
}


TEST(CppIterTools, TestCountDecreasing) {
  const std::vector<int> expected{10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1};
  std::vector<int> counted;
  for (auto c : count(10, -1)) {
    counted.push_back(c);
    if (c <= -1)
      break;
  }
  EXPECT_EQ(expected, counted);
}

/*
 * TODO: Negative float steps not working, check the submitted issue at
 *       https://github.com/ryanhaining/cppitertools/issues/60
TEST(CppIterTools, TestCountNoDefaultStepWithDecreasingFloatRange) {
  const std::vector<double> expected{1.0, 0.5, 0.0, -0.5, -1.0};
  std::vector<double> counted;
  for (auto c : count(1.0, -0.5)) {
    counted.push_back(c);
    if (c <= -1.0) break;
  }
  for (int i : range(expected.size()))
    EXPECT_DOUBLE_EQ(expected[i], counted[i]);
}
*/


TEST(CppIterTools, TestCycleSimpleVector) {
  const std::vector<int> expected{0, 1, 2, 3, 0, 1, 2, 3};
  const std::vector<int> period{0, 1, 2, 3};
  std::vector<int> cycled;
  for (auto c : cycle(period)) {
    cycled.push_back(c);
    if (cycled.size() == 2 * period.size())
      break;
  }
  EXPECT_EQ(expected, cycled);
}


TEST(CppIterTools, TestCycleSimpleString) {
  const std::vector<char> expected{
      'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D'};
  const std::string period("ABCD");
  std::vector<char> cycled;
  for (auto c : cycle(period)) {
    cycled.push_back(c);
    if (cycled.size() == 3 * period.size())
      break;
  }
  EXPECT_EQ(expected, cycled);
}

TEST(CppIterTools, TestDropwhileSimplePiped) {
  const std::vector<int> expected{6, 4, 1};
  std::vector<int> input{1, 4, 6, 4, 1};
  auto d = input | dropwhile([](int i) { return i < 5; });
  const std::vector<int> dropped(std::begin(d), std::end(d));
  EXPECT_EQ(expected, dropped);
}


TEST(CppIterTools, TestDropwhileSimple) {
  const std::vector<int> expected{6, 4, 1};
  auto filter = [](auto i) { return i < 5; };
  auto d = dropwhile(filter, std::vector<int>{1, 4, 6, 4, 1});
  const std::vector<int> dropped(std::begin(d), std::end(d));
  EXPECT_EQ(expected, dropped);
}


TEST(CppIterTools, TestEnumerateSimpleStringVariable) {
  using VectorOfPairs = std::vector<std::pair<std::size_t, char>>;
  const VectorOfPairs expected{{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}};
  std::string str = "ABCD";
  auto e = enumerate(str);
  const VectorOfPairs enumerated(std::begin(e), std::end(e));
  EXPECT_EQ(expected, enumerated);
}

TEST(CppIterTools, TestEnumerateSimpleString) {
  using VectorOfPairs = std::vector<std::pair<std::size_t, char>>;
  const VectorOfPairs expected{{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}};
  auto e = enumerate(std::string("ABCD"));
  const VectorOfPairs enumerated(std::begin(e), std::end(e));
  EXPECT_EQ(expected, enumerated);
}

TEST(CppIterTools, TestEnumerateSimpleArray) {
  using VectorOfPairs = std::vector<std::pair<std::size_t, char>>;
  const VectorOfPairs expected{{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}};
  auto e = enumerate(std::array{'A', 'B', 'C', 'D'});
  const VectorOfPairs enumerated(std::begin(e), std::end(e));
  EXPECT_EQ(expected, enumerated);
}

TEST(CppIterTools, TestFilterSimpleVector) {
  const std::vector<int> expected{-5, -4, -3, -2, -1};
  auto number_list = range(-5, 5);
  auto f = filter([](auto x) { return x < 0; }, number_list);
  const std::vector<int> filtered(std::begin(f), std::end(f));
  EXPECT_EQ(expected, filtered);
}

TEST(CppIterTools, TestFilterSimpleString) {
  const std::string expected{"abcdefghijklmnopqrstuvwxyz"};
  const std::string printable{
      "0123456789abcdefghijklmnopqrstuvwxyzABCD"
      "EFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,"
      "-./:;<=>?@[\\]^_`{|}~ \t\n\r"};
  auto f = printable |
           filter([](auto c) { return !std::isdigit(c) && std::islower(c); });
  const std::string filtered(std::begin(f), std::end(f));
  EXPECT_EQ(expected, filtered);
}

TEST(CppIterTools, TestFilterfalseSimpleVector) {
  const std::vector<int> expected{0, 2, 4, 6, 8};
  auto r = range(10);
  auto f = filterfalse([](auto x) { return x % 2; }, r);
  const std::vector<int> filtered(std::begin(f), std::end(f));
  EXPECT_EQ(expected, filtered);
}

TEST(CppIterTools, TestFilterfalseSimpleString) {
  const std::string expected{"acegikmoqsuwy"};
  const std::string input{"abcdefghijklmnopqrstuvwxyz"};
  auto f = filterfalse([](auto x) { return x % 2 == 0; }, input);
  const  std::string filtered(std::begin(f), std::end(f));
  EXPECT_EQ(expected, filtered);
}




int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
