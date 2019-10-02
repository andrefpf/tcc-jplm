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

/** \file     ViewIOPolicyLimitedMemory.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-27
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEWIOPOLICYLIMITEDMEMORY_H__
#define JPLM_LIB_PART2_COMMON_VIEWIOPOLICYLIMITEDMEMORY_H__

#include "Lib/Part2/Common/ViewIOPolicyQueue.h"


template<typename T>
class ViewIOPolicyLimitedMemory : public ViewIOPolicyQueue<T> {
 protected:
  //value equivalent to 9 1920x1080 views with uint16_t
  std::size_t max_bytes = 111974400;  //about 14 MB
  std::size_t current_bytes = 0;

  virtual void load_image_if_necessary(View<T>& view) override {
    if (!this->is_loaded(&view)) {
      auto expected_number_of_bytes =
          view.get_number_of_pixels() * 3 * sizeof(T);
      if (current_bytes + expected_number_of_bytes > max_bytes) {
        this->release_view_image();
      } else {
        current_bytes += expected_number_of_bytes;
      }
      this->queue.push_back(&view);
      // this->set.insert(&view);
      view.load_image();
    }
  }

 public:
  virtual ViewIOPolicyLimitedMemory<T>* clone() const override {
    return new ViewIOPolicyLimitedMemory(*this);
  }


  void set_max_bytes(std::size_t bytes) {
    max_bytes = bytes;
    while (current_bytes > max_bytes) {
      auto expected_number_of_bytes =
          this->queue.front()->get_number_of_pixels() * 3 * sizeof(T);
      this->release_view_image();
      current_bytes -= expected_number_of_bytes;
    }
  }


  auto get_max_bytes() const {
    return max_bytes;
  }


  auto get_current_bytes() const {
    return current_bytes;
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEWIOPOLICYLIMITEDMEMORY_H__ */