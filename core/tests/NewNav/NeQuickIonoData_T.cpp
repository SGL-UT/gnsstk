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

#include "TestUtil.hpp"
#include "NeQuickIonoData.hpp"
#include "MODIP.hpp"
#include "DebugTrace.hpp"

/// convert test data time stamps that EU used to CivilTime object
gpstk::CivilTime convertTime(double hour, int month)
{
      // convert hour/month to a usable CivilTime object
   int hr = trunc(hour);
   double soh = (hour-hr)*3600;
   int min = trunc(soh/60.0);
   double sec = fmod(soh,60.0);
   gpstk::CivilTime ct(2525,month,1,hr,min,sec,gpstk::TimeSystem::UTC);
   GPSTK_ASSERT(fabs(ct.getUTHour()-hour) < 1e-7);
   return ct;
}

class NeQuickIonoData_T
{
public:
   NeQuickIonoData_T();

      /// Test the NeQuickIonoData constructor
   unsigned constructorTest();
      /// Test NequickIonoData::getEffIonoLevel
   unsigned getEffIonoLevelTest();
      /// Test the NeQuickIonoData::ModelParameters constructor
   unsigned constructor2Test();
      /// Test NeQuickIonoData::ModelParameters::legendre
   unsigned legendreTest();
      /// Test NeQuickIonoData::ModelParameters::height
   unsigned heightTest();
      /// Test NeQuickIonoData::ModelParameters::exosphereAdjust
   unsigned exosphereAdjustTest();
      /// Test NeQuickIonoData::ModelParameters::peakAmplitudes
   unsigned peakAmplitudesTest();
      /// Test NeQuickIonoData::ModelParameters::effSolarZenithAngle
   unsigned effSolarZenithAngleTest();
      /// Test NeQuickIonoData::ModelParameters::thickness
   unsigned thicknessTest();

      /// Hold input/truth data for legendreTest
   class TestData
   {
   public:
      TestData(double hour, int month, double iazr, double lon, double lat,
               double cf, double tf)
            : tAzr(iazr), tPos(lat,lon,0,gpstk::Position::Geodetic),
              criticalFreq(cf), transFactor(tf), ct(convertTime(hour, month))
      {}
      gpstk::CivilTime ct;
      double tAzr;
      gpstk::Position tPos;
      double criticalFreq;
      double transFactor;
   };

      /// Hold input/truth data for heightTest
   class TestDataHeight
   {
   public:
      TestDataHeight(double tf, double cfF2, double cfE, double h)
            : transFactor(tf), criticalFreqF2(cfF2), criticalFreqE(cfE),
              height(h)
      {}
      double transFactor;
      double criticalFreqF2;
      double criticalFreqE;
      double height;
   };

      /** Hold input/truth data for exosphereAdjust method.
       * @note This test data adds the Azr data as an explicit piece
       * of test data while the EU code pulls the value from a
       * different test. */
   class TestDataExosphere
   {
   public:
      TestDataExosphere(double ed, double iazr, double h, double b, double hr,
                        int mn, double t)
            : electronDensity(ed), tAzr(iazr), height(h), bottom(b), hour(hr),
              month(mn), top(t)
      {
      }
      double electronDensity;
      double tAzr;
      double height;
      double bottom;
      double hour;
      int month;
      double top;
   };

      /// Hold input/truth data for peakAmplitudes method.
   class TestDataAmplitude
   {
   public:
      TestDataAmplitude(double edF2, double phF2, double ptF2, double cfF1,
                        double edF1, double phF1, double ptF1, double edE,
                        double phE, double ptE, double eA1, double eA2,
                        double eA3)
            : electronDensityF2(edF2), peakHeightF2(phF2), peakThickBotF2(ptF2),
              critFreqF1(cfF1), electronDensityF1(edF1), peakHeightF1(phF1),
              peakThickBotF1(ptF1), electronDensityE(edE), peakHeightE(phE),
              peakThickTopE(ptE), expA1(eA1), expA2(eA2), expA3(eA3)
      {}
      double electronDensityF2;
      double peakHeightF2;
      double peakThickBotF2;
      double critFreqF1;
      double electronDensityF1;
      double peakHeightF1;
      double peakThickBotF1;
      double electronDensityE;
      double peakHeightE; // EU test data specifies this but it's a constant.
      double peakThickTopE;
      double expA1;
      double expA2;
      double expA3;
   };

