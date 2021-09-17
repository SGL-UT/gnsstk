//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#include "GPSLNavISC.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "FreqConv.hpp"

using namespace std;

namespace gnsstk
{
   GPSLNavISC ::
   GPSLNavISC()
         : pre(0),
           tlm(0),
           alert(false),
           asFlag(false)
   {
      msgLenSec = 6.0;
      iscLabel = "Tgd";
      refOids = {ObsID(ObservationType::Unknown,
                       gnsstk::CarrierBand::L1,
                       gnsstk::TrackingCode::CA),
                 ObsID(gnsstk::ObservationType::Unknown,
                       gnsstk::CarrierBand::L1,
                       gnsstk::TrackingCode::Y)};
      validOids = {ObsID(ObservationType::Unknown,
                         gnsstk::CarrierBand::L2,
                         gnsstk::TrackingCode::Y)};
   }


   bool GPSLNavISC ::
   validate() const
   {
      return ((pre == 0) || (pre == 0x8b));
   }
}
