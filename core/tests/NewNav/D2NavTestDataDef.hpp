//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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
/** @file D1NavTestDataDef.hpp This file contains data definitions
 * for testing BeiDou D1Nav processing in the NavFactory code. */

oidD2NAV = gpstk::ObsID(gpstk::ObservationType::NavMsg,
                        gpstk::CarrierBand::B1,
                        gpstk::TrackingCode::B1I);

navD2Sid = gpstk::SatID(1,gpstk::SatelliteSystem::BeiDou);

navD2SF1p001ct = gpstk::BDSWeekSecond(810,345614.000000);
navD2SF1p001 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p001ct);
navD2SF1p001->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p001->addUnsignedLong(0x38901541,30,1);
navD2SF1p001->addUnsignedLong(0x18004027,30,1);
navD2SF1p001->addUnsignedLong(0x00654A7C,30,1);
navD2SF1p001->addUnsignedLong(0x2303C840,30,1);
navD2SF1p001->addUnsignedLong(0x39B015EA,30,1);
navD2SF1p001->addUnsignedLong(0x1555554B,30,1);
navD2SF1p001->addUnsignedLong(0x1555554B,30,1);
navD2SF1p001->addUnsignedLong(0x1555554B,30,1);
navD2SF1p001->addUnsignedLong(0x1555554B,30,1);
navD2SF1p001->addUnsignedLong(0x1555554B,30,1);
navD2SF1p001->trimsize();

navD2SF2p001ct = gpstk::BDSWeekSecond(810,345614.600000);
navD2SF2p001 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF2p001ct);
navD2SF2p001->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF2p001->addUnsignedLong(0x38902543,30,1);
navD2SF2p001->addUnsignedLong(0x18002723,30,1);
navD2SF2p001->addUnsignedLong(0x3BF20724,30,1);
navD2SF2p001->addUnsignedLong(0x00000000,30,1);
navD2SF2p001->addUnsignedLong(0x0001FE08,30,1);
navD2SF2p001->addUnsignedLong(0x100000D0,30,1);
navD2SF2p001->addUnsignedLong(0x00000000,30,1);
navD2SF2p001->addUnsignedLong(0x00000000,30,1);
navD2SF2p001->addUnsignedLong(0x00000709,30,1);
navD2SF2p001->addUnsignedLong(0x1C20060A,30,1);
navD2SF2p001->trimsize();

navD2SF3p001ct = gpstk::BDSWeekSecond(810,345615.200000);
navD2SF3p001 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF3p001ct);
navD2SF3p001->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF3p001->addUnsignedLong(0x3890354C,30,1);
navD2SF3p001->addUnsignedLong(0x1800602D,30,1);
navD2SF3p001->addUnsignedLong(0x0343FF66,30,1);
navD2SF3p001->addUnsignedLong(0x2F77774A,30,1);
navD2SF3p001->addUnsignedLong(0x1DDDDDDE,30,1);
navD2SF3p001->addUnsignedLong(0x3777776A,30,1);
navD2SF3p001->addUnsignedLong(0x1DDE00D4,30,1);
navD2SF3p001->addUnsignedLong(0x07800769,30,1);
navD2SF3p001->addUnsignedLong(0x20000090,30,1);
navD2SF3p001->addUnsignedLong(0x202E5F64,30,1);
navD2SF3p001->trimsize();

navD2SF4p001ct = gpstk::BDSWeekSecond(810,345615.800000);
navD2SF4p001 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF4p001ct);
navD2SF4p001->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF4p001->addUnsignedLong(0x38904547,30,1);
navD2SF4p001->addUnsignedLong(0x1801102A,30,1);
navD2SF4p001->addUnsignedLong(0x0C0C0822,30,1);
navD2SF4p001->addUnsignedLong(0x2E3C2868,30,1);
navD2SF4p001->addUnsignedLong(0x10A6804A,30,1);
navD2SF4p001->addUnsignedLong(0x10303570,30,1);
navD2SF4p001->addUnsignedLong(0x1555554B,30,1);
navD2SF4p001->addUnsignedLong(0x1555554B,30,1);
navD2SF4p001->addUnsignedLong(0x1555554B,30,1);
navD2SF4p001->addUnsignedLong(0x1555554B,30,1);
navD2SF4p001->trimsize();

navD2SF5p001ct = gpstk::BDSWeekSecond(810,345616.400000);
navD2SF5p001 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p001ct);
navD2SF5p001->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p001->addUnsignedLong(0x38905548,30,1);
navD2SF5p001->addUnsignedLong(0x18000729,30,1);
navD2SF5p001->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p001->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p001->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p001->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p001->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p001->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p001->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p001->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p001->trimsize();

navD2SF1p002ct = gpstk::BDSWeekSecond(810,345617.000000);
navD2SF1p002 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p002ct);
navD2SF1p002->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p002->addUnsignedLong(0x38901541,30,1);
navD2SF1p002->addUnsignedLong(0x180C8114,30,1);
navD2SF1p002->addUnsignedLong(0x304FB0CA,30,1);
navD2SF1p002->addUnsignedLong(0x1CDC2F71,30,1);
navD2SF1p002->addUnsignedLong(0x380755B6,30,1);
navD2SF1p002->addUnsignedLong(0x1555554B,30,1);
navD2SF1p002->addUnsignedLong(0x1555554B,30,1);
navD2SF1p002->addUnsignedLong(0x1555554B,30,1);
navD2SF1p002->addUnsignedLong(0x1555554B,30,1);
navD2SF1p002->addUnsignedLong(0x1555554B,30,1);
navD2SF1p002->trimsize();

navD2SF2p002ct = gpstk::BDSWeekSecond(810,345617.600000);
navD2SF2p002 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF2p002ct);
navD2SF2p002->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF2p002->addUnsignedLong(0x38902543,30,1);
navD2SF2p002->addUnsignedLong(0x180C4717,30,1);
navD2SF2p002->addUnsignedLong(0x3BF20724,30,1);
navD2SF2p002->addUnsignedLong(0x00000000,30,1);
navD2SF2p002->addUnsignedLong(0x0001FF0B,30,1);
navD2SF2p002->addUnsignedLong(0x3E9F002B,30,1);
navD2SF2p002->addUnsignedLong(0x00000000,30,1);
navD2SF2p002->addUnsignedLong(0x00000000,30,1);
navD2SF2p002->addUnsignedLong(0x00000709,30,1);
navD2SF2p002->addUnsignedLong(0x1C0001C3,30,1);
navD2SF2p002->trimsize();

navD2SF3p002ct = gpstk::BDSWeekSecond(810,345618.200000);
navD2SF3p002 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF3p002ct);
navD2SF3p002->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF3p002->addUnsignedLong(0x3890354C,30,1);
navD2SF3p002->addUnsignedLong(0x180C0019,30,1);
navD2SF3p002->addUnsignedLong(0x00C401EA,30,1);
navD2SF3p002->addUnsignedLong(0x0777772A,30,1);
navD2SF3p002->addUnsignedLong(0x1DDDDDDE,30,1);
navD2SF3p002->addUnsignedLong(0x3777776A,30,1);
navD2SF3p002->addUnsignedLong(0x1DDE00D4,30,1);
navD2SF3p002->addUnsignedLong(0x07800769,30,1);
navD2SF3p002->addUnsignedLong(0x2000209A,30,1);
navD2SF3p002->addUnsignedLong(0x1818187E,30,1);
navD2SF3p002->trimsize();

navD2SF4p002ct = gpstk::BDSWeekSecond(810,345618.800000);
navD2SF4p002 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF4p002ct);
navD2SF4p002->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF4p002->addUnsignedLong(0x38904547,30,1);
navD2SF4p002->addUnsignedLong(0x180C3F14,30,1);
navD2SF4p002->addUnsignedLong(0x388F9FD2,30,1);
navD2SF4p002->addUnsignedLong(0x022F038E,30,1);
navD2SF4p002->addUnsignedLong(0x00C082E8,30,1);
navD2SF4p002->addUnsignedLong(0x3020D583,30,1);
navD2SF4p002->addUnsignedLong(0x1555554B,30,1);
navD2SF4p002->addUnsignedLong(0x1555554B,30,1);
navD2SF4p002->addUnsignedLong(0x1555554B,30,1);
navD2SF4p002->addUnsignedLong(0x1555554B,30,1);
navD2SF4p002->trimsize();

navD2SF5p002ct = gpstk::BDSWeekSecond(810,345619.400000);
navD2SF5p002 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p002ct);
navD2SF5p002->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p002->addUnsignedLong(0x38905548,30,1);
navD2SF5p002->addUnsignedLong(0x180C0B17,30,1);
navD2SF5p002->addUnsignedLong(0x3FF81DF1,30,1);
navD2SF5p002->addUnsignedLong(0x20398207,30,1);
navD2SF5p002->addUnsignedLong(0x11FFFF0F,30,1);
navD2SF5p002->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p002->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p002->addUnsignedLong(0x3FFFF0FD,30,1);
navD2SF5p002->addUnsignedLong(0x0C608561,30,1);
navD2SF5p002->addUnsignedLong(0x010400A9,30,1);
navD2SF5p002->trimsize();

navD2SF1p003ct = gpstk::BDSWeekSecond(810,345620.000000);
navD2SF1p003 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p003ct);
navD2SF1p003->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p003->addUnsignedLong(0x38901541,30,1);
navD2SF1p003->addUnsignedLong(0x1818C079,30,1);
navD2SF1p003->addUnsignedLong(0x00000000,30,1);
navD2SF1p003->addUnsignedLong(0x000DC13C,30,1);
navD2SF1p003->addUnsignedLong(0x3C88153A,30,1);
navD2SF1p003->addUnsignedLong(0x1555554B,30,1);
navD2SF1p003->addUnsignedLong(0x1555554B,30,1);
navD2SF1p003->addUnsignedLong(0x1555554B,30,1);
navD2SF1p003->addUnsignedLong(0x1555554B,30,1);
navD2SF1p003->addUnsignedLong(0x1555554B,30,1);
navD2SF1p003->trimsize();

navD2SF2p003ct = gpstk::BDSWeekSecond(810,345620.600000);
navD2SF2p003 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF2p003ct);
navD2SF2p003->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF2p003->addUnsignedLong(0x38902543,30,1);
navD2SF2p003->addUnsignedLong(0x18186774,30,1);
navD2SF2p003->addUnsignedLong(0x3BF20724,30,1);
navD2SF2p003->addUnsignedLong(0x00000000,30,1);
navD2SF2p003->addUnsignedLong(0x00000000,30,1);
navD2SF2p003->addUnsignedLong(0x00000000,30,1);
navD2SF2p003->addUnsignedLong(0x00000000,30,1);
navD2SF2p003->addUnsignedLong(0x00000000,30,1);
navD2SF2p003->addUnsignedLong(0x00000709,30,1);
navD2SF2p003->addUnsignedLong(0x1C400A7D,30,1);
navD2SF2p003->trimsize();

navD2SF3p003ct = gpstk::BDSWeekSecond(810,345621.200000);
navD2SF3p003 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF3p003ct);
navD2SF3p003->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF3p003->addUnsignedLong(0x3890354C,30,1);
navD2SF3p003->addUnsignedLong(0x18180070,30,1);
navD2SF3p003->addUnsignedLong(0x004000B0,30,1);
navD2SF3p003->addUnsignedLong(0x3777776A,30,1);
navD2SF3p003->addUnsignedLong(0x1DDDDDDE,30,1);
navD2SF3p003->addUnsignedLong(0x3777776A,30,1);
navD2SF3p003->addUnsignedLong(0x1DDE00D4,30,1);
navD2SF3p003->addUnsignedLong(0x07800769,30,1);
navD2SF3p003->addUnsignedLong(0x20000395,30,1);
navD2SF3p003->addUnsignedLong(0x02F8180E,30,1);
navD2SF3p003->trimsize();

navD2SF4p003ct = gpstk::BDSWeekSecond(810,345621.800000);
navD2SF4p003 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF4p003ct);
navD2SF4p003->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF4p003->addUnsignedLong(0x38904547,30,1);
navD2SF4p003->addUnsignedLong(0x1818307F,30,1);
navD2SF4p003->addUnsignedLong(0x0C0C0C2E,30,1);
navD2SF4p003->addUnsignedLong(0x0303EBDF,30,1);
navD2SF4p003->addUnsignedLong(0x397EEA3A,30,1);
navD2SF4p003->addUnsignedLong(0x1DB03590,30,1);
navD2SF4p003->addUnsignedLong(0x1555554B,30,1);
navD2SF4p003->addUnsignedLong(0x1555554B,30,1);
navD2SF4p003->addUnsignedLong(0x1555554B,30,1);
navD2SF4p003->addUnsignedLong(0x1555554B,30,1);
navD2SF4p003->trimsize();

navD2SF5p003ct = gpstk::BDSWeekSecond(810,345622.400000);
navD2SF5p003 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p003ct);
navD2SF5p003->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p003->addUnsignedLong(0x38905548,30,1);
navD2SF5p003->addUnsignedLong(0x18180C77,30,1);
navD2SF5p003->addUnsignedLong(0x0497FFDF,30,1);
navD2SF5p003->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p003->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p003->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p003->addUnsignedLong(0x208205C2,30,1);
navD2SF5p003->addUnsignedLong(0x080810C5,30,1);
navD2SF5p003->addUnsignedLong(0x1240731D,30,1);
navD2SF5p003->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p003->trimsize();

