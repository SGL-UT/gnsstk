/** @file LNavTestDataDef.hpp This file contains data definitions
 * for testing GPS LNav processing in the NavFactory code. */

oidLNAVGPS = gpstk::ObsID(gpstk::ObservationType::NavMsg,
                          gpstk::CarrierBand::L1,
                          gpstk::TrackingCode::CA);
ephLNAVGPSSF1ct = gpstk::GPSWeekSecond(1869,6.0);
ephLNAVGPSSF2ct = gpstk::GPSWeekSecond(1869,12.0);
ephLNAVGPSSF3ct = gpstk::GPSWeekSecond(1869,18.0);
almLNAVGPS25ct = gpstk::GPSWeekSecond(1869,54.0);
almLNAVGPS26ct = gpstk::GPSWeekSecond(1869,84.0);
pg51LNAVGPSct = gpstk::GPSWeekSecond(1869,750.0);
pg56LNAVGPSct = gpstk::GPSWeekSecond(1869,528.0);
pg63LNAVGPSct = gpstk::GPSWeekSecond(1869,744.0);
ephLNAVGPSSid = gpstk::SatID(4,gpstk::SatelliteSystem::GPS);
almLNAVGPSSid = gpstk::SatID(1,gpstk::SatelliteSystem::GPS);

ephLNAVGPSSF1 = std::make_shared<gpstk::PackedNavBits>(ephLNAVGPSSid,oidLNAVGPS,
                                                       ephLNAVGPSSF1ct);
ephLNAVGPSSF1->setNavID(gpstk::NavType::GPSLNAV);
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

ephLNAVGPSSF2 = std::make_shared<gpstk::PackedNavBits>(ephLNAVGPSSid,oidLNAVGPS,
                                                       ephLNAVGPSSF2ct);
ephLNAVGPSSF2->setNavID(gpstk::NavType::GPSLNAV);
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

ephLNAVGPSSF3 = std::make_shared<gpstk::PackedNavBits>(ephLNAVGPSSid,oidLNAVGPS,
                                                       ephLNAVGPSSF3ct);
ephLNAVGPSSF3->setNavID(gpstk::NavType::GPSLNAV);
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

almLNAVGPS25 = std::make_shared<gpstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                      almLNAVGPS25ct);
almLNAVGPS25->setNavID(gpstk::NavType::GPSLNAV);
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

almLNAVGPS26 = std::make_shared<gpstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                      almLNAVGPS26ct);
almLNAVGPS26->setNavID(gpstk::NavType::GPSLNAV);
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

pg51LNAVGPS = std::make_shared<gpstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                     pg51LNAVGPSct);
pg51LNAVGPS->setNavID(gpstk::NavType::GPSLNAV);
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

pg63LNAVGPS = std::make_shared<gpstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                     pg63LNAVGPSct);
pg63LNAVGPS->setNavID(gpstk::NavType::GPSLNAV);
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

pg56LNAVGPS = std::make_shared<gpstk::PackedNavBits>(almLNAVGPSSid,oidLNAVGPS,
                                                     pg56LNAVGPSct);
pg56LNAVGPS->setNavID(gpstk::NavType::GPSLNAV);
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
