/** @file CNavTestDataDecl.hpp This file contains data definitions
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

oidCNAVGPSL2 = gpstk::ObsID(gpstk::ObservationType::NavMsg,
                            gpstk::CarrierBand::L2,
                            gpstk::TrackingCode::L2CM);
ephCNAVGPSL2sid = gpstk::SatID(1, gpstk::SatelliteSystem::GPS);

msg10CNAVGPSL2ct = gpstk::GPSWeekSecond(2060,86400);
msg10CNAVGPSL2 = std::make_shared<gpstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg10CNAVGPSL2ct);
msg10CNAVGPSL2->setNavID(gpstk::NavType::GPSCNAVL2);
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

msg11CNAVGPSL2ct = gpstk::GPSWeekSecond(2060,86412);
msg11CNAVGPSL2 = std::make_shared<gpstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg11CNAVGPSL2ct);
msg11CNAVGPSL2->setNavID(gpstk::NavType::GPSCNAVL2);
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

msg30CNAVGPSL2ct = gpstk::GPSWeekSecond(2060,86424);
msg30CNAVGPSL2 = std::make_shared<gpstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg30CNAVGPSL2ct);
msg30CNAVGPSL2->setNavID(gpstk::NavType::GPSCNAVL2);
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

msg32CNAVGPSL2ct = gpstk::GPSWeekSecond(2060,86436);
msg32CNAVGPSL2 = std::make_shared<gpstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg32CNAVGPSL2ct);
msg32CNAVGPSL2->setNavID(gpstk::NavType::GPSCNAVL2);
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

msg33CNAVGPSL2ct = gpstk::GPSWeekSecond(2060,86424);
msg33CNAVGPSL2 = std::make_shared<gpstk::PackedNavBits>(ephCNAVGPSL2sid,
                                                        oidCNAVGPSL2,
                                                        msg33CNAVGPSL2ct);
msg33CNAVGPSL2->setNavID(gpstk::NavType::GPSCNAVL2);
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

// QZSS data

oidCNAVQZSSL5 = gpstk::ObsID(gpstk::ObservationType::NavMsg,
                             gpstk::CarrierBand::L5,
                             gpstk::TrackingCode::L5I);
ephCNAVQZSSL5sid = gpstk::SatID(193, gpstk::SatelliteSystem::QZSS);

msg10CNAVQZSSL5ct = gpstk::GPSWeekSecond(2097,345600);
msg10CNAVQZSSL5 = std::make_shared<gpstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg10CNAVQZSSL5ct);
msg10CNAVQZSSL5->setNavID(gpstk::NavType::GPSCNAVL5);
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

msg11CNAVQZSSL5ct = gpstk::GPSWeekSecond(2097,345606);
msg11CNAVQZSSL5 = std::make_shared<gpstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg11CNAVQZSSL5ct);
msg11CNAVQZSSL5->setNavID(gpstk::NavType::GPSCNAVL5);
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

msg30CNAVQZSSL5ct = gpstk::GPSWeekSecond(2097,345612);
msg30CNAVQZSSL5 = std::make_shared<gpstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg30CNAVQZSSL5ct);
msg30CNAVQZSSL5->setNavID(gpstk::NavType::GPSCNAVL5);
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

msg32CNAVQZSSL5ct = gpstk::GPSWeekSecond(2097,345636);
msg32CNAVQZSSL5 = std::make_shared<gpstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg32CNAVQZSSL5ct);
msg32CNAVQZSSL5->setNavID(gpstk::NavType::GPSCNAVL5);
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

msg35CNAVQZSSL5ct = gpstk::GPSWeekSecond(2097,345738);
msg35CNAVQZSSL5 = std::make_shared<gpstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg35CNAVQZSSL5ct);
msg35CNAVQZSSL5->setNavID(gpstk::NavType::GPSCNAVL5);
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

msg37CNAVQZSSL5ct = gpstk::GPSWeekSecond(2097,345642);
msg37CNAVQZSSL5 = std::make_shared<gpstk::PackedNavBits>(ephCNAVQZSSL5sid,
                                                         oidCNAVQZSSL5,
                                                         msg37CNAVQZSSL5ct);
msg37CNAVQZSSL5->setNavID(gpstk::NavType::GPSCNAVL5);
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