navD2SF1p004ct = gpstk::BDSWeekSecond(810,345623.000000);
navD2SF1p004 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p004ct);
navD2SF1p004->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p004->addUnsignedLong(0x38901541,30,1);
navD2SF1p004->addUnsignedLong(0x18250AEB,30,1);
navD2SF1p004->addUnsignedLong(0x0E4C00E9,30,1);
navD2SF1p004->addUnsignedLong(0x01F8D7D5,30,1);
navD2SF1p004->addUnsignedLong(0x373C55E4,30,1);
navD2SF1p004->addUnsignedLong(0x1555554B,30,1);
navD2SF1p004->addUnsignedLong(0x1555554B,30,1);
navD2SF1p004->addUnsignedLong(0x1555554B,30,1);
navD2SF1p004->addUnsignedLong(0x1555554B,30,1);
navD2SF1p004->addUnsignedLong(0x1555554B,30,1);
navD2SF1p004->trimsize();

navD2SF2p004ct = gpstk::BDSWeekSecond(810,345623.600000);
navD2SF2p004 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF2p004ct);
navD2SF2p004->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF2p004->addUnsignedLong(0x38902543,30,1);
navD2SF2p004->addUnsignedLong(0x182487EE,30,1);
navD2SF2p004->addUnsignedLong(0x3BF20724,30,1);
navD2SF2p004->addUnsignedLong(0x00000000,30,1);
navD2SF2p004->addUnsignedLong(0x00002A07,30,1);
navD2SF2p004->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF2p004->addUnsignedLong(0x00000000,30,1);
navD2SF2p004->addUnsignedLong(0x00000000,30,1);
navD2SF2p004->addUnsignedLong(0x00000709,30,1);
navD2SF2p004->addUnsignedLong(0x1C200908,30,1);
navD2SF2p004->trimsize();

navD2SF3p004ct = gpstk::BDSWeekSecond(810,345624.200000);
navD2SF3p004 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF3p004ct);
navD2SF3p004->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF3p004->addUnsignedLong(0x3890354C,30,1);
navD2SF3p004->addUnsignedLong(0x182420E3,30,1);
navD2SF3p004->addUnsignedLong(0x020BFE45,30,1);
navD2SF3p004->addUnsignedLong(0x277777BA,30,1);
navD2SF3p004->addUnsignedLong(0x1DDDDDDE,30,1);
navD2SF3p004->addUnsignedLong(0x3777776A,30,1);
navD2SF3p004->addUnsignedLong(0x1DDE00D4,30,1);
navD2SF3p004->addUnsignedLong(0x07800769,30,1);
navD2SF3p004->addUnsignedLong(0x2000209A,30,1);
navD2SF3p004->addUnsignedLong(0x1810F84D,30,1);
navD2SF3p004->trimsize();

navD2SF4p004ct = gpstk::BDSWeekSecond(810,345624.800000);
navD2SF4p004 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF4p004ct);
navD2SF4p004->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF4p004->addUnsignedLong(0x38904547,30,1);
navD2SF4p004->addUnsignedLong(0x182420E3,30,1);
navD2SF4p004->addUnsignedLong(0x011A24FB,30,1);
navD2SF4p004->addUnsignedLong(0x030303D7,30,1);
navD2SF4p004->addUnsignedLong(0x00C0C0E9,30,1);
navD2SF4p004->addUnsignedLong(0x30203580,30,1);
navD2SF4p004->addUnsignedLong(0x1555554B,30,1);
navD2SF4p004->addUnsignedLong(0x1555554B,30,1);
navD2SF4p004->addUnsignedLong(0x1555554B,30,1);
navD2SF4p004->addUnsignedLong(0x1555554B,30,1);
navD2SF4p004->trimsize();

navD2SF5p004ct = gpstk::BDSWeekSecond(810,345625.400000);
navD2SF5p004 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p004ct);
navD2SF5p004->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p004->addUnsignedLong(0x38905548,30,1);
navD2SF5p004->addUnsignedLong(0x182413E9,30,1);
navD2SF5p004->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p004->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p004->addUnsignedLong(0x3F051083,30,1);
navD2SF5p004->addUnsignedLong(0x0B40489C,30,1);
navD2SF5p004->addUnsignedLong(0x20B3046E,30,1);
navD2SF5p004->addUnsignedLong(0x05FFFF3F,30,1);
navD2SF5p004->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p004->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p004->trimsize();

navD2SF1p005ct = gpstk::BDSWeekSecond(810,345626.000000);
navD2SF1p005 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p005ct);
navD2SF1p005->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p005->addUnsignedLong(0x38901541,30,1);
navD2SF1p005->addUnsignedLong(0x18317E86,30,1);
navD2SF1p005->addUnsignedLong(0x31E9F0F9,30,1);
navD2SF1p005->addUnsignedLong(0x047BD7C7,30,1);
navD2SF1p005->addUnsignedLong(0x3C015552,30,1);
navD2SF1p005->addUnsignedLong(0x1555554B,30,1);
navD2SF1p005->addUnsignedLong(0x1555554B,30,1);
navD2SF1p005->addUnsignedLong(0x1555554B,30,1);
navD2SF1p005->addUnsignedLong(0x1555554B,30,1);
navD2SF1p005->addUnsignedLong(0x1555554B,30,1);
navD2SF1p005->trimsize();

navD2SF2p005ct = gpstk::BDSWeekSecond(810,345626.600000);
navD2SF2p005 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF2p005ct);
navD2SF2p005->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF2p005->addUnsignedLong(0x38902543,30,1);
navD2SF2p005->addUnsignedLong(0x1830A78D,30,1);
navD2SF2p005->addUnsignedLong(0x3BF20724,30,1);
navD2SF2p005->addUnsignedLong(0x00000000,30,1);
navD2SF2p005->addUnsignedLong(0x0000AA09,30,1);
navD2SF2p005->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF2p005->addUnsignedLong(0x00000000,30,1);
navD2SF2p005->addUnsignedLong(0x00000000,30,1);
navD2SF2p005->addUnsignedLong(0x00000709,30,1);
navD2SF2p005->addUnsignedLong(0x1C1FFC9A,30,1);
navD2SF2p005->trimsize();

navD2SF3p005ct = gpstk::BDSWeekSecond(810,345627.200000);
navD2SF3p005 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF3p005ct);
navD2SF3p005->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF3p005->addUnsignedLong(0x3890354C,30,1);
navD2SF3p005->addUnsignedLong(0x18309F89,30,1);
navD2SF3p005->addUnsignedLong(0x3B93FD50,30,1);
navD2SF3p005->addUnsignedLong(0x1F77770A,30,1);
navD2SF3p005->addUnsignedLong(0x1DDDDDDE,30,1);
navD2SF3p005->addUnsignedLong(0x37022578,30,1);
navD2SF3p005->addUnsignedLong(0x2619675B,30,1);
navD2SF3p005->addUnsignedLong(0x271A72F3,30,1);
navD2SF3p005->addUnsignedLong(0x2A01421E,30,1);
navD2SF3p005->addUnsignedLong(0x30FE803A,30,1);
navD2SF3p005->trimsize();

navD2SF4p005ct = gpstk::BDSWeekSecond(810,345627.800000);
navD2SF4p005 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF4p005ct);
navD2SF4p005->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF4p005->addUnsignedLong(0x38904547,30,1);
navD2SF4p005->addUnsignedLong(0x1831D083,30,1);
navD2SF4p005->addUnsignedLong(0x0C0C0822,30,1);
navD2SF4p005->addUnsignedLong(0x2C022A8A,30,1);
navD2SF4p005->addUnsignedLong(0x1024401E,30,1);
navD2SF4p005->addUnsignedLong(0x303035E0,30,1);
navD2SF4p005->addUnsignedLong(0x1555554B,30,1);
navD2SF4p005->addUnsignedLong(0x1555554B,30,1);
navD2SF4p005->addUnsignedLong(0x1555554B,30,1);
navD2SF4p005->addUnsignedLong(0x1555554B,30,1);
navD2SF4p005->trimsize();

navD2SF5p005ct = gpstk::BDSWeekSecond(810,345628.400000);
navD2SF5p005 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p005ct);
navD2SF5p005->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p005->addUnsignedLong(0x38905548,30,1);
navD2SF5p005->addUnsignedLong(0x18301489,30,1);
navD2SF5p005->addUnsignedLong(0x049030DF,30,1);
navD2SF5p005->addUnsignedLong(0x206282E5,30,1);
navD2SF5p005->addUnsignedLong(0x22FFFF9F,30,1);
navD2SF5p005->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p005->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p005->addUnsignedLong(0x3E09001F,30,1);
navD2SF5p005->addUnsignedLong(0x1660C13A,30,1);
navD2SF5p005->addUnsignedLong(0x018600F4,30,1);
navD2SF5p005->trimsize();

navD2SF1p006ct = gpstk::BDSWeekSecond(810,345629.000000);
navD2SF1p006 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p006ct);
navD2SF1p006->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p006->addUnsignedLong(0x38901541,30,1);
navD2SF1p006->addUnsignedLong(0x183D90BD,30,1);
navD2SF1p006->addUnsignedLong(0x35CA72D3,30,1);
navD2SF1p006->addUnsignedLong(0x2EB5040A,30,1);
navD2SF1p006->addUnsignedLong(0x3FFE55F9,30,1);
navD2SF1p006->addUnsignedLong(0x1555554B,30,1);
navD2SF1p006->addUnsignedLong(0x1555554B,30,1);
navD2SF1p006->addUnsignedLong(0x1555554B,30,1);
navD2SF1p006->addUnsignedLong(0x1555554B,30,1);
navD2SF1p006->addUnsignedLong(0x1555554B,30,1);
navD2SF1p006->trimsize();

navD2SF2p006ct = gpstk::BDSWeekSecond(810,345629.600000);
navD2SF2p006 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF2p006ct);
navD2SF2p006->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF2p006->addUnsignedLong(0x38902543,30,1);
navD2SF2p006->addUnsignedLong(0x183CC7B9,30,1);
navD2SF2p006->addUnsignedLong(0x3BF20724,30,1);
navD2SF2p006->addUnsignedLong(0x00000000,30,1);
navD2SF2p006->addUnsignedLong(0x0000AA09,30,1);
navD2SF2p006->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF2p006->addUnsignedLong(0x00000000,30,1);
navD2SF2p006->addUnsignedLong(0x00000000,30,1);
navD2SF2p006->addUnsignedLong(0x00000709,30,1);
navD2SF2p006->addUnsignedLong(0x1C7FEFEA,30,1);
navD2SF2p006->trimsize();

navD2SF3p006ct = gpstk::BDSWeekSecond(810,345630.200000);
navD2SF3p006 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF3p006ct);
navD2SF3p006->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF3p006->addUnsignedLong(0x3890354C,30,1);
navD2SF3p006->addUnsignedLong(0x183DE0B5,30,1);
navD2SF3p006->addUnsignedLong(0x003FFD99,30,1);
navD2SF3p006->addUnsignedLong(0x3777776A,30,1);
navD2SF3p006->addUnsignedLong(0x1DDDDDDE,30,1);
navD2SF3p006->addUnsignedLong(0x370D5F46,30,1);
navD2SF3p006->addUnsignedLong(0x263BC497,30,1);
navD2SF3p006->addUnsignedLong(0x190E80BA,30,1);
navD2SF3p006->addUnsignedLong(0x00016002,30,1);
navD2SF3p006->addUnsignedLong(0x1818187E,30,1);
navD2SF3p006->trimsize();

navD2SF4p006ct = gpstk::BDSWeekSecond(810,345630.800000);
navD2SF4p006 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF4p006ct);
navD2SF4p006->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF4p006->addUnsignedLong(0x38904547,30,1);
navD2SF4p006->addUnsignedLong(0x183C20B3,30,1);
navD2SF4p006->addUnsignedLong(0x0EC07682,30,1);
navD2SF4p006->addUnsignedLong(0x37A3D9E6,30,1);
navD2SF4p006->addUnsignedLong(0x00C0F7EF,30,1);
navD2SF4p006->addUnsignedLong(0x3BDDD5D5,30,1);
navD2SF4p006->addUnsignedLong(0x1555554B,30,1);
navD2SF4p006->addUnsignedLong(0x1555554B,30,1);
navD2SF4p006->addUnsignedLong(0x1555554B,30,1);
navD2SF4p006->addUnsignedLong(0x1555554B,30,1);
navD2SF4p006->trimsize();

navD2SF5p006ct = gpstk::BDSWeekSecond(810,345631.400000);
navD2SF5p006 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p006ct);
navD2SF5p006->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p006->addUnsignedLong(0x38905548,30,1);
navD2SF5p006->addUnsignedLong(0x183C18B7,30,1);
navD2SF5p006->addUnsignedLong(0x0517FF2F,30,1);
navD2SF5p006->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p006->addUnsignedLong(0x3FFFF8F6,30,1);
navD2SF5p006->addUnsignedLong(0x0D20607D,30,1);
navD2SF5p006->addUnsignedLong(0x20C30678,30,1);
navD2SF5p006->addUnsignedLong(0x040C20D3,30,1);
navD2SF5p006->addUnsignedLong(0x167FFF6F,30,1);
navD2SF5p006->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p006->trimsize();

