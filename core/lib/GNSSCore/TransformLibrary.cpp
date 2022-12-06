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

#include "TransformLibrary.hpp"
#include "GNSSconstants.hpp"
#include "HelmertTransformer.hpp"
#include "YDSTime.hpp"

namespace gnsstk
{
   TransformLibrary ::
   TransformLibrary()
   {
      TimeSystem ts = TimeSystem::UTC;
      const CommonTime PZ90Epoch(YDSTime(2007,263,61200.0,ts));

      addTransform(std::make_shared<HelmertTransformer>(
                      RefFrame(RefFrameSys::WGS84, YDSTime(2001,305,0,ts)),
                      RefFrame(RefFrameSys::ITRF, YDSTime(2001,305,0,ts)),
                      0, 0, 0,  0.0, 0.0, 0.0,  0,
                      "WGS84 to ITRF identity transform, a default value\n"
                      "       (\"...since 1997, the WGS84 GPS broadcast ...\n"
                      "       is consistent with the ITRS at better than 5-cm"
                      " level.\"\n"
                      "       Boucher & Altamimi 2001)",
                      YDSTime(1997,1,0.0,ts)));

         // PZ90 WGS84
      addTransform(std::make_shared<HelmertTransformer>(
                      RefFrame(RefFrameSys::PZ90, YDSTime(2001,305,0,ts)),
                      RefFrame(RefFrameSys::WGS84, YDSTime(2001,305,0,ts)),
                      -19*DEG_PER_MAS, -4*DEG_PER_MAS, 353*DEG_PER_MAS,
                      0.07, 0.0, -0.77,
                      -3*PPB,
                      "PZ90 to WGS84, determined by IGEX-98, reference\n"
                      "       \"ITRS, PZ-90 and WGS 84: current realizations\n"
                      "       and the related transformation parameters,\"\n"
                      "       Journal Geodesy (2001), 75:613, by Boucher and"
                      " Altamimi.\n"
                      "       Use before 20 Sept 2007 17:00 UTC (ICD-2008 v5.1"
                      " table 3.2).",
                      YDSTime(-4713,1,0.0,ts)));

         /** @note This transformation, as the others, was copied from
          * an earlier implementation in HelmertTransform.cpp.  I have
          * been unable to track down the source, and the target
          * reference frame and rotation parameters are somewhat
          * dubious.  The target reference frame does not match the
          * description, and the Z rotation parameter is the opposite
          * sign of two other references I found for similar
          * transformations.
      addTransform(std::make_shared<HelmertTransformer>(
                      RefFrame(RefFrameRlz::PZ90Y2007),
                      RefFrame(RefFrameRlz::WGS84G1150),
                      0, 0, 0,  -0.36, 0.08, 0.18,  0,
                      "PZ90.02 to ITRF2000, from Sergey Revnivykh, GLONASS PNT\n"
                      "       Information Analysis Center, 47th CGSIC Meeting"
                      " and ION\n"
                      "       GNSS 2007, Fort Worth, Texas, implemented by"
                      " GLONASS\n"
                      "       20 Sept 2007 17:00 UTC (ICD-2008 v5.1 table 3.2).",
                      PZ90Epoch));
         */

         // PZ90 ITRF
      addTransform(std::make_shared<HelmertTransformer>(
                      RefFrame(RefFrameRlz::PZ90KGS),
                      RefFrame(RefFrameRlz::ITRF2000), /// @todo correct?
                      -19*DEG_PER_MAS, -4*DEG_PER_MAS, 353*DEG_PER_MAS,
                      0.07, 0.0, -0.77,
                      -3*PPB,
                      "PZ90 to ITRF(WGS84), determined by IGEX-98, reference\n"
                      "       \"ITRS, PZ-90 and WGS 84: current realizations\n"
                      "       and the related transformation parameters,\"\n"
                      "       Journal Geodesy (2001), 75:613, by Boucher and"
                      " Altamimi.\n"
                      "       Use before 20 Sept 2007 17:00 UTC (ICD-2008 v5.1"
                      " table 3.2).",
                      YDSTime(-4713,1,0.0,ts)));

         /* see the note above.
      addTransform(std::make_shared<HelmertTransformer>(
                      RefFrame(RefFrameRlz::PZ90Y2007),
                      RefFrame(RefFrameRlz::ITRF2000),
                      0, 0, 0,  -0.36, 0.08, 0.18,  0,
                      "PZ90.02 to ITRF2000, from Sergey Revnivykh, GLONASS PNT\n"
                      "       Information Analysis Center, 47th CGSIC Meeting"
                      " and ION\n"
                      "       GNSS 2007, Fort Worth, Texas, implemented by"
                      " GLONASS\n"
                      "       20 Sept 2007 17:00 UTC (ICD-2008 v5.1 table 3.2).",
                      PZ90Epoch));
         */
   }


