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

#include "SatelliteSystem.hpp"
#include "SatMetaDataStore.hpp"
#include "TestUtil.hpp"
#include "YDSTime.hpp"

using namespace std;

namespace gnsstk
{
   ostream& operator<<(ostream& s, const gnsstk::SatelliteSystem sys)
   {
      s << gnsstk::StringUtils::asString(sys);
      return s;
   }
}

class SatMetaDataStore_T
{
public:
   unsigned loadDataTest();
   unsigned findSatTest();
   unsigned getSVNTest();
   unsigned findSatBySVNTest();
   unsigned findSatBySlotFdmaTest();
   unsigned getPRNTest();
   unsigned getSignalSetTest();
   unsigned getSatsBySignalTest();
   unsigned operatorEqSignalTest();
};


unsigned SatMetaDataStore_T ::
loadDataTest()
{
   TUDEF("SatMetaDataStore", "loadData");
   gnsstk::SatMetaDataStore testObj;
   TUASSERT(testObj.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                             "sats.csv"));
   TURETURN();
}


unsigned SatMetaDataStore_T ::
findSatTest()
{
   TUDEF("SatMetaDataStore", "loadData");
   gnsstk::SatMetaDataStore testObj;
   gnsstk::SatMetaData sat;
   TUASSERT(testObj.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                             "sat32.csv"));
      // find a satellite
   TUCSM("findSat");
   TUASSERT(testObj.findSat(gnsstk::SatelliteSystem::GPS, 32,
                            gnsstk::YDSTime(2020,1,0), sat));
   TUASSERTE(uint32_t, 32, sat.prn);
   TUASSERTE(std::string, "70", sat.svn);
   TUASSERTE(int32_t, 41328, sat.norad);
   TUASSERTE(int32_t, 0, sat.chl);
   TUASSERTE(uint32_t, 0, sat.slotID);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, sat.sys);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2016,34,49620), sat.launchTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2016,34,49620), sat.startTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2132,244,0), sat.endTime);
   TUASSERTE(std::string, "F", sat.plane);
   TUASSERTE(std::string, "1", sat.slot);
   TUASSERTE(std::string, "IIF", sat.type);
   TUASSERTE(std::string, "GPS IIF", sat.signals);
   TUASSERTE(std::string, "12", sat.mission);
   TUASSERTE(gnsstk::SatMetaData::Status,
             gnsstk::SatMetaData::Status::Operational, sat.status);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[0]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[1]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[2]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Unknown, sat.clocks[3]);
   TUASSERTE(uint8_t, 255, sat.activeClock);
      // find an older mapping of the same PRN
   TUASSERT(testObj.findSat(gnsstk::SatelliteSystem::GPS, 32,
                            gnsstk::YDSTime(1991,1,0), sat));
   TUASSERTE(uint32_t, 32, sat.prn);
   TUASSERTE(std::string, "23", sat.svn);
   TUASSERTE(int32_t, 28361, sat.norad);
   TUASSERTE(int32_t, 0, sat.chl);
   TUASSERTE(uint32_t, 0, sat.slotID);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, sat.sys);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(1990,330,0), sat.launchTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(1990,344,0), sat.startTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2016,25,79199), sat.endTime);
   TUASSERTE(std::string, "E", sat.plane);
   TUASSERTE(std::string, "5", sat.slot);
   TUASSERTE(std::string, "IIA", sat.type);
   TUASSERTE(std::string, "GPS IIA", sat.signals);
   TUASSERTE(std::string, "10", sat.mission);
   TUASSERTE(gnsstk::SatMetaData::Status,
             gnsstk::SatMetaData::Status::Operational, sat.status);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[0]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[1]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[2]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Cesium, sat.clocks[3]);
   TUASSERTE(uint8_t, 255, sat.activeClock);
      // try to find the PRN before it was launched
   TUASSERT(!testObj.findSat(gnsstk::SatelliteSystem::GPS, 32,
                             gnsstk::YDSTime(1989,1,0), sat));
      // try to find the PRN during the transition (between SV assignments)
   TUASSERT(!testObj.findSat(gnsstk::SatelliteSystem::GPS, 32,
                             gnsstk::YDSTime(2016,26,0), sat));
      // find a satellite that is beyond the end of the contents of the map
   TUASSERT(!testObj.findSat(gnsstk::SatelliteSystem::GPS, 33,
                             gnsstk::YDSTime(2020,1,0), sat));
   TURETURN();
}


