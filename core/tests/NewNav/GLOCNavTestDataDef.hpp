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
/** @file GLOCNavTestDataDef.hpp This file contains data definitions
 * for testing GLONASS CDMA NAV processing in the NavFactory code. */

oidGLOC = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                        gnsstk::CarrierBand::G3,
                        gnsstk::TrackingCode::L3OCD);
navGLOCStr10act=gnsstk::CivilTime(2022,5,3,10,9,45,gnsstk::TimeSystem::GLO);
navGLOCStr11act=gnsstk::CivilTime(2022,5,3,10,9,48,gnsstk::TimeSystem::GLO);
navGLOCStr12act=gnsstk::CivilTime(2022,5,3,10,9,51,gnsstk::TimeSystem::GLO);
navGLOCStr20ct=gnsstk::CivilTime(2022,5,3,10,9,54,gnsstk::TimeSystem::GLO);
navGLOCStr31ct=gnsstk::CivilTime(2022,5,3,10,10,9,gnsstk::TimeSystem::GLO);
navGLOCStr32ct=gnsstk::CivilTime(2022,5,3,10,10,12,gnsstk::TimeSystem::GLO);
navGLOCStr10bct=gnsstk::CivilTime(2022,5,3,10,10,15,gnsstk::TimeSystem::GLO);
navGLOCStr11bct=gnsstk::CivilTime(2022,5,3,10,10,18,gnsstk::TimeSystem::GLO);
navGLOCStr12bct=gnsstk::CivilTime(2022,5,3,10,10,21,gnsstk::TimeSystem::GLO);
navGLOCStr25ct=gnsstk::CivilTime(2022,5,3,10,11,9,gnsstk::TimeSystem::GLO);
navGLOCSid = gnsstk::SatID(11,gnsstk::SatelliteSystem::Glonass);

navGLOCStr10a = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                        oidGLOC,
                                                        navGLOCStr10act);
navGLOCStr10a->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr10a->addUnsignedLong(0x0494E29E,32,1);
navGLOCStr10a->addUnsignedLong(0xD997001D,32,1);
navGLOCStr10a->addUnsignedLong(0xAB116120,32,1);
navGLOCStr10a->addUnsignedLong(0x000A1820,32,1);
navGLOCStr10a->addUnsignedLong(0x29D80260,32,1);
navGLOCStr10a->addUnsignedLong(0x08180000,32,1);
navGLOCStr10a->addUnsignedLong(0x00000000,32,1);
navGLOCStr10a->addUnsignedLong(0x30000000,32,1);
navGLOCStr10a->addUnsignedLong(0x000006C2,32,1);
navGLOCStr10a->addUnsignedLong(0x77B,12,1);
navGLOCStr10a->trimsize();

navGLOCStr11a = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                        oidGLOC,
                                                        navGLOCStr11act);
navGLOCStr11a->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr11a->addUnsignedLong(0x0494E2DE,32,1);
navGLOCStr11a->addUnsignedLong(0xDA170040,32,1);
navGLOCStr11a->addUnsignedLong(0x811E37C8,32,1);
navGLOCStr11a->addUnsignedLong(0x8160D339,32,1);
navGLOCStr11a->addUnsignedLong(0x3B42BEB0,32,1);
navGLOCStr11a->addUnsignedLong(0xE35D4826,32,1);
navGLOCStr11a->addUnsignedLong(0xB7FAD927,32,1);
navGLOCStr11a->addUnsignedLong(0xF7C2DE00,32,1);
navGLOCStr11a->addUnsignedLong(0x0000060B,32,1);
navGLOCStr11a->addUnsignedLong(0x4FE,12,1);
navGLOCStr11a->trimsize();

navGLOCStr12a = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                        oidGLOC,
                                                        navGLOCStr12act);
navGLOCStr12a->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr12a->addUnsignedLong(0x0494E31E,32,1);
navGLOCStr12a->addUnsignedLong(0xDA970043,32,1);
navGLOCStr12a->addUnsignedLong(0x26592628,32,1);
navGLOCStr12a->addUnsignedLong(0x21F12442,32,1);
navGLOCStr12a->addUnsignedLong(0x26D6D800,32,1);
navGLOCStr12a->addUnsignedLong(0xC0000007,32,1);
navGLOCStr12a->addUnsignedLong(0x00003480,32,1);
navGLOCStr12a->addUnsignedLong(0x00000000,32,1);
navGLOCStr12a->addUnsignedLong(0x00000862,32,1);
navGLOCStr12a->addUnsignedLong(0xB02,12,1);
navGLOCStr12a->trimsize();

navGLOCStr20 = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                       oidGLOC,
                                                       navGLOCStr20ct);
navGLOCStr20->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr20->addUnsignedLong(0x0494E51E,32,1);
navGLOCStr20->addUnsignedLong(0xDB2B0001,32,1);
navGLOCStr20->addUnsignedLong(0x08D54201,32,1);
navGLOCStr20->addUnsignedLong(0x39247F24,32,1);
navGLOCStr20->addUnsignedLong(0x0BB80B40,32,1);
navGLOCStr20->addUnsignedLong(0x01B46D5A,32,1);
navGLOCStr20->addUnsignedLong(0x0010E000,32,1);
navGLOCStr20->addUnsignedLong(0x00000000,32,1);
navGLOCStr20->addUnsignedLong(0x00000C38,32,1);
navGLOCStr20->addUnsignedLong(0x9EE,12,1);
navGLOCStr20->trimsize();

navGLOCStr31 = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                       oidGLOC,
                                                       navGLOCStr31ct);
