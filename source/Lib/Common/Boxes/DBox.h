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
  \section creating_new_dbox Creating a new DBox
  
  A new DBox class must inherit from DBox. 
  However, to provide a more complete DBox implementation, the DBoxes can inherit from:
    - InMemoryDBox: that keeps its data in memory; it is fast, but may demand too much memory;
    - InFileDBox : that keeps its data in file; it saves memory, but is slow.; 
  \todo InFileDBox is not yet implemented.

  In both cases, the following methods must be overridden:
    - clone : such method is used to copy the DBox without causing object slicing. Its use is mainly done by the Box class constructors; 
    - is_equal : method used to compare DBoxes. Useful for testing;
    - size : method that informs the Box the size of DBox contents (in bytes). 
             This method <b>MUST</b> be correct to allow correct encoding/decoding (parsing) of boxes.
             Moreover, if this method is incorrect, the parsing of other boxes may be affected;

  Most boxes that are already defined (see \link DefinedBoxes \endlink) have their DBox inherited from InMemoryDBox.

  \subsection inheriting_from_mem_d_box Inheriting from InMemoryDBox

  In this case, there is another method to be overridden:
    - get_bytes : this method creates a vector of bytes that will be written in the stream when the boxes are written; (see InMemoryDBox::get_bytes) 

  The standard defines that data must be written in the file considering big-endian bytes. 
  As the endianess depends on the processor architecture, there is a utility called \link BinaryTools \endlink that provides methods to generate sequences of bytes in the correct big-endian order.

  For instance, to include a uint16_t variable in the bytes vector (from InMemoryDBox::get_bytes method):

  \code{.cpp}
    uint16_t myval = 42;
    auto bytes = std::vector<std::byte>();
    BinaryTools::append_big_endian_bytes(bytes, myval);
  \endcode

  After this, bytes vector will contain the two bytes of myval in big-endian order. 

  \note During parsing, the endianess conversion (if needed) is already embedded in BoxParserHelperBase::get_next method. 
  Also, when not needed there will be no loss in performance (the compiler will optimize such calls).

  \subsection inheriting_from_infile_d_box Inheriting from InFileDBox  

  \warning Not implemented yet...

  \section dealing_with_superboxes Dealing with SuperBox (es)

  By definition, a SuperBox is a box that includes other boxes. 
  In the case of a super box contents, it is necessary to inherit from SuperBoxDBox.
  Moreover, the write_to method must be overridden. 
  Let us take for instance the JpegPlenoLightFieldContents.
  It contains a ProfileAndLevelBox, a JpegPlenoLightFieldHeaderBox and a ContiguousCodestreamBox.

  Its overridden write_to function looks like:

  \snippet Lib/Part2/Common/Boxes/JpegPlenoLightFieldContents.h Overridden write_to in JpegPlenoLightFieldContents

  It is in fact very simple: it is just redirecting the boxes to the stream in the order they must appear in file.
  Also, the <b>required</b> boxes must be present (if not, an error/exception will be thrown). 
  Non required boxes should be tested first to see if they are available. 
  In the example above, the Contiguous Codestream Box is optional.

*/