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
#include "NeQuickIonoNavData.hpp"
#include "MODIP.hpp"
#include "GalileoIonoEllipsoid.hpp"
#include "FreqConv.hpp"
#include "DebugTrace.hpp"

/// convert test data time stamps that EU used to CivilTime object
gnsstk::CivilTime convertTime(double hour, int month)
{
      // convert hour/month to a usable CivilTime object
   int hr = trunc(hour);
   double soh = (hour-hr)*3600;
   int min = trunc(soh/60.0);
   double sec = fmod(soh,60.0);
   gnsstk::CivilTime ct(2525,month,1,hr,min,sec,gnsstk::TimeSystem::UTC);
   GNSSTK_ASSERT(fabs(ct.getUTHour()-hour) < 1e-7);
   return ct;
}

/// Use this "ellipsoid" (sphere) for all testing
gnsstk::GalileoIonoEllipsoid galEll;

class NeQuickIonoNavData_T
{
public:
   NeQuickIonoNavData_T();

      /// Test the NeQuickIonoNavData constructor
   unsigned constructorTest();
      /// Test NequickIonoNavData::getEffIonoLevel
   unsigned getEffIonoLevelTest();
      /// Test the NeQuickIonoNavData::ModelParameters constructor
   unsigned constructor2Test();
      /// Test NeQuickIonoNavData::ModelParameters::legendre
   unsigned legendreTest();
      /// Test NeQuickIonoNavData::ModelParameters::height
   unsigned heightTest();
      /// Test NeQuickIonoNavData::ModelParameters::exosphereAdjust
   unsigned exosphereAdjustTest();
      /// Test NeQuickIonoNavData::ModelParameters::peakAmplitudes
   unsigned peakAmplitudesTest();
      /// Test NeQuickIonoNavData::ModelParameters::effSolarZenithAngle
   unsigned effSolarZenithAngleTest();
      /// Test NeQuickIonoNavData::ModelParameters::thickness
   unsigned thicknessTest();
      /// Test NeQuickIonoNavData::getTEC (implicitly getSED, getVED).
   unsigned getTECTest();
      /// Test NeQuickIonoNavData::getIonoCorr
   unsigned getIonoCorrTest();

      /// Hold input/truth data for legendreTest
   class TestData
   {
   public:
      TestData(double hour, int month, double iazr, double lon, double lat,
               double cf, double tf)
            : tAzr(iazr), tPos(lat,lon,0,gnsstk::Position::Geodetic, &galEll),
              criticalFreq(cf), transFactor(tf), ct(convertTime(hour, month))
      {
            // Test data supplies Azr (effective sunspot count), but
            // ModelParameters takes Az (effective ionization level)
            // so convert for use.
         tAz = tAzr + 408.99;
         tAz = (((tAz * tAz) - 167273)/1123.6) + 63.7;
      }
      gnsstk::CivilTime ct;
      double tAzr;
      double tAz;
      gnsstk::Position tPos;
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
            : pos(lat,lon,0,gnsstk::Position::Geodetic, &galEll),
              ct(convertTime(hour,month)), expAngle(expect)
      {}
      gnsstk::Position pos;
      gnsstk::CivilTime ct;
      double expAngle;
   };

      /// Hold input/truth data for testing E layer critical freq computation
   class TestDataCritFreqE
   {
   public:
      TestDataCritFreqE(double az, double lon, double lat, double hour,
                        int month, double expect)
            : tAz(az), pos(lat,lon,0,gnsstk::Position::Geodetic, &galEll),
              ct(convertTime(hour,month)), expFreq(expect)
      {
      }
      double tAz;
      gnsstk::Position pos;
      gnsstk::CivilTime ct;
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

