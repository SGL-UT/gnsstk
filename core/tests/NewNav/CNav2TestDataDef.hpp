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
/** @file CNav2TestDataDef.hpp This file contains data definitions
 * for testing GPS CNav2 processing in the NavFactory code. */

/** @note Conversion from GPSWeekSecond to CommonTime with a
 * fractional second of week results in "noise" in CommonTime::m_fsod,
 * so I add the appropriate offset after-the-fact so the truth data
 * matches. */

oidCNAV2GPS = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::L1CD);
ephCNAV2GPSsid = gnsstk::SatID(4, gnsstk::SatelliteSystem::GPS);

sf123p1CNAV2GPSct = gnsstk::GPSWeekSecond(2049,345600);
sf123p1CNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                         oidCNAV2GPS,
                                                         sf123p1CNAV2GPSct);
sf123p1CNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf123p1CNAV2GPS->addUnsignedLong(0, 9, 1);           // subframe 1
sf123p1CNAV2GPS->addUnsignedLong(0x4009842F, 32, 1); // subframe 2
sf123p1CNAV2GPS->addUnsignedLong(0xF64901C9, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x8940061F, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x72D8B7FE, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xB8B4F053, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x0C30018B, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xBC5AF86B, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x78E9F71F, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x2D6A2721, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x415081C7, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xFF46800A, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x0011000F, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x9080290D, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x81008800, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xCCFC5C4E, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xD8830057, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x6003DA5F, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xE5FFA004, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x3E44C8, 24, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x04040380, 32, 1); // subframe 3 page 1
sf123p1CNAV2GPS->addUnsignedLong(0x0440049E, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xC0100278, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0x97120B02, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xFFFE2B02, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xFDFDFF67, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xC77CD7E6, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xB000000F, 32, 1);
sf123p1CNAV2GPS->addUnsignedLong(0xBA514000 >> 14, 18, 1);
sf123p1CNAV2GPS->trimsize();

sf123p2CNAV2GPSct = gnsstk::GPSWeekSecond(2049,345618);
sf123p2CNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                         oidCNAV2GPS,
                                                         sf123p2CNAV2GPSct);
sf123p2CNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf123p2CNAV2GPS->addUnsignedLong(0, 9, 1);           // subframe 1
sf123p2CNAV2GPS->addUnsignedLong(0x4009842F, 32, 1); // subframe 2
sf123p2CNAV2GPS->addUnsignedLong(0xF64901C9, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x8940061F, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x72D8B7FE, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0xB8B4F053, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x0C30018B, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0xBC5AF86B, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x78E9F71F, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x2D6A2721, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x415081C7, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0xFF46800A, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x0011000F, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x9080290D, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x81008800, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0xCCFC5C4E, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0xD8830057, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x6003DA5F, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0xE5FFA004, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x3E44C8, 24, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x04080000, 32, 1); // subframe 3 page 2
sf123p2CNAV2GPS->addUnsignedLong(0x00000000, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x00001EC0, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x01E2C61E, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0xBCCF10A2, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x24BFEE0D, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x48F6D230, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x00000032, 32, 1);
sf123p2CNAV2GPS->addUnsignedLong(0x16A84000 >> 14, 18, 1);
sf123p2CNAV2GPS->trimsize();

sf123p4CNAV2GPSct = gnsstk::GPSWeekSecond(2049,345636);
sf123p4CNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                         oidCNAV2GPS,
                                                         sf123p4CNAV2GPSct);
sf123p4CNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf123p4CNAV2GPS->addUnsignedLong(0, 9, 1);           // subframe 1
sf123p4CNAV2GPS->addUnsignedLong(0x4009842F, 32, 1); // subframe 2
sf123p4CNAV2GPS->addUnsignedLong(0xF64901C9, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x8940061F, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x72D8B7FE, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xB8B4F053, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x0C30018B, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xBC5AF86B, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x78E9F71F, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x2D6A2721, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x415081C7, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xFF46800A, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x0011000F, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x9080290D, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x81008800, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xCCFC5C4E, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xD8830057, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x6003DA5F, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xE5FFA004, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x3E44C8, 24, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x0411002F, 32, 1); // subframe 3 page 4
sf123p4CNAV2GPS->addUnsignedLong(0x60466B83, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xDFD5421A, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xA25EB8C6, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x044FE6DF, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xF8000000, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x00000000, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0x0000001D, 32, 1);
sf123p4CNAV2GPS->addUnsignedLong(0xB26C4000 >> 14, 18, 1);
sf123p4CNAV2GPS->trimsize();

sf2CNAV2GPSct = gnsstk::GPSWeekSecond(2049,345600);
sf2CNAV2GPSct += 0.52;
sf2CNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                     oidCNAV2GPS,
                                                     sf2CNAV2GPSct);
