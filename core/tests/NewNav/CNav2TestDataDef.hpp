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
/** @file CNav2TestDataDef.hpp This file contains data definitions
 * for testing GPS CNav2 processing in the NavFactory code. */

oidCNAV2GPS = gpstk::ObsID(gpstk::ObservationType::NavMsg,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::L1CD);
ephCNAV2GPSsid = gpstk::SatID(4, gpstk::SatelliteSystem::GPS);

sf2CNAV2GPSct = gpstk::GPSWeekSecond(2049,345600);
sf2CNAV2GPS = std::make_shared<gpstk::PackedNavBits>(ephCNAV2GPSsid,
                                                     oidCNAV2GPS,
                                                     sf2CNAV2GPSct);
sf2CNAV2GPS->setNavID(gpstk::NavType::GPSCNAV2);
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

sf3p1CNAV2GPSct = gpstk::GPSWeekSecond(2049,345600);
sf3p1CNAV2GPS = std::make_shared<gpstk::PackedNavBits>(ephCNAV2GPSsid,
                                                       oidCNAV2GPS,
                                                       sf3p1CNAV2GPSct);
sf3p1CNAV2GPS->setNavID(gpstk::NavType::GPSCNAV2);
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

sf3p2CNAV2GPSct = gpstk::GPSWeekSecond(2049,345618);
sf3p2CNAV2GPS = std::make_shared<gpstk::PackedNavBits>(ephCNAV2GPSsid,
                                                       oidCNAV2GPS,
                                                       sf3p2CNAV2GPSct);
sf3p2CNAV2GPS->setNavID(gpstk::NavType::GPSCNAV2);
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
sf3p2fakeCNAV2GPSct = gpstk::GPSWeekSecond(2049,345618);
sf3p2fakeCNAV2GPS = std::make_shared<gpstk::PackedNavBits>(ephCNAV2GPSsid,
                                                           oidCNAV2GPS,
                                                           sf3p2fakeCNAV2GPSct);
sf3p2fakeCNAV2GPS->setNavID(gpstk::NavType::GPSCNAV2);
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

sf3p4CNAV2GPSct = gpstk::GPSWeekSecond(2049,345636);
sf3p4CNAV2GPS = std::make_shared<gpstk::PackedNavBits>(ephCNAV2GPSsid,
                                                       oidCNAV2GPS,
                                                       sf3p4CNAV2GPSct);
sf3p4CNAV2GPS->setNavID(gpstk::NavType::GPSCNAV2);
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
