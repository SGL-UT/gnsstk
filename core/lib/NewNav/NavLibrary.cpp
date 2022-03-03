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
#include "NavLibrary.hpp"
#include "OrbitData.hpp"
#include "NavHealthData.hpp"
#include "TimeOffsetData.hpp"
#include "NDFUniqConstIterator.hpp"
#include "NDFUniqIterator.hpp"
#include "IonoNavData.hpp"
#include "InterSigCorr.hpp"
#include "DebugTrace.hpp"

namespace gnsstk
{
   bool NavLibrary ::
   getXvt(const NavSatelliteID& sat, const CommonTime& when, Xvt& xvt,
          bool useAlm, SVHealth xmitHealth, NavValidityType valid,
          NavSearchOrder order)
   {
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      IonoNavData *iono = dynamic_cast<IonoNavData*>(navOut.get());
      corrOut = iono->getIonoCorr(when, rxgeo, svgeo, band);
      return true;
   }


   bool NavLibrary ::
   getIonoCorr(const SatID& sat, const CommonTime& when,
               const Position& rxgeo,
               CarrierBand band, double& corrOut, NavType nt, int freqOffs,
               bool freqOffsWild)
   {
      DEBUGTRACE_FUNCTION();
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
   getISC(const ObsID& oid, const CommonTime& when, double& corrOut)
   {
      DEBUGTRACE_FUNCTION();
      std::list<NavMessageID> msgIDs(getISCNMID(oid));
      if (msgIDs.empty())
         return false;
      NavDataPtr navOut;
      for (const auto& nmid : msgIDs)
      {
         if (find(nmid, when, navOut, SVHealth::Healthy,
                  NavValidityType::ValidOnly, NavSearchOrder::Nearest))
         {
            InterSigCorr *isc = dynamic_cast<InterSigCorr*>(navOut.get());
               // This if test allows getISC to fail and the loop to
               // continue and attempt to find another match.
            if (isc->getISC(oid, corrOut))
               return true;
         }
      }
      return false;
   }


   bool NavLibrary ::
   getISC(const ObsID& oid1, const ObsID& oid2, const CommonTime& when,
          double& corrOut)
   {
      DEBUGTRACE_FUNCTION();
         // This ugly mess is a special case as GPS LNAV does not have
         // ISCs that apply to the iono-free combined pseudorange.
      if ((oid1.band == CarrierBand::L1) && (oid2.band == CarrierBand::L2) &&
          ((oid1.code == TrackingCode::CA) ||
           (oid1.code == TrackingCode::P) ||
           (oid1.code == TrackingCode::Y) ||
           (oid1.code == TrackingCode::Ztracking) ||
           (oid1.code == TrackingCode::YCodeless) ||
           (oid1.code == TrackingCode::Semicodeless)) &&
          ((oid2.code == TrackingCode::P) ||
           (oid2.code == TrackingCode::Y) ||
           (oid2.code == TrackingCode::Ztracking) ||
           (oid2.code == TrackingCode::YCodeless) ||
           (oid2.code == TrackingCode::Semicodeless)))
      {
         corrOut = 0.0;
         return true;
      }
      std::list<NavMessageID> msgIDs(getISCNMID(oid1,oid2));
      if (msgIDs.empty())
         return false;
      NavDataPtr navOut;
      for (const auto& nmid : msgIDs)
      {
         if (find(nmid, when, navOut, SVHealth::Healthy,
                  NavValidityType::ValidOnly, NavSearchOrder::Nearest))
         {
            InterSigCorr *isc = dynamic_cast<InterSigCorr*>(navOut.get());
               // This if test allows getISC to fail and the loop to
               // continue and attempt to find another match.
            if (isc->getISC(oid1, oid2, corrOut))
               return true;
         }
      }
      return false;
   }


   std::list<NavMessageID> NavLibrary ::
   getISCNMID(const ObsID& oid)
   {
      DEBUGTRACE_FUNCTION();
      std::list<NavMessageID> rv;
      const SatID gpsAnySat(SatelliteSystem::GPS);
      const ObsID anyObs(ObservationType::NavMsg,
                         CarrierBand::Any,
                         TrackingCode::Any,
                         XmitAnt::Any);
      NavMessageID nmid(
         NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                        NavType::Unknown),
         NavMessageType::ISC);
      switch (oid.code)
      {
         case TrackingCode::CA:
            if (oid.band == CarrierBand::L1)
            {
               nmid.nav = NavType::GPSLNAV;
               rv.push_back(nmid);
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAVL2),
                     NavMessageType::ISC));
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAVL5),
                     NavMessageType::ISC));
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAV2),
                     NavMessageType::ISC));
            }
            break;
         case TrackingCode::P:
         case TrackingCode::Y:
         case TrackingCode::Ztracking:
         case TrackingCode::YCodeless:
         case TrackingCode::Semicodeless:
            switch (oid.band)
            {
               case CarrierBand::L1:
               case CarrierBand::L2:
                  rv.push_back(
                     NavMessageID(
                        NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                       NavType::GPSLNAV),
                        NavMessageType::ISC));
                  break;
            }
            break;
         case TrackingCode::MD:
         case TrackingCode::MDP:
         case TrackingCode::MP:
         case TrackingCode::MPA:
         case TrackingCode::MARLD:
         case TrackingCode::MARLP:
         case TrackingCode::Mprime:
         case TrackingCode::MprimePA:
            switch (oid.band)
            {
               case CarrierBand::L1:
               case CarrierBand::L2:
                  rv.push_back(
                     NavMessageID(
                        NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                       NavType::GPSMNAV),
                        NavMessageType::ISC));
                  break;
            }
            break;
         case TrackingCode::L2CM:
         case TrackingCode::L2CL:
         case TrackingCode::L2CML:
            if (oid.band == CarrierBand::L2)
            {
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAVL2),
                     NavMessageType::ISC));
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAVL5),
                     NavMessageType::ISC));
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAV2),
                     NavMessageType::ISC));
            }
            break;
         case TrackingCode::L5I:
         case TrackingCode::L5Q:
         case TrackingCode::L5IQ:
            if (oid.band == CarrierBand::L5)
            {
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAVL5),
                     NavMessageType::ISC));
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAVL2),
                     NavMessageType::ISC));
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAV2),
                     NavMessageType::ISC));
            }
            break;
         case TrackingCode::L1CP:
         case TrackingCode::L1CD:
         case TrackingCode::L1CDP:
            if (oid.band == CarrierBand::L1)
            {
               rv.push_back(
                  NavMessageID(
                     NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                                    NavType::GPSCNAV2),
                     NavMessageType::ISC));
            }
            break;
      }
      return rv;
   }


   std::list<NavMessageID> NavLibrary ::
   getISCNMID(const ObsID& oid1, const ObsID& oid2)
   {
      DEBUGTRACE_FUNCTION();
      std::list<NavMessageID> rv;
      const SatID gpsAnySat(SatelliteSystem::GPS);
      const ObsID anyObs(ObservationType::NavMsg,
                         CarrierBand::Any,
                         TrackingCode::Any,
                         XmitAnt::Any);
      if ((oid1.band == CarrierBand::L1) &&
          (oid1.code == TrackingCode::CA) &&
          (((oid2.band == CarrierBand::L5) &&
            ((oid2.code == TrackingCode::L5I) ||
             (oid2.code == TrackingCode::L5Q))) ||
           ((oid2.band == CarrierBand::L2) &&
            ((oid2.code == TrackingCode::L2CM) ||
             (oid2.code == TrackingCode::L2CL) ||
             (oid2.code == TrackingCode::L2CML)))))
      {
         rv.push_back(
            NavMessageID(
               NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                              NavType::GPSCNAVL2),
               NavMessageType::ISC));
         NavMessageID l5(
            NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                           NavType::GPSCNAVL5),
            NavMessageType::ISC);
            // Push the codes so they're in order preferring the same
            // band, mostly because it's expected that if you're
            // processing L2 you'll have L2 nav data, etc.
         if (oid2.band == CarrierBand::L5)
            rv.push_front(l5);
         else
            rv.push_back(l5);
         rv.push_back(
            NavMessageID(
               NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                              NavType::GPSCNAV2),
               NavMessageType::ISC));
      }
      else if ((oid1.band == CarrierBand::L1) &&
               ((oid1.code == TrackingCode::L1CP) ||
                (oid1.code == TrackingCode::L1CD)) &&
               (oid2.band == CarrierBand::L2) &&
               ((oid2.code == TrackingCode::L2CM) ||
                (oid2.code == TrackingCode::L2CL) ||
                (oid2.code == TrackingCode::L2CML)))
      {
         rv.push_back(
            NavMessageID(
               NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                              NavType::GPSCNAV2),
               NavMessageType::ISC));
      }
      else if ((oid1.band == CarrierBand::L1) &&
               (oid2.band == CarrierBand::L2) &&
               ((oid1.code == TrackingCode::MD) ||
                (oid1.code == TrackingCode::MDP) ||
                (oid1.code == TrackingCode::MP) ||
                (oid1.code == TrackingCode::MPA) ||
                (oid1.code == TrackingCode::MARLD) ||
                (oid1.code == TrackingCode::MARLP) ||
                (oid1.code == TrackingCode::Mprime) ||
                (oid1.code == TrackingCode::MprimePA)) &&
               ((oid2.code == TrackingCode::MD) ||
                (oid2.code == TrackingCode::MDP) ||
                (oid2.code == TrackingCode::MP) ||
                (oid2.code == TrackingCode::MPA) ||
                (oid2.code == TrackingCode::MARLD) ||
                (oid2.code == TrackingCode::MARLP) ||
                (oid2.code == TrackingCode::Mprime) ||
                (oid2.code == TrackingCode::MprimePA)))
      {
         rv.push_back(
            NavMessageID(
               NavSatelliteID(gpsAnySat, gpsAnySat, anyObs,
                              NavType::GPSMNAV),
               NavMessageType::ISC));
      }
      return rv;
   }


   bool NavLibrary ::
   find(const NavMessageID& nmid, const CommonTime& when, NavDataPtr& navOut,
        SVHealth xmitHealth, NavValidityType valid, NavSearchOrder order)
   {
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
      for (auto& i : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         i.second->setValidityFilter(nvt);
      }
   }


   void NavLibrary ::
   setTypeFilter(const NavMessageTypeSet& nmts)
   {
      DEBUGTRACE_FUNCTION();
      for (auto& i : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         i.second->setTypeFilter(nmts);
      }
   }


   void NavLibrary ::
   addFactory(NavDataFactoryPtr& fact)
   {
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         NavDataFactory *ptr = fi.second.get();
         ptr->dump(s,dl);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime)
   {
      DEBUGTRACE_FUNCTION();
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->edit(fromTime, toTime);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSatelliteID& satID)
   {
      DEBUGTRACE_FUNCTION();
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->edit(fromTime, toTime, satID);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSignalID& signal)
   {
      DEBUGTRACE_FUNCTION();
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->edit(fromTime, toTime, signal);
      }
   }


   void NavLibrary ::
   clear()
   {
      DEBUGTRACE_FUNCTION();
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories))
      {
         fi.second->clear();
      }
   }


   CommonTime NavLibrary ::
   getInitialTime() const
   {
      DEBUGTRACE_FUNCTION();
      CommonTime rv = CommonTime::END_OF_TIME;
      for (const auto& fi : NDFUniqConstIterator<NavDataFactoryMap>(factories))
      {
         gnsstk::CommonTime t(fi.second->getInitialTime());
         rv = std::min(rv, t);
      }
      return rv;
   }


   CommonTime NavLibrary ::
   getFinalTime() const
   {
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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


   std::set<SatID> NavLibrary ::
   getIndexSet(const CommonTime& fromTime,
               const CommonTime& toTime) const
   {
      DEBUGTRACE_FUNCTION();
      NavSatelliteIDSet fullSatSet = getAvailableSats(fromTime,toTime);
      std::set<SatID> rv;
      for (const auto& fssi : fullSatSet)
      {
         rv.insert(fssi.sat);
      }
      return rv;
   }


   std::set<SatID> NavLibrary ::
   getIndexSet(NavMessageType nmt,
               const CommonTime& fromTime,
               const CommonTime& toTime) const
   {
      DEBUGTRACE_FUNCTION();
      NavSatelliteIDSet fullSatSet = getAvailableSats(nmt,fromTime,toTime);
      std::set<SatID> rv;
      for (const auto& fssi : fullSatSet)
      {
         rv.insert(fssi.sat);
      }
      return rv;
   }


   NavMessageIDSet NavLibrary ::
   getAvailableMsgs(const CommonTime& fromTime,
                    const CommonTime& toTime)
      const
   {
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