unsigned SatMetaDataStore_T ::
getSVNTest()
{
   TUDEF("SatMetaDataStore", "loadData");
   gnsstk::SatMetaDataStore testObj;
   gnsstk::SatMetaData sat;
   TUASSERT(testObj.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                             "sat32.csv"));
      // find a satellite
   TUCSM("getSVN");
   std::string svn = "999999";
   TUASSERT(testObj.getSVN(gnsstk::SatelliteSystem::GPS, 32,
                           gnsstk::YDSTime(2020,1,0), svn));
   TUASSERTE(std::string, "70", svn);
      // find an older mapping of the same PRN
   TUASSERT(testObj.getSVN(gnsstk::SatelliteSystem::GPS, 32,
                           gnsstk::YDSTime(1991,1,0), svn));
   TUASSERTE(std::string, "23", svn);
      // try to find the PRN before it was launched
   TUASSERT(!testObj.getSVN(gnsstk::SatelliteSystem::GPS, 32,
                             gnsstk::YDSTime(1989,1,0), svn));
      // try to find the PRN during the transition (between SV assignments)
   TUASSERT(!testObj.getSVN(gnsstk::SatelliteSystem::GPS, 32,
                             gnsstk::YDSTime(2016,26,0), svn));
      // find a satellite that is beyond the end of the contents of the map
   TUASSERT(!testObj.getSVN(gnsstk::SatelliteSystem::GPS, 33,
                             gnsstk::YDSTime(2020,1,0), svn));
   TURETURN();
}


unsigned SatMetaDataStore_T ::
findSatBySVNTest()
{
   TUDEF("SatMetaDataStore", "loadData");
   gnsstk::SatMetaDataStore testObj;
   gnsstk::SatMetaData sat;
   TUASSERT(testObj.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                             "sat32.csv"));
      // find a satellite
   TUCSM("findSatBySVN");
   TUASSERT(testObj.findSatBySVN(gnsstk::SatelliteSystem::GPS, "70",
                                 gnsstk::YDSTime(2020,1,0), sat));
   TUASSERTE(uint32_t, 32, sat.prn);
   TUASSERTE(std::string, "70", sat.svn);
   TUASSERTE(int32_t, 41328, sat.norad);
   TUASSERTE(int32_t, 0, sat.chl);
   TUASSERTE(uint32_t, 0, sat.slotID);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, sat.sys);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2016,34,49620), sat.launchTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2016,34,49620), sat.startTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2132,244,0), sat.endTime);
   TUASSERTE(std::string, "F", sat.plane);
   TUASSERTE(std::string, "1", sat.slot);
   TUASSERTE(std::string, "IIF", sat.type);
   TUASSERTE(std::string, "GPS IIF", sat.signals);
   TUASSERTE(std::string, "12", sat.mission);
   TUASSERTE(gnsstk::SatMetaData::Status,
             gnsstk::SatMetaData::Status::Operational, sat.status);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[0]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[1]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[2]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Unknown, sat.clocks[3]);
   TUASSERTE(uint8_t, 255, sat.activeClock);
      // find a different SVN
   TUASSERT(testObj.findSatBySVN(gnsstk::SatelliteSystem::GPS, "23",
                                 gnsstk::YDSTime(1991,1,0), sat));
   TUASSERTE(uint32_t, 32, sat.prn);
   TUASSERTE(std::string, "23", sat.svn);
   TUASSERTE(int32_t, 28361, sat.norad);
   TUASSERTE(int32_t, 0, sat.chl);
   TUASSERTE(uint32_t, 0, sat.slotID);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, sat.sys);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(1990,330,0), sat.launchTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(1990,344,0), sat.startTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2016,25,79199), sat.endTime);
   TUASSERTE(std::string, "E", sat.plane);
   TUASSERTE(std::string, "5", sat.slot);
   TUASSERTE(std::string, "IIA", sat.type);
   TUASSERTE(std::string, "GPS IIA", sat.signals);
   TUASSERTE(std::string, "10", sat.mission);
   TUASSERTE(gnsstk::SatMetaData::Status,
             gnsstk::SatMetaData::Status::Operational, sat.status);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[0]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[1]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Rubidium, sat.clocks[2]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Cesium, sat.clocks[3]);
   TUASSERTE(uint8_t, 255, sat.activeClock);
      // try to find the SVN before it was launched
   TUASSERT(!testObj.findSatBySVN(gnsstk::SatelliteSystem::GPS, "70",
                                  gnsstk::YDSTime(1989,1,0), sat));
      // try to find the SVN during the transition (between SV assignments)
   TUASSERT(!testObj.findSatBySVN(gnsstk::SatelliteSystem::GPS, "70",
                                  gnsstk::YDSTime(2016,26,0), sat));
      // find a satellite that is beyond the end of the contents of the map
   TUASSERT(!testObj.findSatBySVN(gnsstk::SatelliteSystem::GPS, "71",
                                  gnsstk::YDSTime(2020,1,0), sat));
   TURETURN();
}


