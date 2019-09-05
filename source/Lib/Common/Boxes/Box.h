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


#ifndef JPLM_LIB_COMMON_GENERIC_BOX_H__
#define JPLM_LIB_COMMON_GENERIC_BOX_H__


#include <iostream>
#include <memory>
#include <optional>
#include <variant>
#include "DBox.h"
#include "EmptyDBox.h"
#include "LBox.h"
#include "TBox.h"
#include "XLBox.h"


/**
 * \brief      Abstract class to represent a box.
 * 
 * \details    Remembering the structure of a Box:
 *      \image html box/out.png
 *      
 *      As can be seen, a Box contains: 
 *        - LBox (required) 4-byte big-endian usigned integer: uint32_t
 *        - TBox (required) 4-byte big-endian usigned integer: uint32_t
 *        - XLBox (optional) if LBox.value == 1, 8-byte big-endian usigned integer: uint64_t
 *        - DBox (required) variable size: depends on what this contains
 *        
 *      Yet, in the box class only holds its type and content. 
 *      After all, the box is able to compute its size (Box::size) and then decide wether or not it needs a XLBox.
 */
class Box {
 protected:
  // LBox l_box; //kept here just for illustration
  TBox t_box; //!< The TBox that keeps the id of this box
  // std::optional<XLBox> xl_box; //kept here just for illustration
  std::unique_ptr<DBox> d_box; //!< Unique ptr to the contents kept by the box

  /**
   * \brief      There should be no default constructor for the box. 
   * An id and contents must be set. 
   * Thus, the default constructor is deleted.
   */
  Box() = delete;


 public:
  /**
   * \brief      Constructs a Box.
   *
   * \param[in]  t_box  The TBox that indicates the ID of this Box.
   * \param[in]  d_box  A reference to contents (DBox) that will be copied and kept by this Box.
   */
  Box(TBox t_box, const DBox& d_box = EmptyDBox())
      : t_box(t_box), d_box(std::unique_ptr<DBox>(d_box.clone())) {
  }


  /**
   * \brief      Constructs a Box.
   *
   * \param[in]  t_box  The TBox that indicates the ID of this Box.
   * \param      d_box  A temporary unique_ptr the contents (DBox) that will be moved to be held by this Box.
   */
  Box(TBox t_box, std::unique_ptr<DBox>&& d_box)
      : t_box(t_box), d_box(std::move(d_box)) {
  }


  /**
   * \brief      Copy constructor of the Box.
   *
   * \param[in]  other  The other Box that will be copied to this new Box.
   */
  Box(const Box& other)
      : t_box(other.t_box), d_box(std::unique_ptr<DBox>(other.d_box->clone())) {
  }


  /**
   * \brief      Move constructor for this Box.
   *
   * \param      other  The other Box that will give ownership of its contents to this box.
   */
  Box(Box&& other) : t_box(other.t_box), d_box(std::move(other.d_box)) {
  }

  /**
   * \brief      Destroys the object. 
   */
  virtual ~Box() = default;

  /**
   * \brief      Gets the lenght (within the respective type) of the Box.
   *
   * \return     The lenght, that can be either a LBox or a XLBox, depending on the size of this Box.
   */
  std::variant<LBox, XLBox> get_lenght() const noexcept;


  /**
   * \brief      Gets the size of this Box.
   *
   * \return     Size of this Box (in bytes).
   */
  std::uint64_t size() const noexcept;


  /**
   * \brief      Gets the LBox.
   *
   * \return     The LBox (copy). 
   *             It may represent the size (lenght) of this Box, if such size fits in 32 bits (uint32_t).
   *             Otherwise, it will be 1 to show that a XLBox holds the actual size of this Box.
   */
  LBox get_lbox() const noexcept;


  /**
   * \brief      Gets the TBox.
   *
   * \return     The TBox (copy), that indicates the type (ID) of this box.
   */
  TBox get_tbox() const noexcept;


  /**
   * \brief      Gets the XLBox (if used by this box).
   *
   * \return     Possibily the XLBox. Its is optional since it may not be used by the box.
   * 
   */
  std::optional<XLBox> get_xlbox() const noexcept;


  /**
   * \brief      Gets a clone of the DBox.
   *
   * \return     A unique_ptr to a clone of the DBox that is kept by this Box.
   */
  std::unique_ptr<DBox> get_dbox() const noexcept;


  /**
   * \brief      Gets a const reference to dbox.
   *
   * \return     The const reference to dbox.
   */
  const DBox& get_ref_to_dbox() const noexcept;


  /**
   * \brief      Gets a reference to dbox.
   *
   * \return     The reference to dbox.
   */
  DBox& get_ref_to_dbox() noexcept;


  bool has_same_type(const Box& other) const noexcept {
    if (other.t_box == this->t_box)
      return true;
    return false;
  }


  bool has_same_lenght(const Box& other) const noexcept {
    if (other.size() != this->size()) {
      return false;
    }
    return true;
  }


  bool holds_same_data(const Box& other) const noexcept {
    if (*(other.d_box) != *(this->d_box)) {
      return false;
    }
    return true;
  }


  bool is_equal(const Box& other) const noexcept {
    if (this->holds_same_data(other) && this->has_same_type(other) &&
        this->has_same_lenght(other))
      return true;
    return false;
  }


  virtual const DBox& get_ref_to_contents() const noexcept = 0;


  virtual DBox& get_ref_to_contents() = 0;


  virtual DBox* data() = 0;


  virtual const DBox* data() const = 0;


  bool operator==(const Box& other) const {
    return this->is_equal(other);
  }


  bool operator!=(const Box& other) const {
    return !this->operator==(other);
  }


  friend std::ostream& operator<<(std::ostream& stream, const Box& Box);
};


std::ostream& operator<<(std::ostream& stream, const Box& Box);

#endif /* end of include guard: JPLM_LIB_COMMON_GENERIC_BOX_H__ */


/**
 * \defgroup DefinedBoxes
 * \brief This group has a list of the defined boxes
 */