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

#include "SinexTypes.hpp"
#include "SinexStream.hpp"
#include "SinexData.hpp"
#include "TestUtil.hpp"

using namespace std;
using namespace gnsstk;
using namespace Sinex;

   // Test DataType made to test abstract DataType
struct TestDataType : public DataType {
   TestDataType(){}
   ~TestDataType() {}
   operator string() const {
      return "";
   };
   void operator=(const std::string& line) {
   };
};

class SinexTypes_T
{
public:
   SinexTypes_T(){}
   ~SinexTypes_T() {}

   unsigned dataTypeTest();
   unsigned fileReferenceTest();
   unsigned fileCommentTest();
   unsigned inputHistoryTest();
   unsigned inputFileTest();
   unsigned inputAckTest();
   unsigned nutationDataTest();
   unsigned precessionDataTest();
   unsigned sourceIdTest();
   unsigned siteIdTest();
   unsigned siteDataTest();
   unsigned siteReceiverTest();
   unsigned siteAntennaTest();
   unsigned sitePhaseCenterTest();
   unsigned siteGpsPhaseCenterTest();
   unsigned siteGalPhaseCenterTest();
   unsigned siteEccentricityTest();
   unsigned satelliteIdTest();
   unsigned satellitePhaseCenterTest();
   unsigned biasEpochTest();
   unsigned solutionEpochTest();
   unsigned solutionStatisticsTest();
   unsigned solutionEstimateTest();
   unsigned solutionAprioriTest();
   unsigned solutionMatrixEstimateLCorrTest();
   unsigned solutionMatrixEstimateLCovaTest();
   unsigned solutionMatrixEstimateLInfoTest();
   unsigned solutionMatrixEstimateUCorrTest();
   unsigned solutionMatrixEstimateUCovaTest();
   unsigned solutionMatrixEstimateUInfoTest();
   unsigned solutionMatrixAprioriLCorrTest();
   unsigned solutionMatrixAprioriLCovaTest();
   unsigned solutionMatrixAprioriLInfoTest();
   unsigned solutionMatrixAprioriUCorrTest();
   unsigned solutionMatrixAprioriUCovaTest();
   unsigned solutionMatrixAprioriUInfoTest();
   unsigned solutionNormalEquationVectorTest();
   unsigned solutionNormalEquationMatrixLTest();
   unsigned solutionNormalEquationMatrixUTest();

};

unsigned SinexTypes_T :: dataTypeTest()
{
   TUDEF("SinexTypes", "DataType");
   
   TestDataType uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERT(testStream.str().length() == 0);

   TURETURN();
}

