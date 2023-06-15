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
#include <sstream>
#include <iterator>
#include "FactoryCounter.hpp"
#include "PNBBDSD2NavDataFactory.hpp"
#include "TestUtil.hpp"
#include "BDSD2NavTimeOffset.hpp"
#include "BDSD2NavHealth.hpp"
#include "BDSD2NavEph.hpp"
#include "BDSD2NavAlm.hpp"
#include "BDSD2NavIono.hpp"
#include "BDSD2NavISC.hpp"
#include "TimeString.hpp"
#include "CivilTime.hpp"

using namespace std;

// avoid having to type out template params over and over.
using BDSFactoryCounter = FactoryCounter<gnsstk::BDSD2NavAlm,gnsstk::BDSD2NavEph,gnsstk::BDSD2NavTimeOffset,gnsstk::BDSD2NavHealth,gnsstk::BDSD2NavIono,gnsstk::BDSD2NavISC>;

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}

string asString(const gnsstk::NavMessageTypeSet& nmts)
{
   string rv;
   for (const auto& i : nmts)
   {
      rv = rv + " " + gnsstk::StringUtils::asString(i);
   }
   return rv;
}



class PNBBDSD2NavDataFactory_T
{
public:
   PNBBDSD2NavDataFactory_T();
      /// Test that BDS doesn't process GLONASS data
   unsigned foreignTest();
      /// The meat of all the filter state tests.
   unsigned filterTest(const gnsstk::NavMessageTypeSet& nmts);
      /// addData tests for a variety of filter states
   unsigned filterTests();
      /// Test validity checking.
   unsigned addDataValidityTest();
   unsigned processAlmTest();
   unsigned processEphTest();
   unsigned processSF5Pg35Test();
   unsigned processSF5Pg36Test();
   unsigned processSF5Pg101Test();
   unsigned processSF5Pg102Test();
   unsigned processSF5Pg116Test();
   unsigned isAlmDefaultTest();

#include "D2NavTestDataDecl.hpp"
};


PNBBDSD2NavDataFactory_T ::
PNBBDSD2NavDataFactory_T()
{
#include "D2NavTestDataDef.hpp"
}


