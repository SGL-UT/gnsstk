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
#include <bitset>

#include "DumpDetail.hpp"
#include "GPSLNavNMCT.hpp"
#include "GPSNMCTAI.hpp"  // asString
#include "GPSWeekSecond.hpp"
#include "TimeString.hpp" // printTime

namespace gnsstk
{
   GPSLNavNMCT ::
   GPSLNavNMCT()
   {
         // For ease, the transmit time should be the start of the subframe
         // containing the NMCT and the message length is just the length of
         // subframe, in seconds.
      msgLenSec = 6.0;
   }


   bool GPSLNavNMCT ::
   validate() const
   {
         // IS-GPS-200N 20.3.3.4.4 NMCT Validity Time - "If the AODO term is 27900 seconds 
         // (i.e., binary 11111), then the NMCT currently available from the transmitting SV
         // is invalid and shall not be used."
      if (aodo < 0 || aodo >= 27900)
      {
         return false;
      }

         // IS-GPS-200N 20.3.3.5.1.9 NMCT
         // There are only 30 slots for ERDs and the an SV should not be
         // transmitting an ERD for it's own PRN.
         // PRN 32 is not a valid PRN for NMCT
      if (erds.empty() || 
          (erds.size() > 30) ||
          (erds.count(signal.xmitSat.id) != 0) ||
          (erds.begin()->first != 1 && erds.begin()->first != 2) ||
          (erds.rbegin()->first != 31 && erds.rbegin()->first != 30))
      {
         return false;
      }

      return true;
   }

   bool GPSLNavNMCT ::
   getERD(unsigned prn, double& erd) const
   {
      erd = std::numeric_limits<double>::quiet_NaN();
      if (aodo == 27900 ||
         availabilityIndicator == GPSNMCTAI::NotAvailable ||
         erds.count(prn) == 0)
      {
         return false;
      }

      unsigned erdBits = erds.at(prn);
      
         // 20.3.3.5.1.9 NMCT - "A binary value of “100000” shall indicate 
         // that no valid ERD for the corresponding SV ID is present in that slot."
         // Literal binary value would be easier but unfortunately a C++14 feature.
      if (erdBits == 040)
      {
         return false;
      }

         // Unpacking into signed value. Stolen from PackedNavBits.
         // Seems like there are no general PNB functions that don't require
         // the PNB data structure.
      union
      {
         uint64_t u;
         int64_t s;
      };
      u = erdBits;
      s <<= 58;
      s >>= 58;
      
      erd = s * (3.0 / 10.0);

      return true;
   }


   // std::string GPSLNavNMCT ::
   // getTerseHeader() const
   // {
   //    return "SVN  PRN     Toe          Tnmct       AODO     Availability";
   // }


   void GPSLNavNMCT ::
   dump(std::ostream &s, DumpDetail dl) const
   {
      const std::ios::fmtflags oldFlags{s.flags()};
      s.setf(std::ios::fixed, std::ios::floatfield);
      s.setf(std::ios::right, std::ios::adjustfield);
      s.setf(std::ios::uppercase);
      s.precision(0);
      s.fill(' ');

      std::string svn("0");
      std::string tform = "%3j %02H:%02M:%02S";
      switch(dl)
      {
      case DumpDetail::OneLine:
         s << "NMCT ";
         NavData::dump(s, dl);
         break;
      case DumpDetail::Terse:

         s.setf(std::ios::fixed, std::ios::floatfield);
         s.setf(std::ios::right, std::ios::adjustfield);
         s.setf(std::ios::uppercase);
         s.precision(0);
         s.fill(' ');

         if (getSVN(signal.xmitSat, Toe, svn))
         {
            s << std::setw(2) << " " << svn << "  ";
         }
         else
         {
            s << "  XX  ";
         }

         s << std::setw(2) << signal.xmitSat.id << " ! ";

         s << printTime( Toe, tform ) << " ! ";
         s << printTime( Tnmct, tform ) << " !  ";
         s << std::setw(4) << std::setprecision(1) << aodo << "  ! ";
         s << StringUtils::asString(availabilityIndicator) << " ! " << '\n';
         break;
      case DumpDetail::Brief:
         s << "NMCT ";
         NavData::dump(s, dl);
         s << "Toe: " << getDumpTime(dl, Toe) << ", AODO: " << aodo
            << ", Tnmct: " << getDumpTime(dl, Tnmct) << ", Availiability: " 
            << StringUtils::asString(availabilityIndicator) << '\n';
         s.setf(std::ios::scientific, std::ios::floatfield);
         s.precision(2);
         s.fill(' ');
         for (const auto& kv : erds)
         {
            s << kv.first << "/" << std::bitset<6>(kv.second) << "/";
            double erd;
            if (getERD(kv.first, erd))
            {
               s << std::setw(3) << erd;
            }
            else
            {
               s << "Invalid";
            }
            s << " ";
         }
         break;
      case DumpDetail::Full:
         s << "****************************************************************************\n"
           << "NMCT ERD\n\n"
           << getSignalString() << '\n'
           << "TIMES OF INTEREST\n\n"
           << "           " << getDumpTimeHdr(dl) << '\n'
           << "Transmit (SF4PG13):     " << getDumpTime(dl, timeStamp) << '\n'
           << "Toe (SF1):              " << getDumpTime(dl, Toe) << '\n'
           << "Tnmct:                  " << getDumpTime(dl, Tnmct) << "\n\n"
           << "AODO (SF2):             " << aodo << '\n'
           << "Availability Indicator: " << StringUtils::asString(availabilityIndicator) << '\n';
         s.setf(std::ios::scientific, std::ios::floatfield);
         s.precision(8);
         s.fill(' ');
         for (const auto& kv : erds)
         {
            s << "PRN: " << std::setw(2) << kv.first
               << ", ERD (binary/decimal): " << std::bitset<6>(kv.second) << "/"; 
            double erd;
            if (getERD(kv.first, erd))
            {
               s << std::setw(16) << erd;
            }
            else
            {
               s << "Invalid";
            }
            s << '\n';
         }
      }
      s.flags(oldFlags);
   }


   bool GPSLNavNMCT::
   isSameData(const NavDataPtr& right, bool ignore_timestamp) const
   {
      const std::shared_ptr<GPSLNavNMCT> nmct = std::dynamic_pointer_cast<GPSLNavNMCT>(right);
      
      if (!nmct)
      {
         return false;
      }
      return (NavData::isSameData(right, ignore_timestamp) &&
         (aodo == nmct->aodo) &&
         (Toe == nmct->Toe) &&
         (availabilityIndicator == nmct->availabilityIndicator) &&
         (Tnmct == nmct->Tnmct) &&
         (erds == nmct->erds));
   }


   void GPSLNavNMCT::updateTNMCT()
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