navD2SF1p007ct = gpstk::BDSWeekSecond(810,345632.000000);
navD2SF1p007 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p007ct);
navD2SF1p007->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p007->addUnsignedLong(0x38901541,30,1);
navD2SF1p007->addUnsignedLong(0x1849CEA7,30,1);
navD2SF1p007->addUnsignedLong(0x3002B543,30,1);
navD2SF1p007->addUnsignedLong(0x14600143,30,1);
navD2SF1p007->addUnsignedLong(0x304F55C6,30,1);
navD2SF1p007->addUnsignedLong(0x1555554B,30,1);
navD2SF1p007->addUnsignedLong(0x1555554B,30,1);
navD2SF1p007->addUnsignedLong(0x1555554B,30,1);
navD2SF1p007->addUnsignedLong(0x1555554B,30,1);
navD2SF1p007->addUnsignedLong(0x1555554B,30,1);
navD2SF1p007->trimsize();

navD2SF5p007ct = gpstk::BDSWeekSecond(810,345634.400000);
navD2SF5p007 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p007ct);
navD2SF5p007->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p007->addUnsignedLong(0x38905548,30,1);
navD2SF5p007->addUnsignedLong(0x18481FA7,30,1);
navD2SF5p007->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p007->addUnsignedLong(0x30918374,30,1);
navD2SF5p007->addUnsignedLong(0x220610E1,30,1);
navD2SF5p007->addUnsignedLong(0x0F206905,30,1);
navD2SF5p007->addUnsignedLong(0x20C3057D,30,1);
navD2SF5p007->addUnsignedLong(0x07FFFF4F,30,1);
navD2SF5p007->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p007->addUnsignedLong(0x02660004,30,1);
navD2SF5p007->trimsize();

navD2SF1p008ct = gpstk::BDSWeekSecond(810,345635.000000);
navD2SF1p008 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p008ct);
navD2SF1p008->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p008->addUnsignedLong(0x38901541,30,1);
navD2SF1p008->addUnsignedLong(0x185621FD,30,1);
navD2SF1p008->addUnsignedLong(0x301EAF12,30,1);
navD2SF1p008->addUnsignedLong(0x3C0CB863,30,1);
navD2SF1p008->addUnsignedLong(0x002515CC,30,1);
navD2SF1p008->addUnsignedLong(0x1555554B,30,1);
navD2SF1p008->addUnsignedLong(0x1555554B,30,1);
navD2SF1p008->addUnsignedLong(0x1555554B,30,1);
navD2SF1p008->addUnsignedLong(0x1555554B,30,1);
navD2SF1p008->addUnsignedLong(0x1555554B,30,1);
navD2SF1p008->trimsize();

navD2SF5p008ct = gpstk::BDSWeekSecond(810,345637.400000);
navD2SF5p008 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p008ct);
navD2SF5p008->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p008->addUnsignedLong(0x38905548,30,1);
navD2SF5p008->addUnsignedLong(0x185420F3,30,1);
navD2SF5p008->addUnsignedLong(0x081840A7,30,1);
navD2SF5p008->addUnsignedLong(0x208183C4,30,1);
navD2SF5p008->addUnsignedLong(0x340690AF,30,1);
navD2SF5p008->addUnsignedLong(0x0A2FFF7F,30,1);
navD2SF5p008->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p008->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p008->addUnsignedLong(0x3FE132A6,30,1);
navD2SF5p008->addUnsignedLong(0x024A00FD,30,1);
navD2SF5p008->trimsize();

navD2SF1p009ct = gpstk::BDSWeekSecond(810,345638.000000);
navD2SF1p009 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p009ct);
navD2SF1p009->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p009->addUnsignedLong(0x38901541,30,1);
navD2SF1p009->addUnsignedLong(0x18626153,30,1);
navD2SF1p009->addUnsignedLong(0x005173D2,30,1);
navD2SF1p009->addUnsignedLong(0x0146E114,30,1);
navD2SF1p009->addUnsignedLong(0x238B552F,30,1);
navD2SF1p009->addUnsignedLong(0x1555554B,30,1);
navD2SF1p009->addUnsignedLong(0x1555554B,30,1);
navD2SF1p009->addUnsignedLong(0x1555554B,30,1);
navD2SF1p009->addUnsignedLong(0x1555554B,30,1);
navD2SF1p009->addUnsignedLong(0x1555554B,30,1);
navD2SF1p009->trimsize();

navD2SF5p009ct = gpstk::BDSWeekSecond(810,345640.400000);
navD2SF5p009 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p009ct);
navD2SF5p009->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p009->addUnsignedLong(0x38905548,30,1);
navD2SF5p009->addUnsignedLong(0x18602456,30,1);
navD2SF5p009->addUnsignedLong(0x08A83C58,30,1);
navD2SF5p009->addUnsignedLong(0x30510299,30,1);
navD2SF5p009->addUnsignedLong(0x15FFFFEF,30,1);
navD2SF5p009->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p009->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p009->addUnsignedLong(0x3E15704E,30,1);
navD2SF5p009->addUnsignedLong(0x24812223,30,1);
navD2SF5p009->addUnsignedLong(0x01C40049,30,1);
navD2SF5p009->trimsize();

navD2SF1p010ct = gpstk::BDSWeekSecond(810,345641.000000);
navD2SF1p010 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF1p010ct);
navD2SF1p010->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF1p010->addUnsignedLong(0x38901541,30,1);
navD2SF1p010->addUnsignedLong(0x186E8E6B,30,1);
navD2SF1p010->addUnsignedLong(0x08A8AA59,30,1);
navD2SF1p010->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF1p010->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF1p010->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF1p010->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF1p010->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF1p010->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF1p010->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF1p010->trimsize();

navD2SF5p010ct = gpstk::BDSWeekSecond(810,345643.400000);
navD2SF5p010 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p010ct);
navD2SF5p010->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p010->addUnsignedLong(0x38905548,30,1);
navD2SF5p010->addUnsignedLong(0x186C2868,30,1);
navD2SF5p010->addUnsignedLong(0x04A7FF7F,30,1);
navD2SF5p010->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p010->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p010->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p010->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p010->addUnsignedLong(0x3E112F47,30,1);
navD2SF5p010->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p010->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p010->trimsize();

navD2SF5p011ct = gpstk::BDSWeekSecond(810,345646.400000);
navD2SF5p011 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p011ct);
navD2SF5p011->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p011->addUnsignedLong(0x38905548,30,1);
navD2SF5p011->addUnsignedLong(0x18782F08,30,1);
navD2SF5p011->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p011->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p011->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p011->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p011->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p011->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p011->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p011->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p011->trimsize();

navD2SF5p012ct = gpstk::BDSWeekSecond(810,345649.400000);
navD2SF5p012 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p012ct);
navD2SF5p012->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p012->addUnsignedLong(0x38905548,30,1);
navD2SF5p012->addUnsignedLong(0x18843373,30,1);
navD2SF5p012->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p012->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p012->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p012->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p012->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p012->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p012->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p012->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p012->trimsize();

navD2SF5p013ct = gpstk::BDSWeekSecond(810,345652.400000);
navD2SF5p013 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p013ct);
navD2SF5p013->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p013->addUnsignedLong(0x38905548,30,1);
navD2SF5p013->addUnsignedLong(0x18903716,30,1);
navD2SF5p013->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p013->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p013->addUnsignedLong(0x3F000080,30,1);
navD2SF5p013->addUnsignedLong(0x00000000,30,1);
navD2SF5p013->addUnsignedLong(0x00000000,30,1);
navD2SF5p013->addUnsignedLong(0x00000000,30,1);
navD2SF5p013->addUnsignedLong(0x00000000,30,1);
navD2SF5p013->addUnsignedLong(0x00000000,30,1);
navD2SF5p013->trimsize();

navD2SF5p014ct = gpstk::BDSWeekSecond(810,345655.400000);
navD2SF5p014 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p014ct);
navD2SF5p014->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p014->addUnsignedLong(0x38905548,30,1);
navD2SF5p014->addUnsignedLong(0x189C382D,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->addUnsignedLong(0x00000000,30,1);
navD2SF5p014->trimsize();

navD2SF5p015ct = gpstk::BDSWeekSecond(810,345658.400000);
navD2SF5p015 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p015ct);
navD2SF5p015->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p015->addUnsignedLong(0x38905548,30,1);
navD2SF5p015->addUnsignedLong(0x18A83C88,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->addUnsignedLong(0x00000000,30,1);
navD2SF5p015->trimsize();

navD2SF5p016ct = gpstk::BDSWeekSecond(810,345661.400000);
navD2SF5p016 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p016ct);
navD2SF5p016->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p016->addUnsignedLong(0x38905548,30,1);
navD2SF5p016->addUnsignedLong(0x18B440DE,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->addUnsignedLong(0x00000000,30,1);
navD2SF5p016->trimsize();

navD2SF5p017ct = gpstk::BDSWeekSecond(810,345664.400000);
navD2SF5p017 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p017ct);
navD2SF5p017->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p017->addUnsignedLong(0x38905548,30,1);
navD2SF5p017->addUnsignedLong(0x18C044CB,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->addUnsignedLong(0x00000000,30,1);
navD2SF5p017->trimsize();

navD2SF5p018ct = gpstk::BDSWeekSecond(810,345667.400000);
navD2SF5p018 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p018ct);
navD2SF5p018->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p018->addUnsignedLong(0x38905548,30,1);
navD2SF5p018->addUnsignedLong(0x18CC48F5,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->addUnsignedLong(0x00000000,30,1);
navD2SF5p018->trimsize();

navD2SF5p019ct = gpstk::BDSWeekSecond(810,345670.400000);
navD2SF5p019 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p019ct);
navD2SF5p019->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p019->addUnsignedLong(0x38905548,30,1);
navD2SF5p019->addUnsignedLong(0x18D84C90,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->addUnsignedLong(0x00000000,30,1);
navD2SF5p019->trimsize();

navD2SF5p020ct = gpstk::BDSWeekSecond(810,345673.400000);
navD2SF5p020 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p020ct);
navD2SF5p020->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p020->addUnsignedLong(0x38905548,30,1);
navD2SF5p020->addUnsignedLong(0x18E4500B,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->addUnsignedLong(0x00000000,30,1);
navD2SF5p020->trimsize();

navD2SF5p021ct = gpstk::BDSWeekSecond(810,345676.400000);
navD2SF5p021 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p021ct);
navD2SF5p021->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p021->addUnsignedLong(0x38905548,30,1);
navD2SF5p021->addUnsignedLong(0x18F0546E,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->addUnsignedLong(0x00000000,30,1);
navD2SF5p021->trimsize();

navD2SF5p022ct = gpstk::BDSWeekSecond(810,345679.400000);
navD2SF5p022 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p022ct);
navD2SF5p022->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p022->addUnsignedLong(0x38905548,30,1);
navD2SF5p022->addUnsignedLong(0x18FC5850,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->addUnsignedLong(0x00000000,30,1);
navD2SF5p022->trimsize();

navD2SF5p023ct = gpstk::BDSWeekSecond(810,345682.400000);
navD2SF5p023 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p023ct);
navD2SF5p023->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p023->addUnsignedLong(0x38905548,30,1);
navD2SF5p023->addUnsignedLong(0x19085FB0,30,1);
navD2SF5p023->addUnsignedLong(0x3FFD3FFB,30,1);
navD2SF5p023->addUnsignedLong(0x3F000080,30,1);
navD2SF5p023->addUnsignedLong(0x00001F07,30,1);
navD2SF5p023->addUnsignedLong(0x3FFC00F9,30,1);
navD2SF5p023->addUnsignedLong(0x00102763,30,1);
navD2SF5p023->addUnsignedLong(0x0DF303F7,30,1);
navD2SF5p023->addUnsignedLong(0x00C08AE3,30,1);
navD2SF5p023->addUnsignedLong(0x23C295A9,30,1);
navD2SF5p023->trimsize();

navD2SF5p024ct = gpstk::BDSWeekSecond(810,345685.400000);
navD2SF5p024 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p024ct);
navD2SF5p024->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p024->addUnsignedLong(0x38905548,30,1);
navD2SF5p024->addUnsignedLong(0x191460E4,30,1);
navD2SF5p024->addUnsignedLong(0x214D00B6,30,1);
navD2SF5p024->addUnsignedLong(0x206060ED,30,1);
navD2SF5p024->addUnsignedLong(0x1818187E,30,1);
navD2SF5p024->addUnsignedLong(0x07F8084B,30,1);
navD2SF5p024->addUnsignedLong(0x0F818192,30,1);
navD2SF5p024->addUnsignedLong(0x206060ED,30,1);
navD2SF5p024->addUnsignedLong(0x110F5E48,30,1);
navD2SF5p024->addUnsignedLong(0x085E051B,30,1);
navD2SF5p024->trimsize();

navD2SF5p025ct = gpstk::BDSWeekSecond(810,345688.400000);
navD2SF5p025 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p025ct);
navD2SF5p025->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p025->addUnsignedLong(0x38905548,30,1);
navD2SF5p025->addUnsignedLong(0x19206647,30,1);
navD2SF5p025->addUnsignedLong(0x018103FA,30,1);
navD2SF5p025->addUnsignedLong(0x2041432D,30,1);
navD2SF5p025->addUnsignedLong(0x1308183E,30,1);
navD2SF5p025->addUnsignedLong(0x0606069E,30,1);
navD2SF5p025->addUnsignedLong(0x018181F2,30,1);
navD2SF5p025->addUnsignedLong(0x3D3EC368,30,1);
navD2SF5p025->addUnsignedLong(0x381818EE,30,1);
navD2SF5p025->addUnsignedLong(0x0606059B,30,1);
navD2SF5p025->trimsize();

