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

/// @file AntexStream.hpp
/// File stream for Rinex observation file data

#ifndef GNSSTK_ANTEX_STREAM_HPP
#define GNSSTK_ANTEX_STREAM_HPP

#include <vector>
#include <list>
#include <map>

#include "FFTextStream.hpp"
#include "AntexHeader.hpp"

namespace gnsstk
{
      /// @ingroup FileHandling
      //@{

      /// This class reads an Antex files.
      ///
      /// @sa gnsstk::AntexData and gnsstk::AntexHeader.
   class AntexStream : public FFTextStream
   {
   public:
         /// Default constructor
      AntexStream() : headerRead(false) {}
      
         /// Constructor given a file name
         /// @param fn the Antex file to open
         /// @param mode how to open \a fn.
      AntexStream(const char* fn, std::ios::openmode mode=std::ios::in)
            : FFTextStream(fn, mode), headerRead(false) {}

         /// Destructor
      virtual ~AntexStream() {}
      
         /// overrides open to reset the header
      virtual void open(const char* fn, std::ios::openmode mode)
      { 
         FFTextStream::open(fn, mode); 
         headerRead = false; 
         header = AntexHeader();
      }

         /// Whether or not the AntexHeader has been read
      bool headerRead;

         /// The header for this file.
      AntexHeader header;
   };

      //@}

} // namespace

#endif
