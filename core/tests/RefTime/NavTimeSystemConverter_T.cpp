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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#include <iostream>
#include <sstream>
#include <cmath>

/// Fake factory used for testing addFactory
class TestFactory : public gpstk::NavDataFactoryWithStore
{
public:
   TestFactory()
   {
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::CA,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::P,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::Y,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L2,
                                                 gpstk::TrackingCode::P,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L2,
                                                 gpstk::TrackingCode::Y,
                                                 gpstk::NavType::GPSLNAV));
   }
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
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

   gpstk::CivilTime civ;
   gpstk::CommonTime ct;
};


NavTimeSystemConverter_T ::
NavTimeSystemConverter_T()
      : civ(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned NavTimeSystemConverter_T ::
getOffsetTest()
{
   TUDEF("NavTimeSystemConverter", "getOffset");
   std::shared_ptr<gpstk::NavLibrary> navLib =
      std::make_shared<gpstk::NavLibrary>();
   gpstk::NavDataFactoryPtr ndfp(std::make_shared<TestFactory>());
   TestFactory *fact1 = dynamic_cast<TestFactory*>(ndfp.get());
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavTimeOffset>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::TimeOffset;
   gpstk::GPSLNavTimeOffset *toptr = dynamic_cast<gpstk::GPSLNavTimeOffset*>(
      navOut.get());
   navOut->signal.system = gpstk::SatelliteSystem::GPS;
   navOut->signal.obs.band = gpstk::CarrierBand::L1;
   navOut->signal.obs.code = gpstk::TrackingCode::CA;
   navOut->signal.nav = gpstk::NavType::GPSLNAV;
   navOut->signal.sat = gpstk::SatID(23,gpstk::SatelliteSystem::GPS);
   navOut->signal.xmitSat = gpstk::SatID(32,gpstk::SatelliteSystem::GPS);
   toptr->deltatLS = 23; // set a simple, easy to verify value.
   TUASSERT(fact1->addNavData(navOut));
   TUCATCH(navLib->addFactory(ndfp));
   gpstk::NavTimeSystemConverter uut;
   uut.navLib = navLib;
   double result = 0;
   TUASSERT(uut.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
                          ct+35, result));
   TUASSERTFE(23.0, result);
      // reverse the conversion and expect negative.
   TUASSERT(uut.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::GPS,
                          ct+35, result));
   TUASSERTFE(-23.0, result);
      // expect this to not work
   TUASSERT(!uut.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::BDT,
                           ct+35, result));
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