navD2SF5p026ct = gpstk::BDSWeekSecond(810,345691.400000);
navD2SF5p026 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p026ct);
navD2SF5p026->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p026->addUnsignedLong(0x38905548,30,1);
navD2SF5p026->addUnsignedLong(0x192C6B7A,30,1);
navD2SF5p026->addUnsignedLong(0x32FFD44B,30,1);
navD2SF5p026->addUnsignedLong(0x3B60601D,30,1);
navD2SF5p026->addUnsignedLong(0x1810E848,30,1);
navD2SF5p026->addUnsignedLong(0x04008DEA,30,1);
navD2SF5p026->addUnsignedLong(0x05818112,30,1);
navD2SF5p026->addUnsignedLong(0x206060ED,30,1);
navD2SF5p026->addUnsignedLong(0x18181075,30,1);
navD2SF5p026->addUnsignedLong(0x0000550D,30,1);
navD2SF5p026->trimsize();

navD2SF5p027ct = gpstk::BDSWeekSecond(810,345694.400000);
navD2SF5p027 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p027ct);
navD2SF5p027->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p027->addUnsignedLong(0x38905548,30,1);
navD2SF5p027->addUnsignedLong(0x19386E1C,30,1);
navD2SF5p027->addUnsignedLong(0x018181F2,30,1);
navD2SF5p027->addUnsignedLong(0x206042E1,30,1);
navD2SF5p027->addUnsignedLong(0x311E91BC,30,1);
navD2SF5p027->addUnsignedLong(0x0206067E,30,1);
navD2SF5p027->addUnsignedLong(0x011502C0,30,1);
navD2SF5p027->addUnsignedLong(0x054812C3,30,1);
navD2SF5p027->addUnsignedLong(0x081818AE,30,1);
navD2SF5p027->addUnsignedLong(0x0606059B,30,1);
navD2SF5p027->trimsize();

navD2SF5p028ct = gpstk::BDSWeekSecond(810,345697.400000);
navD2SF5p028 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p028ct);
navD2SF5p028->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p028->addUnsignedLong(0x38905548,30,1);
navD2SF5p028->addUnsignedLong(0x19447237,30,1);
navD2SF5p028->addUnsignedLong(0x018101FC,30,1);
navD2SF5p028->addUnsignedLong(0x3D8160A2,30,1);
navD2SF5p028->addUnsignedLong(0x1818187E,30,1);
navD2SF5p028->addUnsignedLong(0x06077093,30,1);
navD2SF5p028->addUnsignedLong(0x13E0D523,30,1);
navD2SF5p028->addUnsignedLong(0x3C60602D,30,1);
navD2SF5p028->addUnsignedLong(0x1F0E2F2C,30,1);
navD2SF5p028->addUnsignedLong(0x06048598,30,1);
navD2SF5p028->trimsize();

navD2SF5p029ct = gpstk::BDSWeekSecond(810,345700.400000);
navD2SF5p029 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p029ct);
navD2SF5p029->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p029->addUnsignedLong(0x38905548,30,1);
navD2SF5p029->addUnsignedLong(0x19507652,30,1);
navD2SF5p029->addUnsignedLong(0x018181F2,30,1);
navD2SF5p029->addUnsignedLong(0x206060ED,30,1);
navD2SF5p029->addUnsignedLong(0x1818187E,30,1);
navD2SF5p029->addUnsignedLong(0x0606069E,30,1);
navD2SF5p029->addUnsignedLong(0x018181F2,30,1);
navD2SF5p029->addUnsignedLong(0x207D05B9,30,1);
navD2SF5p029->addUnsignedLong(0x2E8F4F9E,30,1);
navD2SF5p029->addUnsignedLong(0x10820582,30,1);
navD2SF5p029->trimsize();

navD2SF5p030ct = gpstk::BDSWeekSecond(810,345703.400000);
navD2SF5p030 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p030ct);
navD2SF5p030->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p030->addUnsignedLong(0x38905548,30,1);
navD2SF5p030->addUnsignedLong(0x195C786A,30,1);
navD2SF5p030->addUnsignedLong(0x121839F7,30,1);
navD2SF5p030->addUnsignedLong(0x0E19C135,30,1);
navD2SF5p030->addUnsignedLong(0x181FF876,30,1);
navD2SF5p030->addUnsignedLong(0x0606069E,30,1);
navD2SF5p030->addUnsignedLong(0x018100FF,30,1);
navD2SF5p030->addUnsignedLong(0x0320601D,30,1);
navD2SF5p030->addUnsignedLong(0x1818187E,30,1);
navD2SF5p030->addUnsignedLong(0x04144986,30,1);
navD2SF5p030->trimsize();

navD2SF5p031ct = gpstk::BDSWeekSecond(810,345706.400000);
navD2SF5p031 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p031ct);
navD2SF5p031->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p031->addUnsignedLong(0x38905548,30,1);
navD2SF5p031->addUnsignedLong(0x19687FCA,30,1);
navD2SF5p031->addUnsignedLong(0x250812E3,30,1);
navD2SF5p031->addUnsignedLong(0x08E046DD,30,1);
navD2SF5p031->addUnsignedLong(0x1813E54E,30,1);
navD2SF5p031->addUnsignedLong(0x09740A44,30,1);
navD2SF5p031->addUnsignedLong(0x010D819B,30,1);
navD2SF5p031->addUnsignedLong(0x206060ED,30,1);
navD2SF5p031->addUnsignedLong(0x100D81EB,30,1);
navD2SF5p031->addUnsignedLong(0x0606059B,30,1);
navD2SF5p031->trimsize();

navD2SF5p032ct = gpstk::BDSWeekSecond(810,345709.400000);
navD2SF5p032 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p032ct);
navD2SF5p032->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p032->addUnsignedLong(0x38905548,30,1);
navD2SF5p032->addUnsignedLong(0x19748097,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->addUnsignedLong(0x00000000,30,1);
navD2SF5p032->trimsize();

navD2SF5p033ct = gpstk::BDSWeekSecond(810,345712.400000);
navD2SF5p033 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p033ct);
navD2SF5p033->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p033->addUnsignedLong(0x38905548,30,1);
navD2SF5p033->addUnsignedLong(0x198084D2,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->addUnsignedLong(0x00000000,30,1);
navD2SF5p033->trimsize();

navD2SF5p034ct = gpstk::BDSWeekSecond(810,345715.400000);
navD2SF5p034 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p034ct);
navD2SF5p034->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p034->addUnsignedLong(0x38905548,30,1);
navD2SF5p034->addUnsignedLong(0x198C88EC,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->addUnsignedLong(0x00000000,30,1);
navD2SF5p034->trimsize();

navD2SF5p035ct = gpstk::BDSWeekSecond(810,345718.400000);
navD2SF5p035 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p035ct);
navD2SF5p035->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p035->addUnsignedLong(0x38905548,30,1);
navD2SF5p035->addUnsignedLong(0x19988C89,30,1);
navD2SF5p035->addUnsignedLong(0x00000000,30,1);
navD2SF5p035->addUnsignedLong(0x00000000,30,1);
navD2SF5p035->addUnsignedLong(0x002000C0,30,1);
navD2SF5p035->addUnsignedLong(0x00000000,30,1);
navD2SF5p035->addUnsignedLong(0x00000000,30,1);
navD2SF5p035->addUnsignedLong(0x0000FF04,30,1);
navD2SF5p035->addUnsignedLong(0x00080739,30,1);
navD2SF5p035->addUnsignedLong(0x3E2C2ADE,30,1);
navD2SF5p035->trimsize();

navD2SF5p036ct = gpstk::BDSWeekSecond(810,345721.400000);
navD2SF5p036 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p036ct);
navD2SF5p036->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p036->addUnsignedLong(0x38905548,30,1);
navD2SF5p036->addUnsignedLong(0x19A49012,30,1);
navD2SF5p036->addUnsignedLong(0x2C160BEA,30,1);
navD2SF5p036->addUnsignedLong(0x0160B0D1,30,1);
navD2SF5p036->addUnsignedLong(0x160B057D,30,1);
navD2SF5p036->addUnsignedLong(0x20B05869,30,1);
navD2SF5p036->addUnsignedLong(0x0B054227,30,1);
navD2SF5p036->addUnsignedLong(0x22AAAA44,30,1);
navD2SF5p036->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF5p036->addUnsignedLong(0x2AAAAAB4,30,1);
navD2SF5p036->trimsize();

navD2SF5p037ct = gpstk::BDSWeekSecond(810,345724.400000);
navD2SF5p037 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p037ct);
navD2SF5p037->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p037->addUnsignedLong(0x38905548,30,1);
navD2SF5p037->addUnsignedLong(0x19B09772,30,1);
navD2SF5p037->addUnsignedLong(0x0AEB5395,30,1);
navD2SF5p037->addUnsignedLong(0x0056D7D1,30,1);
navD2SF5p037->addUnsignedLong(0x0DEA78AE,30,1);
navD2SF5p037->addUnsignedLong(0x1028182E,30,1);
navD2SF5p037->addUnsignedLong(0x063C2808,30,1);
navD2SF5p037->addUnsignedLong(0x00196052,30,1);
navD2SF5p037->addUnsignedLong(0x003FCA9F,30,1);
navD2SF5p037->addUnsignedLong(0x0A09A1B8,30,1);
navD2SF5p037->trimsize();

navD2SF5p038ct = gpstk::BDSWeekSecond(810,345727.400000);
navD2SF5p038 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p038ct);
navD2SF5p038->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p038->addUnsignedLong(0x38905548,30,1);
navD2SF5p038->addUnsignedLong(0x19BC9B4C,30,1);
navD2SF5p038->addUnsignedLong(0x0AEB3B93,30,1);
navD2SF5p038->addUnsignedLong(0x3FBA1E49,30,1);
navD2SF5p038->addUnsignedLong(0x1B5F5812,30,1);
navD2SF5p038->addUnsignedLong(0x304FD8CC,30,1);
navD2SF5p038->addUnsignedLong(0x1AD228DC,30,1);
navD2SF5p038->addUnsignedLong(0x002435C9,30,1);
navD2SF5p038->addUnsignedLong(0x13C68EEB,30,1);
navD2SF5p038->addUnsignedLong(0x3155DD3E,30,1);
navD2SF5p038->trimsize();

navD2SF5p039ct = gpstk::BDSWeekSecond(810,345730.400000);
navD2SF5p039 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p039ct);
navD2SF5p039->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p039->addUnsignedLong(0x38905548,30,1);
navD2SF5p039->addUnsignedLong(0x19C89F59,30,1);
navD2SF5p039->addUnsignedLong(0x0AEC7F93,30,1);
navD2SF5p039->addUnsignedLong(0x0075FA1D,30,1);
navD2SF5p039->addUnsignedLong(0x189A0627,30,1);
navD2SF5p039->addUnsignedLong(0x203A500F,30,1);
navD2SF5p039->addUnsignedLong(0x2862281C,30,1);
navD2SF5p039->addUnsignedLong(0x002803F5,30,1);
navD2SF5p039->addUnsignedLong(0x2A126D74,30,1);
navD2SF5p039->addUnsignedLong(0x03CDD505,30,1);
navD2SF5p039->trimsize();

navD2SF5p040ct = gpstk::BDSWeekSecond(810,345733.400000);
navD2SF5p040 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p040ct);
navD2SF5p040->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p040->addUnsignedLong(0x38905548,30,1);
navD2SF5p040->addUnsignedLong(0x19D4A308,30,1);
navD2SF5p040->addUnsignedLong(0x0AEB5E91,30,1);
navD2SF5p040->addUnsignedLong(0x009F4B02,30,1);
navD2SF5p040->addUnsignedLong(0x15AA4139,30,1);
navD2SF5p040->addUnsignedLong(0x201FA0CF,30,1);
navD2SF5p040->addUnsignedLong(0x1492280C,30,1);
navD2SF5p040->addUnsignedLong(0x002095C4,30,1);
navD2SF5p040->addUnsignedLong(0x2C71EB92,30,1);
navD2SF5p040->addUnsignedLong(0x1EF2A9F1,30,1);
navD2SF5p040->trimsize();

navD2SF5p041ct = gpstk::BDSWeekSecond(810,345736.400000);
navD2SF5p041 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p041ct);
navD2SF5p041->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p041->addUnsignedLong(0x38905548,30,1);
navD2SF5p041->addUnsignedLong(0x19E0A7AD,30,1);
navD2SF5p041->addUnsignedLong(0x0AEC1F9E,30,1);
navD2SF5p041->addUnsignedLong(0x002FCCF5,30,1);
navD2SF5p041->addUnsignedLong(0x18C81BFB,30,1);
navD2SF5p041->addUnsignedLong(0x003188AA,30,1);
navD2SF5p041->addUnsignedLong(0x250628D5,30,1);
navD2SF5p041->addUnsignedLong(0x0027BBC4,30,1);
navD2SF5p041->addUnsignedLong(0x31A2CC73,30,1);
navD2SF5p041->addUnsignedLong(0x2BCE7563,30,1);
navD2SF5p041->trimsize();

