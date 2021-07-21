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

#include "FreqConv.hpp"

namespace gpstk
{
   double getFrequency(CarrierBand band)
   {
      switch (band)
      {
         case CarrierBand::L1:    return FREQ_GPS_L1;
         case CarrierBand::L2:    return FREQ_GPS_L2;
         case CarrierBand::L5:    return FREQ_GPS_L5;
         case CarrierBand::G1:    return FREQ_GLONASS_G1;
         case CarrierBand::G1a:   return FREQ_GLONASS_G1a;
         case CarrierBand::G2a:   return FREQ_GLONASS_G2a;
         case CarrierBand::G2:    return FREQ_GLONASS_G2;
         case CarrierBand::G3:    return FREQ_GLONASS_G3;
         case CarrierBand::E5b:   return FREQ_GALILEO_E5b;
         case CarrierBand::E5ab:  return FREQ_GALILEO_E5;
         case CarrierBand::E6:    return FREQ_GALILEO_E6;
         case CarrierBand::B1:    return FREQ_BEIDOU_B1;
         case CarrierBand::B2:    return FREQ_BEIDOU_B2b;
         case CarrierBand::B3:    return FREQ_BEIDOU_B3;
         case CarrierBand::I9:    return FREQ_NAVIC_S;
         default:                 return 0.0;
      }
   }


   double getGamma(CarrierBand band1, CarrierBand band2)
   {
      double freq1 = getFrequency(band1);
      double freq2 = getFrequency(band2);
      double gammaRoot = freq1/freq2;
      return gammaRoot * gammaRoot;
   }
} // namespace gpstk
