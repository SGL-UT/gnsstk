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
#include "YumaNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "TimeString.hpp"
#include "GPSWeekSecond.hpp"
#include "DumpDetail.hpp"

namespace gnsstk
{
   std::ostream &operator<<(std::ostream &s, gnsstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream &operator<<(std::ostream &s, gnsstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream &operator<<(std::ostream &s,
                            const gnsstk::NavMessageTypeSet &nmts)
   {
      s << "{";
      for (const auto &i : nmts)
      {
         s << " " << gnsstk::StringUtils::asString(i);
      }
      s << " }";
      return s;
   }
}

/// Class to expose NavLibrary::factories rather than making a friend.
class TestClass : public gnsstk::NavLibrary
{
public:
   gnsstk::NavDataFactoryMap &getFactories()
   {
      return factories;
   }
};

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
   {
      return navValidity;
   }
   gnsstk::NavMessageTypeSet getTypeFilter() const
   {
      return procNavTypes;
   }
   bool addDataSource(const std::string &source) override
   {
      return false;
   }
   std::string getFactoryFormats() const override
   {
      return "BUNK";
   }
};

class YumaTestFactory : public gnsstk::YumaNavDataFactory
{
public:
   gnsstk::NavValidityType getValidityFilter() const
   {
      return navValidity;
   }
   gnsstk::NavMessageTypeSet getTypeFilter() const
   {
      return procNavTypes;
   }
};

/// Automated tests for gnsstk::NavLibrary
class NavLibrary_T
{
public:
   NavLibrary_T();

   unsigned getXvtTest();
   unsigned getHealthTest();
   unsigned findTest();
   unsigned getTimeTest();
   unsigned epochTest();
   unsigned getAvailableSatsTest();
   unsigned isPresentTest();

   gnsstk::GPSWeekSecond ws;
   gnsstk::CommonTime toa, ts, te;
};

NavLibrary_T ::
    NavLibrary_T()
    // This is the ToA from the almanac
    : ws(377, 319488.0, gnsstk::TimeSystem::GPS),
      toa(ws), ts(toa - 70 * 3600), te(toa + 74 * 3600)

{
}

const std::string fn("test_input_yuma377.txt");

unsigned NavLibrary_T ::
    getXvtTest()
{
   TUDEF("NavLibraryYuma", "getXvt");
   gnsstk::Xvt xvt;
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<YumaTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() + fn;
   TUCATCH(navLib.addFactory(ndfp));
   YumaTestFactory *rndfp = dynamic_cast<YumaTestFactory *>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   // navLib.dump(std::cout, gnsstk::DumpDetail::Full);
   gnsstk::NavSatelliteID sat(5, 5, gnsstk::SatelliteSystem::GPS,
                              gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                              gnsstk::NavType::GPSLNAV);
   TUASSERT(navLib.getXvt(sat, toa, xvt, gnsstk::SVHealth::Any));
   // @note These values have not been checked for truth..
   TUASSERTFE(-9840983.9597615879029, xvt.x[0]);
   TUASSERTFE(14102042.572324583307, xvt.x[1]);
   TUASSERTFE(-20434671.102481424809, xvt.x[2]);
   TUASSERTFE(-2533.2150959557388887, xvt.v[0]);
   TUASSERTFE(-358.7017619915349087, xvt.v[1]);
   TUASSERTFE(999.01692772898309158, xvt.v[2]);
   TUASSERTFE(0.00036048889160000000204, xvt.clkbias);
   TUASSERTFE(-3.637978807000000109e-12, xvt.clkdrift);
   TUASSERTFE(1.209852803552993825e-08, xvt.relcorr);
   TUASSERTE(gnsstk::Xvt::HealthStatus, gnsstk::Xvt::Unhealthy, xvt.health);
   std::cout << "@toa " << xvt << std::endl;

   TURETURN();
}

unsigned NavLibrary_T ::
    getHealthTest()
{
   TUDEF("NavLibraryYuma", "getHealth");
   gnsstk::SVHealth health;
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<YumaTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() + fn;
   TUCATCH(navLib.addFactory(ndfp));
   YumaTestFactory *rndfp = dynamic_cast<YumaTestFactory *>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::NavSatelliteID sat(10, 10, gnsstk::SatelliteSystem::GPS,
                              gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                              gnsstk::NavType::GPSLNAV);
   TUASSERT(navLib.getHealth(sat, toa, health, gnsstk::SVHealth::Any));
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, health);
   TURETURN();
}

