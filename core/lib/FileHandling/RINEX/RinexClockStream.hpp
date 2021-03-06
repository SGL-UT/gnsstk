//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
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
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
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
 * @file RinexClockStream.hpp
 * File stream for RINEX clock data file
 */

#ifndef GNSSTK_RINEXCLOCKSTREAM_HPP
#define GNSSTK_RINEXCLOCKSTREAM_HPP

#include <string>
#include "FFTextStream.hpp"
#include "RinexClockHeader.hpp"

namespace gnsstk
{
      /// @ingroup FileHandling
      //@{

      /**
       * This class performs file i/o on a RINEX clock file for the
       * RinexClockHeader and RinexClockData classes.
       *
       * @sa gnsstk::RinexClockData and gnsstk::RinexClockHeader.
       */
   class RinexClockStream : public FFTextStream
   {
   public:
         /// Default constructor
      RinexClockStream();

         /** Common constructor.
          *
          * @param fn      the RINEX clock data file to open
          * @param mode    how to open \a fn.
          */
      RinexClockStream( const char* fn,
                        std::ios::openmode mode=std::ios::in );

         /** Common constructor.
          *
          * @param fn      the RINEX clock data file to open
          * @param mode    how to open \a fn.
          */
      RinexClockStream( const std::string& fn,
                        std::ios::openmode mode=std::ios::in );

         /// Destructor
      virtual ~RinexClockStream();

         /** Overrides open to reset the header
          *
          * @param fn      the RINEX clock data file to open
          * @param mode    how to open \a fn.
          */
      virtual void open( const char* fn,
                         std::ios::openmode mode );

         /** Overrides open to reset the header
          *
          * @param fn      the RINEX clock data file to open
          * @param mode    how to open \a fn.
          */
      virtual void open( const std::string& fn,
                         std::ios::openmode mode );

         /// Whether or not the RinexClockHeader has been read
      bool headerRead;

         /// The header for this file.
      RinexClockHeader header;

   private:
         /// Initialize internal data structures
      void init();
   }; // End of class 'RinexClockStream'

      //@}

}  // End of namespace gnsstk

#endif   // GNSSTK_RINEXCLOCKSTREAM_HPP