navD2SF5p042ct = gpstk::BDSWeekSecond(810,345739.400000);
navD2SF5p042 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p042ct);
navD2SF5p042->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p042->addUnsignedLong(0x38905548,30,1);
navD2SF5p042->addUnsignedLong(0x19ECAB93,30,1);
navD2SF5p042->addUnsignedLong(0x0AEE9F9D,30,1);
navD2SF5p042->addUnsignedLong(0x006A8245,30,1);
navD2SF5p042->addUnsignedLong(0x2C7CD4A9,30,1);
navD2SF5p042->addUnsignedLong(0x02E41857,30,1);
navD2SF5p042->addUnsignedLong(0x03462966,30,1);
navD2SF5p042->addUnsignedLong(0x3FD9E934,30,1);
navD2SF5p042->addUnsignedLong(0x35023E03,30,1);
navD2SF5p042->addUnsignedLong(0x34EB11B4,30,1);
navD2SF5p042->trimsize();

navD2SF5p043ct = gpstk::BDSWeekSecond(810,345742.400000);
navD2SF5p043 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p043ct);
navD2SF5p043->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p043->addUnsignedLong(0x38905548,30,1);
navD2SF5p043->addUnsignedLong(0x19F8AFF6,30,1);
navD2SF5p043->addUnsignedLong(0x0AEFC59D,30,1);
navD2SF5p043->addUnsignedLong(0x3FA88B20,30,1);
navD2SF5p043->addUnsignedLong(0x00D17B89,30,1);
navD2SF5p043->addUnsignedLong(0x1238AE35,30,1);
navD2SF5p043->addUnsignedLong(0x385A2352,30,1);
navD2SF5p043->addUnsignedLong(0x3FD3E60B,30,1);
navD2SF5p043->addUnsignedLong(0x0DB2684B,30,1);
navD2SF5p043->addUnsignedLong(0x12966991,30,1);
navD2SF5p043->trimsize();

navD2SF5p044ct = gpstk::BDSWeekSecond(810,345745.400000);
navD2SF5p044 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p044ct);
navD2SF5p044->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p044->addUnsignedLong(0x38905548,30,1);
navD2SF5p044->addUnsignedLong(0x1A04B35D,30,1);
navD2SF5p044->addUnsignedLong(0x0AE68AA7,30,1);
navD2SF5p044->addUnsignedLong(0x3FFC4DFA,30,1);
navD2SF5p044->addUnsignedLong(0x16DBD4F2,30,1);
navD2SF5p044->addUnsignedLong(0x115DF296,30,1);
navD2SF5p044->addUnsignedLong(0x04102982,30,1);
navD2SF5p044->addUnsignedLong(0x3FC7646A,30,1);
navD2SF5p044->addUnsignedLong(0x2617356B,30,1);
navD2SF5p044->addUnsignedLong(0x206101EC,30,1);
navD2SF5p044->trimsize();

navD2SF5p045ct = gpstk::BDSWeekSecond(810,345748.400000);
navD2SF5p045 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p045ct);
navD2SF5p045->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p045->addUnsignedLong(0x38905548,30,1);
navD2SF5p045->addUnsignedLong(0x1A10B738,30,1);
navD2SF5p045->addUnsignedLong(0x0AED4A9C,30,1);
navD2SF5p045->addUnsignedLong(0x3FC19064,30,1);
navD2SF5p045->addUnsignedLong(0x2CEC4694,30,1);
navD2SF5p045->addUnsignedLong(0x321DE065,30,1);
navD2SF5p045->addUnsignedLong(0x09B629A6,30,1);
navD2SF5p045->addUnsignedLong(0x3FD9A830,30,1);
navD2SF5p045->addUnsignedLong(0x0F17FEAC,30,1);
navD2SF5p045->addUnsignedLong(0x075245E5,30,1);
navD2SF5p045->trimsize();

navD2SF5p046ct = gpstk::BDSWeekSecond(810,345751.400000);
navD2SF5p046 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p046ct);
navD2SF5p046->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p046->addUnsignedLong(0x38905548,30,1);
navD2SF5p046->addUnsignedLong(0x1A1CBB06,30,1);
navD2SF5p046->addUnsignedLong(0x0AE86A90,30,1);
navD2SF5p046->addUnsignedLong(0x000FEB36,30,1);
navD2SF5p046->addUnsignedLong(0x00C033EA,30,1);
navD2SF5p046->addUnsignedLong(0x11F45631,30,1);
navD2SF5p046->addUnsignedLong(0x3AAC296B,30,1);
navD2SF5p046->addUnsignedLong(0x3FD2A603,30,1);
navD2SF5p046->addUnsignedLong(0x06FA09E5,30,1);
navD2SF5p046->addUnsignedLong(0x2E05E9FD,30,1);
navD2SF5p046->trimsize();

navD2SF5p047ct = gpstk::BDSWeekSecond(810,345754.400000);
navD2SF5p047 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p047ct);
navD2SF5p047->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p047->addUnsignedLong(0x38905548,30,1);
navD2SF5p047->addUnsignedLong(0x1A28BEA0,30,1);
navD2SF5p047->addUnsignedLong(0x251489BF,30,1);
navD2SF5p047->addUnsignedLong(0x0031DAAE,30,1);
navD2SF5p047->addUnsignedLong(0x0D746EA5,30,1);
navD2SF5p047->addUnsignedLong(0x107988FA,30,1);
navD2SF5p047->addUnsignedLong(0x3E5C29CB,30,1);
navD2SF5p047->addUnsignedLong(0x3F6F6ACB,30,1);
navD2SF5p047->addUnsignedLong(0x3DBABC3B,30,1);
navD2SF5p047->addUnsignedLong(0x0E4E1DE5,30,1);
navD2SF5p047->trimsize();

navD2SF5p048ct = gpstk::BDSWeekSecond(810,345757.400000);
navD2SF5p048 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p048ct);
navD2SF5p048->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p048->addUnsignedLong(0x38905548,30,1);
navD2SF5p048->addUnsignedLong(0x1A34C2F6,30,1);
navD2SF5p048->addUnsignedLong(0x25148DB3,30,1);
navD2SF5p048->addUnsignedLong(0x3FEB569A,30,1);
navD2SF5p048->addUnsignedLong(0x0D56B065,30,1);
navD2SF5p048->addUnsignedLong(0x303E00D4,30,1);
navD2SF5p048->addUnsignedLong(0x3C962966,30,1);
navD2SF5p048->addUnsignedLong(0x3F6FAAC2,30,1);
navD2SF5p048->addUnsignedLong(0x1C9A5ECE,30,1);
navD2SF5p048->addUnsignedLong(0x14DA0985,30,1);
navD2SF5p048->trimsize();

navD2SF5p049ct = gpstk::BDSWeekSecond(810,345760.400000);
navD2SF5p049 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p049ct);
navD2SF5p049->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p049->addUnsignedLong(0x38905548,30,1);
navD2SF5p049->addUnsignedLong(0x1A40C7E0,30,1);
navD2SF5p049->addUnsignedLong(0x0AEC8598,30,1);
navD2SF5p049->addUnsignedLong(0x00215DC9,30,1);
navD2SF5p049->addUnsignedLong(0x16A220D7,30,1);
navD2SF5p049->addUnsignedLong(0x00F61949,30,1);
navD2SF5p049->addUnsignedLong(0x11AE29D6,30,1);
navD2SF5p049->addUnsignedLong(0x3FC4E565,30,1);
navD2SF5p049->addUnsignedLong(0x118D8414,30,1);
navD2SF5p049->addUnsignedLong(0x1F27A1DC,30,1);
navD2SF5p049->trimsize();

navD2SF5p050ct = gpstk::BDSWeekSecond(810,345763.400000);
navD2SF5p050 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p050ct);
navD2SF5p050->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p050->addUnsignedLong(0x38905548,30,1);
navD2SF5p050->addUnsignedLong(0x1A4CCADD,30,1);
navD2SF5p050->addUnsignedLong(0x2514A2BB,30,1);
navD2SF5p050->addUnsignedLong(0x3FB3587B,30,1);
navD2SF5p050->addUnsignedLong(0x227F9FC2,30,1);
navD2SF5p050->addUnsignedLong(0x105BE031,30,1);
navD2SF5p050->addUnsignedLong(0x1C44257C,30,1);
navD2SF5p050->addUnsignedLong(0x3F6DBACA,30,1);
navD2SF5p050->addUnsignedLong(0x297110DA,30,1);
navD2SF5p050->addUnsignedLong(0x0E668D1E,30,1);
navD2SF5p050->trimsize();

navD2SF5p051ct = gpstk::BDSWeekSecond(810,345766.400000);
navD2SF5p051 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p051ct);
navD2SF5p051->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p051->addUnsignedLong(0x38905548,30,1);
navD2SF5p051->addUnsignedLong(0x1A58CCBE,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->addUnsignedLong(0x00000000,30,1);
navD2SF5p051->trimsize();

navD2SF5p052ct = gpstk::BDSWeekSecond(810,345769.400000);
navD2SF5p052 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p052ct);
navD2SF5p052->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p052->addUnsignedLong(0x38905548,30,1);
navD2SF5p052->addUnsignedLong(0x1A64D320,30,1);
navD2SF5p052->addUnsignedLong(0x0AED2C9B,30,1);
navD2SF5p052->addUnsignedLong(0x003437AF,30,1);
navD2SF5p052->addUnsignedLong(0x2C601EF4,30,1);
navD2SF5p052->addUnsignedLong(0x00F76046,30,1);
navD2SF5p052->addUnsignedLong(0x17502932,30,1);
navD2SF5p052->addUnsignedLong(0x3FDB2731,30,1);
navD2SF5p052->addUnsignedLong(0x1FAA2FB5,30,1);
navD2SF5p052->addUnsignedLong(0x10BB9D1D,30,1);
navD2SF5p052->trimsize();

navD2SF5p053ct = gpstk::BDSWeekSecond(810,345772.400000);
navD2SF5p053 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p053ct);
navD2SF5p053->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p053->addUnsignedLong(0x38905548,30,1);
navD2SF5p053->addUnsignedLong(0x1A70D440,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->addUnsignedLong(0x00000000,30,1);
navD2SF5p053->trimsize();

navD2SF5p054ct = gpstk::BDSWeekSecond(810,345775.400000);
navD2SF5p054 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p054ct);
navD2SF5p054->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p054->addUnsignedLong(0x38905548,30,1);
navD2SF5p054->addUnsignedLong(0x1A7CD87E,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->addUnsignedLong(0x00000000,30,1);
navD2SF5p054->trimsize();

navD2SF5p055ct = gpstk::BDSWeekSecond(810,345778.400000);
navD2SF5p055 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p055ct);
navD2SF5p055->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p055->addUnsignedLong(0x38905548,30,1);
navD2SF5p055->addUnsignedLong(0x1A88DE3D,30,1);
navD2SF5p055->addUnsignedLong(0x25149BBC,30,1);
navD2SF5p055->addUnsignedLong(0x0023B6C9,30,1);
navD2SF5p055->addUnsignedLong(0x227D7FCC,30,1);
navD2SF5p055->addUnsignedLong(0x303908D4,30,1);
navD2SF5p055->addUnsignedLong(0x1FC029F2,30,1);
navD2SF5p055->addUnsignedLong(0x3F6A35CD,30,1);
navD2SF5p055->addUnsignedLong(0x26ADCBA1,30,1);
navD2SF5p055->addUnsignedLong(0x0FA1E15F,30,1);
navD2SF5p055->trimsize();

navD2SF5p056ct = gpstk::BDSWeekSecond(810,345781.400000);
navD2SF5p056 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p056ct);
navD2SF5p056->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p056->addUnsignedLong(0x38905548,30,1);
navD2SF5p056->addUnsignedLong(0x1A94E26C,30,1);
navD2SF5p056->addUnsignedLong(0x2514A3B8,30,1);
navD2SF5p056->addUnsignedLong(0x001C875E,30,1);
navD2SF5p056->addUnsignedLong(0x227F1FCC,30,1);
navD2SF5p056->addUnsignedLong(0x303538E2,30,1);
navD2SF5p056->addUnsignedLong(0x1FBE29D6,30,1);
navD2SF5p056->addUnsignedLong(0x3F6A3ACF,30,1);
navD2SF5p056->addUnsignedLong(0x0DC39C5E,30,1);
navD2SF5p056->addUnsignedLong(0x016F61E5,30,1);
navD2SF5p056->trimsize();

navD2SF5p057ct = gpstk::BDSWeekSecond(810,345784.400000);
navD2SF5p057 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p057ct);
navD2SF5p057->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p057->addUnsignedLong(0x38905548,30,1);
navD2SF5p057->addUnsignedLong(0x1AA0E6C9,30,1);
navD2SF5p057->addUnsignedLong(0x25149FB0,30,1);
navD2SF5p057->addUnsignedLong(0x00042B0D,30,1);
navD2SF5p057->addUnsignedLong(0x22836FBD,30,1);
navD2SF5p057->addUnsignedLong(0x002FE8F3,30,1);
navD2SF5p057->addUnsignedLong(0x1ECE2966,30,1);
navD2SF5p057->addUnsignedLong(0x3F6A38C9,30,1);
navD2SF5p057->addUnsignedLong(0x190A36B8,30,1);
navD2SF5p057->addUnsignedLong(0x22BFD528,30,1);
navD2SF5p057->trimsize();