unsigned NavLibrary_T ::
    findTest()
{
   TUDEF("NavLibraryYuma", "find");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<YumaTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() + fn;
   gnsstk::NavDataPtr ndp;
   TUCATCH(navLib.addFactory(ndfp));
   YumaTestFactory *rndfp = dynamic_cast<YumaTestFactory *>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::NavSatelliteID sat(10, 10, gnsstk::SatelliteSystem::GPS,
                              gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                              gnsstk::NavType::GPSLNAV);
   gnsstk::GPSLNavAlm *alm;
   gnsstk::NavMessageID nmida(sat, gnsstk::NavMessageType::Almanac);
   TUASSERT(navLib.find(nmida, toa, ndp, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::ValidOnly,
                        gnsstk::NavSearchOrder::User));
   alm = dynamic_cast<gnsstk::GPSLNavAlm *>(ndp.get());
   TUASSERT(alm != nullptr);
   alm->dump(std::cout, gnsstk::DumpDetail::Full);

   TUASSERT(!navLib.find(nmida, ts,
                         ndp, gnsstk::SVHealth::Any,
                         gnsstk::NavValidityType::ValidOnly,
                         gnsstk::NavSearchOrder::User));

   TUASSERT(navLib.find(nmida, ts + 6,
                        ndp, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::ValidOnly,
                        gnsstk::NavSearchOrder::User));

   TUASSERT(navLib.find(nmida, te,
                        ndp, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::ValidOnly,
                        gnsstk::NavSearchOrder::User));

   TUASSERT(!navLib.find(nmida, te + 1,
                         ndp, gnsstk::SVHealth::Any,
                         gnsstk::NavValidityType::ValidOnly,
                         gnsstk::NavSearchOrder::User));

   TURETURN();
}

unsigned NavLibrary_T ::
    getTimeTest()
{
   TUDEF("NavLibraryYuma", "getTimes");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<YumaTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() + fn;
   TUCATCH(navLib.addFactory(ndfp));
   YumaTestFactory *rndfp = dynamic_cast<YumaTestFactory *>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   TUASSERTE(gnsstk::CommonTime, ts, navLib.getInitialTime());
   TUASSERTE(gnsstk::CommonTime, te, navLib.getFinalTime());
   TURETURN();
}

unsigned NavLibrary_T ::
    epochTest()
{
   TUDEF("NavLibraryYuma", "epoch");
   gnsstk::YumaData::nearFullWeek = 1024 + ws.week;
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<YumaTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() + fn;
   TUCATCH(navLib.addFactory(ndfp));
   YumaTestFactory *rndfp = dynamic_cast<YumaTestFactory *>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::CommonTime toa(gnsstk::GPSWeekSecond(gnsstk::YumaData::nearFullWeek, ws.sow, gnsstk::TimeSystem::GPS));
   std::cout << gnsstk::GPSWeekSecond(toa) << std::endl;
   TUASSERTE(gnsstk::CommonTime, toa - 70*3600, navLib.getInitialTime());
   gnsstk::YumaData::nearFullWeek = 0;
   TURETURN();
}

unsigned NavLibrary_T ::
    getAvailableSatsTest()
{
   TUDEF("NavLibraryYuma", "getAvailableSats");
   gnsstk::NavLibrary uut;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<YumaTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() + fn;
   gnsstk::NavSatelliteIDSet satset;
   TUCATCH(uut.addFactory(ndfp));
   YumaTestFactory *rndfp = dynamic_cast<YumaTestFactory *>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   TUCATCH(satset = uut.getAvailableSats(
               gnsstk::CommonTime::BEGINNING_OF_TIME,
               gnsstk::CommonTime::END_OF_TIME));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 30, satset.size());
   TUCATCH(satset = uut.getAvailableSats(toa, toa + 1));
   TUASSERTE(bool, true, satset.empty());
   TURETURN();
}

unsigned NavLibrary_T ::
    isPresentTest()
{
   TUDEF("NavLibraryYuma", "isPresent");
   gnsstk::NavLibrary uut;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<YumaTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() + fn;
   // really basic tests, the real tests are in NavDataFactoryWithStore_T etc
   TUCATCH(uut.addFactory(ndfp));
   YumaTestFactory *rndfp = dynamic_cast<YumaTestFactory *>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::NavSatelliteID sat1(gnsstk::SatID(23, gnsstk::SatelliteSystem::GPS));
   gnsstk::NavMessageID nmid1e(sat1, gnsstk::NavMessageType::Ephemeris),
       nmid1a(sat1, gnsstk::NavMessageType::Almanac);
   // uut.dump(std::cerr, gnsstk::DumpDetail::Brief);
   TUASSERTE(bool, false, uut.isPresent(nmid1e, toa, toa + 1));
   TUASSERTE(bool, true, uut.isPresent(nmid1a, toa, toa + 1));
   TUCSM("isTypePresent");
   TUASSERTE(bool, false, uut.isTypePresent(gnsstk::NavMessageType::Ephemeris, sat1, toa, toa + 1));
   TUASSERTE(bool, true, uut.isTypePresent(gnsstk::NavMessageType::Almanac, sat1, toa, toa + 1));
   TURETURN();
}

int main()
{
   NavLibrary_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.getHealthTest();
   errorTotal += testClass.findTest();
   errorTotal += testClass.getTimeTest();
   errorTotal += testClass.epochTest();
   errorTotal += testClass.getAvailableSatsTest();
   errorTotal += testClass.isPresentTest();
   /// @todo test edit(), clear()
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
