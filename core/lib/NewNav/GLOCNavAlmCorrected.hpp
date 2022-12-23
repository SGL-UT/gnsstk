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
#ifndef GNSSTK_GLOCNAVALMCORRECTED_CPP
#define GNSSTK_GLOCNAVALMCORRECTED_CPP

/** @file GLOCNavAlmCorrected.cpp Define inline methods for nested private
 * classes of GLOCNavAlm */

#include "DebugTrace.hpp"

namespace gnsstk
{
   inline void GLOCNavAlm::Corrected ::
   setData(double L, const Uncorrected& uncor,
           const Deltas& delta1, const Deltas& delta2)
   {
      a = uncor.geta() + ((delta2.geta() - delta1.geta()) * uncor.geta());
      h = uncor.geth() + delta2.geth() - delta1.geth();
      l = uncor.getl() + delta2.getl() - delta1.getl();
      epsilon = std::sqrt(h*h+l*l);
      i = uncor.geti() + delta2.geti() - delta1.geti();
      lambda = uncor.getlambda() + delta2.getlambda() - delta1.getlambda();
      omega = atan2(h, l);
      Lk = L + delta2.getLk() - delta1.getLk();
      setp();
      setE(L);
      setnu();
      setu();
      setr();
      setvrvu();
   }


   inline Xvt GLOCNavAlm::Corrected ::
   getXvt(const gnsstk::CommonTime& toi)
   {
      Xvt rv;
      double xterm = cos(lambda)*cos(u) - sin(lambda)*sin(u)*cos(i);
      double yterm = sin(lambda)*cos(u) + cos(lambda)*sin(u)*cos(i);
      double zterm = sin(u) * sin(i);
      rv.x[0] = r * xterm;
      rv.x[1] = r * yterm;
      rv.x[2] = r * zterm;
      rv.v[0] = (vr * xterm) -
         (vu * (cos(lambda)*sin(u) + sin(lambda)*cos(u)*cos(i))) +
         (omegaE * rv.x[1]);
      rv.v[1] = (vr * yterm) -
         (vu * (sin(lambda)*sin(u) - cos(lambda)*cos(u)*cos(i))) -
         (omegaE * rv.x[0]);
      rv.v[2] = (vr * zterm) + (vu * cos(u) * sin(i));
         // change km to m
      rv.x[0] *= 1000.0;
      rv.x[1] *= 1000.0;
      rv.x[2] *= 1000.0;
      rv.v[0] *= 1000.0;
      rv.v[1] *= 1000.0;
      rv.v[2] *= 1000.0;
      rv.frame = RefFrame(RefFrameSys::PZ90, toi);
         // clock bias and drift are not available (?).
      rv.relcorr = rv.computeRelativityCorrection();
      return rv;
   }


   inline void GLOCNavAlm::Corrected ::
   setp()
   {
      p = a * (1.0 - epsilon*epsilon);
   }


   inline void GLOCNavAlm::Corrected ::
   setE(double L)
   {
      // DEBUGTRACE("Lprime=" << std::scientific << Lk);
      // DEBUGTRACE("omegaprime=" << std::scientific << omega);
      double Em1 = Lk - omega;
         // -9 just to make sure we get at least one iteration
      double Em = Em1 - 9;
      unsigned m = 0;
      while (fabs(Em1-Em) > 1e-9)
      {
         Em = Em1;
         Em1 = Lk - omega + (epsilon * sin(Em));
         // DEBUGTRACE(std::setw(1) << m << " | " << std::setw(18)
         //            << std::setprecision(12) << std::fixed << Em1);
         m++;
      }
      E = Em1;
   }


   inline void GLOCNavAlm::Corrected ::
   setnu()
   {
      double nu_top = 1.0 + epsilon;
      double nu_bot = 1.0 - epsilon;
      double nu_sqrt = sqrt(nu_top/nu_bot);
      double nu_tan = tan(E/2.0);
      double nu_tanned = nu_sqrt * nu_tan;
      double nu_angle = atan(nu_tanned);
      nu = 2.0 * nu_angle;
   }


   inline void GLOCNavAlm::Corrected ::
   setu()
   {
      u = nu + omega;
   }


   inline void GLOCNavAlm::Corrected ::
   setr()
   {
      r = p / (1.0 + (epsilon * cos(nu)));
   }


   inline void GLOCNavAlm::Corrected ::
   setvrvu()
   {
      double scale = sqrt(ell.gm_km() / p);
      vr = scale * epsilon * sin(nu);
      vu = scale * (1 + (epsilon * cos(nu)));
   }
} // namespace gnsstk

#endif //GNSSTK_GLOCNAVALMCORRECTED_CPP

