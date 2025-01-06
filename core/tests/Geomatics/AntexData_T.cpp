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
#include "AntexData.hpp"
#include "Position.hpp"

class AntexData_T
{
public:
   AntexData_T();
   unsigned testGetPhaseCenterVariation();
   unsigned testGetPhaseCenterOffset();
   unsigned testDump();

};

AntexData_T ::AntexData_T(){
}

unsigned AntexData_T :: testGetPhaseCenterVariation()
{
    TUDEF("AntexData_T", "testGetPhaseCenterVariation");

    std::string antname = "BLOCK I/G05/44278.00";
    double exp_pcv = -0.3;
    gnsstk::AntexData antdata;
    gnsstk::AntennaStore antstore;
    std::string filepath =  gnsstk::getPathData() + gnsstk::getFileSep() + "inputs" + gnsstk::getFileSep() + "ngs14.atx";
    double az =90;
    double el=75;
    try {
        antstore.includeAllSatellites();
        antstore.addANTEXfile(filepath);
        antstore.getAntenna(antname, antdata);
        std::map<std::string, gnsstk::AntexData::antennaPCOandPCVData>::const_iterator it;
        it = antdata.freqPCVmap.begin();
        
        while(it != antdata.freqPCVmap.end()) {
            double pcv = antdata.getPhaseCenterVariation(it->first,az,el);
            TUASSERTE(double, exp_pcv, pcv);
            ++it;
        }

    }   
    catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TURETURN();
}

unsigned AntexData_T :: testGetPhaseCenterOffset()
{
    TUDEF("AntexData_T", "testGetPhaseCenterOffset");

    std::string antname = "BLOCK I/G05/44278.00";
    double exp_total_pco = 492.056 ;
    gnsstk::Triple exp_pco = gnsstk:: Triple(0,0,1900);
    gnsstk::AntexData antdata;
    gnsstk::AntennaStore antstore;
    std::string filepath =  gnsstk::getPathData() + gnsstk::getFileSep() + "inputs" + gnsstk::getFileSep() + "ngs14.atx";
    double az = 90;
    double el = 75;
     
    try {
        antstore.includeAllSatellites();
        antstore.addANTEXfile(filepath);
        antstore.getAntenna(antname, antdata);
        
        //Test both getTotalPhaseCenterOffset() and getPhaseCenterOffset() methods.
        double total_pco = antdata.getTotalPhaseCenterOffset("G01", az,el);
        gnsstk::Triple pco = antdata.getPhaseCenterOffset("G01");
        TUASSERTFEPS(exp_total_pco, total_pco, 1e-3);
        for(int i = 0; i < 3; i++)
            TUASSERTE(double, exp_pco[i], pco[i]);
    }   
    catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TURETURN();
}
unsigned AntexData_T :: testDump()
{
   TUDEF("AntexData_T", "testDump");

   std::ostringstream oss;
   std::string antname = "BLOCK I/G05/44278.00";
   int exp_ssize = 1336;
   gnsstk::AntexData antdata;
   gnsstk::AntennaStore antstore;
   std::string filepath =  gnsstk::getPathData() + gnsstk::getFileSep() + "inputs" + gnsstk::getFileSep() + "ngs14.atx";

   try {
      antstore.includeAllSatellites();
      antstore.addANTEXfile(filepath);
      antstore.getAntenna(antname, antdata);
      antdata.dump(oss, 2);
   }
   catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TUASSERTE(int,exp_ssize, oss.str().size());
   TURETURN();

}

int main()
{
   AntexData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testGetPhaseCenterVariation();
   errorTotal += testClass.testGetPhaseCenterOffset();
   errorTotal += testClass.testDump();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
