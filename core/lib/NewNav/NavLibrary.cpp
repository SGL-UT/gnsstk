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
#include "NavLibrary.hpp"
#include "OrbitData.hpp"
#include "NavHealthData.hpp"
#include "TimeOffsetData.hpp"
#include "NDFUniqConstIterator.hpp"
#include "NDFUniqIterator.hpp"
#include "IonoData.hpp"

namespace gpstk
{
   bool NavLibrary ::
   getXvt(const NavSatelliteID& sat, const CommonTime& when, Xvt& xvt,
          bool useAlm, SVHealth xmitHealth, NavValidityType valid,
          NavSearchOrder order)
   {
      NavMessageID nmid(sat, useAlm ? NavMessageType::Almanac :
                        NavMessageType::Ephemeris);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, xmitHealth, valid, order))
         return false;
      OrbitData *orb = dynamic_cast<OrbitData*>(ndp.get());
      return orb->getXvt(when, xvt);
   }


   bool NavLibrary ::
   getXvt(const NavSatelliteID& sat, const CommonTime& when, Xvt& xvt,
          SVHealth xmitHealth, NavValidityType valid, NavSearchOrder order)
   {
      NavMessageID nmid(sat, NavMessageType::Ephemeris);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, xmitHealth, valid, order))
      {
         NavMessageID nmida(sat, NavMessageType::Almanac);
         if (!find(nmida, when, ndp, xmitHealth, valid, order))
         {
            return false;
         }
      }
      OrbitData *orb = dynamic_cast<OrbitData*>(ndp.get());
      return orb->getXvt(when, xvt);
   }


   bool NavLibrary ::
   getHealth(const NavSatelliteID& sat, const CommonTime& when,
             SVHealth& healthOut, SVHealth xmitHealth, NavValidityType valid,
             NavSearchOrder order)
   {
      NavMessageID nmid(sat, NavMessageType::Health);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, xmitHealth, valid, order))
         return false;
      NavHealthData *healthData = dynamic_cast<NavHealthData*>(ndp.get());
      healthOut = healthData->getHealth();
      return true;
   }


   bool NavLibrary ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset, SVHealth xmitHealth,
             NavValidityType valid)
   {
      NavDataPtr timeOffset;
      if (getOffset(fromSys, toSys, when, timeOffset, xmitHealth, valid))
      {
         TimeOffsetData *top = dynamic_cast<TimeOffsetData*>(timeOffset.get());
         return top->getOffset(fromSys, toSys, when, offset);
      }
      return false;
   }


   bool NavLibrary ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, NavDataPtr& offset, SVHealth xmitHealth,
             NavValidityType valid)
   {
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         if (fi.second->getOffset(fromSys, toSys, when, offset, xmitHealth,
                                  valid))
         {
            return true;
         }
      }
      return false;
   }


   bool NavLibrary ::
   getIonoCorr(SatelliteSystem sys, const CommonTime& when,
               const Position& rxgeo, const Position& svgeo,
               CarrierBand band, double& corrOut, NavType nt)
   {
      SatID sysOnly(sys);
      NavMessageID nmid(
         NavSatelliteID(
            sysOnly, sysOnly,
            ObsID(ObservationType::NavMsg,
                  CarrierBand::Any,
                  TrackingCode::Any,
                  XmitAnt::Any),
            nt),
         NavMessageType::Iono);
      NavDataPtr navOut;
      if (!find(nmid, when, navOut, SVHealth::Healthy,
                NavValidityType::ValidOnly, NavSearchOrder::Nearest))
      {
         return false;
      }
      IonoData *iono = dynamic_cast<IonoData*>(navOut.get());
      corrOut = iono->getCorrection(when, rxgeo, svgeo, band);
      return true;
   }


   bool NavLibrary ::
   getIonoCorr(const SatID& sat, const CommonTime& when,
               const Position& rxgeo,
               CarrierBand band, double& corrOut, NavType nt, int freqOffs,
               bool freqOffsWild)
   {
      if (sat.isWild())
      {
            // wildcards in sat are not allowed, we need a specific
            // satellite's position
         return false;
      }
      Xvt xvt;
      SatID wildSat;
      wildSat.makeWild();
         /// @todo Update the ObsID constructor to include freqOffs data
      NavSatelliteID nsid(sat, wildSat,
                          ObsID(ObservationType::NavMsg,
                                CarrierBand::Any,
                                TrackingCode::Any,
                                XmitAnt::Any),
                          nt);
      if (!getXvt(nsid, when, xvt, SVHealth::Any, NavValidityType::ValidOnly,
                  NavSearchOrder::User))
      {
            // can't get a satellite position
         return false;
      }
      Position svgeo(xvt.x);
      svgeo.transformTo(Position::Geodetic);
      return getIonoCorr(sat.system, when, rxgeo, svgeo, band, corrOut, nt);
   }


   bool NavLibrary ::
   find(const NavMessageID& nmid, const CommonTime& when, NavDataPtr& navOut,
        SVHealth xmitHealth, NavValidityType valid, NavSearchOrder order)
   {
         // Don't use factories.equal_range(nmid), as it can result in
         // range.first and range.second being the same iterator, in
         // which case the loop won't process anything at all.
         // Also don't use the unique iterator as it will result in
         // skipping over valid factories, e.g. looking for CNAV but
         // LNAV is first in the map, the signals don't match and the
         // factory won't be looked at again.
      std::set<NavDataFactory*> uniques;
      for (auto& fi : factories)
      {
         // std::cerr << "fi.first = " << fi.first << "   nmid = " << nmid << std::endl;
         if ((fi.first == nmid) && (uniques.count(fi.second.get()) == 0))
         {
            if (fi.second->find(nmid, when, navOut, xmitHealth, valid, order))
               return true;
            uniques.insert(fi.second.get());
         }
      }
      return false;
   }


   void NavLibrary ::
   setValidityFilter(NavValidityType nvt)
   {
      for (auto& i : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         i.second->setValidityFilter(nvt);
      }
   }


   void NavLibrary ::
   setTypeFilter(const NavMessageTypeSet& nmts)
   {
      for (auto& i : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         i.second->setTypeFilter(nmts);
      }
   }


   void NavLibrary ::
   addFactory(NavDataFactoryPtr& fact)
   {
         // Yes, we do add multiple copies of the NavDataFactoryPtr to
         // the map, it's a convenience.
      for (const auto& si : fact->supportedSignals)
      {
         factories.insert(NavDataFactoryMap::value_type(si,fact));
      }
   }


   void NavLibrary ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         NavDataFactory *ptr = fi.second.get();
         ptr->dump(s,dl);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime)
   {
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->edit(fromTime, toTime);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSatelliteID& satID)
   {
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->edit(fromTime, toTime, satID);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSignalID& signal)
   {
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->edit(fromTime, toTime, signal);
      }
   }


   void NavLibrary ::
   clear()
   {
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->clear();
      }
   }


   CommonTime NavLibrary ::
   getInitialTime() const
   {
      CommonTime rv = CommonTime::END_OF_TIME;
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         gpstk::CommonTime t(fi.second->getInitialTime());
         rv = std::min(rv, t);
      }
      return rv;
   }


   CommonTime NavLibrary ::
   getFinalTime() const
   {
      CommonTime rv = CommonTime::BEGINNING_OF_TIME;
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         rv = std::max(rv, fi.second->getFinalTime());
      }
      return rv;
   }


   NavSatelliteIDSet NavLibrary ::
   getAvailableSats(const CommonTime& fromTime, const CommonTime& toTime)
      const
   {
      NavSatelliteIDSet rv, tmp;
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         tmp = fi.second->getAvailableSats(fromTime, toTime);
         for (const auto& i : tmp)
         {
            rv.insert(i);
         }
      }
      return rv;
   }


   NavSatelliteIDSet NavLibrary ::
   getAvailableSats(NavMessageType nmt,
                    const CommonTime& fromTime,
                    const CommonTime& toTime)
      const
   {
      NavSatelliteIDSet rv, tmp;
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         tmp = fi.second->getAvailableSats(nmt, fromTime, toTime);
         for (const auto& i : tmp)
         {
            rv.insert(i);
         }
      }
      return rv;
   }


   NavMessageIDSet NavLibrary ::
   getAvailableMsgs(const CommonTime& fromTime,
                    const CommonTime& toTime)
      const
   {
      NavMessageIDSet rv, tmp;
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         tmp = fi.second->getAvailableMsgs(fromTime, toTime);
         for (const auto& i : tmp)
         {
            rv.insert(i);
         }
      }
      return rv;
   }


   bool NavLibrary ::
   isPresent(const NavMessageID& nmid,
             const CommonTime& fromTime,
             const CommonTime& toTime)
   {
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         if (fi.second->isPresent(nmid, fromTime, toTime))
            return true;
      }
      return false;
   }


   std::string NavLibrary ::
   getFactoryFormats() const
   {
      std::string rv;
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         NavDataFactory *ptr = fi.second.get();
         std::string ff(ptr->getFactoryFormats());
         if (!ff.empty())
         {
            if (!rv.empty())
               rv += ", ";
            rv += ff;
         }
      }
      return rv;
   }
}
