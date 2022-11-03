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
#ifndef GNSSTK_GLOCNAVALMDELTAS_CPP
#define GNSSTK_GLOCNAVALMDELTAS_CPP

/** @file GLOCNavAlmDeltas.cpp Define inline methods for nested private
 * classes of GLOCNavAlm */

#include "DebugTrace.hpp"

namespace gnsstk
{
   inline void GLOCNavAlm::Deltas ::
   setData(double B, double Lk, const Uncorrected& uncor)
   {
      setdeltaa_a(B, Lk, uncor);
      setdeltah(B, Lk, uncor);
      setdeltal(B, Lk, uncor);
      setdeltalambda(B, Lk, uncor);
      setdeltai(B, Lk, uncor);
      setdeltaLk(B, Lk, uncor);
   }


   inline void GLOCNavAlm::Deltas ::
   setdeltaa_a(double B, double Lk, const Uncorrected& uncor)
   {
      Perturbations::a =
         2.0 * B * (1.0 - ((3.0/2.0)*sin(uncor.geti())*sin(uncor.geti())))*(uncor.getl()*cos(Lk)+uncor.geth()*sin(Lk))
         + (B * sin(uncor.geti())*sin(uncor.geti())
            * ((1.0/2.0)*uncor.geth()*sin(Lk)-((1.0/2.0)*uncor.getl()*cos(Lk))+cos(2.0*Lk)+
               ((7.0/2.0)*uncor.getl()*cos(3.0*Lk))+((7.0/2.0)*uncor.geth()*sin(3.0*Lk))));
   }


   inline void GLOCNavAlm::Deltas ::
   setdeltah(double B, double Lk, const Uncorrected& uncor)
   {
      Perturbations::h = B * (1.0 - ((3.0/2.0)*sin(uncor.geti())*sin(uncor.geti()))) *
         (sin(Lk) + ((3.0/2.0)*uncor.getl()*sin(2.0*Lk)) - ((3.0/2.0)*uncor.geth()*cos(2.0*Lk)))
         - ((1.0/4.0)*B*sin(uncor.geti())*sin(uncor.geti()) *
            (sin(Lk)
             -((7.0/3.0)*sin(3.0*Lk))
             +(5.0*uncor.getl()*sin(2.0*Lk))
             - ((17.0/2.0)*uncor.getl()*sin(4.0*Lk))
             + ((17.0/2.0)*uncor.geth()*cos(4.0*Lk))
             + uncor.geth()*cos(2.0*Lk)))
         - ((1.0/2.0)*B*cos(uncor.geti())*cos(uncor.geti())*uncor.getl()*sin(2.0*Lk));
   }


   inline void GLOCNavAlm::Deltas ::
   setdeltal(double B, double Lk, const Uncorrected& uncor)
   {
      Perturbations::l = B * (1.0 - ((3.0/2.0)*sin(uncor.geti())*sin(uncor.geti()))) *
         (cos(Lk) + ((3.0/2.0)*uncor.getl()*cos(2.0*Lk)) + ((3.0/2.0)*uncor.geth()*sin(2.0*Lk)))
         - ((1.0/4.0)*B*sin(uncor.geti())*sin(uncor.geti()) *
            (-cos(Lk)
             -((7.0/3.0)*cos(3.0*Lk))
             -(5.0*uncor.geth()*sin(2.0*Lk))
             -((17.0/2.0)*uncor.getl()*cos(4.0*Lk))
             -((17.0/2.0)*uncor.geth()*sin(4.0*Lk))
             +(uncor.getl()*cos(2.0*Lk))))
         + ((1.0/2.0)*B*cos(uncor.geti())*cos(uncor.geti())*uncor.geth()*sin(2.0*Lk));
   }


   inline void GLOCNavAlm::Deltas ::
   setdeltalambda(double B, double Lk, const Uncorrected& uncor)
   {
      Perturbations::lambda = -B * cos(uncor.geti()) *
         (((7.0/2.0)*uncor.getl()*sin(Lk))
          -((5.0/2.0)*uncor.geth()*cos(Lk))
          -((1.0/2.0)*sin(2.0*Lk))
          -((7.0/6.0)*uncor.getl()*sin(3.0*Lk))
          +((7.0/6.0)*uncor.geth()*cos(3.0*Lk)));
   }


   inline void GLOCNavAlm::Deltas ::
   setdeltai(double B, double Lk, const Uncorrected& uncor)
   {
      Perturbations::i = (1.0/2.0) * B * sin(uncor.geti()) * cos(uncor.geti()) *
         ((-uncor.getl()*cos(Lk))
          +(uncor.geth()*sin(Lk))
          +(cos(2.0*Lk))
          +((7.0/3.0)*uncor.getl()*cos(3.0*Lk))
          +((7.0/3.0)*uncor.geth()*sin(3.0*Lk)));
   }


   inline void GLOCNavAlm::Deltas ::
   setdeltaLk(double B, double Lk, const Uncorrected& uncor)
   {
      Perturbations::Lk = (2.0 * B * (1.0 - ((3.0/2.0)*sin(uncor.geti())*sin(uncor.geti()))) *
                 (((7.0/4.0)*uncor.getl()*sin(Lk)) - ((7.0/4.0)*uncor.geth()*cos(Lk))))
         +((3.0*B*sin(uncor.geti())*sin(uncor.geti())) *
           ((-(7.0/24.0)*uncor.geth()*cos(Lk))
            -((7.0/24.0)*uncor.getl()*sin(Lk))
            -((49.0/72.0)*uncor.geth()*cos(3.0*Lk))
            +((49.0/72.0)*uncor.getl()*sin(3.0*Lk))
            +((1.0/4.0)*sin(2.0*Lk))))
         +((B*cos(uncor.geti())*cos(uncor.geti())) *
           (((7.0/2.0)*uncor.getl()*sin(Lk))
            -((5.0/2.0)*uncor.geth()*cos(Lk))
            -((1.0/2.0)*sin(2.0*Lk))
            -((7.0/6.0)*uncor.getl()*sin(3.0*Lk))
            +((7.0/6.0)*uncor.geth()*cos(3.0*Lk))));
   }
} // namespace gnsstk

#endif //GNSSTK_GLOCNAVALMDELTAS_CPP

