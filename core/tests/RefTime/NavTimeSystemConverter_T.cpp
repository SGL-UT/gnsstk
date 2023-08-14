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

#include "NavTimeSystemConverter.hpp"
#include "NavDataFactoryWithStore.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "TestUtil.hpp"
#include "CivilTime.hpp"
#include "BasicTimeSystemConverter.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

/// Fake factory used for testing addFactory
class TestFactory : public gnsstk::NavDataFactoryWithStore
{
public:
   TestFactory()
   {
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L1,
                                                 gnsstk::TrackingCode::CA,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L1,
                                                 gnsstk::TrackingCode::P,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L1,
                                                 gnsstk::TrackingCode::Y,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L2,
                                                 gnsstk::TrackingCode::P,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L2,
                                                 gnsstk::TrackingCode::Y,
                                                 gnsstk::NavType::GPSLNAV));
   }
   gnsstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gnsstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
   bool addDataSource(const std::string& source) override
   { return false; }
   std::string getFactoryFormats() const override
   { return "Test"; }
};


class NavTimeSystemConverter_T
{
public:
   NavTimeSystemConverter_T();

   unsigned getOffsetTest();

   gnsstk::CivilTime civ;
   gnsstk::CommonTime ct;
};


NavTimeSystemConverter_T ::
NavTimeSystemConverter_T()
      : civ(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned NavTimeSystemConverter_T ::
getOffsetTest()
{
   TUDEF("NavTimeSystemConverter", "getOffset");
   std::shared_ptr<gnsstk::NavLibrary> navLib =
      std::make_shared<gnsstk::NavLibrary>();
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<TestFactory>());
   TestFactory *fact1 = dynamic_cast<TestFactory*>(ndfp.get());
   gnsstk::NavDataPtr navOut = std::make_shared<gnsstk::GPSLNavTimeOffset>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gnsstk::NavMessageType::TimeOffset;
   gnsstk::GPSLNavTimeOffset *toptr = dynamic_cast<gnsstk::GPSLNavTimeOffset*>(
      navOut.get());
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs.band = gnsstk::CarrierBand::L1;
   navOut->signal.obs.code = gnsstk::TrackingCode::CA;
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;
   navOut->signal.sat = gnsstk::SatID(23,gnsstk::SatelliteSystem::GPS);
   navOut->signal.xmitSat = gnsstk::SatID(32,gnsstk::SatelliteSystem::GPS);
   toptr->deltatLS = 17; // set a simple, easy to verify value.
   toptr->refTime = ct;
   TUASSERT(fact1->addNavData(navOut));
   TUCATCH(navLib->addFactory(ndfp));
   gnsstk::NavTimeSystemConverter uut;
   uut.navLib = navLib;
   double result = 0;
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                          ct+35, result));
   TUASSERTFE(17.0, result);
      // Expect the NavTimeSystemConverter to match BasicTimeSystemConverter
      // for this given data. At the very least, they should have the same
      // sign.
   gnsstk::BasicTimeSystemConverter btsc;
   double result2;
   TUASSERT(btsc.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                           ct+35, result2));
   TUASSERTFE(result2, result);
      // reverse the conversion and expect positive.
   gnsstk::CommonTime utc35(ct+35);
   utc35.setTimeSystem(gnsstk::TimeSystem::UTC);
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GPS,
                          utc35, result));
   TUASSERTFE(-17.0, result);
      // expect this to not work
   TUASSERT(!uut.getOffset(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::BDT,
                           utc35, result));
   TURETURN();
}


int main() //Main function to initialize and run all tests above
{
   NavTimeSystemConverter_T testClass;
   unsigned errorCounter = 0;

   errorCounter += testClass.getOffsetTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter
             << std::endl;

   return errorCounter; //Return the total number of errors
}