navD2SF5p058ct = gpstk::BDSWeekSecond(810,345787.400000);
navD2SF5p058 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p058ct);
navD2SF5p058->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p058->addUnsignedLong(0x38905548,30,1);
navD2SF5p058->addUnsignedLong(0x1AACEAF7,30,1);
navD2SF5p058->addUnsignedLong(0x25149DB6,30,1);
navD2SF5p058->addUnsignedLong(0x00041E0D,30,1);
navD2SF5p058->addUnsignedLong(0x228336B7,30,1);
navD2SF5p058->addUnsignedLong(0x102B6824,30,1);
navD2SF5p058->addUnsignedLong(0x1ED2293F,30,1);
navD2SF5p058->addUnsignedLong(0x3F6A3EC3,30,1);
navD2SF5p058->addUnsignedLong(0x3FF906F5,30,1);
navD2SF5p058->addUnsignedLong(0x38537962,30,1);
navD2SF5p058->trimsize();

navD2SF5p059ct = gpstk::BDSWeekSecond(810,345790.400000);
navD2SF5p059 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p059ct);
navD2SF5p059->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p059->addUnsignedLong(0x38905548,30,1);
navD2SF5p059->addUnsignedLong(0x1AB8EE92,30,1);
navD2SF5p059->addUnsignedLong(0x251479B9,30,1);
navD2SF5p059->addUnsignedLong(0x000C0633,30,1);
navD2SF5p059->addUnsignedLong(0x37F0D837,30,1);
navD2SF5p059->addUnsignedLong(0x2018B8CA,30,1);
navD2SF5p059->addUnsignedLong(0x08A4296B,30,1);
navD2SF5p059->addUnsignedLong(0x3F64EEFB,30,1);
navD2SF5p059->addUnsignedLong(0x110E9D4B,30,1);
navD2SF5p059->addUnsignedLong(0x31F461A7,30,1);
navD2SF5p059->trimsize();

navD2SF5p060ct = gpstk::BDSWeekSecond(810,345793.400000);
navD2SF5p060 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p060ct);
navD2SF5p060->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p060->addUnsignedLong(0x38905548,30,1);
navD2SF5p060->addUnsignedLong(0x1AC4F2B9,30,1);
navD2SF5p060->addUnsignedLong(0x251474BD,30,1);
navD2SF5p060->addUnsignedLong(0x3FDC2C34,30,1);
navD2SF5p060->addUnsignedLong(0x37F05F30,30,1);
navD2SF5p060->addUnsignedLong(0x30194813,30,1);
navD2SF5p060->addUnsignedLong(0x08A2296F,30,1);
navD2SF5p060->addUnsignedLong(0x3F64FFFD,30,1);
navD2SF5p060->addUnsignedLong(0x10176DB2,30,1);
navD2SF5p060->addUnsignedLong(0x260DB134,30,1);
navD2SF5p060->trimsize();

navD2SF5p061ct = gpstk::BDSWeekSecond(810,345796.400000);
navD2SF5p061 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p061ct);
navD2SF5p061->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p061->addUnsignedLong(0x38905548,30,1);
navD2SF5p061->addUnsignedLong(0x1AD0F7DF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p061->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p061->trimsize();

navD2SF5p062ct = gpstk::BDSWeekSecond(810,345799.400000);
navD2SF5p062 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p062ct);
navD2SF5p062->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p062->addUnsignedLong(0x38905548,30,1);
navD2SF5p062->addUnsignedLong(0x1ADCFBE1,30,1);
navD2SF5p062->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p062->addUnsignedLong(0x3FFF82F3,30,1);
navD2SF5p062->addUnsignedLong(0x01FFFFDF,30,1);
navD2SF5p062->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p062->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p062->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p062->addUnsignedLong(0x3FE084A2,30,1);
navD2SF5p062->addUnsignedLong(0x012A005D,30,1);
navD2SF5p062->trimsize();

navD2SF5p063ct = gpstk::BDSWeekSecond(810,345802.400000);
navD2SF5p063 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p063ct);
navD2SF5p063->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p063->addUnsignedLong(0x38905548,30,1);
navD2SF5p063->addUnsignedLong(0x1AE8FC41,30,1);
navD2SF5p063->addUnsignedLong(0x04101C82,30,1);
navD2SF5p063->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p063->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p063->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p063->addUnsignedLong(0x209F0594,30,1);
navD2SF5p063->addUnsignedLong(0x060A30A2,30,1);
navD2SF5p063->addUnsignedLong(0x0E4083DB,30,1);
navD2SF5p063->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p063->trimsize();

navD2SF5p064ct = gpstk::BDSWeekSecond(810,345805.400000);
navD2SF5p064 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p064ct);
navD2SF5p064->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p064->addUnsignedLong(0x38905548,30,1);
navD2SF5p064->addUnsignedLong(0x1AF50313,30,1);
navD2SF5p064->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p064->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p064->addUnsignedLong(0x3F05F88B,30,1);
navD2SF5p064->addUnsignedLong(0x0B30598A,30,1);
navD2SF5p064->addUnsignedLong(0x0092FF39,30,1);
navD2SF5p064->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p064->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p064->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p064->trimsize();

navD2SF5p065ct = gpstk::BDSWeekSecond(810,345808.400000);
navD2SF5p065 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p065ct);
navD2SF5p065->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p065->addUnsignedLong(0x38905548,30,1);
navD2SF5p065->addUnsignedLong(0x1B0104F3,30,1);
navD2SF5p065->addUnsignedLong(0x04002CED,30,1);
navD2SF5p065->addUnsignedLong(0x206202EB,30,1);
navD2SF5p065->addUnsignedLong(0x3205173A,30,1);
navD2SF5p065->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p065->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p065->addUnsignedLong(0x3FFFF0FD,30,1);
navD2SF5p065->addUnsignedLong(0x1400C33C,30,1);
navD2SF5p065->addUnsignedLong(0x018600F4,30,1);
navD2SF5p065->trimsize();

navD2SF5p066ct = gpstk::BDSWeekSecond(810,345811.400000);
navD2SF5p066 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p066ct);
navD2SF5p066->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p066->addUnsignedLong(0x38905548,30,1);
navD2SF5p066->addUnsignedLong(0x1B0D08CD,30,1);
navD2SF5p066->addUnsignedLong(0x05902476,30,1);
navD2SF5p066->addUnsignedLong(0x2FFFFF2F,30,1);
navD2SF5p066->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p066->addUnsignedLong(0x3FF060CD,30,1);
navD2SF5p066->addUnsignedLong(0x00B206F7,30,1);
navD2SF5p066->addUnsignedLong(0x260D203C,30,1);
navD2SF5p066->addUnsignedLong(0x1660A232,30,1);
navD2SF5p066->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p066->trimsize();

navD2SF5p067ct = gpstk::BDSWeekSecond(810,345814.400000);
navD2SF5p067 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p067ct);
navD2SF5p067->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p067->addUnsignedLong(0x38905548,30,1);
navD2SF5p067->addUnsignedLong(0x1B190FAD,30,1);
navD2SF5p067->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p067->addUnsignedLong(0x30820318,30,1);
navD2SF5p067->addUnsignedLong(0x21079030,30,1);
navD2SF5p067->addUnsignedLong(0x0D20717B,30,1);
navD2SF5p067->addUnsignedLong(0x20D20512,30,1);
navD2SF5p067->addUnsignedLong(0x25FFFFAF,30,1);
navD2SF5p067->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p067->addUnsignedLong(0x024600C4,30,1);
navD2SF5p067->trimsize();

navD2SF5p068ct = gpstk::BDSWeekSecond(810,345817.400000);
navD2SF5p068 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p068ct);
navD2SF5p068->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p068->addUnsignedLong(0x38905548,30,1);
navD2SF5p068->addUnsignedLong(0x1B251033,30,1);
navD2SF5p068->addUnsignedLong(0x0810449B,30,1);
navD2SF5p068->addUnsignedLong(0x208103CA,30,1);
navD2SF5p068->addUnsignedLong(0x35071805,30,1);
navD2SF5p068->addUnsignedLong(0x0C2FFFEF,30,1);
navD2SF5p068->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p068->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p068->addUnsignedLong(0x3FE123A0,30,1);
navD2SF5p068->addUnsignedLong(0x02860024,30,1);
navD2SF5p068->trimsize();

navD2SF5p069ct = gpstk::BDSWeekSecond(810,345820.400000);
navD2SF5p069 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p069ct);
navD2SF5p069->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p069->addUnsignedLong(0x38905548,30,1);
navD2SF5p069->addUnsignedLong(0x1B311456,30,1);
navD2SF5p069->addUnsignedLong(0x092845A8,30,1);
navD2SF5p069->addUnsignedLong(0x00698247,30,1);
navD2SF5p069->addUnsignedLong(0x22FFFF9F,30,1);
navD2SF5p069->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p069->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p069->addUnsignedLong(0x3E164043,30,1);
navD2SF5p069->addUnsignedLong(0x2AC124F9,30,1);
navD2SF5p069->addUnsignedLong(0x01E40089,30,1);
navD2SF5p069->trimsize();

navD2SF5p070ct = gpstk::BDSWeekSecond(810,345823.400000);
navD2SF5p070 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p070ct);
navD2SF5p070->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p070->addUnsignedLong(0x38905548,30,1);
navD2SF5p070->addUnsignedLong(0x1B3D1868,30,1);
navD2SF5p070->addUnsignedLong(0x069021A9,30,1);
navD2SF5p070->addUnsignedLong(0x2FFFFF2F,30,1);
navD2SF5p070->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p070->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p070->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p070->addUnsignedLong(0x3FFFF0FD,30,1);
navD2SF5p070->addUnsignedLong(0x205FFF7F,30,1);
navD2SF5p070->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p070->trimsize();

navD2SF5p071ct = gpstk::BDSWeekSecond(810,345826.400000);
navD2SF5p071 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p071ct);
navD2SF5p071->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p071->addUnsignedLong(0x38905548,30,1);
navD2SF5p071->addUnsignedLong(0x1B491F78,30,1);
navD2SF5p071->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p071->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p071->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p071->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p071->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p071->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p071->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p071->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p071->trimsize();

navD2SF5p072ct = gpstk::BDSWeekSecond(810,345829.400000);
navD2SF5p072 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p072ct);
navD2SF5p072->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p072->addUnsignedLong(0x38905548,30,1);
navD2SF5p072->addUnsignedLong(0x1B552329,30,1);
navD2SF5p072->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p072->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p072->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p072->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p072->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p072->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p072->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p072->addUnsignedLong(0x3FFE00F4,30,1);
navD2SF5p072->trimsize();

navD2SF5p073ct = gpstk::BDSWeekSecond(810,345832.400000);
navD2SF5p073 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p073ct);
navD2SF5p073->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p073->addUnsignedLong(0x38905548,30,1);
navD2SF5p073->addUnsignedLong(0x1B61278C,30,1);
navD2SF5p073->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p073->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p073->addUnsignedLong(0x3F000080,30,1);
navD2SF5p073->addUnsignedLong(0x00000000,30,1);
navD2SF5p073->addUnsignedLong(0x00000000,30,1);
navD2SF5p073->addUnsignedLong(0x00000000,30,1);
navD2SF5p073->addUnsignedLong(0x00000000,30,1);
navD2SF5p073->addUnsignedLong(0x00000000,30,1);
navD2SF5p073->trimsize();

navD2SF5p074ct = gpstk::BDSWeekSecond(810,345835.400000);
navD2SF5p074 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p074ct);
navD2SF5p074->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p074->addUnsignedLong(0x38905548,30,1);
navD2SF5p074->addUnsignedLong(0x1B6D28B7,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->addUnsignedLong(0x00000000,30,1);
navD2SF5p074->trimsize();

navD2SF5p075ct = gpstk::BDSWeekSecond(810,345838.400000);
navD2SF5p075 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p075ct);
navD2SF5p075->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p075->addUnsignedLong(0x38905548,30,1);
navD2SF5p075->addUnsignedLong(0x1B792CD2,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->addUnsignedLong(0x00000000,30,1);
navD2SF5p075->trimsize();

navD2SF5p076ct = gpstk::BDSWeekSecond(810,345841.400000);
navD2SF5p076 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p076ct);
navD2SF5p076->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p076->addUnsignedLong(0x38905548,30,1);
navD2SF5p076->addUnsignedLong(0x1B8530A9,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->addUnsignedLong(0x00000000,30,1);
navD2SF5p076->trimsize();

navD2SF5p077ct = gpstk::BDSWeekSecond(810,345844.400000);
navD2SF5p077 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p077ct);
navD2SF5p077->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p077->addUnsignedLong(0x38905548,30,1);
navD2SF5p077->addUnsignedLong(0x1B9134CC,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->addUnsignedLong(0x00000000,30,1);
navD2SF5p077->trimsize();

navD2SF5p078ct = gpstk::BDSWeekSecond(810,345847.400000);
navD2SF5p078 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p078ct);
navD2SF5p078->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p078->addUnsignedLong(0x38905548,30,1);
navD2SF5p078->addUnsignedLong(0x1B9D38F2,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->addUnsignedLong(0x00000000,30,1);
navD2SF5p078->trimsize();

navD2SF5p079ct = gpstk::BDSWeekSecond(810,345850.400000);
navD2SF5p079 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p079ct);
navD2SF5p079->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p079->addUnsignedLong(0x38905548,30,1);
navD2SF5p079->addUnsignedLong(0x1BA93C57,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->addUnsignedLong(0x00000000,30,1);
navD2SF5p079->trimsize();

navD2SF5p080ct = gpstk::BDSWeekSecond(810,345853.400000);
navD2SF5p080 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p080ct);
navD2SF5p080->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p080->addUnsignedLong(0x38905548,30,1);
navD2SF5p080->addUnsignedLong(0x1BB54001,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->addUnsignedLong(0x00000000,30,1);
navD2SF5p080->trimsize();

navD2SF5p081ct = gpstk::BDSWeekSecond(810,345856.400000);
navD2SF5p081 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p081ct);
navD2SF5p081->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p081->addUnsignedLong(0x38905548,30,1);
navD2SF5p081->addUnsignedLong(0x1BC14414,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->addUnsignedLong(0x00000000,30,1);
navD2SF5p081->trimsize();

navD2SF5p082ct = gpstk::BDSWeekSecond(810,345859.400000);
navD2SF5p082 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p082ct);
navD2SF5p082->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p082->addUnsignedLong(0x38905548,30,1);
navD2SF5p082->addUnsignedLong(0x1BCD482A,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->addUnsignedLong(0x00000000,30,1);
navD2SF5p082->trimsize();

navD2SF5p083ct = gpstk::BDSWeekSecond(810,345862.400000);
navD2SF5p083 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p083ct);
navD2SF5p083->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p083->addUnsignedLong(0x38905548,30,1);
navD2SF5p083->addUnsignedLong(0x1BD94F4A,30,1);
navD2SF5p083->addUnsignedLong(0x3FFD3FFB,30,1);
navD2SF5p083->addUnsignedLong(0x3F000080,30,1);
navD2SF5p083->addUnsignedLong(0x00001F07,30,1);
navD2SF5p083->addUnsignedLong(0x3FFC00F9,30,1);
navD2SF5p083->addUnsignedLong(0x00181F57,30,1);
navD2SF5p083->addUnsignedLong(0x0DF303F7,30,1);
navD2SF5p083->addUnsignedLong(0x00C08BE0,30,1);
navD2SF5p083->addUnsignedLong(0x23C295A9,30,1);
navD2SF5p083->trimsize();