      /// Hold input/truth data for effSolarZenithAngle method.
   class TestDataSolar
   {
   public:
      TestDataSolar(double lon, double lat, double hour, int month,
                    double expect)
            : pos(lat,lon,0,gpstk::Position::Geodetic),
              ct(convertTime(hour,month)), expAngle(expect)
      {}
      gpstk::Position pos;
      gpstk::CivilTime ct;
      double expAngle;
   };

      /// Hold input/truth data for testing E layer critical freq computation
   class TestDataCritFreqE
   {
   public:
      TestDataCritFreqE(double az, double lon, double lat, double hour,
                        int month, double expect)
            : tAz(az), pos(lat,lon,0,gpstk::Position::Geodetic),
              ct(convertTime(hour,month)), expFreq(expect)
      {
      }
      double tAz;
      gpstk::Position pos;
      gpstk::CivilTime ct;
      double expFreq;
   };

      /// Hold input/truth data for testing solar flux->eff. ionization
   class TestDataAz
   {
   public:
      TestDataAz(const std::vector<double>& coeff, double mdl, double expect)
            : coefficients(coeff), modip(mdl), expAz(expect)
      {}
      std::vector<double> coefficients;
      double modip;
      double expAz;
   };

      /// Hold input/truth data for testing layer thickness computation
   class TestDataThickness
   {
   public:
      TestDataThickness(double tf, double cf, double ed, double ph,
                        double pttf2, double ptbf2, double pttf1, double ptbf1,
                        double ptte, double ptbe)
            : transFactor(tf), critFreqF2(cf), electronDensityF2(ed),
              peakHeightF2(ph), peakThickTopF2(pttf2), peakThickBotF2(ptbf2),
              peakThickTopF1(pttf1), peakThickBotF1(ptbf1), peakThickTopE(ptte),
              peakThickBotE(ptbe)
      {}
      double transFactor;
      double critFreqF2;
      double electronDensityF2;
      double peakHeightF2;
         // There is no top for F2 layer, but the EU code specifies
         // thickness in pairs of top/bottom, and in the interest of
         // maintaining consistency with the original test code, it's
         // here.
      double peakThickTopF2;
      double peakThickBotF2;
      double peakThickTopF1;
      double peakThickBotF1;
      double peakThickTopE;
      double peakThickBotE; // EU test data specifies this but it's a constant.
   };

      /// Input/truth data for legendreTest
   static const TestData testData[];
      /// Input/truth data for heightTest
   static const TestDataHeight testDataHeight[];
      /// Input/truth data for exosphereAdjustTest
   static const TestDataExosphere testDataExosphere[];
      /// Input/truth data for peakAmplitudesTest
   static const TestDataAmplitude testDataAmplitude[];
      /// Input/truth data for effSolarZenithAngleTest
   static const TestDataSolar testDataSolar[];
      /// Input/truth data for constructor2Test
   static const TestDataCritFreqE testDataCFE[];
      /// Solar flux coefficients for high solar activity.
   static const std::vector<double> highSolarCoeff;
      /// Solar flux coefficients for medium solar activity.
   static const std::vector<double> mediumSolarCoeff;
      /// Solar flux coefficients for low solar activity.
   static const std::vector<double> lowSolarCoeff;
      /// Input/truth data for getEffIonoLevelTest
   static const TestDataAz testDataAz[];
      /// Input/truth data for thicknessTest
   static const TestDataThickness testDataThickness[];
      /// Tool for computing modified dip latitude.
   gpstk::MODIP modip;
      /// Tool for looking up iono model data.
   gpstk::CCIR ccir;
      /// Epsilon for critical frequency checks.
   static const double criticalFreqEps;
      /// Epsilon for trans factor checks.
   static const double transFactorEps;
      /// Epsilon for peak amplitude checks.
   static const double amplitudeEps;
      /// Epsilon for solar effective zenith angle checks.
   static const double solarEps;
      /// Epsilon for effective ionization level checks.
   static const double azEps;
      /// Epsilon for peak thickness checks
   static const double thicknessEps;
};


