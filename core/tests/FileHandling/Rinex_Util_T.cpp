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

#include "RinexUtilities.hpp"
#include "TestUtil.hpp"
#include <string>
#include <vector>

using namespace std;
using namespace gnsstk;



class RinexUtil_T
{
public:
   RinexUtil_T()
   {
      init();
   }
      // initialize tests
   void init();

      // Test methods
   int RegisterARLUTExtendedTypesTests();
   int isSP3FileTests();
   int isRinexNavFileTests();
   int isRinex3NavFileTests();
   int isRinexObsFileTests();
   int isRinex3ObsFileTests();
   int sortRinexObsFilesTests();
   int sortRinex3ObsFilesTests();

private:
   string testSP3;
   string testRinex2Obs;
   string testRinex2Nav;
   string testRinex3Obs;
   string testRinex3Nav;
   string testMissingFile;

};

//============================================================
// Initialize Test Data Filenames and Values
//============================================================

void RinexUtil_T :: init( void )
{

   TestUtil  testUtil;
   string  dataFilePath = gnsstk::getPathData();
   testSP3 = dataFilePath + gnsstk::getFileSep() + "test_input_SP3a.sp3";
   testRinex2Nav = dataFilePath + gnsstk::getFileSep() + "test_input_rinex2_nav_RinexNavExample.99n";
   testRinex2Obs = dataFilePath + gnsstk::getFileSep() + "test_input_rinex2_obs_RinexObsFile.06o";
   testRinex3Nav = dataFilePath + gnsstk::getFileSep() + "test_input_rinex3_nav_RinexNavExample.15n";
   testRinex3Obs = dataFilePath + gnsstk::getFileSep() + "test_input_rinex3_obs_RinexObsFile.15o";
   testMissingFile = "/file/does/not/exist";
}

int RinexUtil_T::RegisterARLUTExtendedTypesTests()
{
   TUDEF("N/A", "RegisterARLUTExtendedTypes");
   TUASSERTE(int , 0,gnsstk::RegisterARLUTExtendedTypes());
   TURETURN();
}

int RinexUtil_T::isSP3FileTests()
{
   TUDEF("N/A", "isSP3File");
   string  dataFilePath = gnsstk::getPathData();

      // Happy Path
   TUASSERTE(bool, true, gnsstk::isSP3File(testSP3));

      // Sad Paths
   TUASSERTE(bool, false, gnsstk::isSP3File(testRinex2Obs));

   TURETURN();
}

int RinexUtil_T::isRinexNavFileTests()
{
   TUDEF("N/A", "isRinexNavFile");
   string  dataFilePath = gnsstk::getPathData();

      // Happy Path
   TUASSERTE(bool, true, gnsstk::isRinexNavFile(testRinex2Nav));

      // Sad Paths
   TUASSERTE(bool, false, gnsstk::isRinexNavFile(testRinex2Obs));
   TUASSERTE(bool, false, gnsstk::isRinexNavFile(testMissingFile));

   TURETURN();
}

int RinexUtil_T::isRinex3NavFileTests()
{
   TUDEF("N/A", "isRinex3NavFile");
   string  dataFilePath = gnsstk::getPathData();

      // Happy Path
   TUASSERTE(bool, true, gnsstk::isRinex3NavFile(testRinex3Nav));

      // Sad Paths
   TUASSERTE(bool, false, gnsstk::isRinex3NavFile(testRinex3Obs));
   TUASSERTE(bool, false, gnsstk::isRinex3NavFile(testMissingFile));

   TURETURN();
}

int RinexUtil_T::isRinexObsFileTests()
{
   TUDEF("N/A", "isRinexObsFile");
   string  dataFilePath = gnsstk::getPathData();

      // Happy Path
   TUASSERTE(bool, true, gnsstk::isRinexObsFile(testRinex2Obs));

      // Sad Paths
   TUASSERTE(bool, false, gnsstk::isRinexObsFile(testRinex2Nav));
   TUASSERTE(bool, false, gnsstk::isRinexObsFile(testMissingFile));

   TURETURN();
}

int RinexUtil_T::isRinex3ObsFileTests()
{
   TUDEF("N/A", "isRinexObs3File");
   string  dataFilePath = gnsstk::getPathData();

      // Happy Path
   TUASSERTE(bool, true, gnsstk::isRinex3ObsFile(testRinex3Obs));

      // Sad Paths
   TUASSERTE(bool, false, gnsstk::isRinex3ObsFile(testRinex3Nav));
   TUASSERTE(bool, false, gnsstk::isRinex3ObsFile(testMissingFile));

   TURETURN();
}

int RinexUtil_T::sortRinexObsFilesTests()
{
   TUDEF("N/A", "sortRinexObsFiles");

   string  dataFilePath = gnsstk::getPathData();
   string testRinex2Obs0411 = dataFilePath + gnsstk::getFileSep() + "test_input_rinex2_obs_RinexObsFile0411.06o";
   string testRinex2Obs0412 = dataFilePath + gnsstk::getFileSep() + "test_input_rinex2_obs_RinexObsFile0412.06o";
   string testRinex2Obs0413 = dataFilePath + gnsstk::getFileSep() + "test_input_rinex2_obs_RinexObsFile0413.06o";
   const std::vector<std::string> sortedRinexObsFiles({testRinex2Obs0411, testRinex2Obs0412, testRinex2Obs0413});
   std::vector<std::string> testRinexObsFiles({testRinex2Obs0413, testRinex2Obs0412, testRinex2Obs0411, testRinex2Nav, testMissingFile});
   gnsstk::sortRinexObsFiles(testRinexObsFiles);
   TUASSERT(testRinexObsFiles == sortedRinexObsFiles);

   TURETURN();
}

int RinexUtil_T::sortRinex3ObsFilesTests()
{
   TUDEF("N/A", "sortRinex3ObsFiles");

   string  dataFilePath = gnsstk::getPathData();
   string testRinex3Obs0411 = dataFilePath + gnsstk::getFileSep() + "test_input_rinex3_obs_RinexObsFile0411.06o";
   string testRinex3Obs0412 = dataFilePath + gnsstk::getFileSep() + "test_input_rinex3_obs_RinexObsFile0412.06o";
   string testRinex3Obs0413 = dataFilePath + gnsstk::getFileSep() + "test_input_rinex3_obs_RinexObsFile0413.06o";
   const std::vector<std::string> sortedRinex3ObsFiles({testRinex3Obs0411, testRinex3Obs0412, testRinex3Obs0413});
   std::vector<std::string> testRinex3ObsFiles({testRinex3Obs0413, testRinex3Obs0412, testRinex3Obs0411, testRinex3Nav, testMissingFile});
   gnsstk::sortRinex3ObsFiles(testRinex3ObsFiles);
   TUASSERT(testRinex3ObsFiles == sortedRinex3ObsFiles);

   TURETURN();
}


   /** Run the program.
    *
    * @return Total error count for all tests
    */
int main(int argc, char *argv[])
{
   int  errorTotal = 0;

   RinexUtil_T  testClass;  // test data is loaded here

   errorTotal += testClass.RegisterARLUTExtendedTypesTests();
   errorTotal += testClass.isSP3FileTests();
   errorTotal += testClass.isRinexNavFileTests();
   errorTotal += testClass.isRinex3NavFileTests();
   errorTotal += testClass.isRinexObsFileTests();
   errorTotal += testClass.isRinex3ObsFileTests();
   errorTotal += testClass.sortRinexObsFilesTests();
   errorTotal += testClass.sortRinex3ObsFilesTests();

      //errorTotal += testClass.doReverseTests();

   return( errorTotal );

} // main()
