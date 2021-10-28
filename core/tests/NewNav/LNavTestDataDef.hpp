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
/** @file LNavTestDataDef.hpp This file contains data definitions
 * for testing GPS and QZSS LNav processing in the NavFactory code. */

oidLNAVGPS = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                          gnsstk::CarrierBand::L1,
                          gnsstk::TrackingCode::CA);
ephLNAVGPSSF1ct = gnsstk::GPSWeekSecond(1869,6.0);
ephLNAVGPSSF2ct = gnsstk::GPSWeekSecond(1869,12.0);
ephLNAVGPSSF3ct = gnsstk::GPSWeekSecond(1869,18.0);
almLNAVGPS25ct = gnsstk::GPSWeekSecond(1869,54.0);
almLNAVGPS26ct = gnsstk::GPSWeekSecond(1869,84.0);
pg51LNAVGPSct = gnsstk::GPSWeekSecond(1869,750.0);
pg56LNAVGPSct = gnsstk::GPSWeekSecond(1869,528.0);
pg63LNAVGPSct = gnsstk::GPSWeekSecond(1869,744.0);
ephLNAVGPSSid = gnsstk::SatID(4,gnsstk::SatelliteSystem::GPS);
almLNAVGPSSid = gnsstk::SatID(1,gnsstk::SatelliteSystem::GPS);

ephLNAVGPSSF1 = std::make_shared<gnsstk::PackedNavBits>(ephLNAVGPSSid,oidLNAVGPS,
                                                       ephLNAVGPSSF1ct);
ephLNAVGPSSF1->setNavID(gnsstk::NavType::GPSLNAV);
ephLNAVGPSSF1->addUnsignedLong(0x22C34D21,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x000029D4,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x34D44000,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x091B1DE7,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x1C33746E,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x2F701369,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x39F53CB5,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x128070A8,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x003FF454,30,1);
ephLNAVGPSSF1->addUnsignedLong(0x3EAFC2F0,30,1);
ephLNAVGPSSF1->trimsize();

ephLNAVGPSSF2 = std::make_shared<gnsstk::PackedNavBits>(ephLNAVGPSSid,oidLNAVGPS,
                                                       ephLNAVGPSSF2ct);
ephLNAVGPSSF2->setNavID(gnsstk::NavType::GPSLNAV);
ephLNAVGPSSF2->addUnsignedLong(0x22C34D21,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x00004A44,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x12BFCB3A,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x0D7A9094,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x3B99FBAF,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x3FC081B8,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x09D171E1,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x04B0A847,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x03497656,30,1);
ephLNAVGPSSF2->addUnsignedLong(0x00709FA0,30,1);
ephLNAVGPSSF2->trimsize();

ephLNAVGPSSF3 = std::make_shared<gnsstk::PackedNavBits>(ephLNAVGPSSid,oidLNAVGPS,
                                                       ephLNAVGPSSF3ct);
ephLNAVGPSSF3->setNavID(gnsstk::NavType::GPSLNAV);
ephLNAVGPSSF3->addUnsignedLong(0x22C34D21,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x00006BCC,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x3FE14ED4,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x05ABBB58,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x3FE3498B,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x145EE03A,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x062ECB6F,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x1C48068F,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x3FE95E1E,30,1);
ephLNAVGPSSF3->addUnsignedLong(0x12844624,30,1);
ephLNAVGPSSF3->trimsize();

almLNAVGPS25 = std::make_shared<gnsstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                      almLNAVGPS25ct);
almLNAVGPS25->setNavID(gnsstk::NavType::GPSLNAV);
almLNAVGPS25->addUnsignedLong(0x22C34D21,30,1);
almLNAVGPS25->addUnsignedLong(0x00012C68,30,1);
almLNAVGPS25->addUnsignedLong(0x16499366,30,1);
almLNAVGPS25->addUnsignedLong(0x0905E5C5,30,1);
almLNAVGPS25->addUnsignedLong(0x3F52C035,30,1);
almLNAVGPS25->addUnsignedLong(0x28431EEE,30,1);
almLNAVGPS25->addUnsignedLong(0x39A2E96D,30,1);
almLNAVGPS25->addUnsignedLong(0x075C3A2E,30,1);
almLNAVGPS25->addUnsignedLong(0x117733C7,30,1);
almLNAVGPS25->addUnsignedLong(0x3E3FF848,30,1);
almLNAVGPS25->trimsize();

