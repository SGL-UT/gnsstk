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
#include <math.h>
#include "InterSigCorr.hpp"
#include "FreqConv.hpp"

using namespace std;

namespace gpstk
{
   InterSigCorr ::
   InterSigCorr()
         : isc(std::numeric_limits<double>::quiet_NaN())
   {
      signal.messageType = NavMessageType::ISC;
   }


   bool InterSigCorr ::
   getISC(const ObsID& oid, double& corr)
      const
   {
      ObsID oidU(oid);
      oidU.type = ObservationType::Unknown;
         // it's possible to have an empty validOids and still be
         // useful, but it is not possible to have an empty refOids or
         // isc=NaN and still be useful.
      if (isnan(isc) || refOids.empty())
         return false;
      if (refOids.count(oidU))
      {
         corr = -isc;
      }
      else if (validOids.count(oidU))
      {
         corr = -(getGamma(refOids.begin()->band, oidU.band) * isc);
      }
      else
      {
         return false;
      }
      return true;
   }


   bool InterSigCorr ::
   getISC(const ObsID& oid1, const ObsID& oid2, double& corr)
      const
   {
      corr = 0;
      return true;
   }
}
