#ifndef METRICS_H__
#define METRICS_H__

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

/** \file     Metrics.h
 *  \brief    Definition of a few metrics
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-20
 */

#include <inttypes.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

namespace Metrics {
template<typename T>
double get_sum_of_squared_errors(const std::vector<T>& errors) {
  auto sum = 0.0;
  for (auto v : errors) {
    auto diff = static_cast<double>(v);
    sum += diff * diff;
  }
  return sum;
}

template<typename T>
double get_mean_squared_error(const std::vector<T>& errors) {
  if (errors.size() == 0)
    return std::numeric_limits<double>::infinity();
  auto sse = get_sum_of_squared_errors<T>(errors);
  return sse / errors.size();
}

template<typename T>
double get_peak_signal_to_noise_ratio(
    const std::vector<T>& errors, T max_value) {
  auto mse = get_mean_squared_error<T>(errors);
  if (mse == 0.0)
    return std::numeric_limits<double>::infinity();
  if (mse == std::numeric_limits<double>::infinity())
    return 0.0;
  return 10.0 * std::log10(max_value*max_value / mse);
}
}  // namespace Metrics

#endif /* end of include guard: METRICS_H__ */