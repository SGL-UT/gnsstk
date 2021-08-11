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
#include <cmath>
#include "BDSD1NavISC.hpp"
#include "FreqConv.hpp"
#include "BDSconsts.hpp"

using namespace std;

namespace gpstk
{
   BDSD1NavISC ::
   BDSD1NavISC()
         : pre(0),
           rev(0),
           fraID(0),
           sow(0),
           tgd1(std::numeric_limits<double>::quiet_NaN()),
           tgd2(std::numeric_limits<double>::quiet_NaN())
   {
      weekFmt = "%4D(%4e)";
      msgLenSec = 6.0;
   }


   bool BDSD1NavISC ::
   validate() const
   {
      return (!(std::isnan(tgd1) || std::isnan(tgd2)) &&
              ((pre == 0) || (pre == bds::Preamble)) &&
              (fraID == 1));
   }


   bool BDSD1NavISC ::
   getISC(const ObsID& oid, double& corrOut)
      const
   {
      if ((oid.band == CarrierBand::B1) && (oid.code == TrackingCode::B1I))
      {
         if (std::isnan(tgd1))
            return false;
         corrOut = -tgd1;
         return true;
      }
      else if ((oid.band == CarrierBand::B2) && (oid.code == TrackingCode::B2I))
      {
         if (std::isnan(tgd2))
            return false;
         corrOut = -tgd2;
         return true;
      }
      return false;
   }


   bool BDSD1NavISC ::
   getISC(const ObsID& oid1, const ObsID& oid2, double& corrOut)
      const
   {
         // This is the only code combination documented in ICD-B3I,
         // other code ICDs may include more.
      if ((oid1.band == CarrierBand::B1) && (oid1.code == TrackingCode::B1I) &&
          (oid2.band == CarrierBand::B3) && (oid2.code == TrackingCode::B3I))
      {
         double gamma13 = getGamma(oid1.band,oid2.band);
         corrOut = (gamma13 * tgd1) / (1-gamma13);
         return true;
      }
      return false;
   }
}
