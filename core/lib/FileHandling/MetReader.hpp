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

#ifndef METREADER_HPP
#define METREADER_HPP

#include <string>

#include "CommandOption.hpp"
#include "WxObsMap.hpp"

namespace gnsstk
{
      /** Class for reading weather (meteorological) data from a RINEX
       * MET file and storing it internally for look-up. */
   class MetReader
   {
   public:
         /// Initialize internal data structures.
      MetReader();

         /** Initialize and load a RINEX MET file.
          * @param[in] fn The path to the RINEX MET file.
          * @throw FileMissingException if unable to open fn. */
      MetReader(const std::string& fn);

         /** Load the data from a RINEX MET file.
          * @param[in] fn The path to the RINEX MET file.
          * @post wx contains the valid weather data loaded from fn. 
          * @return false if the file can't be opened. */
      bool read(const std::string& fn);

         /** This doesn't actually do anything, but it's here for
          * backwards compatibility. */
      unsigned verboseLevel;

         /// The storage for all weather data read.
      gnsstk::WxObsData wx;
   };
}
#endif
