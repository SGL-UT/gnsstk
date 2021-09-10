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
 * @file IonexStream.hpp
 * File stream for IONEX format files
 */

#ifndef GNSSTK_IONEXSTREAM_HPP
#define GNSSTK_IONEXSTREAM_HPP

#include "FFTextStream.hpp"
#include "IonexHeader.hpp"

namespace gnsstk
{

      /// @ingroup FileHandling
      //@{

      /** This class provides access to IONEX files.
       *
       * @sa gnsstk::IonexHeader and gnsstk::IonexData for more information.
       * @sa main_ionextest.cpp for an example.
       */
   class IonexStream : public FFTextStream
   {
   public:

         /// Default constructor
      IonexStream()
            : headerRead(false) {};


         /** Common constructor.
          *
          * @param fn      IONEX file to open
          * @param mode    Mode to open \a fn.
          */
      IonexStream(const char* fn, std::ios::openmode mode=std::ios::in)
            : FFTextStream(fn, mode), headerRead(false) {};


         /// Destructor
      virtual ~IonexStream() {};


         /// Overrides open to reset the header
      virtual void open(const char* fn, std::ios::openmode mode)
      {

         FFTextStream::open(fn, mode);
         headerRead = false;
         header = IonexHeader();

      };


         /// Whether or not the IonexHeader has been read
      bool headerRead;


         /// The header for this file.
      IonexHeader header;


   }; // End of class 'IonexStream'


      //@}


}  // End of namespace gnsstk
#endif   // GNSSTK_IONEXSTREAM_HPP
