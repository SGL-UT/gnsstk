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
/** @file GLOFNavTestDataDef.hpp This file contains data definitions
 * for testing GLONASS FDMA NAV processing in the NavFactory code. */

oidFNAVGLO = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                           gnsstk::CarrierBand::G1,
                           gnsstk::TrackingCode::Standard);
navFNAVGLOStr1ct = gnsstk::CivilTime(2015,6,27,0,40,0,gnsstk::TimeSystem::GLO);
navFNAVGLOStr2ct = gnsstk::CivilTime(2015,6,27,0,40,2,gnsstk::TimeSystem::GLO);
navFNAVGLOStr3ct = gnsstk::CivilTime(2015,6,27,0,40,4,gnsstk::TimeSystem::GLO);
navFNAVGLOStr4ct = gnsstk::CivilTime(2015,6,27,0,40,6,gnsstk::TimeSystem::GLO);
navFNAVGLOStr5ct = gnsstk::CivilTime(2015,6,27,0,40,8,gnsstk::TimeSystem::GLO);
navFNAVGLOStr6ct = gnsstk::CivilTime(2015,6,27,0,40,10,gnsstk::TimeSystem::GLO);
navFNAVGLOStr7ct = gnsstk::CivilTime(2015,6,27,0,40,12,gnsstk::TimeSystem::GLO);
navFNAVGLOStr14ct = gnsstk::CivilTime(2015,6,27,1,2,26,gnsstk::TimeSystem::GLO);
navFNAVGLOSid = gnsstk::SatID(1,gnsstk::SatelliteSystem::Glonass);

navFNAVGLOStr1 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                         oidFNAVGLO,
                                                         navFNAVGLOStr1ct);
navFNAVGLOStr1->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr1->addUnsignedLong(0x080E8452,32,1);
navFNAVGLOStr1->addUnsignedLong(0x5A9449E9,32,1);
navFNAVGLOStr1->addUnsignedLong(0x1B364800,32,1);
navFNAVGLOStr1->trimsize();

navFNAVGLOStr2 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                         oidFNAVGLO,
                                                         navFNAVGLOStr2ct);
navFNAVGLOStr2->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr2->addUnsignedLong(0x108F0186,32,1);
navFNAVGLOStr2->addUnsignedLong(0xB0304527,32,1);
navFNAVGLOStr2->addUnsignedLong(0xCD987800,32,1);
navFNAVGLOStr2->trimsize();

navFNAVGLOStr3 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                         oidFNAVGLO,
                                                         navFNAVGLOStr3ct);
navFNAVGLOStr3->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr3->addUnsignedLong(0x1C003435,32,1);
navFNAVGLOStr3->addUnsignedLong(0xA6949646,32,1);
navFNAVGLOStr3->addUnsignedLong(0x1DCBC800,32,1);
navFNAVGLOStr3->trimsize();

navFNAVGLOStr4 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                         oidFNAVGLO,
                                                         navFNAVGLOStr4ct);
navFNAVGLOStr4->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr4->addUnsignedLong(0x20500C09,32,1);
navFNAVGLOStr4->addUnsignedLong(0x00000113,32,1);
navFNAVGLOStr4->addUnsignedLong(0xE82AE800,32,1);
navFNAVGLOStr4->trimsize();

navFNAVGLOStr5 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                         oidFNAVGLO,
                                                         navFNAVGLOStr5ct);
navFNAVGLOStr5->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr5->addUnsignedLong(0x2CFA8000,32,1);
navFNAVGLOStr5->addUnsignedLong(0x00121600,32,1);
navFNAVGLOStr5->addUnsignedLong(0x0110F800,32,1);
navFNAVGLOStr5->trimsize();

navFNAVGLOStr6 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                         oidFNAVGLO,
                                                         navFNAVGLOStr6ct);
navFNAVGLOStr6->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr6->addUnsignedLong(0x3508512D,32,1);
navFNAVGLOStr6->addUnsignedLong(0xD9A06BB0,32,1);
navFNAVGLOStr6->addUnsignedLong(0x0EAE1000,32,1);
navFNAVGLOStr6->trimsize();

navFNAVGLOStr7 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                         oidFNAVGLO,
                                                         navFNAVGLOStr7ct);
navFNAVGLOStr7->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr7->addUnsignedLong(0x3CF4B851,32,1);
navFNAVGLOStr7->addUnsignedLong(0x3BB4C0AD,32,1);
navFNAVGLOStr7->addUnsignedLong(0x0A151000,32,1);
navFNAVGLOStr7->trimsize();


navFNAVGLOStr14 = std::make_shared<gnsstk::PackedNavBits>(navFNAVGLOSid,
                                                          oidFNAVGLO,
                                                          navFNAVGLOStr14ct);
navFNAVGLOStr14->setNavID(gnsstk::NavType::GloCivilF);
navFNAVGLOStr14->addUnsignedLong(0x76B18AD0,32,1);
navFNAVGLOStr14->addUnsignedLong(0x00000000,32,1);
navFNAVGLOStr14->addUnsignedLong(0x00052000,32,1);
