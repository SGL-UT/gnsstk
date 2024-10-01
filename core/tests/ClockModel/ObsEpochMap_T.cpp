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

#include "SvObsEpoch.hpp"
#include "ObsEpochMap.hpp"
#include "TestUtil.hpp"
#include <iostream>

using namespace std;
using namespace gnsstk;

class ObsEpochMap_T
{
        public:
		ObsEpochMap_T(){}// Default Constructor, set the precision value
		~ObsEpochMap_T() {} // Default Destructor
		int test_stream();
};

int ObsEpochMap_T::test_stream() 
{
	TUDEF("ObsEpochMap", "ostream");
	SvObsEpoch svObsEpoch;

	string expectEmptyOutput = "";
	ostringstream emptyOutput;
    emptyOutput << svObsEpoch;

	TUASSERTE(string, expectEmptyOutput, emptyOutput.str());

    SatID SV1 = SatID(1, SatelliteSystem::GPS);
    SatID SV2 = SatID(2, SatelliteSystem::GPS);
    SatID SV3 = SatID(3, SatelliteSystem::GPS);

	ObsID ObsId1;
	ObsID ObsId2;
	ObsID ObsId3;

	SvObsEpoch svObsEpoch1;
	svObsEpoch1.insert(make_pair(ObsId1, 25.22));
	svObsEpoch1.insert(make_pair(ObsId2, 37.24));
	svObsEpoch1.insert(make_pair(ObsId3, 48.00));
	svObsEpoch1.svid = SV1;
	svObsEpoch1.azimuth = 10;
	svObsEpoch1.elevation = 10;

	SvObsEpoch svObsEpoch2;
	svObsEpoch1.insert(make_pair(ObsId1, 11.3));
	svObsEpoch1.insert(make_pair(ObsId2, 32.4));
	svObsEpoch1.insert(make_pair(ObsId3, 19.45));
	svObsEpoch1.svid = SV2;
	svObsEpoch1.azimuth = 12;
	svObsEpoch1.elevation = 12;

	SvObsEpoch svObsEpoch3;
	svObsEpoch1.insert(make_pair(ObsId1, 20.43));
	svObsEpoch1.insert(make_pair(ObsId2, 17.32));
	svObsEpoch1.insert(make_pair(ObsId3, 31.3));
	svObsEpoch1.svid = SV3;
	svObsEpoch1.azimuth = 15;
	svObsEpoch1.elevation = 15;

	ObsEpoch oe;
	oe.insert(make_pair(SV1, svObsEpoch1));
	oe.insert(make_pair(SV2, svObsEpoch2));
	oe.insert(make_pair(SV3, svObsEpoch3));
	CommonTime time;
	oe.time = time;
	oe.rxClock = vdouble(25.25);

	string expectFullOutput = "0000000 00000000 0.000000000000000 UNK, rxClock: 25.25\n"
							  "GPS 1: UnknownBand UnknownCode UnknownType: 25.22\n"
							  "GPS 2: \n"
							  "GPS 3: \n";
	ostringstream fullOutput;
    fullOutput << oe;

	TUASSERTE(string, expectFullOutput, fullOutput.str());
	
	TURETURN();
}


int main() //Main function to initialize and run all tests above
{
	   int errorTotal = 0;

   		ObsEpochMap_T testClass;
   
		errorTotal += testClass.test_stream();

		cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl; 
		return errorTotal;
}
