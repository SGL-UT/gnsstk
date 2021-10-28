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
/** @file CNavTestDataDef.hpp This file contains data definitions
 * for testing GPS CNav processing in the NavFactory code. */

// message type quick reference
// 10 - ephemeris 1
// 11 - ephemeris 2
// 12 - reduced almanac
// 13 - clock differential correction
// 14 - ephemeris differential correction
// 15 - text
// 30 - clock, iono & group delay
// 31 - clock & reduced almanac
// 32 - clock & EOP
// 33 - clock & UTC
// 34 - clock & differential correction
// 35 - clock & GGTO
// 36 - clock & text
// 37 - clock & midi almanac

oidCNAVGPSL2 = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                            gnsstk::CarrierBand::L2,
                            gnsstk::TrackingCode::L2CM);
ephCNAVGPSL2sid = gnsstk::SatID(1, gnsstk::SatelliteSystem::GPS);

msg10CNAVGPSL2ct = gnsstk::GPSWeekSecond(2060,86400);
msg10CNAVGPSL2 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg10CNAVGPSL2ct);
msg10CNAVGPSL2->setNavID(gnsstk::NavType::GPSCNAVL2);
msg10CNAVGPSL2->addUnsignedLong(0x8B04A1C2, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x11018424, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x00990077, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x26DFFD06, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0xC2B03800, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x727D7558, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x54B824E9, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x11907661, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x66186063, 32, 1);
msg10CNAVGPSL2->addUnsignedLong(0x03A, 12, 1);
msg10CNAVGPSL2->trimsize();

msg11CNAVGPSL2ct = gnsstk::GPSWeekSecond(2060,86412);
msg11CNAVGPSL2 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg11CNAVGPSL2ct);
msg11CNAVGPSL2->setNavID(gnsstk::NavType::GPSCNAVL2);
msg11CNAVGPSL2->addUnsignedLong(0x8B04B1C2, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0x20993694, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0x994A49F2, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0x0ED4609D, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0xA079400F, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0x6015DFF0, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0x224015EE, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0xE02E43FF, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0x20E7C9CE, 32, 1);
msg11CNAVGPSL2->addUnsignedLong(0x3FE, 12, 1);
msg11CNAVGPSL2->trimsize();

msg30CNAVGPSL2ct = gnsstk::GPSWeekSecond(2060,86424);
msg30CNAVGPSL2 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg30CNAVGPSL2ct);
msg30CNAVGPSL2->setNavID(gnsstk::NavType::GPSCNAVL2);
msg30CNAVGPSL2->addUnsignedLong(0x8B05E1C2, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0x30246CF2, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0x65F190CB, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0xFFA6E800, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0x0C0FFA7E, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0x501BA0E1, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0x0502FFFE, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0x2805FFF8, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0x0C142A67, 32, 1);
msg30CNAVGPSL2->addUnsignedLong(0xD56, 12, 1);
msg30CNAVGPSL2->trimsize();

msg32CNAVGPSL2ct = gnsstk::GPSWeekSecond(2060,86436);
msg32CNAVGPSL2 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg32CNAVGPSL2ct);
msg32CNAVGPSL2->setNavID(gnsstk::NavType::GPSCNAVL2);
msg32CNAVGPSL2->addUnsignedLong(0x8B0601C2, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0x40246CF2, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0x65F190CB, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0xFFA6E800, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0x0A8C2870, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0xD18766BF, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0xE1F7B9FF, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0x4D3EA41E, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0x9350CC39, 32, 1);
msg32CNAVGPSL2->addUnsignedLong(0x01A, 12, 1);
msg32CNAVGPSL2->trimsize();

msg33CNAVGPSL2ct = gnsstk::GPSWeekSecond(2060,86424);
msg33CNAVGPSL2 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg33CNAVGPSL2ct);
msg33CNAVGPSL2->setNavID(gnsstk::NavType::GPSCNAVL2);
msg33CNAVGPSL2->addUnsignedLong(0x8B0611C2, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0x80246CF2, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0x65F190CB, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0xFFA6E800, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0x00880110, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0x0240A8C8, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0x0C3C4B89, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0x1E0B69E5, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0xF8FB362A, 32, 1);
msg33CNAVGPSL2->addUnsignedLong(0x4BD, 12, 1);
msg33CNAVGPSL2->trimsize();

// QZSS data

oidCNAVQZSSL5 = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                             gnsstk::CarrierBand::L5,
                             gnsstk::TrackingCode::L5I);
ephCNAVQZSSL5sid = gnsstk::SatID(193, gnsstk::SatelliteSystem::QZSS);

