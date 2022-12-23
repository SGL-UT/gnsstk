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

#include <iostream>
#include <string>

#include "CommonTime.hpp"
#include "Xvt.hpp"
#include "Triple.hpp"
#include "SatID.hpp"
#include "ord.hpp"
#include "NBTropModel.hpp"
#include "ord.hpp"
#include "TestUtil.hpp"
#include "RinexNavDataFactory.hpp"

using namespace gnsstk::ord;

class OrdUnitTests_T
{
public:
   OrdUnitTests_T();
   unsigned testBasicIonosphereFreeRange();
   unsigned testBasicIonosphereFreeRangeRequiresMoreThanOne();
   unsigned testBasicIonosphereFreeRangeRejectsHigherThanDual();
   unsigned testBasicIonosphereFreeRangeRejectsSizeMismatch();
   unsigned testGetXvtFromStore();
   unsigned testRawRange1();
   unsigned testRawRange1HandlesException();
   unsigned testRawRange2();
   unsigned testRawRange2HandlesException();
   unsigned testRawRange3();
   unsigned testRawRange3HandlesException();
   unsigned testRawRange4();
   unsigned testRawRange4HandlesException();
   unsigned testSvRelativityCorrection();
   unsigned testTropoCorrection();
   unsigned testIonoCorrection();

   gnsstk::NavLibrary navLib;
};

OrdUnitTests_T ::
OrdUnitTests_T()
      : navLib()
{
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<gnsstk::RinexNavDataFactory>());

   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";

   navLib.addFactory(ndfp);

   gnsstk::RinexNavDataFactory *rndfp =
      dynamic_cast<gnsstk::RinexNavDataFactory*>(ndfp.get());

   GNSSTK_ASSERT(rndfp->addDataSource(fname));
}


