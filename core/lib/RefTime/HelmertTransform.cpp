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

#include <ostream>
#include <iomanip>

#include "HelmertTransform.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "GNSSconstants.hpp"

using namespace gnsstk;
using namespace std;

namespace gnsstk
{
   HelmertTransform ::
   HelmertTransform() noexcept
         : fromFrame(ReferenceFrame::Unknown),
           toFrame(ReferenceFrame::Unknown),
           rx(std::numeric_limits<double>::quiet_NaN()),
           ry(std::numeric_limits<double>::quiet_NaN()),
           rz(std::numeric_limits<double>::quiet_NaN()),
           tx(std::numeric_limits<double>::quiet_NaN()),
           ty(std::numeric_limits<double>::quiet_NaN()),
           tz(std::numeric_limits<double>::quiet_NaN()),
           scale(std::numeric_limits<double>::quiet_NaN()),
           description("Undefined")
   {
   }

      // Explicit constructor, from the 7 parameters.
   HelmertTransform::HelmertTransform(
      ReferenceFrame from, ReferenceFrame to,
      double irx, double iry, double irz,
      double itx, double ity, double itz,
      double sc, const std::string& desc, const CommonTime& refEpoch)
         : rx(irx*DEG_TO_RAD), ry(iry*DEG_TO_RAD), rz(irz*DEG_TO_RAD),
           tx(itx), ty(ity), tz(itz), scale(sc), description(desc),
           epoch(refEpoch), fromFrame(from), toFrame(to)
   {
      if ((from == ReferenceFrame::Unknown) || (to == ReferenceFrame::Unknown))
      {
         InvalidRequest e("Invalid Helmert transformation with Unknown frame");
         GNSSTK_THROW(e);
      }

         // check that rotation angles are small;
         // sin x ~ x at 0.244 radians = 13.9 deg
      if ((::fabs(rx) > 1.e-3) || (::fabs(ry) > 1.e-3) || (::fabs(rz) > 1.e-3))
      {
         InvalidRequest e("Invalid Helmert transformation : "
                          "small angle approximation.");
         GNSSTK_THROW(e);
      }

         // rotation matrix.
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
      translation(0) = tx;
      translation(1) = ty;
      translation(2) = tz;
   }


      // Dump the object to a multi-line string including reference frames, the
      // 7 parameters and description.
   string HelmertTransform::asString() const noexcept
   {
      ostringstream oss;
      oss << "Helmert Transformation"
          << " from " << fromFrame
          << " to " << toFrame << ":\n"
          << scientific << setprecision(4)
          << "  Scale factor : " << scale
          << fixed << " = " << scale/PPB << " ppb" << endl
          << "  Rotation angles (deg):"
          << scientific
          << "  X : " << rx*RAD_TO_DEG
          << ",  Y : " << ry*RAD_TO_DEG
          << ",  Z : " << rz*RAD_TO_DEG << endl
          << "  Rotation angles (mas):"
          << fixed
          << "  X : " << rx*RAD_TO_DEG/DEG_PER_MAS
          << ",  Y : " << ry*RAD_TO_DEG/DEG_PER_MAS
          << ",  Z : " << rz*RAD_TO_DEG/DEG_PER_MAS << endl
          << "  Translation (meters):"
          << "  X : " << tx
          << ",  Y : " << ty
          << ",  Z : " << tz << endl
          << "  Beginning Epoch: "
          << (epoch == CommonTime::BEGINNING_OF_TIME ? string(" [all times]")
              : printTime(epoch,"%Y/%02m/%02d %2H:%02M:%06.3f = %F %.3g %P"))
          << endl
          << "  Description: " << description;
      return (oss.str());
   }


   void HelmertTransform::transform(const Position& pos, Position& result)
   {
      if (pos.getReferenceFrame() == fromFrame)
      {
            // transform
         result = pos;
         result.transformTo(Position::Cartesian);
         Vector<double> vec(3),res(3);
         vec(0) = result[0];
         vec(1) = result[1];
         vec(2) = result[2];
         res = rotation*vec + scale*vec + translation;
         result[0] = res(0);
         result[1] = res(1);
         result[2] = res(2);
         result.setReferenceFrame(RefFrame(toFrame, epoch));
      }
      else if (pos.getReferenceFrame() == toFrame)
      {
            // inverse transform
         result = pos;
         result.transformTo(Position::Cartesian);
         Vector<double> vec(3),res(3);
         vec(0) = result[0];
         vec(1) = result[1];
         vec(2) = result[2];
         res = transpose(rotation) * (vec - scale*vec - translation);
         result[0] = res(0);
         result[1] = res(1);
         result[2] = res(2);
         result.setReferenceFrame(RefFrame(fromFrame, epoch));
      }
      else
      {
         InvalidRequest e(
            "Helmert tranformation cannot act on frame " +
            gnsstk::StringUtils::asString(pos.getReferenceFrame()));
         GNSSTK_THROW(e);
      }
   }