almLNAVGPS26 = std::make_shared<gnsstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                      almLNAVGPS26ct);
almLNAVGPS26->setNavID(gnsstk::NavType::GPSLNAV);
almLNAVGPS26->addUnsignedLong(0x22C34D21,30,1);
almLNAVGPS26->addUnsignedLong(0x0001CC0C,30,1);
almLNAVGPS26->addUnsignedLong(0x16809922,30,1);
almLNAVGPS26->addUnsignedLong(0x0902F90F,30,1);
almLNAVGPS26->addUnsignedLong(0x3F4EC036,30,1);
almLNAVGPS26->addUnsignedLong(0x28433740,30,1);
almLNAVGPS26->addUnsignedLong(0x3994EFA9,30,1);
almLNAVGPS26->addUnsignedLong(0x3D38AA68,30,1);
almLNAVGPS26->addUnsignedLong(0x0E3E305F,30,1);
almLNAVGPS26->addUnsignedLong(0x3CBFE0A0,30,1);
almLNAVGPS26->trimsize();

pg51LNAVGPS = std::make_shared<gnsstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                     pg51LNAVGPSct);
pg51LNAVGPS->setNavID(gnsstk::NavType::GPSLNAV);
pg51LNAVGPS->addUnsignedLong(0x22C34D21,30,1);
pg51LNAVGPS->addUnsignedLong(0x000FAD48,30,1);
pg51LNAVGPS->addUnsignedLong(0x1CC91377,30,1);
pg51LNAVGPS->addUnsignedLong(0x0000003F,30,1);
pg51LNAVGPS->addUnsignedLong(0x0000003F,30,1);
pg51LNAVGPS->addUnsignedLong(0x00FC0005,30,1);
pg51LNAVGPS->addUnsignedLong(0x00000016,30,1);
pg51LNAVGPS->addUnsignedLong(0x00000029,30,1);
pg51LNAVGPS->addUnsignedLong(0x00000016,30,1);
pg51LNAVGPS->addUnsignedLong(0x0000008C,30,1);
pg51LNAVGPS->trimsize();

pg63LNAVGPS = std::make_shared<gnsstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                     pg63LNAVGPSct);
pg63LNAVGPS->setNavID(gnsstk::NavType::GPSLNAV);
pg63LNAVGPS->addUnsignedLong(0x22C34D21,30,1);
pg63LNAVGPS->addUnsignedLong(0x000F8CC0,30,1);
pg63LNAVGPS->addUnsignedLong(0x1FEE6E77,30,1);
pg63LNAVGPS->addUnsignedLong(0x2AEAEEFF,30,1);
pg63LNAVGPS->addUnsignedLong(0x26A66A4A,30,1);
pg63LNAVGPS->addUnsignedLong(0x2A666659,30,1);
pg63LNAVGPS->addUnsignedLong(0x26EEEE6C,30,1);
pg63LNAVGPS->addUnsignedLong(0x2AEA402C,30,1);
pg63LNAVGPS->addUnsignedLong(0x00000000,30,1);
pg63LNAVGPS->addUnsignedLong(0x00000000,30,1);
pg63LNAVGPS->trimsize();

pg56LNAVGPS = std::make_shared<gnsstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                     pg56LNAVGPSct);
pg56LNAVGPS->setNavID(gnsstk::NavType::GPSLNAV);
pg56LNAVGPS->addUnsignedLong(0x22C34D21,30,1);
pg56LNAVGPS->addUnsignedLong(0x000B2C64,30,1);
pg56LNAVGPS->addUnsignedLong(0x1E05FFE1,30,1);
pg56LNAVGPS->addUnsignedLong(0x3F808FEC,30,1);
pg56LNAVGPS->addUnsignedLong(0x3F7F405F,30,1);
pg56LNAVGPS->addUnsignedLong(0x00000413,30,1);
pg56LNAVGPS->addUnsignedLong(0x0000003F,30,1);
pg56LNAVGPS->addUnsignedLong(0x01491376,30,1);
pg56LNAVGPS->addUnsignedLong(0x044EC0EB,30,1);
pg56LNAVGPS->addUnsignedLong(0x044000D8,30,1);
pg56LNAVGPS->trimsize();

// QZSS

oidLNAVQZSS = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                           gnsstk::CarrierBand::L1,
                           gnsstk::TrackingCode::CA);