unsigned SatMetaDataStore_T ::
findSatBySlotFdmaTest()
{
   TUDEF("SatMetaDataStore", "loadData");
   gnsstk::SatMetaDataStore testObj;
   gnsstk::SatMetaData sat;
   TUASSERT(testObj.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                             "satmeta_GLONASS.csv"));
      // find a satellite
   TUCSM("findSatBySlotFdma");
   TUASSERT(testObj.findSatBySlotFdma(22, -3,
                                 gnsstk::YDSTime(2020,5,27), sat));

   TUASSERTE(uint32_t, 0, sat.prn);
   TUASSERTE(std::string, "731", sat.svn)
   TUASSERTE(int32_t, 36400, sat.norad);
   TUASSERTE(int32_t, -3, sat.chl);
   TUASSERTE(uint32_t, 22, sat.slotID);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::Glonass, sat.sys);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2010,61,0), sat.launchTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2010,61,0), sat.startTime);
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2020,153,86399), sat.endTime);
   TUASSERTE(std::string, "3", sat.plane);
   TUASSERTE(std::string, "?", sat.slot);
   TUASSERTE(std::string, "M", sat.type);
   TUASSERTE(std::string, "M", sat.signals);
   TUASSERTE(std::string, "unk", sat.mission);
   TUASSERTE(gnsstk::SatMetaData::Status,
             gnsstk::SatMetaData::Status::Operational, sat.status);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Unknown, sat.clocks[0]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Unknown, sat.clocks[1]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Unknown, sat.clocks[2]);
   TUASSERTE(gnsstk::SatMetaData::ClockType,
             gnsstk::SatMetaData::ClockType::Unknown, sat.clocks[3]);
   TUASSERTE(uint8_t, 255, sat.activeClock);

      // try to find the SVN before it was launched
   TUASSERT(!testObj.findSatBySlotFdma(22, -3,
                                       gnsstk::YDSTime(1989,1,0), sat));

   TURETURN();
}


