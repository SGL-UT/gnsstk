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

#include "HelmertTransformer.hpp"

namespace gnsstk
{
   HelmertTransformer ::
   HelmertTransformer()
         : scale(std::numeric_limits<double>::quiet_NaN()),
           description("Undefined")
   {
         // rotation, translation and the RefFrame objects we just use
         // their default constructors.
   }


   HelmertTransformer ::
   HelmertTransformer(const RefFrame& from, const RefFrame& to,
                      double irx, double iry, double irz,
                      double itx, double ity, double itz,
                      double sc, const std::string& desc,
                      const CommonTime& refEpoch)
         : description(desc),
           scale(sc)
   {
      if ((from.getRealization() == RefFrameRlz::Unknown) ||
          (to.getRealization() == RefFrameRlz::Unknown))
      {
         InvalidRequest e("Invalid Helmert transformation with Unknown frame");
         GNSSTK_THROW(e);
      }

      fromFrame = from;
      toFrame = to;
      epoch = refEpoch;

      double rx = irx*DEG_TO_RAD;
      double ry = iry*DEG_TO_RAD;
      double rz = irz*DEG_TO_RAD;

         // check that rotation angles are small;
         // sin x ~ x at 0.244 radians = 13.9 deg
      if ((::fabs(rx) > 1.e-3) || (::fabs(ry) > 1.e-3) || (::fabs(rz) > 1.e-3))
      {
         InvalidRequest e("Invalid Helmert transformation : "
                          "small angle approximation.");
         GNSSTK_THROW(e);
      }

         /** @note small angle approximation is used. */
         /** @note by construction, transpose(rotation) == inverse(rotation)
          * (given small angle approximation). */
      rotation = Matrix<double>(3,3,0.0);
      rotation(0,0) = 1.0;
      rotation(0,1) = -rz;
      rotation(0,2) = ry;

      rotation(1,0) = rz;
      rotation(1,1) = 1.0;
      rotation(1,2) = -rx;

      rotation(2,0) = -ry;
      rotation(2,1) = rx;
      rotation(2,2) = 1.0;

         // translation vector
      translation = Vector<double>(3);
      translation(0) = itx;
      translation(1) = ity;
      translation(2) = itz;
   }


   bool HelmertTransformer ::
   transform(const Position& fromPos, Position& toPos)
      const noexcept
   {
      if ((fromPos.getReferenceFrame() == fromFrame) &&
          (toPos.getReferenceFrame() == toFrame))
      {
            // transform
         toPos = fromPos;
         toPos.transformTo(Position::Cartesian);
         Vector<double> vec(3),res(3);
         vec(0) = toPos[0];
         vec(1) = toPos[1];
         vec(2) = toPos[2];
         res = rotation*vec + scale*vec + translation;
         toPos[0] = res(0);
         toPos[1] = res(1);
         toPos[2] = res(2);
         toPos.setReferenceFrame(toFrame);
         return true;
      }
      else if ((fromPos.getReferenceFrame() == toFrame) &&
               (toPos.getReferenceFrame() == fromFrame))
      {
            // inverse transform
         toPos = fromPos;
         toPos.transformTo(Position::Cartesian);
         Vector<double> vec(3),res(3);
         vec(0) = toPos[0];
         vec(1) = toPos[1];
         vec(2) = toPos[2];
         res = transpose(rotation) * (vec - scale*vec - translation);
         toPos[0] = res(0);
         toPos[1] = res(1);
         toPos[2] = res(2);
         toPos.setReferenceFrame(fromFrame);
         return true;
      }
      return false;
   }


   bool HelmertTransformer ::
   transform(const Xvt& fromPos, Xvt& toPos)
      const noexcept
   {
      Position pos(fromPos.x, Position::Cartesian), res;
      pos.setReferenceFrame(fromPos.frame);
      res.setReferenceFrame(toPos.frame);
      if (!transform(pos, res))
      {
         return false;
      }
      toPos = fromPos;
      toPos.x[0] = res[0];
      toPos.x[1] = res[1];
      toPos.x[2] = res[2];
      toPos.frame = res.getReferenceFrame();
      return true;
   }


   bool HelmertTransformer ::
   transform(const Vector<double>& fromPos,
             const RefFrame& srcFrame,
             Vector<double>& toPos)
      const noexcept
   {
      Position pos(fromPos[0],fromPos[1],fromPos[2],Position::Cartesian), res;
      pos.setReferenceFrame(srcFrame);
      if (srcFrame == fromFrame)
      {
         res.setReferenceFrame(toFrame);
      }
      else if (srcFrame == toFrame)
      {
         res.setReferenceFrame(fromFrame);
      }
      else
      {
         return false;
      }
      if (!transform(pos, res))
      {
         return false;
      }
      toPos = Vector<double>(3);
      toPos[0] = res[0];
      toPos[1] = res[1];
      toPos[2] = res[2];
      return true;
   }


   bool HelmertTransformer ::
   transform(const Triple& fromPos,
             const RefFrame& srcFrame,
             Triple& toPos)
      const noexcept
   {
      Position pos(fromPos, Position::Cartesian), res;
      pos.setReferenceFrame(srcFrame);
      if (srcFrame == fromFrame)
      {
         res.setReferenceFrame(toFrame);
      }
      else if (srcFrame == toFrame)
      {
         res.setReferenceFrame(fromFrame);
      }
      else
      {
         return false;
      }
      if (!transform(pos, res))
      {
         return false;
      }
      toPos[0] = res[0];
      toPos[1] = res[1];
      toPos[2] = res[2];
      return true;
   }


   bool HelmertTransformer ::
   transform(double fx, double fy, double fz,
             const RefFrame& srcFrame,
             double& tx, double& ty, double& tz)
      const noexcept
   {
      Position pos(fx,fy,fz,Position::Cartesian), res;
      pos.setReferenceFrame(srcFrame);
      if (srcFrame == fromFrame)
      {
         res.setReferenceFrame(toFrame);
      }
      else if (srcFrame == toFrame)
      {
         res.setReferenceFrame(fromFrame);
      }
      else
      {
         return false;
      }
      if (!transform(pos, res))
      {
         return false;
      }
      tx = res.X();
      ty = res.Y();
      tz = res.Z();
      return true;
   }

} // namespace gnsstk