   void TransformLibrary ::
   addTransform(const TransformerPtr& ptr)
   {
         // Insert the Transformer into the map with keys representing
         // both forward and reverse transformations.
      RefFramePair keyFwd(ptr->getFromFrame(), ptr->getToFrame());
      RefFramePair keyBwd(ptr->getToFrame(), ptr->getFromFrame());
      transformers[keyFwd][ptr->getEpoch()] = ptr;
      transformers[keyBwd][ptr->getEpoch()] = ptr;
   }


   bool TransformLibrary ::
   getTransform(const RefFrame& fromFrame, const RefFrame& toFrame,
                TransformerPtr& ptr, const CommonTime& when)
      const
   {
         // All Transformers should have been added for both forward
         // and reverse direction so no need to search for both here.
      RefFramePair key(fromFrame, toFrame);
      const auto& p = transformers.find(key);
      if (p != transformers.end())
      {
         const auto& t = p->second.upper_bound(when);
         const auto& pt = std::prev(t);
         if (pt == p->second.end())
         {
            return false;
         }
         ptr = pt->second;
         return true;
      }
      return false;
   }


   bool TransformLibrary ::
   transform(const Position& fromPos, Position& toPos, const CommonTime& when)
      const noexcept
   {
      TransformerPtr xform;
      if (!getTransform(fromPos.getReferenceFrame(), toPos.getReferenceFrame(),
                        xform, when))
      {
         return false;
      }
      return xform->transform(fromPos, toPos);
   }


   bool TransformLibrary ::
   transform(const Xvt& fromPos, Xvt& toPos, const CommonTime& when)
      const noexcept
   {
      TransformerPtr xform;
      if (!getTransform(fromPos.frame, toPos.frame, xform, when))
      {
         return false;
      }
      return xform->transform(fromPos, toPos);
   }


   bool TransformLibrary ::
   transform(const Vector<double>& fromPos,
             const RefFrame& srcFrame,
             Vector<double>& toPos,
             const RefFrame& tgtFrame,
             const CommonTime& when)
      const noexcept
   {
      TransformerPtr xform;
      if (!getTransform(srcFrame, tgtFrame, xform, when))
      {
         return false;
      }
      return xform->transform(fromPos, srcFrame, toPos);
   }


   bool TransformLibrary ::
   transform(const Triple& fromPos,
             const RefFrame& srcFrame,
             Triple& toPos,
             const RefFrame& tgtFrame,
             const CommonTime& when)
      const noexcept
   {
      TransformerPtr xform;
      if (!getTransform(srcFrame, tgtFrame, xform, when))
      {
         return false;
      }
      return xform->transform(fromPos, srcFrame, toPos);
   }


   bool TransformLibrary ::
   transform(double fx, double fy, double fz,
             const RefFrame& srcFrame,
             double& tx, double& ty, double& tz,
             const RefFrame& tgtFrame,
             const CommonTime& when)
      const noexcept
   {
      TransformerPtr xform;
      if (!getTransform(srcFrame, tgtFrame, xform, when))
      {
         return false;
      }
      return xform->transform(fx, fy, fz, srcFrame, tx, ty, tz);
   }

} // namespace gnsstk