sf2CNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf2CNAV2GPS->addUnsignedLong(0x4009842F, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0xF64901C9, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x8940061F, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x72D8B7FE, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0xB8B4F053, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x0C30018B, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0xBC5AF86B, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x78E9F71F, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x2D6A2721, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x415081C7, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0xFF46800A, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x0011000F, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x9080290D, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x81008800, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0xCCFC5C4E, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0xD8830057, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x6003DA5F, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0xE5FFA004, 32, 1);
sf2CNAV2GPS->addUnsignedLong(0x3E44C8, 24, 1);
sf2CNAV2GPS->trimsize();

sf2CNAV2GPS2ct = gnsstk::GPSWeekSecond(2049,352800);
sf2CNAV2GPS2ct += 0.52;
sf2CNAV2GPS2 = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                      oidCNAV2GPS,
                                                      sf2CNAV2GPS2ct);
sf2CNAV2GPS2->setNavID(gnsstk::NavType::GPSCNAV2);
sf2CNAV2GPS2->addUnsignedLong(0x40098C2F, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0xF65501CB, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x629FF8B7, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0xB2D4A001, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x9547900C, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x1E480191, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x1AB6FBA0, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x848DF71E, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x02162721, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x401281FF, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0xFEC90013, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0xFFF28014, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x0A0037AC, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x80E7D401, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x05FC5C4E, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0xDD4F0057, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0x4003DA5F, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0xE5FFA004, 32, 1);
sf2CNAV2GPS2->addUnsignedLong(0xA5AAC5, 24, 1);
sf2CNAV2GPS2->trimsize();

sf3p1CNAV2GPSct = gnsstk::GPSWeekSecond(2049,345600);
sf3p1CNAV2GPSct += 12.52;
sf3p1CNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                       oidCNAV2GPS,
                                                       sf3p1CNAV2GPSct);
sf3p1CNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf3p1CNAV2GPS->addUnsignedLong(0x04040380, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0x0440049E, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0xC0100278, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0x97120B02, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0xFFFE2B02, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0xFDFDFF67, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0xC77CD7E6, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0xB000000F, 32, 1);
sf3p1CNAV2GPS->addUnsignedLong(0xBA514000 >> 14, 18, 1);
sf3p1CNAV2GPS->trimsize();

sf3p2CNAV2GPSct = gnsstk::GPSWeekSecond(2049,345618);
sf3p2CNAV2GPSct += 12.52;
sf3p2CNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                       oidCNAV2GPS,
                                                       sf3p2CNAV2GPSct);
sf3p2CNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf3p2CNAV2GPS->addUnsignedLong(0x04080000, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0x00000000, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0x00001EC0, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0x01E2C61E, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0xBCCF10A2, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0x24BFEE0D, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0x48F6D230, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0x00000032, 32, 1);
sf3p2CNAV2GPS->addUnsignedLong(0x16A84000 >> 14, 18, 1);
sf3p2CNAV2GPS->trimsize();

// faking some data since we don't have any data that contains real offsets
sf3p2fakeCNAV2GPSct = gnsstk::GPSWeekSecond(2049,345618);
sf3p2fakeCNAV2GPSct += 12.52;
sf3p2fakeCNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                           oidCNAV2GPS,
                                                           sf3p2fakeCNAV2GPSct);
sf3p2fakeCNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf3p2fakeCNAV2GPS->addUnsignedLong(4, 8, 1); // prn
sf3p2fakeCNAV2GPS->addUnsignedLong(2, 6, 1); // page number
sf3p2fakeCNAV2GPS->addUnsignedLong(2, 3, 1); // GNSS ID = GLONASS
sf3p2fakeCNAV2GPS->addUnsignedLong(3456, 16, 16); // tggto
sf3p2fakeCNAV2GPS->addUnsignedLong(2044, 13, 1); // WNggto
sf3p2fakeCNAV2GPS->addSignedDouble(6.5192580223e-09, 16, -35); // A0
sf3p2fakeCNAV2GPS->addSignedDouble(1.5099033135e-14, 13, -51); // A1
sf3p2fakeCNAV2GPS->addSignedDouble(0, 7, -68); // A2
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 16, 1); // tEOP
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 21, 1); // PM_X
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 15, 1); // PM_X dot
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 21, 1); // PM_Y
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 15, 1); // PM_Y dot
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 31, 1); // delta UTGPS
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 19, 1); // delta UTGPS dot
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 30, 1); // reserved
sf3p2fakeCNAV2GPS->addUnsignedLong(0, 24, 1); // CRC
sf3p2fakeCNAV2GPS->trimsize();

