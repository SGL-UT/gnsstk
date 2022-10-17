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
#ifndef GNSSTK_GLOCNAVALMUNCORRECTED_CPP
#define GNSSTK_GLOCNAVALMUNCORRECTED_CPP

/** @file GLOCNavAlmUncorrected.cpp Define inline methods for nested private
 * classes of GLOCNavAlm */

#include "DebugTrace.hpp"

namespace gnsstk
{
   inline void GLOCNavAlm::Uncorrected ::
   setData(const GLOCNavAlm& alm, double Tdr, double n, double Deltatpr)
   {
      seti(alm.deltai);
      seta(alm.ecc, alm.omega, Tdr);
      setp(alm.ecc);
      setlambda(alm.lambda, n, Deltatpr);
      setomega(alm.omega, n, Deltatpr);
      sethl(alm.ecc);
   }


   inline void GLOCNavAlm::Uncorrected ::
   seti(double DeltaiA)
   {
      i = ((iav / 180.0) + DeltaiA) * gnsstk::PI;
   }


   inline void GLOCNavAlm::Uncorrected ::
   seta(double epsilonA, double omegaA, double Tdr)
   {
         // approximate semi-major axis
      double Tosc = Tdr;
      double am = -9, am1 = 9, pm1 = 0;
      double ecc2obv = (1.0 - (epsilonA*epsilonA));
      double ecc2obvTH = sqrt(ecc2obv*ecc2obv*ecc2obv);
      unsigned m = 0;
         // 1e-5 = 1cm in km, as specified in ICD
      while (fabs(am1-am) > 1e-5)
      {
         double f1 = Tosc / (2.0 * gnsstk::PI);
         am = am1;
         am1 = std::cbrt(f1 * f1 * GM);
         pm1 = am1 * ecc2obv;
         double top1 = 1 + (epsilonA * cos(omegaA * gnsstk::PI));
         double term1 = top1 * top1 * top1 / ecc2obv;
         double term2 = ecc2obvTH / (top1 * top1);
         double term3 = 2.0 - ((5.0/2.0) * sin(i)*sin(i));
         double term321 = (term3*term2) + term1;
         double term4a = ae / pm1;
         double term4 = (3.0/2.0) * J20 * term4a * term4a;
         double term4321 = term4 * term321;
         Tosc = Tdr / (1.0-term4321);
         // DEBUGTRACE(std::setw(1) << m << " | " << std::setw(18)
         //            << std::setprecision(12) << std::fixed << am1 << " | "
         //            << pm1 << " | " << Tosc);
         m++;
      }
      Perturbations::a = am1;
   }


   inline void GLOCNavAlm::Uncorrected ::
   setp(double epsilonA)
   {
      p = a * (1.0 - epsilonA*epsilonA);
   }


   inline void GLOCNavAlm::Uncorrected ::
   sethl(double epsilonA)
   {
      h = epsilonA * sin(omega);
      l = epsilonA * cos(omega);
   }


   inline void GLOCNavAlm::Uncorrected ::
   setlambda(double lambdaA, double n, double Deltatpr)
   {
      double ev = ae / p;
      double lambdaTerm = ell.angVelocity() + (3.0/2.0)*J20*n*ev*ev*cos(i);
      Perturbations::lambda = (lambdaA * gnsstk::PI) - (lambdaTerm * Deltatpr);
      // DEBUGTRACE("lambda ae = " << std::scientific << ae);
      // DEBUGTRACE("lambda p = " << std::scientific << p);
      // DEBUGTRACE("lambda ev = " << std::scientific << ev);
      // DEBUGTRACE("lambda_term = " << std::scientific << lambdaTerm);
      // DEBUGTRACE("lambda = " << lambda);
   }


   inline void GLOCNavAlm::Uncorrected ::
   setomega(double omegaA, double n, double Deltatpr)
   {
      double ev = ae / p;
      double omegaTerm = (3.0/4.0)*J20*n*ev*ev*(1.0-(5.0*cos(i)*cos(i)));
      Perturbations::omega = (omegaA * gnsstk::PI) - (omegaTerm * Deltatpr);
   }
} // namespace gnsstk

#endif //GNSSTK_GLOCNAVALMUNCORRECTED_CPP