navGLOCStr31->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr31->addUnsignedLong(0x0494E7DE,32,1);
navGLOCStr31->addUnsignedLong(0xDD970042,32,1);
navGLOCStr31->addUnsignedLong(0x40000000,32,1);
navGLOCStr31->addUnsignedLong(0x00000000,32,1);
navGLOCStr31->addUnsignedLong(0x00000000,32,1);
navGLOCStr31->addUnsignedLong(0x00000000,32,1);
navGLOCStr31->addUnsignedLong(0x00000000,32,1);
navGLOCStr31->addUnsignedLong(0x00000000,32,1);
navGLOCStr31->addUnsignedLong(0x00000DB5,32,1);
navGLOCStr31->addUnsignedLong(0xF7E,12,1);
navGLOCStr31->trimsize();

navGLOCStr32 = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                       oidGLOC,
                                                       navGLOCStr32ct);
navGLOCStr32->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr32->addUnsignedLong(0x0494E81E,32,1);
navGLOCStr32->addUnsignedLong(0xDE170042,32,1);
navGLOCStr32->addUnsignedLong(0x40000000,32,1);
navGLOCStr32->addUnsignedLong(0x00000000,32,1);
navGLOCStr32->addUnsignedLong(0x00000000,32,1);
navGLOCStr32->addUnsignedLong(0x00000000,32,1);
navGLOCStr32->addUnsignedLong(0x00000000,32,1);
navGLOCStr32->addUnsignedLong(0x00000000,32,1);
navGLOCStr32->addUnsignedLong(0x0000036B,32,1);
navGLOCStr32->addUnsignedLong(0xB6D,12,1);
navGLOCStr32->trimsize();

navGLOCStr10b = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                        oidGLOC,
                                                        navGLOCStr10bct);
navGLOCStr10b->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr10b->addUnsignedLong(0x0494E29E,32,1);
navGLOCStr10b->addUnsignedLong(0xDE97001D,32,1);
navGLOCStr10b->addUnsignedLong(0xAB116120,32,1);
navGLOCStr10b->addUnsignedLong(0x000A1820,32,1);
navGLOCStr10b->addUnsignedLong(0x29D80260,32,1);
navGLOCStr10b->addUnsignedLong(0x08180000,32,1);
navGLOCStr10b->addUnsignedLong(0x00000000,32,1);
navGLOCStr10b->addUnsignedLong(0x30000000,32,1);
navGLOCStr10b->addUnsignedLong(0x00000D78,32,1);
navGLOCStr10b->addUnsignedLong(0xB25,12,1);
navGLOCStr10b->trimsize();

navGLOCStr11b = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                        oidGLOC,
                                                        navGLOCStr11bct);
navGLOCStr11b->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr11b->addUnsignedLong(0x0494E2DE,32,1);
navGLOCStr11b->addUnsignedLong(0xDF170040,32,1);
navGLOCStr11b->addUnsignedLong(0x811E37C8,32,1);
navGLOCStr11b->addUnsignedLong(0x8160D339,32,1);
navGLOCStr11b->addUnsignedLong(0x3B42BEB0,32,1);
navGLOCStr11b->addUnsignedLong(0xE35D4826,32,1);
navGLOCStr11b->addUnsignedLong(0xB7FAD927,32,1);
navGLOCStr11b->addUnsignedLong(0xF7C2DE00,32,1);
navGLOCStr11b->addUnsignedLong(0x00000499,32,1);
navGLOCStr11b->addUnsignedLong(0xA2B,12,1);
navGLOCStr11b->trimsize();

navGLOCStr12b = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                        oidGLOC,
                                                        navGLOCStr12bct);
navGLOCStr12b->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr12b->addUnsignedLong(0x0494E31E,32,1);
navGLOCStr12b->addUnsignedLong(0xDF970043,32,1);
navGLOCStr12b->addUnsignedLong(0x26592628,32,1);
navGLOCStr12b->addUnsignedLong(0x21F12442,32,1);
navGLOCStr12b->addUnsignedLong(0x26D6D800,32,1);
navGLOCStr12b->addUnsignedLong(0xC0000007,32,1);
navGLOCStr12b->addUnsignedLong(0x00003480,32,1);
navGLOCStr12b->addUnsignedLong(0x00000000,32,1);
navGLOCStr12b->addUnsignedLong(0x00000AF0,32,1);
navGLOCStr12b->addUnsignedLong(0x5D7,12,1);
navGLOCStr12b->trimsize();

navGLOCStr25 = std::make_shared<gnsstk::PackedNavBits>(navGLOCSid,
                                                       oidGLOC,
                                                       navGLOCStr25ct);
navGLOCStr25->setNavID(gnsstk::NavType::GloCivilC);
navGLOCStr25->addUnsignedLong(0x0494E65E,32,1);
navGLOCStr25->addUnsignedLong(0xE7970035,32,1);
navGLOCStr25->addUnsignedLong(0x505F71E6,32,1);
navGLOCStr25->addUnsignedLong(0x01082000,32,1);
navGLOCStr25->addUnsignedLong(0x0800C680,32,1);
navGLOCStr25->addUnsignedLong(0x4E00803D,32,1);
navGLOCStr25->addUnsignedLong(0x00324A00,32,1);
navGLOCStr25->addUnsignedLong(0x00000000,32,1);
navGLOCStr25->addUnsignedLong(0x00000C10,32,1);
navGLOCStr25->addUnsignedLong(0x572,12,1);
navGLOCStr25->trimsize();