navD2SF5p084ct = gpstk::BDSWeekSecond(810,345865.400000);
navD2SF5p084 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p084ct);
navD2SF5p084->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p084->addUnsignedLong(0x38905548,30,1);
navD2SF5p084->addUnsignedLong(0x1BE551D7,30,1);
navD2SF5p084->addUnsignedLong(0x074B00B2,30,1);
navD2SF5p084->addUnsignedLong(0x206060ED,30,1);
navD2SF5p084->addUnsignedLong(0x1818187E,30,1);
navD2SF5p084->addUnsignedLong(0x0410088B,30,1);
navD2SF5p084->addUnsignedLong(0x0F818192,30,1);
navD2SF5p084->addUnsignedLong(0x206060ED,30,1);
navD2SF5p084->addUnsignedLong(0x110F1E4F,30,1);
navD2SF5p084->addUnsignedLong(0x005A05E2,30,1);
navD2SF5p084->trimsize();

navD2SF5p085ct = gpstk::BDSWeekSecond(810,345868.400000);
navD2SF5p085 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p085ct);
navD2SF5p085->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p085->addUnsignedLong(0x38905548,30,1);
navD2SF5p085->addUnsignedLong(0x1BF156B7,30,1);
navD2SF5p085->addUnsignedLong(0x018103FA,30,1);
navD2SF5p085->addUnsignedLong(0x2040D02C,30,1);
navD2SF5p085->addUnsignedLong(0x02A818DE,30,1);
navD2SF5p085->addUnsignedLong(0x0606069E,30,1);
navD2SF5p085->addUnsignedLong(0x018181F2,30,1);
navD2SF5p085->addUnsignedLong(0x3E7E4306,30,1);
navD2SF5p085->addUnsignedLong(0x1818187E,30,1);
navD2SF5p085->addUnsignedLong(0x0606059B,30,1);
navD2SF5p085->trimsize();

navD2SF5p086ct = gpstk::BDSWeekSecond(810,345871.400000);
navD2SF5p086 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p086ct);
navD2SF5p086->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p086->addUnsignedLong(0x38905548,30,1);
navD2SF5p086->addUnsignedLong(0x1BFD5B8A,30,1);
navD2SF5p086->addUnsignedLong(0x32FCD340,30,1);
navD2SF5p086->addUnsignedLong(0x3B2060AD,30,1);
navD2SF5p086->addUnsignedLong(0x1810E848,30,1);
navD2SF5p086->addUnsignedLong(0x040151EE,30,1);
navD2SF5p086->addUnsignedLong(0x00818152,30,1);
navD2SF5p086->addUnsignedLong(0x206060ED,30,1);
navD2SF5p086->addUnsignedLong(0x18181075,30,1);
navD2SF5p086->addUnsignedLong(0x1BF451B8,30,1);
navD2SF5p086->trimsize();

navD2SF5p087ct = gpstk::BDSWeekSecond(810,345874.400000);
navD2SF5p087 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p087ct);
navD2SF5p087->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p087->addUnsignedLong(0x38905548,30,1);
navD2SF5p087->addUnsignedLong(0x1C095EFC,30,1);
navD2SF5p087->addUnsignedLong(0x018181F2,30,1);
navD2SF5p087->addUnsignedLong(0x206041E4,30,1);
navD2SF5p087->addUnsignedLong(0x008E1061,30,1);
navD2SF5p087->addUnsignedLong(0x220606EE,30,1);
navD2SF5p087->addUnsignedLong(0x010FFC9A,30,1);
navD2SF5p087->addUnsignedLong(0x034FCF50,30,1);
navD2SF5p087->addUnsignedLong(0x1818187E,30,1);
navD2SF5p087->addUnsignedLong(0x0606059B,30,1);
navD2SF5p087->trimsize();

navD2SF5p088ct = gpstk::BDSWeekSecond(810,345877.400000);
navD2SF5p088 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p088ct);
navD2SF5p088->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p088->addUnsignedLong(0x38905548,30,1);
navD2SF5p088->addUnsignedLong(0x1C1562AD,30,1);
navD2SF5p088->addUnsignedLong(0x018100FF,30,1);
navD2SF5p088->addUnsignedLong(0x3BBFA0AF,30,1);
navD2SF5p088->addUnsignedLong(0x1818187E,30,1);
navD2SF5p088->addUnsignedLong(0x06076096,30,1);
navD2SF5p088->addUnsignedLong(0x0CDCD1A6,30,1);
navD2SF5p088->addUnsignedLong(0x3B2060AD,30,1);
navD2SF5p088->addUnsignedLong(0x1EFDEEC4,30,1);
navD2SF5p088->addUnsignedLong(0x3204693C,30,1);
navD2SF5p088->trimsize();

navD2SF5p089ct = gpstk::BDSWeekSecond(810,345880.400000);
navD2SF5p089 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p089ct);
navD2SF5p089->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p089->addUnsignedLong(0x38905548,30,1);
navD2SF5p089->addUnsignedLong(0x1C216608,30,1);
navD2SF5p089->addUnsignedLong(0x018181F2,30,1);
navD2SF5p089->addUnsignedLong(0x206060ED,30,1);
navD2SF5p089->addUnsignedLong(0x1818187E,30,1);
navD2SF5p089->addUnsignedLong(0x0606069E,30,1);
navD2SF5p089->addUnsignedLong(0x018181F2,30,1);
navD2SF5p089->addUnsignedLong(0x207840B7,30,1);
navD2SF5p089->addUnsignedLong(0x2D5E1EC0,30,1);
navD2SF5p089->addUnsignedLong(0x003205A2,30,1);
navD2SF5p089->trimsize();

navD2SF5p090ct = gpstk::BDSWeekSecond(810,345883.400000);
navD2SF5p090 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p090ct);
navD2SF5p090->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p090->addUnsignedLong(0x38905548,30,1);
navD2SF5p090->addUnsignedLong(0x1C2D6830,30,1);
navD2SF5p090->addUnsignedLong(0x010425AC,30,1);
navD2SF5p090->addUnsignedLong(0x0E547CB6,30,1);
navD2SF5p090->addUnsignedLong(0x381EA8EB,30,1);
navD2SF5p090->addUnsignedLong(0x0606069E,30,1);
navD2SF5p090->addUnsignedLong(0x0181EDF8,30,1);
navD2SF5p090->addUnsignedLong(0x3EA060BD,30,1);
navD2SF5p090->addUnsignedLong(0x1818187E,30,1);
navD2SF5p090->addUnsignedLong(0x04205121,30,1);
navD2SF5p090->trimsize();

navD2SF5p091ct = gpstk::BDSWeekSecond(810,345886.400000);
navD2SF5p091 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p091ct);
navD2SF5p091->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p091->addUnsignedLong(0x38905548,30,1);
navD2SF5p091->addUnsignedLong(0x1C396F50,30,1);
navD2SF5p091->addUnsignedLong(0x290C15F3,30,1);
navD2SF5p091->addUnsignedLong(0x09A047CE,30,1);
navD2SF5p091->addUnsignedLong(0x2814250C,30,1);
navD2SF5p091->addUnsignedLong(0x2D7C1A01,30,1);
navD2SF5p091->addUnsignedLong(0x010E8199,30,1);
navD2SF5p091->addUnsignedLong(0x206060ED,30,1);
navD2SF5p091->addUnsignedLong(0x108DC1BC,30,1);
navD2SF5p091->addUnsignedLong(0x1606054B,30,1);
navD2SF5p091->trimsize();

navD2SF5p092ct = gpstk::BDSWeekSecond(810,345889.400000);
navD2SF5p092 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p092ct);
navD2SF5p092->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p092->addUnsignedLong(0x38905548,30,1);
navD2SF5p092->addUnsignedLong(0x1C45707E,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->addUnsignedLong(0x00000000,30,1);
navD2SF5p092->trimsize();

navD2SF5p093ct = gpstk::BDSWeekSecond(810,345892.400000);
navD2SF5p093 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p093ct);
navD2SF5p093->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p093->addUnsignedLong(0x38905548,30,1);
navD2SF5p093->addUnsignedLong(0x1C51741B,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->addUnsignedLong(0x00000000,30,1);
navD2SF5p093->trimsize();

navD2SF5p094ct = gpstk::BDSWeekSecond(810,345895.400000);
navD2SF5p094 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p094ct);
navD2SF5p094->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p094->addUnsignedLong(0x38905548,30,1);
navD2SF5p094->addUnsignedLong(0x1C5D7825,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->addUnsignedLong(0x00000000,30,1);
navD2SF5p094->trimsize();

navD2SF5p095ct = gpstk::BDSWeekSecond(810,345898.400000);
navD2SF5p095 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p095ct);
navD2SF5p095->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p095->addUnsignedLong(0x38905548,30,1);
navD2SF5p095->addUnsignedLong(0x1C697E86,30,1);
navD2SF5p095->addUnsignedLong(0x25147FB3,30,1);
navD2SF5p095->addUnsignedLong(0x00145E6A,30,1);
navD2SF5p095->addUnsignedLong(0x37AE7BD2,30,1);
navD2SF5p095->addUnsignedLong(0x1017A0BF,30,1);
navD2SF5p095->addUnsignedLong(0x0A4A290F,30,1);
navD2SF5p095->addUnsignedLong(0x3F6500F6,30,1);
navD2SF5p095->addUnsignedLong(0x2411BB10,30,1);
navD2SF5p095->addUnsignedLong(0x0A5EAD64,30,1);
navD2SF5p095->trimsize();

navD2SF5p096ct = gpstk::BDSWeekSecond(810,345901.400000);
navD2SF5p096 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p096ct);
navD2SF5p096->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p096->addUnsignedLong(0x38905548,30,1);
navD2SF5p096->addUnsignedLong(0x1C7582DE,30,1);
navD2SF5p096->addUnsignedLong(0x25147EB0,30,1);
navD2SF5p096->addUnsignedLong(0x3FFB9BF7,30,1);
navD2SF5p096->addUnsignedLong(0x37AFF5D2,30,1);
navD2SF5p096->addUnsignedLong(0x302A60B0,30,1);
navD2SF5p096->addUnsignedLong(0x0A5C296B,30,1);
navD2SF5p096->addUnsignedLong(0x3F6541F2,30,1);
navD2SF5p096->addUnsignedLong(0x2C320E2C,30,1);
navD2SF5p096->addUnsignedLong(0x37098D45,30,1);
navD2SF5p096->trimsize();

navD2SF5p097ct = gpstk::BDSWeekSecond(810,345904.400000);
navD2SF5p097 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p097ct);
navD2SF5p097->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p097->addUnsignedLong(0x38905548,30,1);
navD2SF5p097->addUnsignedLong(0x1C81869B,30,1);
navD2SF5p097->addUnsignedLong(0x25149FB0,30,1);
navD2SF5p097->addUnsignedLong(0x00124E6B,30,1);
navD2SF5p097->addUnsignedLong(0x0D5D1C54,30,1);
navD2SF5p097->addUnsignedLong(0x302FF0BD,30,1);
navD2SF5p097->addUnsignedLong(0x1F1E2946,30,1);
navD2SF5p097->addUnsignedLong(0x3F6B8CCB,30,1);
navD2SF5p097->addUnsignedLong(0x118DDE1B,30,1);
navD2SF5p097->addUnsignedLong(0x384EE134,30,1);
navD2SF5p097->trimsize();