msg10CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345600,gnsstk::TimeSystem::QZS);
msg10CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg10CNAVQZSSL5ct);
msg10CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg10CNAVQZSSL5->addUnsignedLong(0x8B04A708, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0x09062246, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0x6246787E, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0x23E02AE2, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0x217BEFFC, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0xA16C1918, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0x114932A7, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0x9F22FFAF, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0x6D4809A0, 32, 1);
msg10CNAVQZSSL5->addUnsignedLong(0xC8B, 12, 1);
msg10CNAVQZSSL5->trimsize();

msg11CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345606,gnsstk::TimeSystem::QZS);
msg11CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg11CNAVQZSSL5ct);
msg11CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg11CNAVQZSSL5->addUnsignedLong(0x8B04B708, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0x12467604, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0x96FA8764, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0x47D44802, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0xE176805F, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0x60272011, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0xED5FB974, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0xE064EE00, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0x27400CE7, 32, 1);
msg11CNAVQZSSL5->addUnsignedLong(0x909, 12, 1);
msg11CNAVQZSSL5->trimsize();

msg12CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345666,gnsstk::TimeSystem::QZS);
msg12CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg12CNAVQZSSL5ct);
msg12CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg12CNAVQZSSL5->addUnsignedLong(0x8B04C708, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0x61062FE0, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0x55555540, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0xAAAAAA81, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0x55555502, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0xAAAAAA05, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0x5555540A, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0xAAAAA815, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0x55555C04, 32, 1);
msg12CNAVQZSSL5->addUnsignedLong(0xF72, 12, 1);
msg12CNAVQZSSL5->trimsize();

msg30CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345612,gnsstk::TimeSystem::QZS);
msg30CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg30CNAVQZSSL5ct);
msg30CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg30CNAVQZSSL5->addUnsignedLong(0x8B05E708, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0x1A467409, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0x19A63DCF, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0x804B7001, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0xF4800001, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0xDC0AA048, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0x09FEF7EC, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0x2DFEFE7F, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0x31000F9C, 32, 1);
msg30CNAVQZSSL5->addUnsignedLong(0x964, 12, 1);
msg30CNAVQZSSL5->trimsize();

msg31CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345660,gnsstk::TimeSystem::QZS);
msg31CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg31CNAVQZSSL5ct);
msg31CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg31CNAVQZSSL5->addUnsignedLong(0x8B05F708, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x5A467409, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x19A63DCF, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x804B7000, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x8317F07C, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x3B3F0101, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0xA5240700, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x00C807F5, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x1F780567, 32, 1);
msg31CNAVQZSSL5->addUnsignedLong(0x88D, 12, 1);
msg31CNAVQZSSL5->trimsize();

msg32CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345636,gnsstk::TimeSystem::QZS);
msg32CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg32CNAVQZSSL5ct);
msg32CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg32CNAVQZSSL5->addUnsignedLong(0x8B060708, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0x3A467409, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0x19A63DCF, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0x804B7000, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0xA8C00948, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0xA0BCA624, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0x060D3BFF, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0x1EB923E7, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0xD7000F86, 32, 1);
msg32CNAVQZSSL5->addUnsignedLong(0xF96, 12, 1);
msg32CNAVQZSSL5->trimsize();

msg35CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345738,gnsstk::TimeSystem::QZS);
msg35CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg35CNAVQZSSL5ct);
msg35CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg35CNAVQZSSL5->addUnsignedLong(0x8B063708, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0xC2467409, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0x19A63DCF, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0x804B7000, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0xAE068316, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0x00000000, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0x00000000, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0x00000000, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0x000001B2, 32, 1);
msg35CNAVQZSSL5->addUnsignedLong(0x05C, 12, 1);
msg35CNAVQZSSL5->trimsize();

msg37CNAVQZSSL5ct = gnsstk::GPSWeekSecond(2097,345642,gnsstk::TimeSystem::QZS);
msg37CNAVQZSSL5 = std::make_shared<gnsstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg37CNAVQZSSL5ct);
msg37CNAVQZSSL5->setNavID(gnsstk::NavType::GPSCNAVL5);
msg37CNAVQZSSL5->addUnsignedLong(0x8B065708, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0x42467409, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0x19A63DCF, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0x804B7000, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0x8317F043, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0xCED93FEB, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0x2B9BD80B, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0x7FD57B7F, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0xA6C010C0, 32, 1);
msg37CNAVQZSSL5->addUnsignedLong(0xC91, 12, 1);
msg37CNAVQZSSL5->trimsize();