// epsilons (thresholds) and test data pulled from
// NeQuickG_JRC_iono_F2_layer_test.c
// NeQuickG_JRC_iono_layer_amplitudes_test.c
// NeQuickG_JRC_Az_test.c
// NeQuickG_JRC_iono_layer_thickness_test.c
const double NeQuickIonoData_T::criticalFreqEps = 1e-5;
const double NeQuickIonoData_T::transFactorEps = 1e-5;
const double NeQuickIonoData_T::amplitudeEps = 1e-5;
const double NeQuickIonoData_T::solarEps = 1e-5;
const double NeQuickIonoData_T::azEps = 1e-6;
const double NeQuickIonoData_T::thicknessEps = 1e-5;

const NeQuickIonoData_T::TestData NeQuickIonoData_T::testData[] =
{
   {
      0.0, 4,
      186.328060298,
      297.438314733,
      82.481346266,
      6.575612801,
      2.366982653
   },
   {
      2.666667, 4,
      186.328060298,
      378.105699987,
      74.653105616,
      6.289174069,
      2.375445167
   },
   {
      10.500000, 4,
      203.036737976,
      31.022679098,
      -8.649092730,
      16.638422783,
      2.244593590
   },
};


const NeQuickIonoData_T::TestDataHeight NeQuickIonoData_T::testDataHeight[] =
{
   {
      2.366982653,
      6.575612801,
      2.409694059,
      412.551921084
   },
};


const NeQuickIonoData_T::TestDataExosphere
NeQuickIonoData_T::testDataExosphere[] =
{
   {
      5.361596779,
      186.328060298,
      412.551921084,
      45.987213389,
      0.0, 4,
      50.657704983
   },
};


const NeQuickIonoData_T::TestDataAmplitude
NeQuickIonoData_T::testDataAmplitude[] =
{
   {
      5.361596779,
      412.551921084,
      45.987213389,

      3.373571683,
      1.411242252,
      266.275960542,
      73.137980271,

      0.720021557,
      120.000000000,
      73.137980271,

      21.446387117,
      1.310172197,
      2.182239112,
   },
   {
      5.107156754,
      429.080085583,
      46.507547598,

      0.000000000,
      0.000000000,
      274.540042792,
      77.270021396,

      0.466645104,
      120.000000000,
      77.270021396,

      20.428627016,
      0.000000000,
      1.760670936
   }
};


/** @note I've discarded the extraneous (unused, set to zero) data
 * that was present in the EU test code */
const NeQuickIonoData_T::TestDataSolar NeQuickIonoData_T::testDataSolar[] =
{
   {
      297.438315, 82.481346,
      0.000000, 4,
      82.978526
   },
   {
      139.790965, 26.595660,
      0.833333, 4,
      30.664744
   },
   {
      298.035128, 82.566581,
      12.0, 4,
      76.209610
   },
   {
      260.236348, 35.803032,
      23.833333, 4,
      74.148769
   },
   {
      114.989502, -50.092818,
      23.833333, 4,
      83.933196
   },
};


/** @note I've discarded the extraneous (unused, set to zero) data
 * that was present in the EU test code */
const NeQuickIonoData_T::TestDataCritFreqE NeQuickIonoData_T::testDataCFE[] =
{
   {
      230.245562,
      297.438315, 82.481346,
      0.0, 4,
      2.409694
   },
   {
      248.199580,
      40.173216, -3.023059,
      1.0, 4,
      0.726159
   },
   {
      285.871846,
      137.301902, -82.090099,
      11.0, 4,
      0.971728
   },
};


const std::vector<double> NeQuickIonoData_T::highSolarCoeff
{236.831641, -0.39362878, 0.00402826613};

const std::vector<double> NeQuickIonoData_T::mediumSolarCoeff
{121.129893, 0.351254133, 0.0134635348};

const std::vector<double> NeQuickIonoData_T::lowSolarCoeff
{2.580271, 0.127628236, 0.0252748384};


