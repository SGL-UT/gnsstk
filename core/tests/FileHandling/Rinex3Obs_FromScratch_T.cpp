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

#include "Rinex3ObsStream.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <string>
#include "SystemTime.hpp"

using namespace gnsstk;
using namespace std;

string tempFilePath = gnsstk::getPathTestTemp();
string dataFilePath = gnsstk::getPathData();
string file_sep = getFileSep();

class Rinex3Obs_FromScratch_T {

public:
      /* Create two Rinex Obs files - versions 3.02, 2.11
       * @param satString entered into header.mapObsTypes to test throw for bad satString
       * @param testID a string to identify the relevant files
       * @param includeConflict include two RinexObsIDs that convert to the same R2 Obs Type
       * @param completeR include glonass freqNo and codPhsBias */
   void Rinex3ObsFromScratch(string satString, string testID = "", bool completeR = false) {

      Rinex3ObsHeader header;
      Rinex3ObsData data;

      header.fileProgram = "rinex3Test";
      if(satString == "M")
         header.fileSysSat.system = SatelliteSystem::Mixed;
      else if(satString == "R")
         header.fileSysSat.system = SatelliteSystem::Glonass;
      else
         header.fileSysSat.system = SatelliteSystem::GPS;
      std::ostringstream ostr;
      ostr << gnsstk::CivilTime(SystemTime());
      header.date = ostr.str();
      header.fileAgency = "From_Scratch_T_" + testID;
      header.valid |= Rinex3ObsHeader::validRunBy;
      header.markerName = "NCCM1";
      header.valid |= Rinex3ObsHeader::validMarkerName;
      header.observer = "Monitor Station";
      header.agency = "NGA";
      header.valid |= Rinex3ObsHeader::validObserver;
      header.recNo = "1";
      header.recType = "ITT MSN SAASM";
      header.recVers = "3.2.13";
      header.valid |= Rinex3ObsHeader::validReceiver;
      header.antNo = "1";
      header.antType = "Topcon CR-G5";
      header.valid |= Rinex3ObsHeader::validAntennaType;
      Triple ntPosition(-18000.27, -4991850.12, 3957008.50);
      header.antennaPosition = ntPosition;
      header.valid |= Rinex3ObsHeader::validAntennaPosition;
      Triple ntDelta(0, 0, 0);
      header.antennaDeltaHEN = ntDelta;
      header.valid |= Rinex3ObsHeader::validAntennaDeltaHEN;
      header.firstObs = CivilTime(2016, 10, 2, 11, 15, 30.0, TimeSystem::GPS);
      header.valid |= Rinex3ObsHeader::validFirstTime;
      header.markerNumber = "85401";
      header.valid |= Rinex3ObsHeader::validMarkerNumber;
      header.interval = 30;
      header.valid |= Rinex3ObsHeader::validInterval;
      header.validEoH = true;


      header.valid |= Rinex3ObsHeader::validSystemPhaseShift;
      if(completeR || satString == "M") {
         header.glonassFreqNo = Rinex3ObsHeader::GLOFreqNumMap();
         header.valid |= Rinex3ObsHeader::validGlonassSlotFreqNo;
         header.glonassCodPhsBias = Rinex3ObsHeader::GLOCodPhsBias();
         header.valid |= Rinex3ObsHeader::validGlonassCodPhsBias;
      }

      data = Rinex3ObsData();
      data.epochFlag = 0;
      data.numSVs = 2;
      data.clockOffset = 0;
      data.time = header.firstObs.convertToCommonTime();

      std::vector<RinexObsID> newObsIds = setupObsIDs();


      RinexDatum datumL1_1;
      datumL1_1.data = 1;
      datumL1_1.lli = 0;
      datumL1_1.ssi = 0;
      RinexDatum datumL1_2;
      datumL1_2.data = 2;
      datumL1_2.lli = 0;
      datumL1_2.ssi = 0;


      RinexDatum datumP1_1;
      datumP1_1.data = 3;
      datumP1_1.lli = 0;
      datumP1_1.ssi = 0;
      RinexDatum datumP1_2;
      datumP1_2.data = 4;
      datumP1_2.lli = 0;
      datumP1_2.ssi = 0;


      RinexDatum datumC1_1;
      datumC1_1.data = 5;
      datumC1_1.lli = 0;
      datumC1_1.ssi = 0;
      RinexDatum datumC1_2;
      datumC1_2.data = 6;
      datumC1_2.lli = 0;
      datumC1_2.ssi = 0;


      RinexDatum datumL2_1;
      datumL2_1.data = 7;
      datumL2_1.lli = 0;
      datumL2_1.ssi = 0;
      RinexDatum datumL2_2;
      datumL2_2.data = 8;
      datumL2_2.lli = 0;
      datumL2_2.ssi = 0;

      RinexDatum datumP2_1;
      datumP2_1.data = 9;
      datumP2_1.lli = 0;
      datumP2_1.ssi = 0;
      RinexDatum datumP2_2;
      datumP2_2.data = 10;
      datumP2_2.lli = 0;
      datumP2_2.ssi = 0;

      if(satString == "M")
      {
         header.mapObsTypes.insert(std::pair<std::string, std::vector<RinexObsID> >("R", newObsIds));
         header.mapObsTypes.insert(std::pair<std::string, std::vector<RinexObsID> >("G", newObsIds));
      }
      else
         header.mapObsTypes.insert(std::pair<std::string, std::vector<RinexObsID> >(satString, newObsIds));
      header.valid |= Rinex3ObsHeader::validNumObs;
      header.valid |= Rinex3ObsHeader::validSystemNumObs;

      std::vector<RinexDatum> datumVec1(5);
      std::vector<RinexDatum> datumVec2(5);
      datumVec1[0] = datumL1_1;
      datumVec2[0] = datumL1_2;
      datumVec1[1] = datumP1_1;
      datumVec2[1] = datumP1_2;
      datumVec1[2] = datumC1_1;
      datumVec2[2] = datumC1_2;
      datumVec1[3] = datumL2_1;
      datumVec2[3] = datumL2_2;
      datumVec1[4] = datumP2_1;
      datumVec2[4] = datumP2_2;
      RinexSatID S3 = (satString == "G") ? RinexSatID("03") : RinexSatID("R03");
      RinexSatID S6 = (satString == "G" || satString == "M") ? RinexSatID("06") : RinexSatID("R06");
      data.obs.insert(std::pair<RinexSatID, std::vector<RinexDatum> >(S3, datumVec1));
      data.obs.insert(std::pair<RinexSatID, std::vector<RinexDatum> >(S6, datumVec2));

      header.version = 3.02;
      header.valid |= Rinex3ObsHeader::validVersion;
      Rinex3ObsStream strm(
         tempFilePath + file_sep + "rinex3ObsTest_v302_" + testID + ".out",
         std::ios::out | std::ios::trunc);

      strm.exceptions(ifstream::failbit);
      strm << header;
      strm << data;

      header.prepareVer2Write();
      Rinex3ObsStream strm2(
         tempFilePath + file_sep + "rinex3ObsTest_v211_" + testID + ".out",
         std::ios::out | std::ios::trunc);
      strm2.exceptions(ifstream::failbit);
      strm2 << header;
      strm2 << data;
   }

