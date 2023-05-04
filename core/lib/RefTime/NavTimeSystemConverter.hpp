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

#ifndef GNSSTK_NAVTIMESYSTEMCONVERTER_HPP
#define GNSSTK_NAVTIMESYSTEMCONVERTER_HPP

#include "TimeSystemConverter.hpp"
#include "NavLibrary.hpp"

namespace gnsstk
{
      /// @ingroup TimeHandling
      //@{

      /** Define a class for doing time system conversions using
       * navigation messages provided by a NavLibrary object.
       *
       * Example use:
       *
       * \code
       *    // Some time to convert
       * gnsstk::CommonTime ct(gnsstk::GPSWeekSecond(2020,2021));
       *    // Construct the time system converter
       * gnsstk::CommonTime::tsConv =
       *    make_shared<gnsstk::NavTimeSystemConverter>();
       *    // Get a pointer to the NavTimeSystemConverter
       * NavTimeSystemConverter *ntsc = dynamic_cast<NavTimeSystemConverter*>(
       *    CommonTime::tsConv.get());
       *    // Construct a NavLibrary object.
       * ntsc->navLib = make_shared<gnsstk::NavLibrary>();
       *    // Construct a NavDataFactory object
       * gnsstk::NavDataFactoryPtr ndfp(
       *    std::make_shared<gnsstk::MultiFormatNavDataFactory>());
       *    // Add the NavDataFactory to the NavLibrary
       * ntsc->navLib->addFactory(ndfp);
       *    // Add input data (files) to the NavDataFactory
       * if (!ndfp->addDataSource(inputFileName))
       * {
       *    cerr << "Unable to load \"" << inputFileName << "\"" << endl;
       *    return false;
       * }
       *   // Convert the time to UTC
       * if (!ct.changeTimeSystem(gnsstk::TimeSystem::UTC))
       * {
       *    cerr << "Unable to change time system to UTC" << endl;
       *    return false;
       * }
       * cout << "Time is "
       *      << gnsstk::printTime("%Y/%02m/%02d %02H:%02M:%02S %P") << endl;
       * \endcode
       */
   class NavTimeSystemConverter : public TimeSystemConverter
   {
   public:
         /** Get the offset in seconds between fromSys and toSys.
          * @pre navLib must be set.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] t The time at which the offset is being
          *   requested (i.e. the time being converted).
          * @param[out] offs The resulting offset in seconds, 
          *   if available, where when(toSys)=when(fromSys)-offset.
          * @return true if successful, false if unavailable. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& t, double& offs) override;

         /// Pointer to the nav library from which we will get time offset data.
      std::shared_ptr<NavLibrary> navLib;
   };
}
      //@}

#endif // GNSSTK_NAVTIMESYSTEMCONVERTER_HPP
