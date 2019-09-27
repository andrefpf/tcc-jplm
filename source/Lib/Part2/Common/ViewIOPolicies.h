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

/** \file     ViewIOPolicies.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-06
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEWIOPOLICIES_H__
#define JPLM_LIB_PART2_COMMON_VIEWIOPOLICIES_H__


#include <unordered_set>
#include "Lib/Part2/Common/CommonExceptions.h"
#include "Lib/Part2/Common/View.h"
#include "Lib/Utils/Image/Image.h"

template<typename T>
class ViewIOPolicy {
 protected:
  virtual void load_image_if_necessary(View<T>& view) = 0;

 public:
  ViewIOPolicy() = default;

  ViewIOPolicy(const ViewIOPolicy<T>& other) = default;

  virtual ~ViewIOPolicy() = default;


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) {
    load_image_if_necessary(view);
    return view.get_value_at(channel, coordinate);
  }


  virtual const Image<T>& get_image_at(View<T>& view) {
    load_image_if_necessary(view);
    return *view.get_image_ptr();
  }

  virtual ViewIOPolicy<T>* clone() const = 0;
};








#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEWIOPOLICIES_H__ */