      /// Hold input/truth data for getTEC
   class TestDataTEC
   {
   public:
      TestDataTEC(const std::vector<double>& coeff,
                  int month, double hour,
                  double stalon, double stalat, double stah,
                  double satlon, double satlat, double sath,
                  double expect)
            : coefficients(coeff),
              ct(convertTime(hour,month)),
              station(stalat, stalon, stah, gnsstk::Position::Geodetic, &galEll),
              satellite(satlat, satlon, sath, gnsstk::Position::Geodetic,
                        &galEll),
              expTEC(expect)
      {}
      std::vector<double> coefficients;
      gnsstk::CivilTime ct;
      gnsstk::Position station;
      gnsstk::Position satellite;
      double expTEC;
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
      /// Input/truth data for getTECTest
   static const TestDataTEC testDataTEC[];
      /// Tool for computing modified dip latitude.
   gnsstk::MODIP modip;
      /// Tool for looking up iono model data.
   gnsstk::CCIR ccir;
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
      /// Epsilon for testing TEC computation using galileo:iono test data
   static const double docEps;
      /// Epsilon for comparing derived Azr against stated Azr
   static const double ionoSpotsEps;
};


// epsilons (thresholds) and test data pulled from
// NeQuickG_JRC_iono_F2_layer_test.c
// NeQuickG_JRC_iono_layer_amplitudes_test.c
// NeQuickG_JRC_Az_test.c
// NeQuickG_JRC_iono_layer_thickness_test.c
const double NeQuickIonoNavData_T::criticalFreqEps = 1e-5;
const double NeQuickIonoNavData_T::transFactorEps = 1e-5;
const double NeQuickIonoNavData_T::amplitudeEps = 1e-5;
const double NeQuickIonoNavData_T::solarEps = 1e-5;
const double NeQuickIonoNavData_T::azEps = 1e-6;
const double NeQuickIonoNavData_T::thicknessEps = 1e-5;
// precision is not great, but this is the level of precision that is
// supplied in the truth table in Annex E.1, plus fudge factor for
// accumulated differences that particularly show up in
// integrateGaussKronrod as it adds large numbers together.
const double NeQuickIonoNavData_T::docEps = 4e-2;
const double NeQuickIonoNavData_T::ionoSpotsEps = 1e-13;

const NeQuickIonoNavData_T::TestData NeQuickIonoNavData_T::testData[] =
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


const NeQuickIonoNavData_T::TestDataHeight NeQuickIonoNavData_T::testDataHeight[] =
{
   {
      2.366982653,
      6.575612801,
      2.409694059,
      412.551921084
   },
};


const NeQuickIonoNavData_T::TestDataExosphere
NeQuickIonoNavData_T::testDataExosphere[] =
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


const NeQuickIonoNavData_T::TestDataAmplitude
NeQuickIonoNavData_T::testDataAmplitude[] =
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
const NeQuickIonoNavData_T::TestDataSolar NeQuickIonoNavData_T::testDataSolar[] =
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
const NeQuickIonoNavData_T::TestDataCritFreqE NeQuickIonoNavData_T::testDataCFE[] =
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


const std::vector<double> NeQuickIonoNavData_T::highSolarCoeff
{236.831641, -0.39362878, 0.00402826613};

const std::vector<double> NeQuickIonoNavData_T::mediumSolarCoeff
{121.129893, 0.351254133, 0.0134635348};

const std::vector<double> NeQuickIonoNavData_T::lowSolarCoeff
{2.580271, 0.127628236, 0.0252748384};


const NeQuickIonoNavData_T::TestDataAz NeQuickIonoNavData_T::testDataAz[] =
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


const NeQuickIonoNavData_T::TestDataThickness
NeQuickIonoNavData_T::testDataThickness[] =
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


const NeQuickIonoNavData_T::TestDataTEC
NeQuickIonoNavData_T::testDataTEC[] =
{
      // pulled from Annex E of galileo:iono
   {highSolarCoeff,4,0,297.66,82.49,78.11,8.23,54.29,20281546.18,20.40},
   {highSolarCoeff,4,0,297.66,82.49,78.11,-158.03,24.05,20275295.43,53.45},
   {highSolarCoeff,4,0,297.66,82.49,78.11,-30.86,41.04,19953770.93,25.91},
   {highSolarCoeff,4,4,297.66,82.49,78.11,-85.72,53.69,20544786.65,18.78},
   {highSolarCoeff,4,4,297.66,82.49,78.11,-130.77,54.40,20121312.46,20.00},
   {highSolarCoeff,4,4,297.66,82.49,78.11,140.68,35.85,19953735.00,37.81},
   {highSolarCoeff,4,8,297.66,82.49,78.11,-126.28,51.26,20513440.10,21.31},
   {highSolarCoeff,4,8,297.66,82.49,78.11,84.26,54.68,20305726.79,27.72},
   {highSolarCoeff,4,8,297.66,82.49,78.11,-96.21,37.33,19956072.48,24.13},
   {highSolarCoeff,4,12,297.66,82.49,78.11,81.09,35.20,20278071.03,49.30},
   {highSolarCoeff,4,12,297.66,82.49,78.11,175.57,51.89,19995445.72,26.61},
   {highSolarCoeff,4,12,297.66,82.49,78.11,4.25,53.43,20107681.66,29.21},
   {highSolarCoeff,4,16,297.66,82.49,78.11,14.89,32.88,20636367.33,37.95},
   {highSolarCoeff,4,16,297.66,82.49,78.11,-70.26,50.63,20043030.82,23.93},
   {highSolarCoeff,4,16,297.66,82.49,78.11,-130.60,49.21,20288021.34,26.97},
   {highSolarCoeff,4,20,297.66,82.49,78.11,-52.46,24.28,19831557.96,48.45},
   {highSolarCoeff,4,20,297.66,82.49,78.11,-165.78,35.06,20196268.24,41.71},
   {highSolarCoeff,4,20,297.66,82.49,78.11,168.73,52.58,20288372.95,27.79},
   {highSolarCoeff,4,0,307.19,5.25,-25.76,-89.48,-29.05,20081457.33,240.59},
   {highSolarCoeff,4,0,307.19,5.25,-25.76,-46.73,-24.08,19975517.42,152.19},
   {highSolarCoeff,4,0,307.19,5.25,-25.76,-99.26,34.47,20275286.46,204.92},
   {highSolarCoeff,4,4,307.19,5.25,-25.76,-46.61,54.84,20258938.89,124.39},
   {highSolarCoeff,4,4,307.19,5.25,-25.76,-85.72,53.68,20544786.61,140.16},
   {highSolarCoeff,4,4,307.19,5.25,-25.76,-18.13,14.17,20267783.18,95.52},
   {highSolarCoeff,4,8,307.19,5.25,-25.76,7.14,-19.55,20226657.45,26.47},
   {highSolarCoeff,4,8,307.19,5.25,-25.76,-48.38,-31.04,20069586.93,21.34},
   {highSolarCoeff,4,8,307.19,5.25,-25.76,-58.59,21.93,20008556.82,21.68},
   {highSolarCoeff,4,12,307.19,5.25,-25.76,-102.83,-40.74,20153844.84,169.86},
   {highSolarCoeff,4,12,307.19,5.25,-25.76,-0.60,10.75,20272829.17,178.43},
   {highSolarCoeff,4,12,307.19,5.25,-25.76,-120.35,11.00,20283503.35,146.95},
   {highSolarCoeff,4,16,307.19,5.25,-25.76,-70.26,50.63,20043030.72,198.43},
   {highSolarCoeff,4,16,307.19,5.25,-25.76,-72.73,-9.78,19936049.27,149.02},
   {highSolarCoeff,4,16,307.19,5.25,-25.76,-66.77,2.37,19986966.89,133.16},
   {highSolarCoeff,4,20,307.19,5.25,-25.76,-1.57,-7.90,20373709.74,255.31},
   {highSolarCoeff,4,20,307.19,5.25,-25.76,0.44,50.83,19975412.45,292.41},
   {highSolarCoeff,4,20,307.19,5.25,-25.76,10.94,44.72,20450566.19,336.74},
   {mediumSolarCoeff,4,0,40.19,-3.00,-23.32,76.65,-41.43,20157673.93,18.26},
   {mediumSolarCoeff,4,0,40.19,-3.00,-23.32,-13.11,-4.67,20194168.22,35.84},
   {mediumSolarCoeff,4,0,40.19,-3.00,-23.32,26.31,-39.04,20671871.64,17.18},
   {mediumSolarCoeff,4,4,40.19,-3.00,-23.32,79.33,-55.34,20679595.44,36.00},
   {mediumSolarCoeff,4,4,40.19,-3.00,-23.32,107.19,-10.65,19943686.06,76.77},
   {mediumSolarCoeff,4,4,40.19,-3.00,-23.32,56.35,47.54,20322471.38,38.01},
   {mediumSolarCoeff,4,8,40.19,-3.00,-23.32,7.14,-19.55,20226657.34,69.17},
   {mediumSolarCoeff,4,8,40.19,-3.00,-23.32,51.96,-1.90,20218595.37,59.53},
   {mediumSolarCoeff,4,8,40.19,-3.00,-23.32,89.22,-40.56,20055109.63,101.26},
   {mediumSolarCoeff,4,12,40.19,-3.00,-23.32,90.78,-28.26,20081398.25,127.83},
   {mediumSolarCoeff,4,12,40.19,-3.00,-23.32,35.75,-14.88,20010521.91,81.34},
   {mediumSolarCoeff,4,12,40.19,-3.00,-23.32,81.09,35.20,20278071.09,113.92},
   {mediumSolarCoeff,4,16,40.19,-3.00,-23.32,14.89,32.88,20636367.52,91.07},
   {mediumSolarCoeff,4,16,40.19,-3.00,-23.32,2.04,11.23,20394926.95,96.70},
   {mediumSolarCoeff,4,16,40.19,-3.00,-23.32,22.79,-35.87,20125991.19,71.45},
   {mediumSolarCoeff,4,20,40.19,-3.00,-23.32,54.11,3.15,20251696.28,48.06},
   {mediumSolarCoeff,4,20,40.19,-3.00,-23.32,95.06,17.94,20246498.07,77.64},
   {mediumSolarCoeff,4,20,40.19,-3.00,-23.32,-1.81,-52.00,20332764.38,50.10},
   {mediumSolarCoeff,4,0,115.89,-31.80,12.78,119.90,-8.76,19941513.27,24.84},
   {mediumSolarCoeff,4,0,115.89,-31.80,12.78,165.14,-13.93,20181976.57,43.94},
   {mediumSolarCoeff,4,0,115.89,-31.80,12.78,76.65,-41.43,20157673.77,19.90},
   {mediumSolarCoeff,4,4,115.89,-31.80,12.78,107.19,-10.65,19943685.24,46.25},
   {mediumSolarCoeff,4,4,115.89,-31.80,12.78,79.33,-55.34,20679595.29,46.10},
   {mediumSolarCoeff,4,4,115.89,-31.80,12.78,64.90,-17.58,20177185.06,64.59},
   {mediumSolarCoeff,4,8,115.89,-31.80,12.78,127.35,23.46,19837695.71,88.58},
   {mediumSolarCoeff,4,8,115.89,-31.80,12.78,89.22,-40.56,20055109.56,40.62},
   {mediumSolarCoeff,4,8,115.89,-31.80,12.78,148.31,-29.93,20109263.99,40.82},
   {mediumSolarCoeff,4,12,115.89,-31.80,12.78,90.78,-28.26,20081398.25,14.48},
   {mediumSolarCoeff,4,12,115.89,-31.80,12.78,133.47,-24.87,19975574.41,13.63},
   {mediumSolarCoeff,4,12,115.89,-31.80,12.78,166.97,-3.87,20196778.56,27.69},
   {mediumSolarCoeff,4,16,115.89,-31.80,12.78,124.09,-14.31,20100697.90,6.96},
   {mediumSolarCoeff,4,16,115.89,-31.80,12.78,154.31,-45.19,20116286.17,7.48},
   {mediumSolarCoeff,4,16,115.89,-31.80,12.78,-167.50,-43.24,20095343.13,13.87},
   {mediumSolarCoeff,4,20,115.89,-31.80,12.78,131.65,-31.56,20066111.12,4.36},
   {mediumSolarCoeff,4,20,115.89,-31.80,12.78,115.68,-52.78,20231909.06,4.35},
   {mediumSolarCoeff,4,20,115.89,-31.80,12.78,50.87,-50.69,20186511.77,6.97},
   {lowSolarCoeff,4,0,141.13,39.14,117.00,165.14,-13.93,20181976.50,36.44},
   {lowSolarCoeff,4,0,141.13,39.14,117.00,85.59,36.64,20015444.79,14.24},
   {lowSolarCoeff,4,0,141.13,39.14,117.00,119.90,-8.76,19941513.27,23.54},
   {lowSolarCoeff,4,4,141.13,39.14,117.00,107.19,-10.65,19943685.88,77.49},
   {lowSolarCoeff,4,4,141.13,39.14,117.00,38.39,51.98,20457198.52,29.28},
   {lowSolarCoeff,4,4,141.13,39.14,117.00,-130.77,54.40,20121312.41,23.02},
   {lowSolarCoeff,4,8,141.13,39.14,117.00,179.50,51.35,19967933.94,13.62},
   {lowSolarCoeff,4,8,141.13,39.14,117.00,97.28,21.46,19941941.52,24.28},
   {lowSolarCoeff,4,8,141.13,39.14,117.00,84.26,54.68,20305726.98,15.90},
   {lowSolarCoeff,4,12,141.13,39.14,117.00,62.65,54.77,20370905.24,16.33},
   {lowSolarCoeff,4,12,141.13,39.14,117.00,115.63,-1.28,20165065.92,11.05},
   {lowSolarCoeff,4,12,141.13,39.14,117.00,81.09,35.20,20278071.22,14.25},
   {lowSolarCoeff,4,16,141.13,39.14,117.00,124.09,-14.31,20100698.19,8.12},
   {lowSolarCoeff,4,16,141.13,39.14,117.00,-130.60,49.21,20288020.98,6.45},
   {lowSolarCoeff,4,16,141.13,39.14,117.00,161.97,13.35,20265041.53,4.69},
   {lowSolarCoeff,4,20,141.13,39.14,117.00,84.18,36.59,19953853.27,6.06},
   {lowSolarCoeff,4,20,141.13,39.14,117.00,54.67,51.65,20511861.27,8.28},
   {lowSolarCoeff,4,20,141.13,39.14,117.00,-136.92,46.53,20309713.36,10.78},
   {lowSolarCoeff,4,0,204.54,19.80,3754.69,165.14,-13.93,20181976.58,94.98},
   {lowSolarCoeff,4,0,204.54,19.80,3754.69,179.32,9.92,20274303.54,60.87},
   {lowSolarCoeff,4,0,204.54,19.80,3754.69,-144.16,-15.44,20007317.84,72.83},
   {lowSolarCoeff,4,4,204.54,19.80,3754.69,-130.77,54.40,20121312.45,30.77},
   {lowSolarCoeff,4,4,204.54,19.80,3754.69,-99.26,37.44,20066769.88,36.18},
   {lowSolarCoeff,4,4,204.54,19.80,3754.69,-85.72,53.69,20544786.60,37.25},
   {lowSolarCoeff,4,8,204.54,19.80,3754.69,178.35,-7.05,20372509.81,1.62},
   {lowSolarCoeff,4,8,204.54,19.80,3754.69,-125.97,2.30,20251559.90,0.12},
   {lowSolarCoeff,4,8,204.54,19.80,3754.69,179.50,51.35,19967934.29,0.56},
   {lowSolarCoeff,4,12,204.54,19.80,3754.69,158.88,-12.61,20145417.20,1.77},
   {lowSolarCoeff,4,12,204.54,19.80,3754.69,-146.53,22.03,20069033.97,0.64},
   {lowSolarCoeff,4,12,204.54,19.80,3754.69,-153.30,-39.75,20672066.87,2.99},
   {lowSolarCoeff,4,16,204.54,19.80,3754.69,-140.58,51.70,20455387.61,2.16},
   {lowSolarCoeff,4,16,204.54,19.80,3754.69,-167.50,-43.24,20095343.11,3.11},
   {lowSolarCoeff,4,16,204.54,19.80,3754.69,-164.50,27.08,20494802.61,1.22},
   {lowSolarCoeff,4,20,204.54,19.80,3754.69,-172.71,-20.37,20225145.06,24.53},
   {lowSolarCoeff,4,20,204.54,19.80,3754.69,-136.92,46.53,20309713.37,13.14},
   {lowSolarCoeff,4,20,204.54,19.80,3754.69,-82.52,20.64,19937791.48,38.20},
      // created to test vertical TEC which the provided truth seems to ignore
   {highSolarCoeff,4,12,257.17,-40.74,-25.76,257.17,-40.74,20153844.84,14.08},
};


NeQuickIonoNavData_T ::
NeQuickIonoNavData_T()
{
}


unsigned NeQuickIonoNavData_T ::
constructorTest()
{
   TUDEF("NeQuickIonoNavData", "NeQuickIonoNavData()");
   gnsstk::NeQuickIonoNavData uut;
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


unsigned NeQuickIonoNavData_T ::
getEffIonoLevelTest()
{
   TUDEF("NeQuickIonoNavData", "getEffIonoLevel");
   unsigned numTests = sizeof(testDataAz)/sizeof(testDataAz[0]);
   gnsstk::NeQuickIonoNavData uut;
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

unsigned NeQuickIonoNavData_T ::
constructor2Test()
{
   TUDEF("NeQuickIonoNavData::ModelParameters", "constructor");
   unsigned numTests = sizeof(testDataCFE)/sizeof(testDataCFE[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataCritFreqE& td(testDataCFE[testNum]);
      gnsstk::NeQuickIonoNavData::ModelParameters uut(
         0, td.pos, td.tAz, ccir, td.ct);
      TUASSERTFEPS(td.expFreq, uut.ffoE, criticalFreqEps);
   }
   TURETURN();
}


unsigned NeQuickIonoNavData_T ::
legendreTest()
{
   TUDEF("NeQuickIonoNavData::ModelParameters", "legendre");
   unsigned numTests = sizeof(testData)/sizeof(testData[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestData& td(testData[testNum]);
      double modip_u = modip.stModip(td.tPos);
      gnsstk::NeQuickIonoNavData::ModelParameters uut(
         modip_u, td.tPos, td.tAz, ccir, td.ct);
         // sanity check
      TUASSERTFEPS(td.tAzr, uut.fAzr, ionoSpotsEps);
      TUASSERTFEPS(td.criticalFreq, uut.ffoF2, criticalFreqEps);
      TUASSERTFEPS(td.transFactor, uut.fM3000F2, transFactorEps);
   }
   TURETURN();
}


unsigned NeQuickIonoNavData_T ::
heightTest()
{
   TUDEF("NeQuickIonoNavData::ModelParameters", "height");
   unsigned numTests = sizeof(testDataHeight)/sizeof(testDataHeight[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataHeight& td(testDataHeight[testNum]);
      gnsstk::NeQuickIonoNavData::ModelParameters uut(ccir);
      uut.fM3000F2 = td.transFactor;
      uut.ffoF2 = td.criticalFreqF2;
      uut.ffoE = td.criticalFreqE;
      TUCATCH(uut.height());
      TUASSERTFEPS(td.height, uut.fhmF2, transFactorEps);
   }
   TURETURN();
}


unsigned NeQuickIonoNavData_T ::
exosphereAdjustTest()
{
   TUDEF("NeQuickIonoNavData::ModelParameters", "exosphereAdjust");
   unsigned numTests = sizeof(testDataExosphere)/sizeof(testDataExosphere[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataExosphere& td(testDataExosphere[testNum]);
      gnsstk::NeQuickIonoNavData::ModelParameters uut(ccir);
      uut.fNmF2 = td.electronDensity;
      uut.fB2bot = td.bottom;
      uut.fhmF2 = td.height;
      uut.fAzr = td.tAzr;
      TUCATCH(uut.exosphereAdjust(td.month));
      TUASSERTFEPS(td.top, uut.fH0, transFactorEps);
   }
   TURETURN();
}


unsigned NeQuickIonoNavData_T ::
peakAmplitudesTest()
{
   TUDEF("NeQuickIonoNavData::ModelParameters", "peakAmplitudes");
   unsigned numTests = sizeof(testDataAmplitude)/sizeof(testDataAmplitude[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataAmplitude& td(testDataAmplitude[testNum]);
      gnsstk::NeQuickIonoNavData::ModelParameters uut(ccir);
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
      TUASSERTFEPS(td.expA1, uut.fA[0], amplitudeEps);
      TUASSERTFEPS(td.expA2, uut.fA[1], amplitudeEps);
      TUASSERTFEPS(td.expA3, uut.fA[2], amplitudeEps);
   }
   TURETURN();
}


unsigned NeQuickIonoNavData_T ::
effSolarZenithAngleTest()
{
   TUDEF("NeQuickIonoNavData::ModelParameters", "effSolarZenithAngle");
   unsigned numTests = sizeof(testDataSolar)/sizeof(testDataSolar[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataSolar& td(testDataSolar[testNum]);
      gnsstk::NeQuickIonoNavData::ModelParameters uut(ccir);
      gnsstk::Angle rv = uut.effSolarZenithAngle(td.pos, td.ct);
      TUASSERTFEPS(td.expAngle, rv.deg(), solarEps);
   }
   TURETURN();
}


unsigned NeQuickIonoNavData_T ::
thicknessTest()
{
   TUDEF("NeQuickIonoNavData::ModelParameters", "thickness");
   unsigned numTests = sizeof(testDataThickness)/sizeof(testDataThickness[0]);
      /// E layer maximum density height in km.
   constexpr double hmE = 120.0;                                        //eq.78
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataThickness& td(testDataThickness[testNum]);
      gnsstk::NeQuickIonoNavData::ModelParameters uut(ccir);
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


unsigned NeQuickIonoNavData_T ::
getTECTest()
{
   using namespace std;
   TUDEF("NeQuickIonoNavData::ModelParameters", "getTEC");
   unsigned numTests = sizeof(testDataTEC)/sizeof(testDataTEC[0]);
   gnsstk::NeQuickIonoNavData uut;
      /// E layer maximum density height in km.
   constexpr double hmE = 120.0;                                        //eq.78
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataTEC& td(testDataTEC[testNum]);
      uut.ai[0] = td.coefficients[0];
      uut.ai[1] = td.coefficients[1];
      uut.ai[2] = td.coefficients[2];
/*
      cerr << "getTEC #" << testNum << " for "
           << fixed << setprecision(6) << uut.ai[0] << " "
           << setprecision(8) << uut.ai[1] << " "
           << setprecision(11) << uut.ai[2] << " "
           << td.ct.month << " " << setprecision(0) << td.ct.getUTHour() << " "
           << setprecision(2) << td.station.longitude() << " "
           << td.station.geodeticLatitude() << " "
           << td.station.height() << " "
           << td.satellite.longitude() << " "
           << td.satellite.geodeticLatitude() << " "
           << td.satellite.height() << endl;
*/
      TUASSERTFEPS(td.expTEC, uut.getTEC(td.ct, td.station, td.satellite),
                   docEps);
   }
   TURETURN();
}


inline double getFactor(gnsstk::CarrierBand band)
{
   double freq = gnsstk::getFrequency(band);
   return 40.3 / (freq*freq);
}

unsigned NeQuickIonoNavData_T ::
getIonoCorrTest()
{
   using namespace std;
   TUDEF("NeQuickIonoNavData::ModelParameters", "getIonoCorr");
   unsigned numTests = sizeof(testDataTEC)/sizeof(testDataTEC[0]);
   gnsstk::NeQuickIonoNavData uut;
      /// E layer maximum density height in km.
   constexpr double hmE = 120.0;                                        //eq.78
   gnsstk::CarrierBand band;
   double expCorr;
      // Only testing two carrier bands to make sure the formula
      // works.  The code is too slow for more thorough testing.
   const double factorL1 = getFactor(gnsstk::CarrierBand::L1);
   const double factorL5 = getFactor(gnsstk::CarrierBand::L5);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestDataTEC& td(testDataTEC[testNum]);
      uut.ai[0] = td.coefficients[0];
      uut.ai[1] = td.coefficients[1];
      uut.ai[2] = td.coefficients[2];
         // test L1
      band = gnsstk::CarrierBand::L1;
      expCorr = td.expTEC * factorL1 * 1e16;
      TUASSERTFEPS(expCorr, uut.getIonoCorr(td.ct, td.station, td.satellite,
                                              band),
                   docEps);
         // test L5
      band = gnsstk::CarrierBand::L5;
      expCorr = td.expTEC * factorL5 * 1e16;
      TUASSERTFEPS(expCorr, uut.getIonoCorr(td.ct, td.station, td.satellite,
                                              band),
                   docEps);
   }
   TURETURN();
}


int main(int argc, char *argv[])
{
   NeQuickIonoNavData_T testClass;
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
   errorTotal += testClass.getTECTest();
   errorTotal += testClass.getIonoCorrTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