    std::vector<RinexObsID> setupObsIDs(){
       std::vector<RinexObsID> newObsIds;

       RinexObsID obsID1; //L1
       obsID1.band = CarrierBand::L1;
       obsID1.code = TrackingCode::P;
       obsID1.type = ObservationType::Phase;
       newObsIds.push_back(obsID1);

       RinexObsID obsID2; //P1
       obsID2.band = CarrierBand::L1;
       obsID2.code = TrackingCode::P;
       obsID2.type = ObservationType::Range;
       newObsIds.push_back(obsID2);

       RinexObsID obsID3; //C1
       obsID3.band = CarrierBand::L1;
       obsID3.code = TrackingCode::CA;
       obsID3.type = ObservationType::Range;
       newObsIds.push_back(obsID3);

       RinexObsID obsID4; //L2
       obsID4.band = CarrierBand::L2;
       obsID4.code = TrackingCode::P;
       obsID4.type = ObservationType::Phase;
       newObsIds.push_back(obsID4);

       RinexObsID obsID5;
       obsID5.band = CarrierBand::L2;
       obsID5.code = TrackingCode::P;
       obsID5.type = ObservationType::Range;
       newObsIds.push_back(obsID5);

       return newObsIds;
    }

    bool compareOutExp(string testID)
    {
       TestUtil tester;
       return
       (
         tester.fileEqualTest( dataFilePath + file_sep + "rinex3ObsTest_v302_" + testID + ".exp",
                                    tempFilePath + file_sep + "rinex3ObsTest_v302_" + testID + ".out", 2)
         &&
         tester.fileEqualTest( dataFilePath + file_sep + "rinex3ObsTest_v211_" + testID + ".exp",
                                    tempFilePath + file_sep + "rinex3ObsTest_v211_" + testID + ".out", 2)
       );
    }

   int runFromScratch(void){
      TUDEF("Rinex3Obs", "Rinex3ObsFromScratch");
      string testID;
         //Try to create varios Rinex Obs (2.11 and 3.02) files
      try
      {
            //create a valid Rinex Obs file
         testID = "ValidTest";
         Rinex3ObsFromScratch("G",testID);
         TUASSERT( compareOutExp(testID) );
      }
      catch(...)
      {
         TUFAIL( "valid input threw exception");
      }
      try
      {
         cerr << "starting mixed test" << endl;
            //create a valid Rinex Obs file
         testID = "MixedTest";
         Rinex3ObsFromScratch("M",testID);
         TUASSERT( compareOutExp(testID) );
         cerr << "mixed test complete" << endl;
      }
      catch(...)
      {
         TUFAIL( "valid input threw exception");
      }
      try
      {
            //create a glonass Rinex file
         testID = "IncompleteR";
         Rinex3ObsFromScratch("R", testID);
         TUFAIL("Glonass file should need GlonassSlotFreqNo and GlonassCodPhsBias");
      }
      catch(...)
      {
         TUPASS( "Glonass file failed for lacking necessary fields");
      }
      try
      {
            //create a glonass Rinex file. Fill glonass-required fields
         testID = "CompleteR";
         Rinex3ObsFromScratch("R",testID, true);
         TUASSERT( compareOutExp(testID) );
      }
      catch(...)
      {
         TUFAIL( "Glonass file failed despite having all necessary fields");
      }
      try
      {
            //create a Rinex file with the invalid satString "GPS"
         testID = "BadSys";
         Rinex3ObsFromScratch("GPS", testID);
         TUFAIL( "no exception for invalid sys char string");
      }
      catch(...)
      {
         TUPASS("exception thrown for invalid input");
      }
      TURETURN();
   }
};

int main()
{
   int errorTotal = 0;
   Rinex3Obs_FromScratch_T testClass;

   errorTotal += testClass.runFromScratch();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return( errorTotal );
}
