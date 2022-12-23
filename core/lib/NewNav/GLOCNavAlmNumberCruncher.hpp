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
#ifndef GNSSTK_GLOCNAVALMNUMBERCRUNCHER_CPP
#define GNSSTK_GLOCNAVALMNUMBERCRUNCHER_CPP

/** @file GLOCNavAlmNumberCruncher.cpp Define inline methods for nested private
 * classes of GLOCNavAlm */

#include "DebugTrace.hpp"

namespace gnsstk
{
   inline Xvt GLOCNavAlm::NumberCruncher ::
   getXvt(const GLOCNavAlm& alm,
          const gnsstk::CommonTime& toi)
   {
      setDeltatpr(alm.Toa, toi);
      setW(alm.deltaT);
      setTdr(alm.deltaT, alm.deltaTdot);
      setn();
      uncorrected.setData(alm, Tdr, n, Deltatpr);
      setE0(alm.ecc);
      setL1(alm.ecc);
      setL(alm.deltaT, alm.deltaTdot);
      setB();
      k1.setData(B, L1, uncorrected);
      k2.setData(B, L, uncorrected);
      corrected.setData(L, uncorrected, k1, k2);
      return corrected.getXvt(toi);
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setDeltatpr(const gnsstk::CommonTime& almTime, const gnsstk::CommonTime& ti)
   {
      Deltatpr = ti - almTime;
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setW(double DeltaTA)
   {
      W = Deltatpr / (Tav + DeltaTA);
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setTdr(double DeltaTA, double DeltaTdotA)
   {
      Tdr = Tav + DeltaTA + (2*W + 1) * DeltaTdotA;
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setn()
   {
      n = (2.0*gnsstk::PI)/Tdr;
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setE0(double epsilonA)
   {
      double E0Term1 = (1.0 - epsilonA) / (1.0 + epsilonA);
      double E0Term2 = sqrt(E0Term1) * tan(uncorrected.getomega() / 2.0);
      E0 = -2.0 * atan(E0Term2);
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setL1(double epsilonA)
   {
      L1 = uncorrected.getomega() + E0 - (epsilonA * sin(E0));
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setL(double DeltaTA, double DeltaTdotA)
   {
      L = L1 + n * (Deltatpr - ((Tav+DeltaTA)*W) - (DeltaTdotA*W*W));
   }


   inline void GLOCNavAlm::NumberCruncher ::
   setB()
   {
      B = ((3.0 * J20 * ae * ae) /
           (2.0 * uncorrected.geta() * uncorrected.geta()));
   }
} // namespace gnsstk

#endif //GNSSTK_GLOCNAVALMNUMBERCRUNCHER_CPP