navD2SF5p098ct = gpstk::BDSWeekSecond(810,345907.400000);
navD2SF5p098 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p098ct);
navD2SF5p098->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p098->addUnsignedLong(0x38905548,30,1);
navD2SF5p098->addUnsignedLong(0x1C8D8AA5,30,1);
navD2SF5p098->addUnsignedLong(0x25149FB0,30,1);
navD2SF5p098->addUnsignedLong(0x000FCA3F,30,1);
navD2SF5p098->addUnsignedLong(0x0D5CA552,30,1);
navD2SF5p098->addUnsignedLong(0x000BD03E,30,1);
navD2SF5p098->addUnsignedLong(0x1EFA29CF,30,1);
navD2SF5p098->addUnsignedLong(0x3F6B9CCE,30,1);
navD2SF5p098->addUnsignedLong(0x1AED3C4E,30,1);
navD2SF5p098->addUnsignedLong(0x3676E9CF,30,1);
navD2SF5p098->trimsize();

navD2SF5p099ct = gpstk::BDSWeekSecond(810,345910.400000);
navD2SF5p099 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p099ct);
navD2SF5p099->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p099->addUnsignedLong(0x38905548,30,1);
navD2SF5p099->addUnsignedLong(0x1C998EC0,30,1);
navD2SF5p099->addUnsignedLong(0x25149BBC,30,1);
navD2SF5p099->addUnsignedLong(0x0009B737,30,1);
navD2SF5p099->addUnsignedLong(0x0CF88268,30,1);
navD2SF5p099->addUnsignedLong(0x30164023,30,1);
navD2SF5p099->addUnsignedLong(0x1D8C290B,30,1);
navD2SF5p099->addUnsignedLong(0x3F6B50C0,30,1);
navD2SF5p099->addUnsignedLong(0x25CF2909,30,1);
navD2SF5p099->addUnsignedLong(0x33D73117,30,1);
navD2SF5p099->trimsize();

navD2SF5p100ct = gpstk::BDSWeekSecond(810,345913.400000);
navD2SF5p100 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p100ct);
navD2SF5p100->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p100->addUnsignedLong(0x38905548,30,1);
navD2SF5p100->addUnsignedLong(0x1CA5925B,30,1);
navD2SF5p100->addUnsignedLong(0x251496B8,30,1);
navD2SF5p100->addUnsignedLong(0x0001F10A,30,1);
navD2SF5p100->addUnsignedLong(0x0CF9E867,30,1);
navD2SF5p100->addUnsignedLong(0x001DB85C,30,1);
navD2SF5p100->addUnsignedLong(0x1D7A294F,30,1);
navD2SF5p100->addUnsignedLong(0x3F6B46CF,30,1);
navD2SF5p100->addUnsignedLong(0x2E01DEF2,30,1);
navD2SF5p100->addUnsignedLong(0x15346D30,30,1);
navD2SF5p100->trimsize();

navD2SF5p101ct = gpstk::BDSWeekSecond(810,345916.400000);
navD2SF5p101 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p101ct);
navD2SF5p101->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p101->addUnsignedLong(0x38905548,30,1);
navD2SF5p101->addUnsignedLong(0x1CB19438,30,1);
navD2SF5p101->addUnsignedLong(0x00000000,30,1);
navD2SF5p101->addUnsignedLong(0x00000000,30,1);
navD2SF5p101->addUnsignedLong(0x00000000,30,1);
navD2SF5p101->addUnsignedLong(0x00000000,30,1);
navD2SF5p101->addUnsignedLong(0x00000000,30,1);
navD2SF5p101->addUnsignedLong(0x0015556B,30,1);
navD2SF5p101->addUnsignedLong(0x1555554B,30,1);
navD2SF5p101->addUnsignedLong(0x1555554B,30,1);
navD2SF5p101->trimsize();

navD2SF5p102ct = gpstk::BDSWeekSecond(810,345919.400000);
navD2SF5p102 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p102ct);
navD2SF5p102->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p102->addUnsignedLong(0x38905548,30,1);
navD2SF5p102->addUnsignedLong(0x1CBD9806,30,1);
navD2SF5p102->addUnsignedLong(0x04043DE2,30,1);
navD2SF5p102->addUnsignedLong(0x3FFFFFFF,30,1);
navD2SF5p102->addUnsignedLong(0x3F9000B0,30,1);
navD2SF5p102->addUnsignedLong(0x0000190D,30,1);
navD2SF5p102->addUnsignedLong(0x1555554B,30,1);
navD2SF5p102->addUnsignedLong(0x1555554B,30,1);
navD2SF5p102->addUnsignedLong(0x1555554B,30,1);
navD2SF5p102->addUnsignedLong(0x1555554B,30,1);
navD2SF5p102->trimsize();

navD2SF5p103ct = gpstk::BDSWeekSecond(810,345922.400000);
navD2SF5p103 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p103ct);
navD2SF5p103->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p103->addUnsignedLong(0x38905548,30,1);
navD2SF5p103->addUnsignedLong(0x1CC99C13,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->addUnsignedLong(0x00000000,30,1);
navD2SF5p103->trimsize();

navD2SF5p104ct = gpstk::BDSWeekSecond(810,345925.400000);
navD2SF5p104 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p104ct);
navD2SF5p104->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p104->addUnsignedLong(0x38905548,30,1);
navD2SF5p104->addUnsignedLong(0x1CD5A042,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->addUnsignedLong(0x00000000,30,1);
navD2SF5p104->trimsize();

navD2SF5p105ct = gpstk::BDSWeekSecond(810,345928.400000);
navD2SF5p105 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p105ct);
navD2SF5p105->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p105->addUnsignedLong(0x38905548,30,1);
navD2SF5p105->addUnsignedLong(0x1CE1A4E7,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->addUnsignedLong(0x00000000,30,1);
navD2SF5p105->trimsize();

navD2SF5p106ct = gpstk::BDSWeekSecond(810,345931.400000);
navD2SF5p106 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p106ct);
navD2SF5p106->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p106->addUnsignedLong(0x38905548,30,1);
navD2SF5p106->addUnsignedLong(0x1CEDA8D9,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->addUnsignedLong(0x00000000,30,1);
navD2SF5p106->trimsize();

navD2SF5p107ct = gpstk::BDSWeekSecond(810,345934.400000);
navD2SF5p107 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p107ct);
navD2SF5p107->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p107->addUnsignedLong(0x38905548,30,1);
navD2SF5p107->addUnsignedLong(0x1CF9ACBC,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->addUnsignedLong(0x00000000,30,1);
navD2SF5p107->trimsize();

navD2SF5p108ct = gpstk::BDSWeekSecond(810,345937.400000);
navD2SF5p108 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p108ct);
navD2SF5p108->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p108->addUnsignedLong(0x38905548,30,1);
navD2SF5p108->addUnsignedLong(0x1D05B067,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->addUnsignedLong(0x00000000,30,1);
navD2SF5p108->trimsize();

navD2SF5p109ct = gpstk::BDSWeekSecond(810,345940.400000);
navD2SF5p109 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p109ct);
navD2SF5p109->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p109->addUnsignedLong(0x38905548,30,1);
navD2SF5p109->addUnsignedLong(0x1D11B402,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->addUnsignedLong(0x00000000,30,1);
navD2SF5p109->trimsize();

navD2SF5p110ct = gpstk::BDSWeekSecond(810,345943.400000);
navD2SF5p110 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p110ct);
navD2SF5p110->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p110->addUnsignedLong(0x38905548,30,1);
navD2SF5p110->addUnsignedLong(0x1D1DB83C,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->addUnsignedLong(0x00000000,30,1);
navD2SF5p110->trimsize();

navD2SF5p111ct = gpstk::BDSWeekSecond(810,345946.400000);
navD2SF5p111 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p111ct);
navD2SF5p111->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p111->addUnsignedLong(0x38905548,30,1);
navD2SF5p111->addUnsignedLong(0x1D29BC99,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->addUnsignedLong(0x00000000,30,1);
navD2SF5p111->trimsize();

navD2SF5p112ct = gpstk::BDSWeekSecond(810,345949.400000);
navD2SF5p112 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p112ct);
navD2SF5p112->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p112->addUnsignedLong(0x38905548,30,1);
navD2SF5p112->addUnsignedLong(0x1D35C0CF,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->addUnsignedLong(0x00000000,30,1);
navD2SF5p112->trimsize();

navD2SF5p113ct = gpstk::BDSWeekSecond(810,345952.400000);
navD2SF5p113 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p113ct);
navD2SF5p113->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p113->addUnsignedLong(0x38905548,30,1);
navD2SF5p113->addUnsignedLong(0x1D41C4DA,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->addUnsignedLong(0x00000000,30,1);
navD2SF5p113->trimsize();

navD2SF5p114ct = gpstk::BDSWeekSecond(810,345955.400000);
navD2SF5p114 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p114ct);
navD2SF5p114->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p114->addUnsignedLong(0x38905548,30,1);
navD2SF5p114->addUnsignedLong(0x1D4DC8E4,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->addUnsignedLong(0x00000000,30,1);
navD2SF5p114->trimsize();

navD2SF5p115ct = gpstk::BDSWeekSecond(810,345958.400000);
navD2SF5p115 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p115ct);
navD2SF5p115->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p115->addUnsignedLong(0x38905548,30,1);
navD2SF5p115->addUnsignedLong(0x1D59CC81,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->addUnsignedLong(0x00000000,30,1);
navD2SF5p115->trimsize();

navD2SF5p116ct = gpstk::BDSWeekSecond(810,345961.400000);
navD2SF5p116 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p116ct);
navD2SF5p116->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p116->addUnsignedLong(0x38905548,30,1);
navD2SF5p116->addUnsignedLong(0x1D65D01A,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->addUnsignedLong(0x00000000,30,1);
navD2SF5p116->trimsize();

navD2SF5p117ct = gpstk::BDSWeekSecond(810,345964.400000);
navD2SF5p117 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p117ct);
navD2SF5p117->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p117->addUnsignedLong(0x38905548,30,1);
navD2SF5p117->addUnsignedLong(0x1D71D47F,30,1);
navD2SF5p117->addUnsignedLong(0x002003C5,30,1);
navD2SF5p117->addUnsignedLong(0x3FF000C0,30,1);
navD2SF5p117->addUnsignedLong(0x00000000,30,1);
navD2SF5p117->addUnsignedLong(0x00004007,30,1);
navD2SF5p117->addUnsignedLong(0x00FFE078,30,1);
navD2SF5p117->addUnsignedLong(0x02005F70,30,1);
navD2SF5p117->addUnsignedLong(0x3E2006EA,30,1);
navD2SF5p117->addUnsignedLong(0x3FF401CA,30,1);
navD2SF5p117->trimsize();

navD2SF5p118ct = gpstk::BDSWeekSecond(810,345967.400000);
navD2SF5p118 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p118ct);
navD2SF5p118->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p118->addUnsignedLong(0x38905548,30,1);
navD2SF5p118->addUnsignedLong(0x1D7DDA47,30,1);
navD2SF5p118->addUnsignedLong(0x00000000,30,1);
navD2SF5p118->addUnsignedLong(0x00000000,30,1);
navD2SF5p118->addUnsignedLong(0x00000000,30,1);
navD2SF5p118->addUnsignedLong(0x0000D00C,30,1);
navD2SF5p118->addUnsignedLong(0x01FFEFDA,30,1);
navD2SF5p118->addUnsignedLong(0x3FF000C0,30,1);
navD2SF5p118->addUnsignedLong(0x0000040C,30,1);
navD2SF5p118->addUnsignedLong(0x00000000,30,1);
navD2SF5p118->trimsize();

navD2SF5p119ct = gpstk::BDSWeekSecond(810,345970.400000);
navD2SF5p119 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p119ct);
navD2SF5p119->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p119->addUnsignedLong(0x38905548,30,1);
navD2SF5p119->addUnsignedLong(0x1D89DC04,30,1);
navD2SF5p119->addUnsignedLong(0x0FFBFFB6,30,1);
navD2SF5p119->addUnsignedLong(0x30017F45,30,1);
navD2SF5p119->addUnsignedLong(0x27002FA8,30,1);
navD2SF5p119->addUnsignedLong(0x3C405CE5,30,1);
navD2SF5p119->addUnsignedLong(0x00800153,30,1);
navD2SF5p119->addUnsignedLong(0x00000000,30,1);
navD2SF5p119->addUnsignedLong(0x00000000,30,1);
navD2SF5p119->addUnsignedLong(0x00000000,30,1);
navD2SF5p119->trimsize();

navD2SF5p120ct = gpstk::BDSWeekSecond(810,345973.400000);
navD2SF5p120 = std::make_shared<gpstk::PackedNavBits>(navD2Sid,oidD2NAV,
                                                      navD2SF5p120ct);
navD2SF5p120->setNavID(gpstk::NavType::BeiDou_D2);
navD2SF5p120->addUnsignedLong(0x38905548,30,1);
navD2SF5p120->addUnsignedLong(0x1D95E055,30,1);
navD2SF5p120->addUnsignedLong(0x00000000,30,1);
navD2SF5p120->addUnsignedLong(0x00000000,30,1);
navD2SF5p120->addUnsignedLong(0x00000000,30,1);
navD2SF5p120->addUnsignedLong(0x00000000,30,1);
navD2SF5p120->addUnsignedLong(0x00000000,30,1);
navD2SF5p120->addUnsignedLong(0x011111C9,30,1);
navD2SF5p120->addUnsignedLong(0x040404E5,30,1);
navD2SF5p120->addUnsignedLong(0x11001075,30,1);
navD2SF5p120->trimsize();

