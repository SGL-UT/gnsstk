/** @file CNavTestDataDecl.hpp This file contains data definitions
 * for testing GPS CNav processing in the NavFactory code. */

oidCNAV = gpstk::ObsID(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::L2,
                       gpstk::TrackingCode::L2CM);
ephCNAVsid = gpstk::SatID(1, gpstk::SatelliteSystem::GPS);
msg10CNAVct = gpstk::GPSWeekSecond(2060,86400);
msg11CNAVct = gpstk::GPSWeekSecond(2060,86412);
msg30CNAVct = gpstk::GPSWeekSecond(2060,86424);
msg10CNAV = std::make_shared<gpstk::PackedNavBits>(ephCNAVsid,oidCNAV,
                                                   msg10CNAVct);
msg10CNAV->setNavID(gpstk::NavType::GPSCNAVL2);
msg10CNAV->addUnsignedLong(0x8B04A1C2, 32, 1);
msg10CNAV->addUnsignedLong(0x11018424, 32, 1);
msg10CNAV->addUnsignedLong(0x00990077, 32, 1);
msg10CNAV->addUnsignedLong(0x26DFFD06, 32, 1);
msg10CNAV->addUnsignedLong(0xC2B03800, 32, 1);
msg10CNAV->addUnsignedLong(0x727D7558, 32, 1);
msg10CNAV->addUnsignedLong(0x54B824E9, 32, 1);
msg10CNAV->addUnsignedLong(0x11907661, 32, 1);
msg10CNAV->addUnsignedLong(0x66186063, 32, 1);
msg10CNAV->addUnsignedLong(0x03A, 12, 1);
msg11CNAV = std::make_shared<gpstk::PackedNavBits>(ephCNAVsid,oidCNAV,
                                                   msg11CNAVct);
msg11CNAV->setNavID(gpstk::NavType::GPSCNAVL2);
msg11CNAV->addUnsignedLong(0x8B04B1C2, 32, 1);
msg11CNAV->addUnsignedLong(0x20993694, 32, 1);
msg11CNAV->addUnsignedLong(0x994A49F2, 32, 1);
msg11CNAV->addUnsignedLong(0x0ED4609D, 32, 1);
msg11CNAV->addUnsignedLong(0xA079400F, 32, 1);
msg11CNAV->addUnsignedLong(0x6015DFF0, 32, 1);
msg11CNAV->addUnsignedLong(0x224015EE, 32, 1);
msg11CNAV->addUnsignedLong(0xE02E43FF, 32, 1);
msg11CNAV->addUnsignedLong(0x20E7C9CE, 32, 1);
msg11CNAV->addUnsignedLong(0x3FE, 12, 1);
msg30CNAV = std::make_shared<gpstk::PackedNavBits>(ephCNAVsid,oidCNAV,
                                                   msg30CNAVct);
msg30CNAV->setNavID(gpstk::NavType::GPSCNAVL2);
msg30CNAV->addUnsignedLong(0x8B05E1C2, 32, 1);
msg30CNAV->addUnsignedLong(0x30246CF2, 32, 1);
msg30CNAV->addUnsignedLong(0x65F190CB, 32, 1);
msg30CNAV->addUnsignedLong(0xFFA6E800, 32, 1);
msg30CNAV->addUnsignedLong(0x0C0FFA7E, 32, 1);
msg30CNAV->addUnsignedLong(0x501BA0E1, 32, 1);
msg30CNAV->addUnsignedLong(0x0502FFFE, 32, 1);
msg30CNAV->addUnsignedLong(0x2805FFF8, 32, 1);
msg30CNAV->addUnsignedLong(0x0C142A67, 32, 1);
msg30CNAV->addUnsignedLong(0xD56, 12, 1);
