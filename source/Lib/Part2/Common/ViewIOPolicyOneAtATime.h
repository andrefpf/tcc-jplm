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

/** \file     ViewIOPolicyOneAtATime.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-27
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEWIOPOLICYONEATATIME_H__
#define JPLM_LIB_PART2_COMMON_VIEWIOPOLICYONEATATIME_H__

#include "Lib/Part2/Common/ViewIOPolicy.h"


/**
 * @brief      This class describes a view i/o policy that opens one view at a time.
 *
 * @tparam     T     Type of the pixel in the view
 */
template<typename T>
class ViewIOPolicyOneAtATime : public ViewIOPolicy<T> {
 protected:
  View<T>* last = nullptr;


  /**
   * @brief      Loads an image if necessary.
   *
   * @param      view  The view
   */
  void load_image_if_necessary(View<T>& view) {
    if (last != nullptr && last != &view) {
      this->release_image_from_view(*last);
    }
    view.load_image();
    last = &view;
  }

 public:
  /**
   * @brief      Creates a new instance of the object with same properties than original.
   *
   * @return     Copy of this object.
   */
  virtual ViewIOPolicyOneAtATime<T>* clone() const override {
    return new ViewIOPolicyOneAtATime(*this);
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEWIOPOLICYONEATATIME_H__ */