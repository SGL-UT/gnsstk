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

#include "TestUtil.hpp"
#include "AntennaStore.hpp"
#include "Position.hpp"

class AntennaStore_T
{
public:
   AntennaStore_T();
   unsigned testGetSatelliteAntenna();
   unsigned testGetNames();
   unsigned testComToPcVector_v1();
   unsigned testComToPcVector_v2();
   unsigned testGetReceiverNames();
   unsigned testDump();
   unsigned testAddAntenna();
   unsigned testGetAntenna();
   unsigned testIncludeReceivers();
};

AntennaStore_T ::AntennaStore_T(){
}

unsigned AntennaStore_T :: testGetSatelliteAntenna()
{
   TUDEF("AntennaStore_T", "testGetSatelliteAntenna");

   std::string antname;
   std::string expected_antname = "BLOCK I/G05/44278.00";

   gnsstk::AntexData data;
   gnsstk::AntennaStore antstore;
   std::string filepath = std::string(TESTDIR) + "ngs14.atx";

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      TUASSERT(antstore.getSatelliteAntenna('G', 5, antname, data, false));
   }
   catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
   }
   TUASSERTE(std::string, expected_antname, antname);
   TURETURN();
}

unsigned AntennaStore_T :: testGetNames()
{
   TUDEF("AntennaStore_T", "testGetNames");

   std::vector<std::string> names;
   int exp_vsize = 1166;
   gnsstk::AntennaStore antstore;

   std::string filepath = std::string(TESTDIR) + "ngs14.atx";

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      antstore.getNames(names);
   }
   catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
   }

   TUASSERTE(int, exp_vsize, names.size());
   TURETURN();
}

unsigned AntennaStore_T :: testComToPcVector_v1()
{
   TUDEF("AntennaStore_T", "testComToPcVector_v1");

   gnsstk::AntennaStore antstore;
   gnsstk::Triple gpsRetVector,glonassRetVector,galileoretVector, beidouRetVector;

   gnsstk::Triple satVector = gnsstk::Position(1.5, 6.2, 3.5);
   gnsstk::Triple gpsVector = gnsstk:: Triple(-0.391699, -1.61902, -0.913965);
   gnsstk::Triple glonassVector = gnsstk:: Triple (0.00405201, -2.09527, -1.38763);
   gnsstk::Triple galileoVector = gnsstk:: Triple (-0.165495, -0.697561, -0.396359);
   gnsstk::Triple beidouVector = gnsstk:: Triple (-1.12041, -2.34891, -1.11733);

   std::string filepath = std::string(TESTDIR) + "ngs14.atx";
   gnsstk::CommonTime t = gnsstk::CivilTime(2015,7,19,0,0,0,
                                                 gnsstk::TimeSystem::GPS);
   antstore.includeAllSatellites();
   antstore.addANTEXfile(filepath);
   try {
      gpsRetVector = antstore.ComToPcVector('G', 5, t, satVector);
      glonassRetVector = antstore.ComToPcVector('R', 12, t, satVector);
      galileoretVector = antstore.ComToPcVector('E', 51, t, satVector);
      beidouRetVector = antstore.ComToPcVector('C', 1, t, satVector);

   }
   catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   for (int i=0; i<3; i++)
   {
      TUASSERTFEPS(gpsRetVector[i], gpsVector[i], 1e-4);
      TUASSERTFEPS(glonassRetVector[i], glonassVector[i], 1e-4);
      TUASSERTFEPS(galileoretVector[i], galileoVector[i], 1e-4);
      TUASSERTFEPS(beidouRetVector[i], beidouVector[i], 1e-4);
   }
   TURETURN();
}

unsigned AntennaStore_T :: testComToPcVector_v2()
{
   TUDEF("AntennaStore_T", "testComToPcVector_v2");

   gnsstk::AntennaStore antstore;
   gnsstk::Triple gpsRetVector,glonassRetVector,galileoretVector, beidouRetVector;

   gnsstk::Triple satVector = gnsstk::Position(1.5, 6.2, 3.5);
   gnsstk::Triple expVector = gnsstk:: Triple(-0.391699, -1.61902, -0.913965);

   std::string filepath = std::string(TESTDIR) + "ngs14.atx";

   gnsstk::SatID gpsSatId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::SatID glonassSatId(12, gnsstk::SatelliteSystem::Glonass);
   gnsstk::SatID galileoSatId(51, gnsstk::SatelliteSystem::Galileo);
   gnsstk::SatID beidouSatId(1, gnsstk::SatelliteSystem::BeiDou);

   gnsstk::Triple gpsVector = gnsstk:: Triple(-0.391699, -1.61902, -0.913965);
   gnsstk::Triple glonassVector = gnsstk:: Triple (0.00405201, -2.09527, -1.38763);
   gnsstk::Triple galileoVector = gnsstk:: Triple (-0.165495, -0.697561, -0.396359);
   gnsstk::Triple beidouVector = gnsstk:: Triple (-1.12041, -2.34891, -1.11733);

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      gnsstk::CommonTime t = gnsstk::CivilTime(2015,7,19,0,0,0,
                                                 gnsstk::TimeSystem::GPS);
      gpsRetVector = antstore.ComToPcVector(gpsSatId, t, satVector);
      glonassRetVector = antstore.ComToPcVector(glonassSatId, t, satVector);
      galileoretVector = antstore.ComToPcVector(galileoSatId, t, satVector);
      beidouRetVector = antstore.ComToPcVector(beidouSatId, t, satVector);

   }
   catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
   }
   for (int i=0; i<3; i++)
   {
      TUASSERTFEPS(gpsRetVector[i], gpsVector[i], 1e-4);
      TUASSERTFEPS(glonassRetVector[i], glonassVector[i],1e-4);
      TUASSERTFEPS(galileoretVector[i], galileoVector[i],1e-4);
      TUASSERTFEPS(beidouRetVector[i], beidouVector[i], 1e-4);
   }
   TURETURN();
}

