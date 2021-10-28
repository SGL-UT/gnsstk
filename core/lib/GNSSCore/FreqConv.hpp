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

#ifndef GNSSTK_FREQCONV_HPP
#define GNSSTK_FREQCONV_HPP

#include "FreqConsts.hpp"
#include "CarrierBand.hpp"

namespace gnsstk
{
      /** Get the carrier frequency in Hz for a given carrier band enumeration.
       * @param[in] band The carrier band whose carrier frequency is needed.
       * @return The carrier frequency, or 0 if not known. */
   double getFrequency(CarrierBand band);

      /** Get the gamma value relative to two carrier bands, which is
       * (f1/f2)^2.
       * @param[in] band1 The primary carrier band in a dual-frequency
       *   observation.
       * @param[in] band2 The secondary carrier band in a dual-frequency
       *   observation.
       * @return The nominal center frequency, gamma, between band1 and band2.
       */
   double getGamma(CarrierBand band1, CarrierBand band2);
} // namespace gnsstk

#endif // GNSSTK_FREQCONV_HPP
