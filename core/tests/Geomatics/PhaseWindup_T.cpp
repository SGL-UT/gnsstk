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
#include "PhaseWindup.hpp"
#include "SolarSystem.hpp"

class PhaseWindup_T
{
public:
   PhaseWindup_T();
   unsigned testPhaseWindupSimple();
   unsigned testPhaseWindupSSEph();
};

PhaseWindup_T ::PhaseWindup_T(){
}

unsigned PhaseWindup_T :: testPhaseWindupSimple()
{
    TUDEF("PhaseWindup_T", "testPhaseWindupSimple");

    bool blockR = false;
    double prev = 0.0;
    gnsstk::Position satPos(-11534035.0695, -23961732.6116, -41813.5001);
    gnsstk::Position rx2TX(-0.4750, -0.8667, -0.1523);
    gnsstk::Position westTriple(-0.9705, 0.2410, 0.0000);
    gnsstk::Position northTriple(0.1228, 0.4945, 0.8604);
    gnsstk::CommonTime
          tt(gnsstk::CivilTime(2025,4,21,17,0,0.000002595949228,gnsstk::TimeSystem::GPS));
    
    double pw = 0.0;
    try {
        pw = PhaseWindup(prev, tt, satPos, rx2TX, westTriple, northTriple, blockR);

    }   
    catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
    }

    TUASSERTFEPS(-0.20777157871964083147, pw, 1e-10);
    TURETURN();
}

unsigned PhaseWindup_T :: testPhaseWindupSSEph()
{
    TUDEF("PhaseWindup_T", "testPhaseWindupSSEph");

    bool blockR = false;
    double prev = 0.0;
    gnsstk::Position satPos(-11534035.0695, -23961732.6116, -41813.5001);
    gnsstk::Position rx2TX(-0.4750, -0.8667, -0.1523);
    gnsstk::Position westTriple(-0.9705, 0.2410, 0.0000);
    gnsstk::Position northTriple(0.1228, 0.4945, 0.8604);
    gnsstk::CommonTime
          tt(gnsstk::CivilTime(2016,4,21,17,0,0.000002595949228,gnsstk::TimeSystem::GPS));
    gnsstk::SolarSystem SSEph;
    std::string fnameEph = gnsstk::getPathData() + gnsstk::getFileSep() +
                 "inputs" + gnsstk::getFileSep() + "JPLeph" + gnsstk::getFileSep() +"JPL" + gnsstk::getFileSep()
                 + "SolarSystem1975to2050.403.bin";
    std::string fnameEOP = gnsstk::getPathData() + gnsstk::getFileSep() + "data" +  gnsstk::getFileSep() + "test_input_ddbase.eop";
    SSEph.initializeWithBinaryFile(fnameEph);
    SSEph.addFile(fnameEOP);
    
    double pw_1 = 0.0;
    double pw_2 = 0.0;
    try {
        pw_1 = PhaseWindup(prev, tt, satPos, rx2TX, westTriple, northTriple, SSEph, blockR);
        pw_2 = PhaseWindup(prev, tt, satPos, rx2TX, westTriple, northTriple, SSEph);

    }   
    catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
    }
    TUASSERTFEPS(-0.2076610617688560978, pw_1, 1e-10);
    TUASSERTFEPS(-0.2076610617688560978, pw_2, 1e-10);
    TURETURN();
}

int main()
{
   PhaseWindup_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testPhaseWindupSimple();
   errorTotal += testClass.testPhaseWindupSSEph();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