unsigned AntennaStore_T :: testGetReceiverNames()
{
   TUDEF("AntennaStore_T", "testGetReceiverNames");

   gnsstk::AntennaStore antstore;
   std::vector<std::string> names;
   int exp_vsize = 839;
   std::string filepath = std::string(TESTDIR) + "ngs14.atx";

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      antstore.getReceiverNames(names);
   }
   catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
   }
   TUASSERTE(int, exp_vsize, names.size());
   TURETURN();
}

unsigned AntennaStore_T :: testDump()
{
   TUDEF("AntennaStore_T", "testDump");

   std::ostringstream oss;
   int exp_ssize = 795107;
   gnsstk::AntennaStore antstore;
   std::string filepath = std::string(TESTDIR) + "ngs14.atx";

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      antstore.dump(oss, 1);
   }
   catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TUASSERTE(int,exp_ssize, oss.str().size());
   TURETURN();
}

unsigned AntennaStore_T :: testAddAntenna()
{
   TUDEF("AntennaStore_T", "testAddAntenna");

   std::ostringstream oss;
   gnsstk::AntennaStore antstore;
   gnsstk::AntexData antexData;
   gnsstk::AntexData antexData_t;
   std::string exp_data = "Antenna Type/SN: [BLOCK I/G05/44278.00] (GPS PRN 5 SVN 5) code G005, COSPAR 1980-011A\n";

   std::string filepath = std::string(TESTDIR) + "ngs14.atx";

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      TUASSERT(!antstore.getAntenna("Test_Antenna", antexData_t));

      // Get a good antexData and add it under the name "Test"
      TUASSERT(antstore.getAntenna("BLOCK I/G05/44278.00", antexData));
      antstore.addAntenna("Test_Antenna", antexData);
      TUASSERT(antstore.getAntenna("Test_Antenna", antexData_t));
      antexData_t.dump(oss, 0);

   }
   catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TUASSERTE(std::string, exp_data, oss.str());
   TURETURN();
}

unsigned AntennaStore_T :: testGetAntenna()
{
   TUDEF("AntennaStore_T", "testGetAntenna");

   std::ostringstream oss;
   gnsstk::AntennaStore antstore;
   gnsstk::AntexData antexData;
   std::string exp_data = "Antenna Type/SN: [BLOCK I/G05/44278.00] (GPS PRN 5 SVN 5) code G005, COSPAR 1980-011A\n";
   std::string filepath = std::string(TESTDIR) + "ngs14.atx";

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      TUASSERT(antstore.getAntenna("BLOCK I/G05/44278.00", antexData));
      antexData.dump(oss, 0);
   }
   catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TUASSERTE(std::string, exp_data, oss.str());
   TURETURN();
}

unsigned AntennaStore_T :: testIncludeReceivers()
{
   TUDEF("AntennaStore_T", "testIncludeReceivers");

   gnsstk::AntennaStore antstore;
   gnsstk::AntennaStore antstore_ir;
   std::string filepath = std::string(TESTDIR) + "ngs14.atx";
   std::vector<std::string> names;
   std::vector<std::string> new_names;
   std::vector<std::string> rec_names;

   rec_names.push_back("AERAT1675_120   SPKE");
   rec_names.push_back("AERAT1675_300   SPKE");

   try {
      antstore.addANTEXfile(filepath);
      antstore.getReceiverNames(names); //get all receiver names
      antstore.includeReceivers(rec_names); //include only 2 receiver names
      antstore.addANTEXfile(filepath);
      antstore.getNames(new_names); //Check if the store removed old receiver names
   }
   catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TUASSERTE(int, 839, names.size()); //before includeReceivers() call
   TUASSERTE(int, 2, new_names.size()); //after includeReceivers() call

   TURETURN();
}

int main()
{
   AntennaStore_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testGetSatelliteAntenna();
   errorTotal += testClass.testComToPcVector_v1();
   errorTotal += testClass.testComToPcVector_v2();
   errorTotal += testClass.testGetNames();
   errorTotal += testClass.testGetReceiverNames();
   errorTotal += testClass.testDump();
   errorTotal += testClass.testGetAntenna();
   errorTotal += testClass.testAddAntenna();
   errorTotal += testClass.testIncludeReceivers();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