unsigned SatMetaDataStore_T ::
getPRNTest()
{
   TUDEF("SatMetaDataStore", "loadData");
   gnsstk::SatMetaDataStore testObj;
   gnsstk::SatMetaData sat;
   TUASSERT(testObj.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                             "sat32.csv"));
      // find a satellite
   TUCSM("getPRN");
   uint32_t prn = 999999;
   TUASSERT(testObj.getPRN(gnsstk::SatelliteSystem::GPS, "70",
                           gnsstk::YDSTime(2020,1,0), prn));
   TUASSERTE(uint32_t, 32, prn);
      // find a different SVN
   prn = 999999;
   TUASSERT(testObj.getPRN(gnsstk::SatelliteSystem::GPS, "23",
                           gnsstk::YDSTime(1991,1,0), prn));
   TUASSERTE(uint32_t, 32, prn);
      // try to find the PRN before it was launched
   TUASSERT(!testObj.getPRN(gnsstk::SatelliteSystem::GPS, "70",
                             gnsstk::YDSTime(1989,1,0), prn));
      // try to find the PRN during the transition (between SV assignments)
   TUASSERT(!testObj.getPRN(gnsstk::SatelliteSystem::GPS, "70",
                             gnsstk::YDSTime(2016,26,0), prn));
      // find a satellite that is beyond the end of the contents of the map
   TUASSERT(!testObj.getPRN(gnsstk::SatelliteSystem::GPS, "71",
                             gnsstk::YDSTime(2020,1,0), prn));
   TURETURN();
}


unsigned SatMetaDataStore_T ::
getSignalSetTest()
{
   TUDEF("SatMetaDataStore", "getSignalSet");
   gnsstk::SatMetaDataStore uut;
   TUASSERT(uut.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                         "sats.csv"));
   std::set<std::string> groups;
   TUCATCH(groups = uut.getSignalSet(gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA,
                                     gnsstk::NavType::GPSLNAV));
   TUASSERTE(size_t, 5, groups.size());
   TUASSERTE(size_t, 1, groups.count("GPS I"));
   TUASSERTE(size_t, 1, groups.count("GPS IIR"));
   TUASSERTE(size_t, 1, groups.count("GPS IIR-M"));
   TUASSERTE(size_t, 1, groups.count("GPS IIF"));
   TUASSERTE(size_t, 1, groups.count("GPS III"));
   TUCATCH(groups = uut.getSignalSet(gnsstk::CarrierBand::L2,
                                     gnsstk::TrackingCode::L2CM,
                                     gnsstk::NavType::GPSCNAVL2));
   TUASSERTE(size_t, 3, groups.size());
   TUASSERTE(size_t, 1, groups.count("GPS IIR-M"));
   TUASSERTE(size_t, 1, groups.count("GPS IIF"));
   TUASSERTE(size_t, 1, groups.count("GPS III"));
   TUCATCH(groups = uut.getSignalSet(gnsstk::CarrierBand::L2,
                                     gnsstk::TrackingCode::Any,
                                     gnsstk::NavType::GPSCNAVL2));
   TUASSERTE(size_t, 3, groups.size());
   TUASSERTE(size_t, 1, groups.count("GPS IIR-M"));
   TUASSERTE(size_t, 1, groups.count("GPS IIF"));
   TUASSERTE(size_t, 1, groups.count("GPS III"));
   TUCATCH(groups = uut.getSignalSet(gnsstk::CarrierBand::Any,
                                     gnsstk::TrackingCode::Any,
                                     gnsstk::NavType::GPSCNAVL2));
   TUASSERTE(size_t, 3, groups.size());
   TUASSERTE(size_t, 1, groups.count("GPS IIR-M"));
   TUASSERTE(size_t, 1, groups.count("GPS IIF"));
   TUASSERTE(size_t, 1, groups.count("GPS III"));
   gnsstk::SatMetaDataStore::Signal sig1(gnsstk::CarrierBand::L2,
                                         gnsstk::TrackingCode::L2CM,
                                         gnsstk::NavType::GPSCNAVL2);
   TUCATCH(groups = uut.getSignalSet(sig1));
   TUASSERTE(size_t, 3, groups.size());
   TUASSERTE(size_t, 1, groups.count("GPS IIR-M"));
   TUASSERTE(size_t, 1, groups.count("GPS IIF"));
   TUASSERTE(size_t, 1, groups.count("GPS III"));
   gnsstk::SatMetaDataStore::Signal sig2(gnsstk::CarrierBand::L1,
                                         gnsstk::TrackingCode::CA,
                                         gnsstk::NavType::GPSLNAV);
   TUCATCH(groups = uut.getSignalSet({sig1, sig2}));
   TUASSERTE(size_t, 3, groups.size());
   TUASSERTE(size_t, 1, groups.count("GPS IIR-M"));
   TUASSERTE(size_t, 1, groups.count("GPS IIF"));
   TUASSERTE(size_t, 1, groups.count("GPS III"));
   gnsstk::SatMetaDataStore::Signal sig3(gnsstk::CarrierBand::L5,
                                         gnsstk::TrackingCode::L5I,
                                         gnsstk::NavType::GPSCNAVL5);
   TUCATCH(groups = uut.getSignalSet({sig1, sig3}));
   TUASSERTE(size_t, 2, groups.size());
   TUASSERTE(size_t, 1, groups.count("GPS IIF"));
   TUASSERTE(size_t, 1, groups.count("GPS III"));
   TURETURN();
}