const NeQuickIonoData_T::TestDataAz NeQuickIonoData_T::testDataAz[] =
{
   { highSolarCoeff, 76.284073, 230.245562 },
   { mediumSolarCoeff, 76.284073, 226.272795 },
   { lowSolarCoeff, 76.284073, 159.397123 },
   { highSolarCoeff, 19.528774, 230.680826 },
   { mediumSolarCoeff, 19.528774, 133.124084 },
   { lowSolarCoeff, 19.528774, 14.711835 },
   { highSolarCoeff, 47.857688, 227.219650 },
   { mediumSolarCoeff, 47.857688, 168.776422 },
   { lowSolarCoeff, 47.857688, 66.576699 },
   { highSolarCoeff, -23.316310, 248.199580 },
   { mediumSolarCoeff, -23.316310, 120.259398 },
   { lowSolarCoeff, -23.316310, 13.345125 },
   { highSolarCoeff, -71.811295, 285.871846 },
   { mediumSolarCoeff, -71.811295, 165.335471 },
   { lowSolarCoeff, -71.811295, 123.753978 },
   { highSolarCoeff, 46.487417, 227.238261 },
   { mediumSolarCoeff, 46.487417, 166.554565 },
   { lowSolarCoeff, 46.487417, 63.134324 },
   { highSolarCoeff, 33.054571, 228.221713 },
   { mediumSolarCoeff, 33.054571, 147.450769 },
   { lowSolarCoeff, 33.054571, 34.414374 },
   { highSolarCoeff, -51.379818, 267.690378 },
   { mediumSolarCoeff, -51.379818, 138.624713 },
   { lowSolarCoeff, -51.379818, 62.745440 }
};


const NeQuickIonoData_T::TestDataThickness
NeQuickIonoData_T::testDataThickness[] =
{
   {
      2.366982653,
      6.5756128,
      5.361596779,
      412.551921084,
      0.0, 45.987213389,
      43.882788163, 73.137980271,
      73.137980271, 5.000000000,
   },
};


NeQuickIonoData_T ::
NeQuickIonoData_T()
{
}


unsigned NeQuickIonoData_T ::
constructorTest()
{
   TUDEF("NeQuickIonoData", "NeQuickIonoData()");
   gpstk::NeQuickIonoData uut;
   TUASSERTFE(0.0, uut.ai[0]);
   TUASSERTFE(0.0, uut.ai[1]);
   TUASSERTFE(0.0, uut.ai[2]);
   TUASSERTE(bool, false, uut.idf[0]);
   TUASSERTE(bool, false, uut.idf[1]);
   TUASSERTE(bool, false, uut.idf[2]);
   TUASSERTE(bool, false, uut.idf[3]);
   TUASSERTE(bool, false, uut.idf[4]);
   TURETURN();
}


unsigned NeQuickIonoData_T ::
getEffIonoLevelTest()
{
   TUDEF("NeQuickIonoData", "getEffIonoLevel");
   unsigned numTests = sizeof(testDataAz)/sizeof(testDataAz[0]);
   gpstk::NeQuickIonoData uut;
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataAz& td(testDataAz[testNum]);
      uut.ai[0] = td.coefficients[0];
      uut.ai[1] = td.coefficients[1];
      uut.ai[2] = td.coefficients[2];
      TUASSERTFEPS(td.expAz, uut.getEffIonoLevel(td.modip), azEps);
   }
   TURETURN();
}