unsigned SinexTypes_T :: fileReferenceTest()
{
   TUDEF("SinexTypes", "FileReference");
   
   FileReference uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: fileCommentTest()
{
   TUDEF("SinexTypes", "FileComment");
   
   FileComment uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: inputHistoryTest()
{
   TUDEF("SinexTypes", "InputHistoryDump");
   
   InputHistory uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TUCSM("InputHistoryAssignEqual");

      // InputHistory cannot be an empty strings
   string emptyString = "";
   TUTHROW(uut = emptyString);

   TURETURN();
}

unsigned SinexTypes_T :: inputFileTest()
{
   TUDEF("SinexTypes", "InputFile");
   
   InputFile uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: inputAckTest()
{
   TUDEF("SinexTypes", "InputAck");
   
   InputAck uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: nutationDataTest()
{
   TUDEF("SinexTypes", "NutationData");
   
   NutationData uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: precessionDataTest()
{
   TUDEF("SinexTypes", "PrecessionData");
   
   PrecessionData uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: sourceIdTest()
{
   TUDEF("SinexTypes", "SourceId");
   
   SourceId uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: siteIdTest()
{
   TUDEF("SinexTypes", "SiteId");
   
   SiteId uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: siteDataTest()
{
   TUDEF("SinexTypes", "SiteData");
   
   SiteData uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}


unsigned SinexTypes_T :: siteReceiverTest()
{
   TUDEF("SinexTypes", "SiteReceiver");
   
   SiteReceiver uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: siteAntennaTest()
{
   TUDEF("SinexTypes", "SiteAntenna");
   
   SiteAntenna uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: sitePhaseCenterTest()
{
   TUDEF("SinexTypes", "SitePhaseCenter");
   
   SitePhaseCenter uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: siteGpsPhaseCenterTest()
{
   TUDEF("SinexTypes", "SiteGpsPhaseCenter");
   
   SiteGpsPhaseCenter uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: siteGalPhaseCenterTest()
{
   TUDEF("SinexTypes", "SiteGalPhaseCenter");
   
   SiteGalPhaseCenter uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: siteEccentricityTest()
{
   TUDEF("SinexTypes", "SiteEccentricity");
   
   SiteEccentricity uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: satelliteIdTest()
{
   TUDEF("SinexTypes", "SatelliteId");
   
   SatelliteId uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: satellitePhaseCenterTest()
{
   TUDEF("SinexTypes", "SatellitePhaseCenter");
   
   SatellitePhaseCenter uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: biasEpochTest()
{
   TUDEF("SinexTypes", "BiasEpoch");
   
   BiasEpoch uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionEpochTest()
{
   TUDEF("SinexTypes", "SolutionEpoch");
   
   SolutionEpoch uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionStatisticsTest()
{
   TUDEF("SinexTypes", "SolutionStatistics");
   
   SolutionStatistics uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionEstimateTest()
{
   TUDEF("SinexTypes", "SolutionEstimate");
   
   SolutionEstimate uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionAprioriTest()
{
   TUDEF("SinexTypes", "SolutionApriori");
   
   SolutionApriori uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixEstimateLCorrTest()
{
   TUDEF("SinexTypes", "SolutionMatrixEstimateLCorr");
   
   SolutionMatrixEstimateLCorr uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixEstimateLCovaTest()
{
   TUDEF("SinexTypes", "SolutionMatrixEstimateLCova");
   
   SolutionMatrixEstimateLCova uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixEstimateLInfoTest()
{
   TUDEF("SinexTypes", "SolutionMatrixEstimateLInfo");
   
   SolutionMatrixEstimateLInfo uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixEstimateUCorrTest()
{
   TUDEF("SinexTypes", "SolutionMatrixEstimateUCorr");
   
   SolutionMatrixEstimateUCorr uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixEstimateUCovaTest()
{
   TUDEF("SinexTypes", "SolutionMatrixEstimateUCova");
   
   SolutionMatrixEstimateUCova uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixEstimateUInfoTest()
{
   TUDEF("SinexTypes", "SolutionMatrixEstimateUInfo");
   
   SolutionMatrixEstimateUInfo uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixAprioriLCorrTest()
{
   TUDEF("SinexTypes", "SolutionMatrixAprioriLCorr");
   
   SolutionMatrixAprioriLCorr uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixAprioriLCovaTest()
{
   TUDEF("SinexTypes", "SolutionMatrixAprioriLCova");
   
   SolutionMatrixAprioriLCova uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixAprioriLInfoTest()
{
   TUDEF("SinexTypes", "SolutionMatrixAprioriLInfo");
   
   SolutionMatrixAprioriLInfo uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixAprioriUCorrTest()
{
   TUDEF("SinexTypes", "SolutionMatrixAprioriUCorr");
   
   SolutionMatrixAprioriUCorr uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixAprioriUCovaTest()
{
   TUDEF("SinexTypes", "SolutionMatrixAprioriUCova");
   
   SolutionMatrixAprioriUCova uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionMatrixAprioriUInfoTest()
{
   TUDEF("SinexTypes", "SolutionMatrixAprioriUInfo");
   
   SolutionMatrixAprioriUInfo uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionNormalEquationVectorTest()
{
   TUDEF("SinexTypes", "SolutionNormalEquationVector");
   
   SolutionNormalEquationVector uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionNormalEquationMatrixLTest()
{
   TUDEF("SinexTypes", "SolutionNormalEquationMatrixL");
   
   SolutionNormalEquationMatrixL uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}

unsigned SinexTypes_T :: solutionNormalEquationMatrixUTest()
{
   TUDEF("SinexTypes", "SolutionNormalEquationMatrixU");
   
   SolutionNormalEquationMatrixU uut;
   stringstream testStream;
   uut.dump(testStream);
   TUASSERTE(bool, false, testStream.str().empty())

   TURETURN();
}


int main()
{
   SinexTypes_T testClass;
   unsigned errorTotal = 0;
   errorTotal += testClass.dataTypeTest();
   errorTotal += testClass.fileReferenceTest();
   errorTotal += testClass.fileCommentTest();
   errorTotal += testClass.inputHistoryTest();
   errorTotal += testClass.inputFileTest();
   errorTotal += testClass.inputAckTest();
   errorTotal += testClass.nutationDataTest();
   errorTotal += testClass.precessionDataTest();
   errorTotal += testClass.sourceIdTest();
   errorTotal += testClass.siteIdTest();
   errorTotal += testClass.siteDataTest();
   errorTotal += testClass.siteReceiverTest();
   errorTotal += testClass.siteAntennaTest();
   errorTotal += testClass.sitePhaseCenterTest();
   errorTotal += testClass.siteGpsPhaseCenterTest();
   errorTotal += testClass.siteGalPhaseCenterTest();
   errorTotal += testClass.siteEccentricityTest();
   errorTotal += testClass.satelliteIdTest();
   errorTotal += testClass.satellitePhaseCenterTest();
   errorTotal += testClass.biasEpochTest();
   errorTotal += testClass.solutionEpochTest();
   errorTotal += testClass.solutionStatisticsTest();
   errorTotal += testClass.solutionEstimateTest();
   errorTotal += testClass.solutionAprioriTest();
   errorTotal += testClass.solutionMatrixEstimateLCorrTest();
   errorTotal += testClass.solutionMatrixEstimateLCovaTest();
   errorTotal += testClass.solutionMatrixEstimateLInfoTest();
   errorTotal += testClass.solutionMatrixEstimateUCorrTest();
   errorTotal += testClass.solutionMatrixEstimateUCovaTest();
   errorTotal += testClass.solutionMatrixEstimateUInfoTest();
   errorTotal += testClass.solutionMatrixAprioriLCorrTest();
   errorTotal += testClass.solutionMatrixAprioriLCovaTest();
   errorTotal += testClass.solutionMatrixAprioriLInfoTest();
   errorTotal += testClass.solutionMatrixAprioriUCorrTest();
   errorTotal += testClass.solutionMatrixAprioriUCovaTest();
   errorTotal += testClass.solutionMatrixAprioriUInfoTest();
   errorTotal += testClass.solutionNormalEquationVectorTest();
   errorTotal += testClass.solutionNormalEquationMatrixLTest();
   errorTotal += testClass.solutionNormalEquationMatrixUTest();

   cout << "Total Errors for " << __FILE__ <<": "<< errorTotal << endl;
   return errorTotal;
}