unsigned SatMetaDataStore_T ::
getSatsBySignalTest()
{
   TUDEF("SatMetaDataStore", "getSatsBySignal");
   gnsstk::SatMetaDataStore uut;
   TUASSERT(uut.loadData(gnsstk::getPathData() + gnsstk::getFileSep() +
                         "sats.csv"));
   gnsstk::SatMetaDataStore::Signal sig1(gnsstk::CarrierBand::L1,
                                         gnsstk::TrackingCode::L1CD,
                                         gnsstk::NavType::GPSCNAV2);
   gnsstk::SatMetaDataStore::SatSet sats;
   TUCATCH(sats = uut.getSatsBySignal({sig1}));
   TUASSERTE(size_t, 1, sats.size());
   if (sats.size() >= 1)
   {
      gnsstk::SatMetaData got = *sats.begin();
      TUASSERTE(uint32_t, 4, got.prn);
      TUASSERTE(std::string, "74", got.svn);
      TUASSERTE(int32_t, 43873, got.norad);
      TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, got.sys);
   }
   gnsstk::SatMetaDataStore::Signal sig2(gnsstk::CarrierBand::L1,
                                         gnsstk::TrackingCode::MD,
                                         gnsstk::NavType::GPSMNAV);
   TUCATCH(sats = uut.getSatsBySignal({sig2}));
   TUASSERTE(size_t, 28, sats.size());
   gnsstk::YDSTime when(2131,0,0,gnsstk::TimeSystem::Any);
   TUCATCH(sats = uut.getSatsBySignal({sig2}, when));
   TUASSERTE(size_t, 20, sats.size());
   gnsstk::SatMetaDataStore::Signal sig3(gnsstk::CarrierBand::L1,
                                         gnsstk::TrackingCode::Y,
                                         gnsstk::NavType::GPSLNAV);
   TUCATCH(sats = uut.getSatsBySignal(
      {sig3},
      gnsstk::YDSTime(2020,71,0,gnsstk::TimeSystem::Any),
      gnsstk::YDSTime(2020,360,0,gnsstk::TimeSystem::Any)));
   TUASSERTE(size_t, 32, sats.size());
   TUCATCH(sats = uut.getSatsBySignal(
      {sig3},
      gnsstk::YDSTime(2020,71,0,gnsstk::TimeSystem::Any),
      gnsstk::YDSTime(2020,360,0,gnsstk::TimeSystem::Any),
      { gnsstk::SatMetaData::Status::Decommissioned }));
   TUASSERTE(size_t, 1, sats.size());
   TUCATCH(sats = uut.getSatsBySignal(
      {sig3},
      gnsstk::YDSTime(2020,71,0,gnsstk::TimeSystem::Any),
      gnsstk::YDSTime(2020,360,0,gnsstk::TimeSystem::Any),
      { gnsstk::SatMetaData::Status::Decommissioned,
            gnsstk::SatMetaData::Status::Test }));
   TUASSERTE(size_t, 1, sats.size());
   TUCATCH(sats = uut.getSatsBySignal(
      {sig3},
      gnsstk::YDSTime(2004,190,0,gnsstk::TimeSystem::Any),
      gnsstk::YDSTime(2004,192,0,gnsstk::TimeSystem::Any),
      { gnsstk::SatMetaData::Status::Operational,
            gnsstk::SatMetaData::Status::Test }));
   TUASSERTE(size_t, 12, sats.size());
   TUCATCH(sats = uut.getSatsBySignal(
      {sig3},
      gnsstk::YDSTime(2004,190,0,gnsstk::TimeSystem::Any),
      gnsstk::YDSTime(2004,192,0,gnsstk::TimeSystem::Any),
      { gnsstk::SatMetaData::Status::Test }));
   TUASSERTE(size_t, 1, sats.size());

   gnsstk::SatMetaDataStore::Signal sig4(gnsstk::CarrierBand::L1,
                                         gnsstk::TrackingCode::CA,
                                         gnsstk::NavType::Any);
   TUCATCH(sats = uut.getSatsBySignal(
      {sig4},
      gnsstk::YDSTime(2020,71,0,gnsstk::TimeSystem::Any),
      gnsstk::YDSTime(2020,360,0,gnsstk::TimeSystem::Any)));
   TUASSERTE(size_t, 32, sats.size());
   TURETURN();
}