unsigned NeQuickIonoData_T ::
constructor2Test()
{
   TUDEF("NeQuickIonoData::ModelParameters", "constructor");
   unsigned numTests = sizeof(testDataCFE)/sizeof(testDataCFE[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataCritFreqE& td(testDataCFE[testNum]);
      gpstk::NeQuickIonoData::ModelParameters uut(
         0, td.pos, td.tAz, ccir, td.ct);
      TUASSERTFEPS(td.expFreq, uut.ffoE, criticalFreqEps);
   }
   TURETURN();
}


unsigned NeQuickIonoData_T ::
legendreTest()
{
   TUDEF("NeQuickIonoData::ModelParameters", "legendre");
   unsigned numTests = sizeof(testData)/sizeof(testData[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestData& td(testData[testNum]);
      double modip_u = modip.stModip(td.tPos);
      gpstk::NeQuickIonoData::ModelParameters uut(
         modip_u, td.tPos, td.tAzr, ccir, td.ct);
      TUASSERTFEPS(td.criticalFreq, uut.ffoF2, criticalFreqEps);
      TUASSERTFEPS(td.transFactor, uut.fM3000F2, transFactorEps);
   }
   TURETURN();
}


unsigned NeQuickIonoData_T ::
heightTest()
{
   TUDEF("NeQuickIonoData::ModelParameters", "height");
   unsigned numTests = sizeof(testDataHeight)/sizeof(testDataHeight[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataHeight& td(testDataHeight[testNum]);
      gpstk::NeQuickIonoData::ModelParameters uut(ccir);
      uut.fM3000F2 = td.transFactor;
      uut.ffoF2 = td.criticalFreqF2;
      uut.ffoE = td.criticalFreqE;
      TUCATCH(uut.height());
      TUASSERTFEPS(td.height, uut.fhmF2, transFactorEps);
   }
   TURETURN();
}


unsigned NeQuickIonoData_T ::
exosphereAdjustTest()
{
   TUDEF("NeQuickIonoData::ModelParameters", "exosphereAdjust");
   unsigned numTests = sizeof(testDataExosphere)/sizeof(testDataExosphere[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataExosphere& td(testDataExosphere[testNum]);
      gpstk::NeQuickIonoData::ModelParameters uut(ccir);
      uut.fNmF2 = td.electronDensity;
      uut.fB2bot = td.bottom;
      uut.fhmF2 = td.height;
      uut.fAzr = td.tAzr;
      TUCATCH(uut.exosphereAdjust(td.month));
      TUASSERTFEPS(td.top, uut.fH0, transFactorEps);
   }
   TURETURN();
}


unsigned NeQuickIonoData_T ::
peakAmplitudesTest()
{
   TUDEF("NeQuickIonoData::ModelParameters", "peakAmplitudes");
   unsigned numTests = sizeof(testDataAmplitude)/sizeof(testDataAmplitude[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataAmplitude& td(testDataAmplitude[testNum]);
      gpstk::NeQuickIonoData::ModelParameters uut(ccir);
      uut.fNmE = td.electronDensityE;
      uut.fBEtop = td.peakThickTopE;
      uut.ffoF1 = td.critFreqF1;
      uut.fNmF1 = td.electronDensityF1;
      uut.fhmF1 = td.peakHeightF1;
      uut.fB1bot = td.peakThickBotF1;
      uut.fNmF2 = td.electronDensityF2;
      uut.fhmF2 = td.peakHeightF2;
      uut.fB2bot = td.peakThickBotF2;
      TUCATCH(uut.peakAmplitudes());
      TUASSERTFEPS(td.expA1, uut.fA1, amplitudeEps);
      TUASSERTFEPS(td.expA2, uut.fA2, amplitudeEps);
      TUASSERTFEPS(td.expA3, uut.fA3, amplitudeEps);
   }
   TURETURN();
}


unsigned NeQuickIonoData_T ::
effSolarZenithAngleTest()
{
   TUDEF("NeQuickIonoData::ModelParameters", "effSolarZenithAngle");
   unsigned numTests = sizeof(testDataSolar)/sizeof(testDataSolar[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataSolar& td(testDataSolar[testNum]);
      gpstk::NeQuickIonoData::ModelParameters uut(ccir);
      TUASSERTFEPS(td.expAngle,
                   uut.effSolarZenithAngle(td.pos, td.ct),
                   solarEps);
   }
   TURETURN();
}


unsigned NeQuickIonoData_T ::
thicknessTest()
{
   TUDEF("NeQuickIonoData::ModelParameters", "thickness");
   unsigned numTests = sizeof(testDataThickness)/sizeof(testDataThickness[0]);
      /// E layer maximum density height in km.
   constexpr double hmE = 120.0;                                        //eq.78
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataThickness& td(testDataThickness[testNum]);
      gpstk::NeQuickIonoData::ModelParameters uut(ccir);
      uut.fM3000F2 = td.transFactor;
      uut.ffoF2 = td.critFreqF2;
      uut.fNmF2 = td.electronDensityF2;
      uut.fhmF2 = td.peakHeightF2;
         // normally set in height() method
      uut.fhmF1 = (uut.fhmF2+hmE) / 2.0;                                //eq.79
      TUCATCH(uut.thickness());
      TUASSERTFEPS(td.peakThickBotF2, uut.fB2bot, thicknessEps);
      TUASSERTFEPS(td.peakThickTopF1, uut.fB1top, thicknessEps);
      TUASSERTFEPS(td.peakThickBotF1, uut.fB1bot, thicknessEps);
      TUASSERTFEPS(td.peakThickTopE, uut.fBEtop, thicknessEps);
   }
   TURETURN();
}


int main(int argc, char *argv[])
{
   NeQuickIonoData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getEffIonoLevelTest();
   errorTotal += testClass.constructor2Test();
   errorTotal += testClass.legendreTest();
   errorTotal += testClass.heightTest();
   errorTotal += testClass.exosphereAdjustTest();
   errorTotal += testClass.peakAmplitudesTest();
   errorTotal += testClass.effSolarZenithAngleTest();
   errorTotal += testClass.thicknessTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
