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
#include "GPSCNav2ISC.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "FreqConv.hpp"

namespace gnsstk
{
   GPSCNav2ISC ::
   GPSCNav2ISC()
         : haveSF2(false),
           haveSF3(false),
           iscL1CP(std::numeric_limits<double>::quiet_NaN()),
           iscL1CD(std::numeric_limits<double>::quiet_NaN()),
           iscL1CA(std::numeric_limits<double>::quiet_NaN()),
           iscL2C(std::numeric_limits<double>::quiet_NaN()),
           iscL5I5(std::numeric_limits<double>::quiet_NaN()),
           iscL5Q5(std::numeric_limits<double>::quiet_NaN())
   {
         // ignore refOids/validOids as they're not used
   }


   void GPSCNav2ISC ::
   dumpCorrections(std::ostream& s) const
   {
      using namespace std;
      const ios::fmtflags oldFlags = s.flags();
      s << "           CORRECTION"
        << endl << endl
        << scientific << setprecision(8) << setfill(' ')
        << setw(20) << left << "Tgd:" << setw(15) << isc << endl
        << setw(20) << left << "ISC_L1CP:" << setw(15) << iscL1CP << endl
        << setw(20) << left << "ISC_L1CD:" << setw(15) << iscL1CD << endl
        << setw(20) << left << "ISC_L1CA:" << setw(15) << iscL1CA << endl
        << setw(20) << left << "ISC_L2C:" << setw(15) << iscL2C << endl
        << setw(20) << left << "ISC_L5I5:" << setw(15) << iscL5I5 << endl
        << setw(20) << left << "ISC_L5Q5:" << setw(15) << iscL5Q5 << endl;
      s.flags(oldFlags);
   }


   CommonTime GPSCNav2ISC ::
   getUserTime() const
   {
         // subframe 2 = 12 seconds
         // subframe 3 = 5.48 seconds
      return std::max(xmit2+12, xmit3+5.48);
   }


   bool GPSCNav2ISC ::
   validate() const
   {
         /// @todo implement some checks.
      return true;
   }


   bool GPSCNav2ISC ::
   getISC(const ObsID& oid, double& corr) const
   {
         // reminder: InterSigCorr::isc is used to store Tgd
      switch (oid.band)
      {
         case CarrierBand::L1:
            switch (oid.code)
            {
               case TrackingCode::CA:
                  corr = iscL1CA - isc;
                  return true;
               case TrackingCode::L1CP:
                  corr = iscL1CP - isc;
                  return true;
               case TrackingCode::L1CD:
                  corr = iscL1CD - isc;
                  return true;
            }
            break;
         case CarrierBand::L2:
            switch (oid.code)
            {
               case TrackingCode::L2CM:
               case TrackingCode::L2CL:
               case TrackingCode::L2CML:
                  corr = iscL2C - isc;
                  return true;
            }
            break;
         case CarrierBand::L5:
               // note this is only described in IS-GPS-705
            switch (oid.code)
            {
               case TrackingCode::L5I:
                  corr = iscL5I5 - isc;
                  return true;
               case TrackingCode::L5Q:
                  corr = iscL5Q5 - isc;
                  return true;
            }
            break;
      }
      return false;
   }


   bool GPSCNav2ISC ::
   getISC(const ObsID& oid1, const ObsID& oid2, double& corr)
      const
   {
         // reminder: InterSigCorr::isc is used to store Tgd
      if ((oid1.band == CarrierBand::L1) &&
          (oid1.code == TrackingCode::CA) &&
          (oid2.band == CarrierBand::L5))
      {
            // per IS-GPS-705 20.3.3.3.1.2.2
         double gamma15 = getGamma(oid1.band,oid2.band);
         if (oid2.code == TrackingCode::L5I)
         {
            corr = ((iscL5I5 - (gamma15 * iscL1CA)) / (1-gamma15)) - isc;
            return true;
         }
         else if (oid2.code == TrackingCode::L5Q)
         {
            corr = ((iscL5Q5 - (gamma15 * iscL1CA)) / (1-gamma15)) - isc;
            return true;
         }
      }
      else if ((oid2.band == CarrierBand::L2) &&
               ((oid2.code == TrackingCode::L2CM) ||
                (oid2.code == TrackingCode::L2CL) ||
                (oid2.code == TrackingCode::L2CML)))
      {
         double gamma12 = getGamma(oid1.band,oid2.band);
         if ((oid1.band == CarrierBand::L1) &&
             (oid1.code == TrackingCode::CA))
         {
               // per IS-GPS-200 30.3.3.3.1.1.2
            corr = ((iscL2C - (gamma12 * iscL1CA)) / (1-gamma12)) - isc;
            return true;
         }
         else if ((oid1.band == CarrierBand::L1) &&
                  (oid1.code == TrackingCode::L1CP))
         {
               // per IS-GPS-800 3.5.3.9.2
            corr = ((iscL2C - (gamma12 * iscL1CP)) / (1-gamma12)) - isc;
            return true;
         }
         else if ((oid1.band == CarrierBand::L1) &&
                  (oid1.code == TrackingCode::L1CD))
         {
               // per IS-GPS-800 3.5.3.9.2
            corr = ((iscL2C - (gamma12 * iscL1CD)) / (1-gamma12)) - isc;
            return true;
         }
      }
      return false;
   }
}