unsigned SatMetaDataStore_T ::
operatorEqSignalTest()
{
   TUDEF("SatMetaDataStore::Signal", "operator==");
   gnsstk::SatMetaDataStore::Signal
      uut1(gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV),
      uut2(gnsstk::CarrierBand::L2, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV),
      uut3(gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y,
           gnsstk::NavType::GPSLNAV),
      uut4(gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSMNAV),
      uut5(gnsstk::CarrierBand::Any, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV),
      uut6(gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Any,
           gnsstk::NavType::GPSLNAV),
      uut7(gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::Any),
      uut8(gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV);
   TUASSERTE(bool, false, uut1.operator==(uut2));
   TUASSERTE(bool, false, uut1.operator==(uut3));
   TUASSERTE(bool, false, uut1.operator==(uut4));
   TUASSERTE(bool, true, uut1.operator==(uut5));
   TUASSERTE(bool, true, uut1.operator==(uut6));
   TUASSERTE(bool, true, uut1.operator==(uut7));
   TUASSERTE(bool, true, uut1.operator==(uut8));
   TUCSM("operator!=");
   TUASSERTE(bool, true, uut1.operator!=(uut2));
   TUASSERTE(bool, true, uut1.operator!=(uut3));
   TUASSERTE(bool, true, uut1.operator!=(uut4));
   TUASSERTE(bool, false, uut1.operator!=(uut5));
   TUASSERTE(bool, false, uut1.operator!=(uut6));
   TUASSERTE(bool, false, uut1.operator!=(uut7));
   TUASSERTE(bool, false, uut1.operator!=(uut8));
   TURETURN();
}


int main()
{
   SatMetaDataStore_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.loadDataTest();
   errorTotal += testClass.findSatTest();
   errorTotal += testClass.getSVNTest();
   errorTotal += testClass.findSatBySVNTest();
   errorTotal += testClass.findSatBySlotFdmaTest();
   errorTotal += testClass.getPRNTest();
   errorTotal += testClass.getSignalSetTest();
   errorTotal += testClass.getSatsBySignalTest();
   errorTotal += testClass.operatorEqSignalTest();
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;
   return errorTotal;
}