sf3p4CNAV2GPSct = gnsstk::GPSWeekSecond(2049,345636);
sf3p4CNAV2GPSct += 12.52;
sf3p4CNAV2GPS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2GPSsid,
                                                       oidCNAV2GPS,
                                                       sf3p4CNAV2GPSct);
sf3p4CNAV2GPS->setNavID(gnsstk::NavType::GPSCNAV2);
sf3p4CNAV2GPS->addUnsignedLong(0x0411002F, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0x60466B83, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0xDFD5421A, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0xA25EB8C6, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0x044FE6DF, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0xF8000000, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0x00000000, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0x0000001D, 32, 1);
sf3p4CNAV2GPS->addUnsignedLong(0xB26C4000 >> 14, 18, 1);
sf3p4CNAV2GPS->trimsize();

// QZSS

oidCNAV2QZSS = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::L1CD);
ephCNAV2QZSSsid = gnsstk::SatID(193, gnsstk::SatelliteSystem::QZSS);

sf2CNAV2QZSSct = gnsstk::GPSWeekSecond(2155,324504,gnsstk::TimeSystem::QZS);
sf2CNAV2QZSSct += 0.52;
sf2CNAV2QZSS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2QZSSsid,
                                                      oidCNAV2QZSS,
                                                      sf2CNAV2QZSSct);
sf2CNAV2QZSS->setNavID(gnsstk::NavType::GPSCNAV2);
sf2CNAV2QZSS->addUnsignedLong(0x43596C44, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x62227D90, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0xA5000639, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0xD2A3AFFF, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x525A5E35, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0xCC9935D3, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x777300F8, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x431B96C1, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x22CC1DC5, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x63721795, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x42C4804A, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x00547FEB, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0xAF00E339, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0xFF69A800, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x0BBD033D, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x3DA8FFA6, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0xE003E880, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x0A0079AC, 32, 1);
sf2CNAV2QZSS->addUnsignedLong(0x710206, 24, 1);
sf2CNAV2QZSS->trimsize();

sf3p1CNAV2QZSSct = gnsstk::GPSWeekSecond(2155,324576,gnsstk::TimeSystem::QZS);
sf3p1CNAV2QZSSct += 12.52;
sf3p1CNAV2QZSS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2QZSSsid,
                                                        oidCNAV2QZSS,
                                                        sf3p1CNAV2QZSSct);
sf3p1CNAV2QZSS->setNavID(gnsstk::NavType::GPSCNAV2);
sf3p1CNAV2QZSS->addUnsignedLong(0xC107FA6C, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0x000004A0, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0x4CD0D678, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0x97120E00, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0xF9F533F2, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0xCA7F0000, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0x1E80AC04, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0x40000005, 32, 1);
sf3p1CNAV2QZSS->addUnsignedLong(0xA9184000 >> 14, 18, 1);
sf3p1CNAV2QZSS->trimsize();


sf3p2CNAV2QZSSct = gnsstk::GPSWeekSecond(2155,324612,gnsstk::TimeSystem::QZS);
sf3p2CNAV2QZSSct += 12.52;
sf3p2CNAV2QZSS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2QZSSsid,
                                                        oidCNAV2QZSS,
                                                        sf3p2CNAV2QZSSct);
sf3p2CNAV2QZSS->setNavID(gnsstk::NavType::GPSCNAV2);
sf3p2CNAV2QZSS->addUnsignedLong(0xC109AB81, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0xA1AC0000, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0x000013FE, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0xC34E8425, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0x98DEB281, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0x047FE886, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0xE5F94FB6, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0xB000000E, 32, 1);
sf3p2CNAV2QZSS->addUnsignedLong(0xA6E50000 >> 14, 18, 1);
sf3p2CNAV2QZSS->trimsize();

sf3p4CNAV2QZSSct = gnsstk::GPSWeekSecond(2155,324522,gnsstk::TimeSystem::QZS);
sf3p4CNAV2QZSSct += 12.52;
sf3p4CNAV2QZSS = std::make_shared<gnsstk::PackedNavBits>(ephCNAV2QZSSsid,
                                                        oidCNAV2QZSS,
                                                        sf3p4CNAV2QZSSct);
sf3p4CNAV2QZSS->setNavID(gnsstk::NavType::GPSCNAV2);
sf3p4CNAV2QZSS->addUnsignedLong(0xC1110D6F, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0xF861D9E8, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0x5FF395D7, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0x6DBAC048, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0x3EA5FF60, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0x00000000, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0x00000000, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0x00000022, 32, 1);
sf3p4CNAV2QZSS->addUnsignedLong(0x55238000 >> 14, 18, 1);
sf3p4CNAV2QZSS->trimsize();