   void HelmertTransform ::
   transform(const Vector<double>& vec, ReferenceFrame frame,
             Vector<double>& result)
   {
      if (vec.size() > 3)
      {
         InvalidRequest e("Input Vector is not of length 3");
         GNSSTK_THROW(e);
      }
      try
      {
         Position pos(vec[0],vec[1],vec[2],Position::Cartesian), res;
         pos.setReferenceFrame(RefFrame(frame, epoch));
         transform(pos, res);
         result = Vector<double>(3);
         result[0] = res.X();
         result[1] = res.Y();
         result[2] = res.Z();
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }


   void HelmertTransform ::
   transform(const Triple& vec, ReferenceFrame frame, Triple& result)
   {
      try
      {
         Position pos(vec, Position::Cartesian), res;
         pos.setReferenceFrame(RefFrame(frame, epoch));
         transform(pos, res);
         result[0] = res[0];
         result[1] = res[1];
         result[2] = res[2];
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }


   void HelmertTransform ::
   transform(const Xvt& xvt, Xvt& result)
   {
      try
      {
         Position pos(xvt.x, Position::Cartesian), res;
         pos.setReferenceFrame(xvt.frame);
         transform(pos, res);
         result = xvt;
         result.x[0] = res[0];
         result.x[1] = res[1];
         result.x[2] = res[2];
         result.frame = res.getReferenceFrame();
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }


   void HelmertTransform ::
   transform(double x, double y, double z,
             ReferenceFrame frame,
             double& rx, double& ry, double& rz)
   {
      try
      {
         Position pos(x,y,z,Position::Cartesian), res;
         pos.setReferenceFrame(RefFrame(frame, epoch));
         transform(pos, res);
         rx = res.X();
         ry = res.Y();
         rz = res.Z();
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }


      // time of PZ90 change
   const CommonTime HelmertTransform::PZ90Epoch(
      YDSTime(2007,263,61200.0,TimeSystem::UTC));
      //HelmertTransform::PZ90Epoch(2454364L,61200L,0.0,TimeSystem::UTC);

      // array of pre-defined HelmertTransforms
   const HelmertTransform HelmertTransform::stdTransforms[stdCount] =
   {
      HelmertTransform(
         ReferenceFrame::WGS84, ReferenceFrame::ITRF,
         0, 0, 0,  0.0, 0.0, 0.0,  0,
         "WGS84 to ITRF identity transform, a default value\n"
         "       (\"...since 1997, the WGS84 GPS broadcast ...\n"
         "       is consistent with the ITRS at better than 5-cm level.\"\n"
         "       Boucher & Altamimi 2001)",
         YDSTime(1997,1,0.0,TimeSystem::UTC)),
         //CommonTime(2450450L,0L,0.0,TimeSystem::UTC)),

         // PZ90 WGS84
      HelmertTransform(
         ReferenceFrame::PZ90, ReferenceFrame::WGS84,
         -19*DEG_PER_MAS, -4*DEG_PER_MAS, 353*DEG_PER_MAS,
         0.07, 0.0, -0.77,
         -3*PPB,
         "PZ90 to WGS84, determined by IGEX-98, reference\n"
         "       \"ITRS, PZ-90 and WGS 84: current realizations\n"
         "       and the related transformation parameters,\"\n"
         "       Journal Geodesy (2001), 75:613, by Boucher and Altamimi.\n"
         "       Use before 20 Sept 2007 17:00 UTC (ICD-2008 v5.1 table 3.2).",
         YDSTime(-4713,1,0.0,TimeSystem::UTC)),

      HelmertTransform(
         ReferenceFrame::PZ90, ReferenceFrame::WGS84,
         0, 0, 0,  -0.36, 0.08, 0.18,  0,
         "PZ90.02 to ITRF2000, from Sergey Revnivykh, GLONASS PNT\n"
         "       Information Analysis Center, 47th CGSIC Meeting and ION\n"
         "       GNSS 2007, Fort Worth, Texas, implemented by GLONASS\n"
         "       20 Sept 2007 17:00 UTC (ICD-2008 v5.1 table 3.2).",
         PZ90Epoch),

         // PZ90 ITRF
      HelmertTransform(
         ReferenceFrame::PZ90, ReferenceFrame::ITRF,
         -19*DEG_PER_MAS, -4*DEG_PER_MAS, 353*DEG_PER_MAS,
         0.07, 0.0, -0.77,
         -3*PPB,
         "PZ90 to ITRF(WGS84), determined by IGEX-98, reference\n"
         "       \"ITRS, PZ-90 and WGS 84: current realizations\n"
         "       and the related transformation parameters,\"\n"
         "       Journal Geodesy (2001), 75:613, by Boucher and Altamimi.\n"
         "       Use before 20 Sept 2007 17:00 UTC (ICD-2008 v5.1 table 3.2).",
         YDSTime(-4713,1,0.0,TimeSystem::UTC)),

      HelmertTransform(
         ReferenceFrame::PZ90, ReferenceFrame::ITRF,
         0, 0, 0,  -0.36, 0.08, 0.18,  0,
         "PZ90.02 to ITRF2000, from Sergey Revnivykh, GLONASS PNT\n"
         "       Information Analysis Center, 47th CGSIC Meeting and ION\n"
         "       GNSS 2007, Fort Worth, Texas, implemented by GLONASS\n"
         "       20 Sept 2007 17:00 UTC (ICD-2008 v5.1 table 3.2).",
         PZ90Epoch),

         // add more transforms here, and increase
         // HelmertTransform::stdCount in .cpp
   };

} // end namespace gnsstk
