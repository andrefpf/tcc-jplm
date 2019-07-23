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

/** \file     Box.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-23
 */


#ifndef JPLM_LIB_PART1_COMMON_BOX_H__
#define JPLM_LIB_PART1_COMMON_BOX_H__

#include <cstdint>
#include "BoxDataHolder.h"
#include <optional>
#include <variant>



class LBox : BoxDataHolder<uint32_t> {
 public:
  LBox();
  ~LBox();
};

class TBox : BoxDataHolder<uint32_t>  {
 public:
  TBox();
  ~TBox();
};


class XLBox : BoxDataHolder<uint64_t> {
 public:
  XLBox();
  ~XLBox();
};


class DBox {
 public:
  DBox();
  ~DBox();
};


class Box {
 protected:
  LBox l_box;
  TBox t_box;
  std::optional<XLBox> xl_box;
  DBox d_box;

 public:
  Box() = default;
  ~Box() = default;

  std::variant<LBox, XLBox> get_lenght();

  //LBox (required) 4-byte big-endian usigned integer: uint32_t
  //TBox (required) 4-byte big-endian usigned integer: uint32_t
  //XLBox (optional) if LBox.value = 1, 8-byte big-endian usigned integer: uint64_t
  //DBox (required) variable size: depends on what this contains
};


// std::ostream& operator<< (std::ostream& stream, const Box& Box);

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOX_H__ */