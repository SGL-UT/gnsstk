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

#include "EpochClockModel.hpp"
#include "CommonTime.hpp"
#include "TestUtil.hpp"
#include <iostream>

#include "ObsRngDev.hpp"

#include "NavLibrary.hpp"
#include "RinexNavDataFactory.hpp"

#include "EphemerisRange.hpp"
#include "SimpleTropModel.hpp"

class EpochClockModel_T
{
    public:
	EpochClockModel_T(){}// Default Constructor, set the precision value
	~EpochClockModel_T() {} // Default Desructor
	unsigned testClockGetOffSet();
	unsigned testClockIsOffsetValid();
	unsigned testAddEpoch();
};

unsigned EpochClockModel_T ::
testClockGetOffSet()
{

	TUDEF("EpochClockModel_T", "getOffSet");
	gnsstk::EpochClockModel clock;

	// Expect offset to be zero
	double offSet = clock.getOffset();
	TUASSERTE(double, 0, offSet);

	// return offset if time and clock.time are the same.
	gnsstk::CommonTime time;
	TUASSERTE(double, offSet, clock.getOffset(time));

	// Throw error if time is different then clock
	time += 1;
	TUTHROW(clock.getOffset(time));
	TURETURN();
};

unsigned EpochClockModel_T ::
testClockIsOffsetValid()
{

	TUDEF("EpochClockModel_T", "isOffsetValid");
	gnsstk::EpochClockModel clock;

	// Expect offset to be invalid
	bool offSetValidity = clock.isOffsetValid();
	TUASSERTE(bool, false, offSetValidity);

	// return true if time and clock.time are the same.
	gnsstk::CommonTime time;
	TUASSERTE(bool, offSetValidity, clock.isOffsetValid(time));

	// Throw error if time is different then clock
	time += 1;
	TUTHROW(clock.isOffsetValid(time));
	TURETURN();
};

unsigned EpochClockModel_T ::
testAddEpoch() 
{
	TUDEF("EpochClockModel_T", "addEpoch");
	gnsstk::EpochClockModel clock;

	TUASSERTE(double, 0, clock.getOffset());
	TUASSERTE(bool, false, clock.isOffsetValid());

	// Make receiver positions
	gnsstk::Position receiverPos;
	receiverPos.setGeodetic(30.387577, -97.727607, 240);

	// Make PRanges
	std::vector<float> prange;
    prange.push_back(21665483.747f);
    prange.push_back(21662553.318f);
    prange.push_back(21659650.826f);
    
	std::vector<float> prange2;
	prange2.push_back(21665487.640f);
    prange2.push_back(21662557.419f);
    prange2.push_back(21659655.019f);
	
	// Make Sat IDs
	gnsstk::SatID id{1, gnsstk::SatelliteSystem::GPS};
	gnsstk::SatID id2{2, gnsstk::SatelliteSystem::GPS};
	gnsstk::SatID id3{3, gnsstk::SatelliteSystem::GPS};
	
	// Make time vector
	std::vector<gnsstk::CommonTime> cTimeVec;
	gnsstk::CommonTime ct1 = gnsstk::CivilTime(2006, 1, 31, 4, 0, 0, gnsstk::TimeSystem::GPS);
    gnsstk::CommonTime ct2 = gnsstk::CivilTime(2006, 1, 31, 4, 0, 0, gnsstk::TimeSystem::GPS);
    gnsstk::CommonTime ct3 = gnsstk::CivilTime(2006, 1, 31, 4, 0, 0, gnsstk::TimeSystem::GPS);
	cTimeVec.push_back(ct1);
    cTimeVec.push_back(ct2);
    cTimeVec.push_back(ct3);
	
	// Make ephemeris store
	gnsstk::NavLibrary ephemStore;
	gnsstk::NavDataFactoryPtr ndfp;
	ndfp = std::make_shared<gnsstk::RinexNavDataFactory>();
	std::string path = gnsstk::getPathData() + "/test_input_rinex_nav_ephemerisData.031";
	GNSSTK_ASSERT(ndfp->addDataSource(path));
	ephemStore.addFactory(ndfp);

	// Make ORDs
	gnsstk::WGS84Ellipsoid em;
	std::vector<gnsstk::ObsRngDev> ordVec;
	for (int i=0; i < cTimeVec.size(); i++)
	{
		gnsstk::ObsRngDev ord(prange[i], id, cTimeVec[i], receiverPos, ephemStore, em);
		ordVec.push_back(ord);
	}

	// Make ORD Epochs
	gnsstk::ORDEpoch ord;
	gnsstk::ORDEpoch ord1;
	gnsstk::ORDEpoch ord2;

	ord.ords[id] = ordVec[0];
	ord.ords[id2] = ordVec[1];
	ord.ords[id3] = ordVec[2];
	
	ord1.ords[id] = ordVec[0];
	ord1.ords[id2] = ordVec[1];
	ord1.ords[id3] = ordVec[2];
	
	ord2.ords[id] = ordVec[0];
	ord2.ords[id2] = ordVec[1];
	ord2.ords[id3] = ordVec[2];

	clock.addEpoch(ord);
	clock.addEpoch(ord1);
	clock.addEpoch(ord2);

	clock.setSvMode(id, gnsstk::ObsClockModel::ALWAYS);
	clock.setSvMode(id2, gnsstk::ObsClockModel::ALWAYS);
	clock.setSvMode(id3, gnsstk::ObsClockModel::ALWAYS);

	TUASSERTFEPS(-158426.8193, clock.getOffset(), 1);

	TUASSERTE(bool, true, clock.isOffsetValid());

	TURETURN();

};


int main() //Main function to initialize and run all tests above
{
	EpochClockModel_T testClass;
	unsigned errorTotal = 0;
	
	errorTotal += testClass.testClockGetOffSet();
	errorTotal += testClass.testClockIsOffsetValid();
	errorTotal += testClass.testAddEpoch();

	std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;
			
	return errorTotal;
}
