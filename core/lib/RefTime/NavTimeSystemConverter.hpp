//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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

#ifndef GPSTK_NAVTIMESYSTEMCONVERTER_HPP
#define GPSTK_NAVTIMESYSTEMCONVERTER_HPP

#include "TimeSystemConverter.hpp"
#include "NavLibrary.hpp"

namespace gpstk
{
      /// @ingroup TimeHandling
      //@{

      /** Define a class for doing time system conversions using
       * navigation messages provided by a NavLibrary object. */
   class NavTimeSystemConverter : public TimeSystemConverter
   {
   public:
         /** Get the offset in seconds between fromSys and toSys.
          * @pre navLib must be set.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] t The time at which the offset is being
          *   requested (i.e. the time being converted).
          * @param[out] offs The resulting offset, if available.
          * @return true if successful, false if unavailable. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& t, double& offs) override;

         /// Pointer to the nav library from which we will get time offset data.
      std::shared_ptr<NavLibrary> navLib;
   };
}
      //@}

#endif // GPSTK_NAVTIMESYSTEMCONVERTER_HPP
