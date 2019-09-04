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

/** \file     DBox.h
 *  \brief    This file describes the DBox abstract class
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-29
 */

#ifndef JPLM_LIB_COMMON_GENERIC_DBOX_H__
#define JPLM_LIB_COMMON_GENERIC_DBOX_H__

#include <cstddef>
#include <stdexcept>
#include <vector>


/**
 * \brief      Abstract class to represent a DBox (the contents of a box)
 * 
 * \details    Remembering the structure of a Box:
 *      \image html box/out.png
 *      
 *      Notice that the DBox have variable size. Thus the correct size of the must be provided in derived DBox implementations.
 *      
 *      \sa \ref data_box_info and \ref parser_info
 *          
 *          
 *      
 */
class DBox {
 public:
  DBox() = default;
  virtual ~DBox() = default;

  /**
   * \brief      Gets the size, in bytes, of the data contained in this DBox
   *
   * \return     The size, i.e., the number of bytes
   */
  virtual uint64_t size() const noexcept = 0;


  /**
   * \brief      Creates a new instance of the object with same properties than original.
   *
   * \return     A raw pointer to a copy of this object. 
   * 
   * \warning    Given that this function returns a raw pointer, it is important to delete the created new object.
   *             In fact, the ideal is to manage the cloned object by means of smart pointers.
   *             The use of this method is to avoid object slicing when copying objects referenced by the base classes.
   */
  virtual DBox* clone() const = 0;


  /**
   * \brief      Determines if this is equal to other.
   *
   * \param[in]  other  The other to compare with this
   *
   * \return     True if equal, false otherwise.
   */
  virtual bool is_equal(const DBox& other) const = 0;


  /**
   * \brief      Writes the contents (data) of this DBox into the ostream.
   *
   * \param      stream  The stream to write into
   *
   * \return     A reference to the same stream that whas passed as parameter
   */
  virtual std::ostream& write_to(std::ostream& stream) const = 0;


  /**
   * \brief      Equal comparision operator
   *
   * \param[in]  other  The other DBox to compare with this
   *
   * \return     True if both DBoxes are equal and false otherwise.
   *
   * \details    The equality is defined by the derived classes, by overriding the is_equal method.
   */
  bool operator==(const DBox& other) const {
    return this->is_equal(other);
  }

  /**
   * \brief      Not equal comparison operator
   *
   * \param[in]  other  The other DBox to compare with this
   *                    
   * \return     False if both DBoxes are equal and true otherwise. 
   *
   * \details    The equality is defined by the derived classes, by overriding the is_equal method.
   *             This operator is the negation of operator==
   */
  bool operator!=(const DBox& other) const {
    return !this->operator==(other);
  }
};

#endif /* end of include guard: JPLM_LIB_COMMON_GENERIC_DBOX_H__ */

/*! \page data_box_info How to implement a DBox (contents of a box)
  \tableofcontents
  This page will introduce briefly how to implement a new DBox.
  \section sec1 Creating a new DBox
  
  \section sec2 Dealing with superboxes

  By definition, a SuperBox is a box that includes other boxes. 
  In the case of a super box contents, it is necessary to inherit from SuperBoxDBox.
  Moreover, the write_to method must be overriden. 
  Let us take for instance the JpegPlenoLightFieldContents.
  It contains a ProfileAndLevelBox, a JpegPlenoLightFieldHeaderBox and a ContiguousCodestreamBox.

*/