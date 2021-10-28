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
#include "GPSCNavISC.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "FreqConv.hpp"

using namespace std;

namespace gnsstk
{
   GPSCNavISC ::
   GPSCNavISC()
         : pre(0),
           alert(false),
           iscL1CA(std::numeric_limits<double>::quiet_NaN()),
           iscL2C(std::numeric_limits<double>::quiet_NaN()),
           iscL5I5(std::numeric_limits<double>::quiet_NaN()),
           iscL5Q5(std::numeric_limits<double>::quiet_NaN())
   {
         // ignore refOids/validOids as they're not used
   }


   bool GPSCNavISC ::
   validate() const
   {
      return ((pre == 0) || (pre == 0x8b));
   }


   CommonTime GPSCNavISC ::
   getUserTime() const
   {
      if (signal.nav == NavType::GPSCNAVL2)
         return timeStamp + 12.0;
      return timeStamp + 6.0;
   }


   bool GPSCNavISC ::
   getISC(const ObsID& oid, double& corr) const
   {
         // reminder: InterSigCorr::isc is used to store Tgd
      switch (oid.band)
      {
         case CarrierBand::L1:
            if (oid.code == TrackingCode::CA)
            {
               corr = iscL1CA - isc;
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


   bool GPSCNavISC ::
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
      else if ((oid1.band == CarrierBand::L1) &&
               (oid1.code == TrackingCode::CA) &&
               (oid2.band == CarrierBand::L2) &&
               ((oid2.code == TrackingCode::L2CM) ||
                (oid2.code == TrackingCode::L2CL) ||
                (oid2.code == TrackingCode::L2CML)))
      {
            // per IS-GPS-200 30.3.3.3.1.1.2
         double gamma12 = getGamma(oid1.band,oid2.band);
         corr = ((iscL2C - (gamma12 * iscL1CA)) / (1-gamma12)) - isc;
         return true;
      }
      return false;
   }
}
