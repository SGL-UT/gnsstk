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
//  Copyright 2004-2023, The Board of Regents of The University of Texas System
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
#include <cmath>  // isnan
#include <iomanip>  // setw
#include <ios>
#include <ostream>

#include "DumpDetail.hpp"
#include "GPSLNavNMC.hpp"
#include "GPSNMCTAI.hpp"  // asString
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   GPSLNavNMC ::
   GPSLNavNMC()
   {
      // For ease, the transmit time should be the start of the subframe
      // containing the NMCT and the message length is just the length of
      // subframe, in seconds.
      msgLenSec = 6.0;
   }


   bool GPSLNavNMC ::
   validate() const
   {
      // 20.3.3.4.4 NMCT Validity Time - "If the AODO term is 27900 seconds 
      // (i.e., binary 11111), then the NMCT currently available from the transmitting SV
      // is invalid and shall not be used."
      if (aodo < 0 || aodo >= 27900)
         return false;

      // 20.3.3.5.1.9 NMCT - "A binary value of “100000” shall indicate 
      // that no valid ERD for the corresponding SV ID is present in that slot."
      if (erd > 9.3 || erd < -9.3)
         return false;

      return true;
   }


   void GPSLNavNMC ::
   dump(std::ostream &s, DumpDetail dl) const
   {
      const std::ios::fmtflags oldFlags{s.flags()};
      s.setf(std::ios::fixed, std::ios::floatfield);
      s.setf(std::ios::right, std::ios::adjustfield);
      s.setf(std::ios::uppercase);
      s.precision(0);
      s.fill(' ');

      switch(dl)
      {
      case DumpDetail::OneLine:
         NavData::dump(s, dl);
         break;
      case DumpDetail::Brief:
         NavData::dump(s, dl);
         s << StringUtils::asString(availabilityIndicator) << '\n'
           << "Tnmct = " << getDumpTime(dl, timeStamp) << " ERD = " << erd;
         break;
      case DumpDetail::Full:
         s << "****************************************************************************\n"
           << "NMCT ERD\n\n"
           << getSignalString() << '\n'
           << "TIMES OF INTEREST\n\n"
           << "           " << getDumpTimeHdr(dl) << '\n'
           << "Transmit (SF4PG18):     " << getDumpTime(dl, timeStamp) << '\n'
           << "Toe (SF1):              " << getDumpTime(dl, Toe) << '\n'
           << "Tnmct:                  " << getDumpTime(dl, Tnmct) << "\n\n"
           << "AODO (SF2):             " << aodo << '\n'
           << "Availability Indicator: " << StringUtils::asString(availabilityIndicator) << '\n';
         s.setf(std::ios::scientific, std::ios::floatfield);
         s.precision(8);
         s.fill(' ');
         s  << "ERD:                   " << std::setw(16) << erd << '\n';
      }
      s.flags(oldFlags);
   }


   bool GPSLNavNMC::
   isSameData(const NavDataPtr& right, bool ignore_timestamp) const
   {
      const std::shared_ptr<GPSLNavNMC> nmc = std::dynamic_pointer_cast<GPSLNavNMC>(right);
      
      if (!nmc)
      {
         return false;
      }
      return (NavData::isSameData(right, ignore_timestamp) &&
         (aodo == nmc->aodo) &&
         (Toe == nmc->Toe) &&
         (availabilityIndicator == nmc->availabilityIndicator) &&
         (
            (erd == nmc->erd) ||
            (std::isnan(erd) && std::isnan(nmc->erd))
         ) && 
         (Tnmct == nmc->Tnmct));
   }


   void GPSLNavNMC::updateTNMCT()
   {
      double offset = static_cast<unsigned long>(GPSWeekSecond{Toe}.getSOW()) % 7200;

      if (offset == 0)
      {
         Tnmct = Toe - aodo;
      }
      else
      {
         Tnmct = Toe - offset + 7200 - aodo;
      }

      // The week crossover check provided by the IS-GPS-200 is not necessary here.
      // The IS-GPS-200 is assuming that Toe is seconds-of-week whereas Toe here
      // is a full timestamp and should already be week-disambiguated.
   }
} // namespace gnsstk