unsigned PNBBDSD2NavDataFactory_T ::
foreignTest()
{
   TUDEF("PNBBDSD2NavDataFactory", "addData");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::SatID gloSid(1,gnsstk::SatelliteSystem::Glonass);
   gnsstk::ObsID gloid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                      gnsstk::TrackingCode::Standard);
   gnsstk::NavID gloNav(gnsstk::NavType::GloCivilF);
   gnsstk::PackedNavBitsPtr nonBDS = std::make_shared<gnsstk::PackedNavBits>(
      gloSid, gloid, gloNav, "XX", navD2SF1p001ct);
   gnsstk::NavDataPtrList navOut;
      // should refuse non-BDS data
   TUASSERTE(bool, false, uut.addData(nonBDS, navOut));
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
filterTest(const gnsstk::NavMessageTypeSet& nmts)
{
   TUDEF("PNBBDSD2NavDataFactory", "addData("+asString(nmts)+")");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter(nmts));
   fc.setInc(nmts);
   TUASSERTE(bool, true, uut.addData(navD2SF1p001, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF2p001, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF3p001, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF4p001, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p001, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p002, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF2p002, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF3p002, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF4p002, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p002, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p003, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF2p003, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF3p003, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF4p003, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p003, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p004, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF2p004, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF3p004, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF4p004, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p004, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p005, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF2p005, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF3p005, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF4p005, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p005, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p006, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF2p006, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF3p006, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF4p006, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p006, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p007, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p007, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p008, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p008, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p009, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p009, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF1p010, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p010, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p011, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p012, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p013, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p014, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p015, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p016, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p017, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p018, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p019, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p020, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p021, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p022, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p023, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p024, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p025, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p026, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p027, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p028, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p029, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p030, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p031, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p032, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p033, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p034, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p035, navOut));
   fc.validateResults(navOut, __LINE__, 19, 0, 0, 0, 19);
   TUASSERTE(bool, true, uut.addData(navD2SF5p036, navOut));
   fc.validateResults(navOut, __LINE__, 11, 0, 0, 0, 11);
   TUASSERTE(bool, true, uut.addData(navD2SF5p037, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p038, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p039, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p040, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p041, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p042, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p043, navOut));
   fc.validateResults(navOut, __LINE__); // no alm, toa mismatch
   TUASSERTE(bool, true, uut.addData(navD2SF5p044, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p045, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p046, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p047, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p048, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p049, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p050, navOut));
   fc.validateResults(navOut, __LINE__); // no alm, toa mismatch
   TUASSERTE(bool, true, uut.addData(navD2SF5p051, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p052, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p053, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p054, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p055, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p056, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p057, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p058, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p059, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p060, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p061, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p062, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p063, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p064, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p065, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p066, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p067, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p068, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p069, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p070, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p071, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p072, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p073, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p074, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p075, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p076, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p077, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p078, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p079, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p080, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p081, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p082, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p083, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p084, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p085, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p086, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p087, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p088, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p089, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p090, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p091, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p092, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p093, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p094, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p095, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p096, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p097, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p098, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p099, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p100, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p101, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 3);
   TUASSERTE(bool, true, uut.addData(navD2SF5p102, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navD2SF5p103, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p104, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p105, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p106, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p107, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p108, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p109, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p110, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p111, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p112, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p113, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p114, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p115, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p116, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p117, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p118, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p119, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navD2SF5p120, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
addDataValidityTest()
{
   TUDEF("PNBBDSD2NavDataFactory", "addData");
      /// @todo implement some tests after we have parity checking and such.
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBBDSD2NavDataFactory", "processAlm");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(20, 1, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D2),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toeExp = gnsstk::BDSWeekSecond(810,81920.0);
   gnsstk::CommonTime beginExp = navD2SF5p056ct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD2NavAlm *alm;
   endExp.setTimeSystem(gnsstk::TimeSystem::BDT);
      // First store the subframes with the extra necessary data.
   TUASSERTE(bool, true, uut.addData(navD2SF5p036, navOut));  // WNa + health
   navOut.clear();
      // success, almanac data only
      /// @warning data values not vetted
   TUASSERTE(bool, true, uut.addData(navD2SF5p056, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::BDSD2NavAlm*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p056ct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p056ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, alm->health);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(-0.49719715118408203125, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.000812053680419921875, alm->ecc);
         TUASSERTFE(5282.57958984375, alm->Ahalf);
         TUASSERTFE(27905647.123033761978, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.92198193073272705078, alm->OMEGA0);
         TUASSERTFE(0.3077495574951171764, alm->i0);
         TUASSERTFE(-0.18077933788299560547, alm->w);
         TUASSERTFE(-2.182787284255027771e-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.00084781646728515625, alm->af0);
         TUASSERTFE(1.0913936421275138855e-11, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // BDSD2NavData fields
         TUASSERTE(uint32_t, 0x712, alm->pre);
         TUASSERTE(uint32_t, 0, alm->rev);
         TUASSERTE(unsigned, 5, alm->fraID);
         TUASSERTE(uint32_t, 345765, alm->sow);
            // BDSD2NavAlm fields
         TUASSERTE(unsigned, 56, alm->pnum);
         TUASSERTFE(0.0077495574951171875, alm->deltai);
         TUASSERTFE(81920, alm->toa);
         TUASSERTE(uint16_t, 88, alm->healthBits);
         TUASSERTE(bool, false, alm->isDefault);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 1);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBBDSD2NavDataFactory", "processEph");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D2),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::BDSWeekSecond(810,345600.0);
   gnsstk::CommonTime beginExp = gnsstk::BDSWeekSecond(810,345614.0);
   gnsstk::CommonTime endExp = gnsstk::BDSWeekSecond(810,432044.0);
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD2NavHealth *hea;
   gnsstk::BDSD2NavEph *eph;
   gnsstk::BDSD2NavIono *iono;
   gnsstk::BDSD2NavISC *isc;
      // success, health, ISC and health data only
   TUASSERTE(bool, true, uut.processEph(1, navD2SF1p001, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::BDSD2NavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF1p001ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // BDSD2NavHealth
         TUASSERTE(bool, false, hea->isAlmHealth);
         TUASSERTE(bool, false, hea->satH1);
         TUASSERTE(uint16_t, 255, hea->svHealth);
      }
      else if ((isc = dynamic_cast<gnsstk::BDSD2NavISC*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::ISC;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF1p001ct, isc->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
            // InterSigCorr
         TUASSERTE(bool, true, std::isnan(isc->isc));
            // BDSD2NavISC
         TUASSERTE(uint32_t, 0x712, isc->pre);
         TUASSERTE(uint32_t, 0, isc->rev);
         TUASSERTE(unsigned, 1, isc->fraID);
         TUASSERTE(uint32_t, 345600, isc->sow);
         TUASSERTFE(-5.6e-9, isc->tgd1);
         TUASSERTFE(-1.01e-8, isc->tgd2);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 1, 0, 1);
      // success, iono data only
   TUASSERTE(bool, true, uut.processEph(2, navD2SF1p002, navOut));
   for (const auto& i : navOut)
   {
      if ((iono = dynamic_cast<gnsstk::BDSD2NavIono*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF1p002ct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
            // KlobucharIonoNavData
         TUASSERTFE( 6.519258E-09, iono->alpha[0]);
         TUASSERTFE( 2.98023223876953125e-08, iono->alpha[1]);
         TUASSERTFE(-2.98023223876953125e-07, iono->alpha[2]);
         TUASSERTFE( 4.17232513427734375e-07, iono->alpha[3]);
         TUASSERTFE( 112640, iono->beta[0]);
         TUASSERTFE( 180224, iono->beta[1]);
         TUASSERTFE(-524288, iono->beta[2]);
         TUASSERTFE( 458752, iono->beta[3]);
            // BDSD2NavIono
         TUASSERTE(uint32_t, 0x712, iono->pre);
         TUASSERTE(uint32_t, 0, iono->rev);
         TUASSERTE(unsigned, 1, iono->fraID);
         TUASSERTE(uint32_t, 345603, iono->sow);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 1);
      // success, no further output at this time
   TUASSERTE(bool, true, uut.processEph(3, navD2SF1p003, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(4, navD2SF1p004, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(5, navD2SF1p005, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(6, navD2SF1p006, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(7, navD2SF1p007, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(8, navD2SF1p008, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(9, navD2SF1p009, navOut));
   fc.validateResults(navOut, __LINE__);
      // success and we have an ephemeris.
   TUASSERTE(bool, true, uut.processEph(10, navD2SF1p010, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::BDSD2NavEph*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF1p001ct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, navD2SF1p001ct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
         TUASSERTFE(-7.9278834164142608643E-06, eph->Cus);
         TUASSERTFE(-1.6712117940187454224E-05, eph->Cuc);
         TUASSERTFE( 2.45484375E+02, eph->Crc);
         TUASSERTFE(-5.05640625E+02, eph->Crs);
         TUASSERTFE(-2.10944563E-07, eph->Cic);
         TUASSERTFE( 8.05594027E-08, eph->Cis);
         TUASSERTFE(-1.916523214107468176E+00, eph->M0);
         TUASSERTFE(-6.54670127E-10, eph->dn);
         TUASSERTFE(0, eph->dndot);
         TUASSERTFE( 6.1676383484154939651E-04, eph->ecc);
         TUASSERTFE(sqrt( 4.2164428143935076892E+07), eph->Ahalf);
         TUASSERTFE( 4.2164428143935076892E+07, eph->A);
         TUASSERTFE(0, eph->Adot);
         TUASSERTFE(-3.1259749468667630268E+00, eph->OMEGA0);
         TUASSERTFE( 8.6141192515002310004E-02, eph->i0);
         TUASSERTFE( 1.3510960489050376498E+00, eph->w);
         TUASSERTFE( 1.69721355E-09, eph->OMEGAdot);
         TUASSERTFE( 3.9573076900000001079E-10, eph->idot);
         TUASSERTFE(-2.7373037301003932953E-04, eph->af0);
         TUASSERTFE( 3.71924713E-11, eph->af1);
         TUASSERTFE( 0.00000000E+00, eph->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // BDSD2NavData fields
         TUASSERTE(uint32_t, 0x712, eph->pre);
         TUASSERTE(uint32_t, 0, eph->rev);
         TUASSERTE(unsigned, 1, eph->fraID);
         TUASSERTE(uint32_t, 345600, eph->sow);
            // BDSD2NavEph fields
         TUASSERTE(bool, false, eph->satH1);
         TUASSERTE(unsigned, 0, eph->aodc);
         TUASSERTE(unsigned, 1, eph->aode);
         TUASSERTE(unsigned, 0, eph->uraIndex);
         TUASSERTFE(-5.6e-9, eph->tgd1);
         TUASSERTFE(-1.01e-8, eph->tgd2);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
processSF5Pg35Test()
{
   TUDEF("PNBBDSD2NavDataFactory", "processSF5Pg35");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD2NavHealth *hea;
   uint16_t expHea;
      // success, health data only
   TUASSERTE(bool, true, uut.processSF5Pg35(navD2SF5p035, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::BDSD2NavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p035ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // BDSD2NavHealth
         TUASSERTE(bool, true, hea->isAlmHealth);
         TUASSERTE(bool, true, hea->satH1);
         if ((nmidExp.sat.id == 7) || (nmidExp.sat.id == 17))
            expHea = 256;
         else if ((nmidExp.sat.id == 15) || (nmidExp.sat.id == 18))
            expHea = 510;
         else if (nmidExp.sat.id >= 19)
            expHea = 88;
         else
            expHea = 0;
         TUASSERTE(uint16_t, expHea, hea->svHealth);
            // While it's not strictly necessary that the results come
            // in PRN order, they do currently and this statement
            // addresses that fact.
         nmidExp.sat.id++;
      }
   }
   fc.validateResults(navOut, __LINE__, 19, 0, 0, 0, 19);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
processSF5Pg36Test()
{
   TUDEF("PNBBDSD2NavDataFactory", "processSF5Pg36");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(20, 1, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D2),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::BDSWeekSecond(810,81920.0);
   gnsstk::CommonTime beginExp = navD2SF5p056ct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD2NavAlm *alm;
   gnsstk::BDSD2NavHealth *hea;
   uint16_t expHea = 88;
      // First store the subframes with almanacs that shouldn't be
      // immediately returned.  This is not in time order, which is
      // atypical, but it is a more effective test.
   TUASSERTE(bool, true, uut.addData(navD2SF5p056, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processSF5Pg36(navD2SF5p036, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::BDSD2NavAlm*>(i.get())) != nullptr)
      {
            /// @warning data values not vetted
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p056ct, alm->timeStamp);
         gnsstk::SatID tmpSat = nmidExp.sat;
         nmidExp.sat.id = 20;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
         nmidExp.sat = tmpSat;
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p056ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, alm->health);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(-0.49719715118408203125, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.000812053680419921875, alm->ecc);
         TUASSERTFE(5282.57958984375, alm->Ahalf);
         TUASSERTFE(27905647.123033761978, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.92198193073272705078, alm->OMEGA0);
         TUASSERTFE(0.3077495574951171764, alm->i0);
         TUASSERTFE(-0.18077933788299560547, alm->w);
         TUASSERTFE(-2.182787284255027771e-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.00084781646728515625, alm->af0);
         TUASSERTFE(1.0913936421275138855e-11, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // BDSD2NavData fields
         TUASSERTE(uint32_t, 0x712, alm->pre);
         TUASSERTE(uint32_t, 0, alm->rev);
         TUASSERTE(unsigned, 5, alm->fraID);
         TUASSERTE(uint32_t, 345765, alm->sow);
            // BDSD2NavAlm fields
         TUASSERTE(unsigned, 56, alm->pnum);
         TUASSERTFE(0.0077495574951171875, alm->deltai);
         TUASSERTFE(81920, alm->toa);
         TUASSERTE(uint16_t, 88, alm->healthBits);
         TUASSERTE(bool, false, alm->isDefault);
      }
      else if ((hea = dynamic_cast<gnsstk::BDSD2NavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p036ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // BDSD2NavHealth
         TUASSERTE(bool, true, hea->isAlmHealth);
         TUASSERTE(bool, true, hea->satH1);
         TUASSERTE(uint16_t, expHea, hea->svHealth);
            // While it's not strictly necessary that the results come
            // in PRN order, they do currently and this statement
            // addresses that fact.
         nmidExp.sat.id++;
      }
   }
   fc.validateResults(navOut, __LINE__, 12, 1, 0, 0, 11);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
processSF5Pg101Test()
{
   TUDEF("PNBBDSD2NavDataFactory", "processSF5Pg101");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D2),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::CommonTime refTimeExp = gnsstk::BDSWeekSecond(810,0);
   gnsstk::CommonTime effTimeExp;
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD2NavTimeOffset *to;
   uint16_t expHea;
      // success, time offset data only
   TUASSERTE(bool, true, uut.processSF5Pg101(navD2SF5p101, navOut));
      // Time systems are processed in the order they appear in the
      // nav message. Doesn't need to be enforced, but doing so
      // simplifies the validation of the output.
   gnsstk::TimeSystem expTS = gnsstk::TimeSystem::GPS;
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::BDSD2NavTimeOffset*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p101ct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // StdNavTimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::BDT, to->src);
         TUASSERTE(gnsstk::TimeSystem, expTS, to->tgt);
         TUASSERTFE(0.0, to->a0);
         TUASSERTFE(0.0, to->a1);
         TUASSERTFE(0.0, to->a2);
         TUASSERTFE(0.0, to->deltatLS);
         TUASSERTE(gnsstk::CommonTime, refTimeExp, to->refTime);
         TUASSERTE(gnsstk::CommonTime, effTimeExp, to->effTime);
         TUASSERTFE(0.0, to->tot);
         TUASSERTE(unsigned, 0, to->wnot);
         TUASSERTE(unsigned, 0, to->wnLSF);
         TUASSERTE(unsigned, 0, to->dn);
         TUASSERTFE(0.0, to->deltatLSF);
            // BDSD2NavTimeOffset has no fields, but we check validate here
         TUCSM("validate");
         TUASSERTE(bool, false, to->validate());
         if (expTS == gnsstk::TimeSystem::GPS)
            expTS = gnsstk::TimeSystem::GAL;
         else if (expTS == gnsstk::TimeSystem::GAL)
            expTS = gnsstk::TimeSystem::GLO;
         TUCSM("processSF5Pg101");
      }
   }
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 3);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
processSF5Pg102Test()
{
   TUDEF("PNBBDSD2NavDataFactory", "processSF5Pg102");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D2),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::CommonTime refTimeExp = gnsstk::BDSWeekSecond(810,0);
      // Seems strange that this would be in the future rather than the past.
   gnsstk::CommonTime effTimeExp = gnsstk::BDSWeekSecond(829,518400);
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD2NavTimeOffset *to;
   uint16_t expHea;
      // success, time offset data only
   TUASSERTE(bool, true, uut.processSF5Pg102(navD2SF5p102, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::BDSD2NavTimeOffset*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navD2SF5p102ct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // StdNavTimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::BDT, to->src);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, to->tgt);
         TUASSERTFE(-6.519258e-09, to->a0);
         TUASSERTFE(0.0, to->a1);
         TUASSERTFE(0.0, to->a2);
         TUASSERTFE(4.0, to->deltatLS);
         TUASSERTE(gnsstk::CommonTime, refTimeExp, to->refTime);
         TUASSERTE(gnsstk::CommonTime, effTimeExp, to->effTime);
         TUASSERTFE(0.0, to->tot);
         TUASSERTE(unsigned, 0, to->wnot);
         TUASSERTE(unsigned, 829, to->wnLSF);
         TUASSERTE(unsigned, 6, to->dn);
         TUASSERTFE(4.0, to->deltatLSF);
            // BDSD2NavTimeOffset has no fields, but we check validate here
         TUCSM("validate");
         TUASSERTE(bool, true, to->validate());
         TUCSM("processSF5Pg10");
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
processSF5Pg116Test()
{
   TUDEF("PNBBDSD2NavDataFactory", "processSF5Pg116");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD2NavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
      // We don't have any data with AmEpID=b11 but we can still test others.
   TUASSERTE(bool, true, uut.addData(navD2SF5p100, navOut));
      // success, no data
   TUASSERTE(bool, true, uut.processSF5Pg116(navD2SF5p116, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
isAlmDefaultTest()
{
   TUDEF("PNBBDSD2NavDataFactory", "isAlmDefault");
   gnsstk::PNBBDSD2NavDataFactory uut;
   TUASSERTE(bool, true, uut.isAlmDefault(navD2SF5p051));
   TUASSERTE(bool, false, uut.isAlmDefault(navD2SF5p037));
   TURETURN();
}


unsigned PNBBDSD2NavDataFactory_T ::
filterTests()
{
   return filterTest(gnsstk::allNavMessageTypes) +
      filterTest({gnsstk::NavMessageType::Almanac}) +
      filterTest({gnsstk::NavMessageType::Ephemeris}) +
      filterTest({gnsstk::NavMessageType::Health}) +
      filterTest({gnsstk::NavMessageType::TimeOffset}) +
      filterTest({gnsstk::NavMessageType::Iono}) +
      filterTest({gnsstk::NavMessageType::ISC}) +
      filterTest({gnsstk::NavMessageType::Ephemeris,
                  gnsstk::NavMessageType::Health});
}

int main()
{
   PNBBDSD2NavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.foreignTest();
   errorTotal += testClass.filterTests();
   errorTotal += testClass.addDataValidityTest();
   errorTotal += testClass.processAlmTest();
   errorTotal += testClass.processEphTest();
   errorTotal += testClass.processSF5Pg35Test();
   errorTotal += testClass.processSF5Pg36Test();
   errorTotal += testClass.processSF5Pg101Test();
   errorTotal += testClass.processSF5Pg102Test();
   errorTotal += testClass.processSF5Pg116Test();
   errorTotal += testClass.isAlmDefaultTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