ephLNAVQZSSSF1ct = gnsstk::GPSWeekSecond(2118,518400,gnsstk::TimeSystem::QZS);
ephLNAVQZSSSF2ct = gnsstk::GPSWeekSecond(2118,518406,gnsstk::TimeSystem::QZS);
ephLNAVQZSSSF3ct = gnsstk::GPSWeekSecond(2118,518412,gnsstk::TimeSystem::QZS);
almLNAVQZSS1ct = gnsstk::GPSWeekSecond(2118,518424,gnsstk::TimeSystem::QZS);
almLNAVQZSS2ct = gnsstk::GPSWeekSecond(2118,518484,gnsstk::TimeSystem::QZS);
pg51LNAVQZSSct = gnsstk::GPSWeekSecond(2118,518538,gnsstk::TimeSystem::QZS);
pg56LNAVQZSSct = gnsstk::GPSWeekSecond(2118,518514,gnsstk::TimeSystem::QZS);
pg61LNAVQZSSct = gnsstk::GPSWeekSecond(2118,518454,gnsstk::TimeSystem::QZS);
ephLNAVQZSSSid = gnsstk::SatID(193,gnsstk::SatelliteSystem::QZSS);
almLNAVQZSSSid = gnsstk::SatID(193,gnsstk::SatelliteSystem::QZSS);

ephLNAVQZSSSF1 = std::make_shared<gnsstk::PackedNavBits>(ephLNAVQZSSSid,
                                                        oidLNAVQZSS,
                                                        ephLNAVQZSSSF1ct);
ephLNAVQZSSSF1->setNavID(gnsstk::NavType::GPSLNAV);
ephLNAVQZSSSF1->addUnsignedLong(0x22C0AA24,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x2A302164,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x046840DF,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x20000015,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x00000016,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x00000029,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x00003D3C,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x205FDC4B,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x0000092D,30,1);
ephLNAVQZSSSF1->addUnsignedLong(0x39CF43FC,30,1);
ephLNAVQZSSSF1->trimsize();

ephLNAVQZSSSF2 = std::make_shared<gnsstk::PackedNavBits>(ephLNAVQZSSSid,
                                                        oidLNAVQZSS,
                                                        ephLNAVQZSSSF2ct);
ephLNAVQZSSSF2->setNavID(gnsstk::NavType::GPSLNAV);
ephLNAVQZSSSF2->addUnsignedLong(0x22C0AA24,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x2A3042F4,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x207F0CCA,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x06A60B06,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x3C48F93B,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x3FD449AB,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x36CFD9C8,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x074DF288,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x3B724294,30,1);
ephLNAVQZSSSF2->addUnsignedLong(0x1FDC5FD4,30,1);
ephLNAVQZSSSF2->trimsize();

ephLNAVQZSSSF3 = std::make_shared<gnsstk::PackedNavBits>(ephLNAVQZSSSid,
                                                        oidLNAVQZSS,
                                                        ephLNAVQZSSSF3ct);
ephLNAVQZSSSF3->setNavID(gnsstk::NavType::GPSLNAV);
ephLNAVQZSSSF3->addUnsignedLong(0x22C0AA24,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x2A30637C,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x3FF3E0D0,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x1EC816E4,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x3FB8074C,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x2AABCE23,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x38897015,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x2AD457DF,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x3FF8CC38,30,1);
ephLNAVQZSSSF3->addUnsignedLong(0x204A5D8C,30,1);
ephLNAVQZSSSF3->trimsize();

almLNAVQZSS1 = std::make_shared<gnsstk::PackedNavBits>(almLNAVQZSSSid,
                                                      oidLNAVQZSS,
                                                      almLNAVQZSS1ct);
almLNAVQZSS1->setNavID(gnsstk::NavType::GPSLNAV);
almLNAVQZSS1->addUnsignedLong(0x22C0AA24,30,1);
almLNAVQZSS1->addUnsignedLong(0x2A30A504,30,1);
almLNAVQZSS1->addUnsignedLong(0x30607FD0,30,1);
almLNAVQZSS1->addUnsignedLong(0x0576B107,30,1);
almLNAVQZSS1->addUnsignedLong(0x3FD08035,30,1);
almLNAVQZSS1->addUnsignedLong(0x32BB681A,30,1);
almLNAVQZSS1->addUnsignedLong(0x1FA3DD84,30,1);
almLNAVQZSS1->addUnsignedLong(0x3029E6BE,30,1);
almLNAVQZSS1->addUnsignedLong(0x089CE67C,30,1);
almLNAVQZSS1->addUnsignedLong(0x39C00BA4,30,1);
almLNAVQZSS1->trimsize();

almLNAVQZSS2 = std::make_shared<gnsstk::PackedNavBits>(almLNAVQZSSSid,
                                                      oidLNAVQZSS,
                                                      almLNAVQZSS2ct);