unsigned OrdUnitTests_T ::
testBasicIonosphereFreeRange()
{
   TUDEF("ORD", "IonosphereFreeRange");
   static const double arr[] = {1.0, 2.0};

   std::vector<double> frequencies(std::begin(arr), std::end(arr));
   std::vector<double> pseudoranges(std::begin(arr), std::end(arr));

   double return_value = IonosphereFreeRange(frequencies, pseudoranges);

   TUASSERT(return_value > 0);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testBasicIonosphereFreeRangeRequiresMoreThanOne()
{
   TUDEF("ORD", "IonosphereFreeRange");
   static const double arr[] = {1.0};

   std::vector<double> frequencies(std::begin(arr), std::end(arr));
   std::vector<double> pseudoranges(std::begin(arr), std::end(arr));

   TUTHROW(IonosphereFreeRange(frequencies, pseudoranges));
   TURETURN();
}

unsigned OrdUnitTests_T ::
testBasicIonosphereFreeRangeRejectsHigherThanDual()
{
   TUDEF("ORD", "IonosphereFreeRange");
   static const double arr[] = {1.0, 2.0, 3.0, 4.0, 5.0};

   std::vector<double> frequencies(std::begin(arr), std::end(arr));
   std::vector<double> pseudoranges(std::begin(arr), std::end(arr));

   TUTHROW(IonosphereFreeRange(frequencies, pseudoranges));
   TURETURN();

}

unsigned OrdUnitTests_T ::
testBasicIonosphereFreeRangeRejectsSizeMismatch()
{
   TUDEF("ORD", "IonosphereFreeRange");
   static const double arr[] = {1.0, 2.0, 3.0, 4.0, 5.0};
   static const double shortarr[] = {1.0, 2.0, 3.0, 4.0};

   std::vector<double> frequencies(std::begin(shortarr), std::end(shortarr));
   std::vector<double> pseudoranges(std::begin(arr), std::end(arr));

   TUTHROW(IonosphereFreeRange(frequencies, pseudoranges));
   TURETURN();
}

unsigned OrdUnitTests_T ::
testGetXvtFromStore()
{
   TUDEF("ORD", "getSvXvt");

   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));

   gnsstk::Xvt xvt = getSvXvt(satId, time+35, navLib);

   TUASSERTFESMRT(9345531.5274733770639,  xvt.x[0]);
   TUASSERTFESMRT(-12408177.088141856715, xvt.x[1]);
   TUASSERTFESMRT(21486320.848036296666,  xvt.x[2]);
   TUASSERTFESMRT(2081.276961058104007,   xvt.v[0]);
   TUASSERTFESMRT(1792.4445008638492709,  xvt.v[1]);
   TUASSERTFESMRT(148.29209115082824155,  xvt.v[2]);
   TUASSERTFESMRT(-0.00021641018042870913346, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8197758101551758427e-09, xvt.relcorr);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange1()
{
   TUDEF("ORD", "RawRange1");

   gnsstk::Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));

   gnsstk::Xvt xvt;
   double range = RawRange1(rxLocation, satId, time, navLib, xvt);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERTFESMRT(9272533.3762740660459,  xvt.x[0]);
   TUASSERTFESMRT(-12471159.893898375332, xvt.x[1]);
   TUASSERTFESMRT(21480836.886172864586,  xvt.x[2]);
   TUASSERTFESMRT(2077.3531590469665389,  xvt.v[0]);
   TUASSERTFESMRT(1796.0597201257969573,  xvt.v[1]);
   TUASSERTFESMRT(164.40771415613258455,  xvt.v[2]);
   TUASSERTFESMRT(-0.00021641042654059759786, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8008986915573278037e-09, xvt.relcorr);
   TUASSERTFESMRT(22289257.145863413811, range);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange1HandlesException()
{
   TUDEF("ORD", "RawRange1");

   gnsstk::Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
      // PRN 1 should not exist in the Nav file
   gnsstk::SatID satId(1, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   TUTHROW(RawRange1(rxLocation, satId, time, navLib, xvt));
   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange2()
{
   TUDEF("ORD", "RawRange2");

   gnsstk::Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   double pseduorange = 2e7;
   double range = RawRange2(pseduorange, rxLocation, satId, time, navLib, xvt);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERTFESMRT(9272549.688764328137,   xvt.x[0]);
   TUASSERTFESMRT(-12471145.790274851024, xvt.x[1]);
   TUASSERTFESMRT(21480838.177179995924,  xvt.x[2]);
   TUASSERTFESMRT(2077.3540461841112119,  xvt.v[0]);
   TUASSERTFESMRT(1796.058914385570688,  xvt.v[1]);
   TUASSERTFESMRT(164.40410655412986785,  xvt.v[2]);
   TUASSERTFESMRT(-0.000216410426506673752147, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8009032843852035206e-09, xvt.relcorr);
   TUASSERTFESMRT(22289260.787328250706, range);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange2HandlesException()
{
   TUDEF("ORD", "RawRange2");

   gnsstk::Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(1, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;
   double pseduorange = 2e7;

   TUTHROW(RawRange2(pseduorange, rxLocation, satId, time, navLib, xvt));
   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange3()
{
   TUDEF("ORD", "RawRange3");

   gnsstk::Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   double pseduorange = 2e7;
   double range = RawRange3(pseduorange, rxLocation, satId, time, navLib, xvt);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERTFESMRT(9272694.5732459314167,  xvt.x[0]);
   TUASSERTFESMRT(-12471021.342128152028, xvt.x[1]);
   TUASSERTFESMRT(21480849.108445473015,  xvt.x[2]);
   TUASSERTFESMRT(2077.3605866395218982,  xvt.v[0]);
   TUASSERTFESMRT(1796.0532152374469206,  xvt.v[1]);
   TUASSERTFESMRT(164.37355694668559636,  xvt.v[2]);
   TUASSERTFESMRT(-0.00021641042621940267715, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8009421765298224418e-09, xvt.relcorr);
   TUASSERTFESMRT(22289292.96130572632, range);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange3HandlesException()
{
   TUDEF("ORD", "RawRange3");

   gnsstk::Position rxLocation(10, 10, 0);
   gnsstk::SatID satId(1, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   double pseduorange = 2e7;
   TUTHROW(RawRange3(pseduorange, rxLocation, satId, time, navLib, xvt));

   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange4()
{
   TUDEF("ORD", "RawRange4");

   gnsstk::Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   double range = RawRange4(rxLocation, satId, time, navLib, xvt);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERTFESMRT(9272533.3759945072234, xvt.x[0]);
   TUASSERTFESMRT(-12471159.894135156646, xvt.x[1]);
   TUASSERTFESMRT(21480836.886153958738, xvt.x[2]);
   TUASSERTFESMRT(2077.353159039827915, xvt.v[0]);
   TUASSERTFESMRT(1796.0597201308269177, xvt.v[1]);
   TUASSERTFESMRT(164.40771420896862764, xvt.v[2]);
   TUASSERTFESMRT(-0.00021641042654059808575, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8008986914900614763e-09, xvt.relcorr);
   TUASSERTFESMRT(22289257.145802032202, range);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testRawRange4HandlesException()
{
   TUDEF("ORD", "RawRange4");

   gnsstk::Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(1, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   TUTHROW(RawRange4(rxLocation, satId, time, navLib, xvt));
   TURETURN();
}

unsigned OrdUnitTests_T ::
testSvRelativityCorrection()
{
   class MockXvt : public gnsstk::Xvt
   {
   public:
      double computeRelativityCorrection()
      {
         return 5.6;
      }
   };

   TUDEF("ORD", "SvRelativityCorrection");
   MockXvt svXvt;

   double relcorr = SvRelativityCorrection(svXvt);

   TUASSERTFE(-1678837764.7999999523, relcorr);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testTropoCorrection()
{
   class MockTropo : public gnsstk::NBTropModel
   {
      double correction(double elevation) const
      {
         return 42.0;
      }
   };

   TUDEF("ORD", "TroposphereCorrection");
   gnsstk::Xvt fakeXvt;
   fakeXvt.x = gnsstk::Triple(100, 100, 100);
   fakeXvt.v = gnsstk::Triple(0, 0, 0);
   MockTropo tropo;

   gnsstk::Position rxLocation(10, 10, 0);

   double return_value = TroposphereCorrection(tropo, rxLocation, fakeXvt);

   TUASSERTFE(return_value, 42.0);
   TURETURN();
}

unsigned OrdUnitTests_T ::
testIonoCorrection()
{
   class MockIono : public gnsstk::IonoModelStore
   {
   public:
      double getCorrection(
               const gnsstk::CommonTime& time,
               const gnsstk::Position& rxgeo,
               double svel,
               double svaz,
               gnsstk::CarrierBand band) const override
      {
         return 42.0;
      }

   };

   TUDEF("ORD", "IonosphereModelCorrection");
   gnsstk::Xvt fakeXvt;
   fakeXvt.x = gnsstk::Triple(100, 100, 100);
   fakeXvt.v = gnsstk::Triple(0, 0, 0);
   gnsstk::CommonTime time(gnsstk::CommonTime::BEGINNING_OF_TIME);
   MockIono iono;

   gnsstk::Position rxLocation(10, 10, 0);

   double return_value = IonosphereModelCorrection(
      iono, time, gnsstk::CarrierBand::L1, rxLocation, fakeXvt);

   TUASSERTFE(return_value, -42.0);

   TURETURN();
}

int main()
{
   OrdUnitTests_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testBasicIonosphereFreeRange();
   errorTotal += testClass.testBasicIonosphereFreeRangeRequiresMoreThanOne();
   errorTotal += testClass.testBasicIonosphereFreeRangeRejectsHigherThanDual();
   errorTotal += testClass.testBasicIonosphereFreeRangeRejectsSizeMismatch();
   errorTotal += testClass.testGetXvtFromStore();
   errorTotal += testClass.testRawRange1();
   errorTotal += testClass.testRawRange1HandlesException();
   errorTotal += testClass.testRawRange2();
   errorTotal += testClass.testRawRange2HandlesException();
   errorTotal += testClass.testRawRange3();
   errorTotal += testClass.testRawRange3HandlesException();
   errorTotal += testClass.testRawRange4();
   errorTotal += testClass.testRawRange4HandlesException();
   errorTotal += testClass.testSvRelativityCorrection();
   errorTotal += testClass.testTropoCorrection();
   errorTotal += testClass.testIonoCorrection();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
