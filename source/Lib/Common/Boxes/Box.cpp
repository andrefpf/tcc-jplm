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

/** \file     Box.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-23
 */

#include "Box.h"

std::variant<LBox, XLBox> Box::get_lenght() const noexcept {
  constexpr uint64_t LBox_size = 4;  //bytes = 32 bits
  constexpr uint64_t TBox_size = 4;  //bytes = 32 bits
  //this means that the LBox will contain at least 8 bytes and thus
  //never use the reserved for ISO use values;

  auto total_box_size = LBox_size + TBox_size + d_box->size();
  //assuming that size fits in LBox (i.e., 32 bits)

  if (total_box_size > std::numeric_limits<uint32_t>::max()) {
    // l_box.set_value(1); //kept for illustration...
    // if XLBox is present, l_box must be 1;
    constexpr uint64_t XLBox_size = 8;  //bytes = 64 bits
    total_box_size += XLBox_size;  //needs more XLBox_size bytes
    return XLBox(total_box_size);
  }
  return LBox(total_box_size);
}


LBox Box::get_lbox() const noexcept {
  auto lenght = this->get_lenght();
  if (lenght.index() == 0) {  //the lbox;
    return std::get<0>(lenght);
  }
  //else, the type is XLBox, thus needs to set lbox to 1
  return LBox(1);
}


TBox Box::get_tbox() const noexcept {
  return this->t_box;
}


t_box_id_type Box::get_id() const noexcept {
  return this->t_box.get_value();
}


std::optional<XLBox> Box::get_xlbox() const noexcept {
  auto lenght = this->get_lenght();
  if (lenght.index() == 0) {  //the lbox
    return {};  //empty XLBox
  }
  return std::get<1>(lenght);
}

std::unique_ptr<DBox> Box::get_dbox() const noexcept {
  return std::unique_ptr<DBox>(this->d_box->clone());
}


const DBox& Box::get_ref_to_dbox() const noexcept {
  return *(this->d_box);
}


DBox& Box::get_ref_to_dbox() noexcept {
  return *(this->d_box);
}


std::uint64_t Box::size() const noexcept {
  auto lenght = this->get_lenght();
  return std::visit(
      [](auto& value_holder) { return (uint64_t) value_holder.get_value(); },
      lenght);
}


std::ostream& operator<<(std::ostream& stream, const Box& box) {
  stream << box.get_lbox() << box.get_tbox();
  auto xlbox = box.get_xlbox();
  if (xlbox) {
    stream << xlbox.value();
  }
  return box.d_box->write_to(stream);
}