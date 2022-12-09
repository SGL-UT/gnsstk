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

#include "TestUtil.hpp"
#include "CorrectionResults.hpp"

/// Simple class that lets us set the corrType field directly.
class TestCorrector : public gnsstk::GroupPathCorrector
{
public:
   TestCorrector(gnsstk::CorrectorType ct)
   { corrType = ct; }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Position& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 2.0;
      return true;
   }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Xvt& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 2.0;
      return true;
   }
};


class CorrectionResults_T
{
public:
   unsigned constructorTest();
   unsigned addGetClearResultsTest();
   unsigned getCorrSumTest();
};


unsigned CorrectionResults_T ::
constructorTest()
{
   TUDEF("CorrectionResults", "CorrectionResults");
   gnsstk::CorrectionResults uut;
   TUASSERTE(bool, true, uut.getResults().empty());
   TURETURN();
}


unsigned CorrectionResults_T ::
addGetClearResultsTest()
{
   TUDEF("CorrectionResults", "addResult/getResults");
   gnsstk::CorrectionResults uut;
      // Add an arbitrarily chosen number of results and make sure
      // they all appear to be added properly.
   for (unsigned i = 1; i < 5; i++)
   {
      gnsstk::CorrectionResult foo;
      foo.result = i;
      TUCATCH(uut.addResult(foo));
      const gnsstk::CorrectionResultList &results = uut.getResults();
      TUASSERTE(size_t, i, results.size());
      TUASSERTFE((double)i, results.rbegin()->result);
   }
   TUCSM("clear");
   uut.clear();
   TUASSERTE(bool, true, uut.getResults().empty());
   TURETURN();
}


unsigned CorrectionResults_T ::
getCorrSumTest()
{
   TUDEF("CorrectionResults", "getCorrSum");
   gnsstk::CorrectionResults uut;
      // create our references for the results to use
   auto tc1 = std::make_shared<TestCorrector>(gnsstk::CorrectorType::ISC);
   auto tc2 = std::make_shared<TestCorrector>(gnsstk::CorrectorType::Iono);
   auto tc3 = std::make_shared<TestCorrector>(gnsstk::CorrectorType::Trop);
   auto tc4 = std::make_shared<TestCorrector>(gnsstk::CorrectorType::ISC);
   auto tc5 = std::make_shared<TestCorrector>(gnsstk::CorrectorType::Multipath);
      // create our "results"
   gnsstk::CorrectionResult r1(1.0, tc1);
   gnsstk::CorrectionResult r2(2.0, tc2);
   gnsstk::CorrectionResult r3(4.0, tc3);
   gnsstk::CorrectionResult r4(8.0, tc4);
   gnsstk::CorrectionResult r5(16.0, tc5);
      // Add the results.
   uut.addResult(r1);
   uut.addResult(r2);
   uut.addResult(r3);
   uut.addResult(r4);
   uut.addResult(r5);
      // Now check the getCorrSum() method.
   TUASSERTFE(23.0, uut.getCorrSum(gnsstk::CorrDupHandling::ComputeFirst));
   TUASSERTFE(23.0, uut.getCorrSum(gnsstk::CorrDupHandling::UseFirst));
   TUASSERTFE(30.0, uut.getCorrSum(gnsstk::CorrDupHandling::ComputeLast));
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   CorrectionResults_T testClass;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.addGetClearResultsTest();
   errorTotal += testClass.getCorrSumTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
