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
/** @file GalFNavTestDataDef.hpp This file contains data definitions
 * for testing Galileo F/NAV processing in the NavFactory code. */

oidFNAVGal = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                          gnsstk::CarrierBand::L5,
                          gnsstk::TrackingCode::E5aI);
navFNAVGalPT1ct = gnsstk::GALWeekSecond(1014,518400);
navFNAVGalPT2ct = gnsstk::GALWeekSecond(1014,518410);
navFNAVGalPT3ct = gnsstk::GALWeekSecond(1014,518420);
navFNAVGalPT4ct = gnsstk::GALWeekSecond(1014,518430);
navFNAVGalPT5ct = gnsstk::GALWeekSecond(1014,518440);
navFNAVGalPT6ct = gnsstk::GALWeekSecond(1014,518490);
navFNAVGalSid = gnsstk::SatID(1,gnsstk::SatelliteSystem::Galileo);

navFNAVGalPT1 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGalSid,oidFNAVGal,
                                                       navFNAVGalPT1ct);
navFNAVGalPT1->setNavID(gnsstk::NavType::GalFNAV);
navFNAVGalPT1->addUnsignedLong(0x04117A1A,32,1);
navFNAVGalPT1->addUnsignedLong(0xCFEE8487,32,1);
navFNAVGalPT1->addUnsignedLong(0xBFFDC701,32,1);
navFNAVGalPT1->addUnsignedLong(0xAC5B82A0,32,1);
navFNAVGalPT1->addUnsignedLong(0x0581F607,32,1);
navFNAVGalPT1->addUnsignedLong(0xECFD200A,32,1);
navFNAVGalPT1->addUnsignedLong(0xAAAAAA0C,32,1);
navFNAVGalPT1->addUnsignedLong(0x20700,20,1);
navFNAVGalPT1->trimsize();

navFNAVGalPT2 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGalSid,oidFNAVGal,
                                                       navFNAVGalPT2ct);
navFNAVGalPT2->setNavID(gnsstk::NavType::GalFNAV);
navFNAVGalPT2->addUnsignedLong(0x085E8518,32,1);
navFNAVGalPT2->addUnsignedLong(0x180AFFC8,32,1);
navFNAVGalPT2->addUnsignedLong(0x1B000E6E,32,1);
navFNAVGalPT2->addUnsignedLong(0x2FAA04FB,32,1);
navFNAVGalPT2->addUnsignedLong(0x1D78BB0E,32,1);
navFNAVGalPT2->addUnsignedLong(0x29F7FCFD,32,1);
navFNAVGalPT2->addUnsignedLong(0x9FA42863,32,1);
navFNAVGalPT2->addUnsignedLong(0x906C0,20,1);
navFNAVGalPT2->trimsize();

navFNAVGalPT3 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGalSid,oidFNAVGal,
                                                       navFNAVGalPT3ct);
navFNAVGalPT3->setNavID(gnsstk::NavType::GalFNAV);
navFNAVGalPT3->addUnsignedLong(0x0C5E2854,32,1);
navFNAVGalPT3->addUnsignedLong(0x403C05B7,32,1);
navFNAVGalPT3->addUnsignedLong(0x23FF1BAB,32,1);
navFNAVGalPT3->addUnsignedLong(0x09AB1967,32,1);
navFNAVGalPT3->addUnsignedLong(0x0C0B0C49,32,1);
navFNAVGalPT3->addUnsignedLong(0x86B0FD9F,32,1);
navFNAVGalPT3->addUnsignedLong(0xA4515734,32,1);
navFNAVGalPT3->addUnsignedLong(0x8A300,20,1);
navFNAVGalPT3->trimsize();

navFNAVGalPT4 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGalSid,oidFNAVGal,
                                                       navFNAVGalPT4ct);
navFNAVGalPT4->setNavID(gnsstk::NavType::GalFNAV);
navFNAVGalPT4->addUnsignedLong(0x105EFFF8,32,1);
navFNAVGalPT4->addUnsignedLong(0xFFFEFFFF,32,1);
navFNAVGalPT4->addUnsignedLong(0xFFFF0000,32,1);
navFNAVGalPT4->addUnsignedLong(0x011278F6,32,1);
navFNAVGalPT4->addUnsignedLong(0x89E2521F,32,1);
navFNAVGalPT4->addUnsignedLong(0xE4FFE3B3,32,1);
navFNAVGalPT4->addUnsignedLong(0xF48F299B,32,1);
navFNAVGalPT4->addUnsignedLong(0x07040,20,1);
navFNAVGalPT4->trimsize();

navFNAVGalPT5 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGalSid,oidFNAVGal,
                                                       navFNAVGalPT5ct);
navFNAVGalPT5->setNavID(gnsstk::NavType::GalFNAV);
navFNAVGalPT5->addUnsignedLong(0x17AD7810,32,1);
navFNAVGalPT5->addUnsignedLong(0x0900705B,32,1);
navFNAVGalPT5->addUnsignedLong(0x7082F177,32,1);
navFNAVGalPT5->addUnsignedLong(0xFCA1463F,32,1);
navFNAVGalPT5->addUnsignedLong(0xBA3FFC04,32,1);
navFNAVGalPT5->addUnsignedLong(0x01402608,32,1);
navFNAVGalPT5->addUnsignedLong(0x6E105E87,32,1);
navFNAVGalPT5->addUnsignedLong(0xAC940,20,1);
navFNAVGalPT5->trimsize();

navFNAVGalPT6 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGalSid,oidFNAVGal,
                                                       navFNAVGalPT6ct);
navFNAVGalPT6->setNavID(gnsstk::NavType::GalFNAV);
navFNAVGalPT6->addUnsignedLong(0x1BA2EFF9,32,1);
navFNAVGalPT6->addUnsignedLong(0x0343000C,32,1);
navFNAVGalPT6->addUnsignedLong(0x00000C01,32,1);
navFNAVGalPT6->addUnsignedLong(0x204050BF,32,1);
navFNAVGalPT6->addUnsignedLong(0xBF91BA7F,32,1);
navFNAVGalPT6->addUnsignedLong(0x04BD6FFB,32,1);
navFNAVGalPT6->addUnsignedLong(0xCFFF8B9D,32,1);
navFNAVGalPT6->addUnsignedLong(0xC3780,20,1);
navFNAVGalPT6->trimsize();
