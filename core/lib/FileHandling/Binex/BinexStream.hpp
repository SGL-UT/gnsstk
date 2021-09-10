//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024 
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public 
//                            release, distribution is unlimited.
//
//==============================================================================

/**
 * @file BinexStream.hpp
 * File stream for BINEX files
 */

#ifndef GNSSTK_BINEXSTREAM_HPP
#define GNSSTK_BINEXSTREAM_HPP

#include "FFBinaryStream.hpp"

namespace gnsstk
{
      /// @ingroup FileHandling
      //@{

      /**
       * This class performs file i/o on a BINEX file for the 
       * BinexData classes.
       *
       * @sa binex_read_write.cpp for an example.
       * @sa binex_test.cpp for an example.
       * @sa BinexData.
       *
       */
   class BinexStream : public FFBinaryStream
   {
   public:
         /// Destructor
      virtual ~BinexStream() {}
      
         /// Default constructor
      BinexStream() {}
      
         /** Constructor 
          * Opens a file named \a fn using ios::openmode \a mode.
          */
      BinexStream(const char* fn,
                  std::ios::openmode mode=std::ios::in | std::ios::binary)
            : FFBinaryStream(fn, mode) {};

   protected:
         /** @warning This is used by FFBinaryStream's getData and
          * writeData methods to determine how to write binary encoded
          * data.  BINEX can be either big-endian or little-endian so
          * this isn't really useful.  As such, DO NOT USE writeData
          * or getData in the implementation of BinexData. */
      virtual bool isStreamLittleEndian() const throw()
      { return true; }
   };

      //@}

} // namespace gnsstk

#endif // GNSSTK_BINEXSTREAM_HPP