almLNAVQZSS2->setNavID(gnsstk::NavType::GPSLNAV);
almLNAVQZSS2->addUnsignedLong(0x22C0AA24,30,1);
almLNAVQZSS2->addUnsignedLong(0x2A31E534,30,1);
almLNAVQZSS2->addUnsignedLong(0x309F071C,30,1);
almLNAVQZSS2->addUnsignedLong(0x0579F2BB,30,1);
almLNAVQZSS2->addUnsignedLong(0x3FC98019,30,1);
almLNAVQZSS2->addUnsignedLong(0x32BAB677,30,1);
almLNAVQZSS2->addUnsignedLong(0x36E8A57E,30,1);
almLNAVQZSS2->addUnsignedLong(0x2FEC8EBC,30,1);
almLNAVQZSS2->addUnsignedLong(0x322A7E03,30,1);
almLNAVQZSS2->addUnsignedLong(0x3FC005D8,30,1);
almLNAVQZSS2->trimsize();

pg56LNAVQZSS = std::make_shared<gnsstk::PackedNavBits>(almLNAVQZSSSid,
                                                      oidLNAVQZSS,
                                                      pg56LNAVQZSSct);
pg56LNAVQZSS->setNavID(gnsstk::NavType::GPSLNAV);
pg56LNAVQZSS->addUnsignedLong(0x22C0AA24,30,1);
pg56LNAVQZSS->addUnsignedLong(0x2A326470,30,1);
pg56LNAVQZSS->addUnsignedLong(0x3E01BFFD,30,1);
pg56LNAVQZSS->addUnsignedLong(0x3E3DC8BE,30,1);
pg56LNAVQZSS->addUnsignedLong(0x3828DFC7,30,1);
pg56LNAVQZSS->addUnsignedLong(0x0000003F,30,1);
pg56LNAVQZSS->addUnsignedLong(0x0000003F,30,1);
pg56LNAVQZSS->addUnsignedLong(0x00C5D1F0,30,1);
pg56LNAVQZSS->addUnsignedLong(0x04A241CF,30,1);
pg56LNAVQZSS->addUnsignedLong(0x0480009C,30,1);
pg56LNAVQZSS->trimsize();

pg51LNAVQZSS = std::make_shared<gnsstk::PackedNavBits>(almLNAVQZSSSid,
                                                      oidLNAVQZSS,
                                                      pg51LNAVQZSSct);
pg51LNAVQZSS->setNavID(gnsstk::NavType::GPSLNAV);
pg51LNAVQZSS->addUnsignedLong(0x22C0AA24,30,1);
pg51LNAVQZSS->addUnsignedLong(0x2A330490,30,1);
pg51LNAVQZSS->addUnsignedLong(0x3CC551E9,30,1);
pg51LNAVQZSS->addUnsignedLong(0x00000F9B,30,1);
pg51LNAVQZSS->addUnsignedLong(0x3EF80FA6,30,1);
pg51LNAVQZSS->addUnsignedLong(0x3EF8003D,30,1);
pg51LNAVQZSS->addUnsignedLong(0x00000016,30,1);
pg51LNAVQZSS->addUnsignedLong(0x00000029,30,1);
pg51LNAVQZSS->addUnsignedLong(0x00000016,30,1);
pg51LNAVQZSS->addUnsignedLong(0x0000008C,30,1);
pg51LNAVQZSS->trimsize();

pg61LNAVQZSS = std::make_shared<gnsstk::PackedNavBits>(almLNAVQZSSSid,
                                                      oidLNAVQZSS,
                                                      pg61LNAVQZSSct);
pg61LNAVQZSS->setNavID(gnsstk::NavType::GPSLNAV);
pg61LNAVQZSS->addUnsignedLong(0x22C0AA24,30,1);
pg61LNAVQZSS->addUnsignedLong(0x2A3145A8,30,1);
pg61LNAVQZSS->addUnsignedLong(0x3F42FE37,30,1);
pg61LNAVQZSS->addUnsignedLong(0x3EC00989,30,1);
pg61LNAVQZSS->addUnsignedLong(0x04E09FE9,30,1);
pg61LNAVQZSS->addUnsignedLong(0x00000016,30,1);
pg61LNAVQZSS->addUnsignedLong(0x00000029,30,1);
pg61LNAVQZSS->addUnsignedLong(0x00C5D1D9,30,1);
pg61LNAVQZSS->addUnsignedLong(0x04A241D9,30,1);
pg61LNAVQZSS->addUnsignedLong(0x04800010,30,1);
pg61LNAVQZSS->trimsize();
