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


#include "GroupPathCorr.hpp"
#include "BCISCorrector.hpp"
#include "BCIonoCorrector.hpp"
#include "TropCorrector.hpp"

namespace gnsstk
{
   bool GroupPathCorr ::
   init(std::shared_ptr<NavLibrary>& navLib)
   {
      GroupPathCorrectorPtr ec1, ec2;
      ec1 = std::make_shared<BCISCorrector>();
      ec2 = std::make_shared<BCIonoCorrector>();
      BCISCorrector *isc = dynamic_cast<BCISCorrector*>(ec1.get());
      BCIonoCorrector *iono = dynamic_cast<BCIonoCorrector*>(ec2.get());
      isc->navLib = navLib;
      iono->navLib = navLib;
      calcs.push_back(ec1);
      calcs.push_back(ec2);
         // No error conditions.  Yet.  Still returning true/false so
         // that we CAN indicate error conditions in the future
         // without an API change.
      return true;
   }


   bool GroupPathCorr ::
   initGlobal(std::shared_ptr<NavLibrary>& navLib,
              const std::string& rinMetFile)
   {
      if (!init(navLib))
      {
         return false;
      }
      GroupPathCorrectorPtr ec3;
      ec3 = std::make_shared<gnsstk::GlobalTropCorrector>();
      if (!rinMetFile.empty())
      {
         gnsstk::GlobalTropCorrector *trop;
         trop = dynamic_cast<gnsstk::GlobalTropCorrector*>(ec3.get());
         if (!trop->loadFile(rinMetFile))
         {
            return false;
         }
      }
      calcs.push_back(ec3);
      return true;
   }


   bool GroupPathCorr ::
   initNB(std::shared_ptr<NavLibrary>& navLib,
          const std::string& rinMetFile)
   {
      if (!init(navLib))
      {
         return false;
      }
      GroupPathCorrectorPtr ec3;
      ec3 = std::make_shared<gnsstk::NBTropCorrector>();
      if (!rinMetFile.empty())
      {
         gnsstk::NBTropCorrector *trop;
         trop = dynamic_cast<gnsstk::NBTropCorrector*>(ec3.get());
         if (!trop->loadFile(rinMetFile))
         {
            return false;
         }
      }
      calcs.push_back(ec3);
      return true;
   }


   bool GroupPathCorr ::
   getCorr(const Position& rxPos, const Position& svPos,
           const SatID& sat, const ObsID& obs, const CommonTime& when,
           NavType nav, CorrectionResults& corrOut, CorrDupHandling dups)
   {
         // We always iterate through calcs in the same direction
         // regardless of the value of dups.  It is a known
         // compromise, so we don't have to duplicate code to use
         // forward or reverse iterators.  You're never going to have
         // hundreds of group path correctors, as there simply aren't
         // that many models in existence, so it's a reasonable choice
         // for a handful of items to iterate over.
      bool rv = true;
      corrOut.clear();
      CorrectorTypeSet seen;
      double tmp;
      for (const auto& calc : calcs)
      {
         if ((dups == CorrDupHandling::ComputeFirst) &&
             (seen.count(calc->corrType) > 0))
         {
            continue;
         }
         if (!calc->getCorr(rxPos, svPos, sat, obs, when, nav, tmp))
         {
            rv = false;
         }
         else
         {
            CorrectionResult res(tmp, calc);
            corrOut.addResult(res);
            seen.insert(calc->corrType);
         }
      }
      return rv;
   }


   bool GroupPathCorr ::
   getCorr(const Position& rxPos, const Xvt& svPos,
           const SatID& sat, const ObsID& obs, const CommonTime& when,
           NavType nav, CorrectionResults& corrOut, CorrDupHandling dups)
   {
      Position sp(svPos.x);
      return getCorr(rxPos, sp, sat, obs, when, nav, corrOut, dups);
   }


   bool GroupPathCorr ::
   getCorr(const Position& rxPos, const Position& svPos,
           const SatID& sat, const ObsID& obs, const CommonTime& when,
           NavType nav, double& corrOut, CorrDupHandling dups)
   {
      CorrectionResults res;
      bool rv = getCorr(rxPos, svPos, sat, obs, when, nav, res, dups);
      corrOut = res.getCorrSum(dups);
      return rv;
   }


   bool GroupPathCorr ::
   getCorr(const Position& rxPos, const Xvt& svPos, const SatID& sat,
           const ObsID& obs, const CommonTime& when, NavType nav,
           double& corrOut, CorrDupHandling dups)
   {
      CorrectionResults res;
      bool rv = getCorr(rxPos, svPos, sat, obs, when, nav, res, dups);
      corrOut = res.getCorrSum(dups);
      return rv;
   }
} // namespace gnsstk
