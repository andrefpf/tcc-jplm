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

/** \file     ViewIOPolicyQueue.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-27
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEWIOPOLICYQUEUE_H__
#define JPLM_LIB_PART2_COMMON_VIEWIOPOLICYQUEUE_H__


#include <unordered_set>
#include <queue>
#include "Lib/Part2/Common/ViewIOPolicy.h"

//abstract
template<typename T>
class ViewIOPolicyQueue : public ViewIOPolicy<T> {
 protected:
  std::deque<View<T>*> queue; //!< The queue must keep the least accessed item at the front
  // std::unordered_set<const View<T>*> set; //!< the set is used to find faster a given view...


  bool is_loaded(const View<T>* view) {
    if(auto it = std::find(queue.begin(), queue.end(), view); it != queue.end()) {
      auto view_ref = *it;
      queue.erase(it);
      queue.push_back(view_ref);
      return true;
    }
    return false;
    // return set.count(view) > 0;
  }

  void release_view_image() {
    if (queue.size() > 0) {
      auto ptr_to_view = queue.front();
      // ptr_to_view->release_image();
      this->release_image_from_view(*ptr_to_view);
      queue.pop_front();
      // set.erase(ref_to_view);
    }
  }

  public:
  virtual ViewIOPolicyQueue<T>* clone() const = 0;

};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEWIOPOLICYQUEUE_H__ */