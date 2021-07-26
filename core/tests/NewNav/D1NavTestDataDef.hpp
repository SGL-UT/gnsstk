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
/** @file D1NavTestDataDef.hpp This file contains data definitions
 * for testing BeiDou D1Nav processing in the NavFactory code. */

oidD1NAV = gpstk::ObsID(gpstk::ObservationType::NavMsg,
                        gpstk::CarrierBand::B1,
                        gpstk::TrackingCode::B1I);

ephD1NAVSid = gpstk::SatID(6,gpstk::SatelliteSystem::BeiDou);
almD1NAVSid = gpstk::SatID(6,gpstk::SatelliteSystem::BeiDou);
almD1NAVSidB = gpstk::SatID(19,gpstk::SatelliteSystem::BeiDou);

ephD1NAVSF1ct = gpstk::BDSWeekSecond(810,345614);
ephD1NAVSF1 = std::make_shared<gpstk::PackedNavBits>(ephD1NAVSid,oidD1NAV,
                                                     ephD1NAVSF1ct);
ephD1NAVSF1->setNavID(gpstk::NavType::BeiDou_D1);
ephD1NAVSF1->addUnsignedLong(0x38901541,30,1);
ephD1NAVSF1->addUnsignedLong(0x18000020,30,1);
ephD1NAVSF1->addUnsignedLong(0x0654A846,30,1);
ephD1NAVSF1->addUnsignedLong(0x30052F4E,30,1);
ephD1NAVSF1->addUnsignedLong(0x2D070427,30,1);
ephD1NAVSF1->addUnsignedLong(0x3EC1CDC2,30,1);
ephD1NAVSF1->addUnsignedLong(0x30BF8085,30,1);
ephD1NAVSF1->addUnsignedLong(0x1C0028C1,30,1);
ephD1NAVSF1->addUnsignedLong(0x3F47E038,30,1);
ephD1NAVSF1->addUnsignedLong(0x1A8AE13D,30,1);
ephD1NAVSF1->trimsize();


ephD1NAVSF2ct = gpstk::BDSWeekSecond(810,345620);
ephD1NAVSF2 = std::make_shared<gpstk::PackedNavBits>(ephD1NAVSid,oidD1NAV,
                                                     ephD1NAVSF2ct);
ephD1NAVSF2->setNavID(gpstk::NavType::BeiDou_D1);
ephD1NAVSF2->addUnsignedLong(0x38902543,30,1);
ephD1NAVSF2->addUnsignedLong(0x18183473,30,1);
ephD1NAVSF2->addUnsignedLong(0x25F68C9D,30,1);
ephD1NAVSF2->addUnsignedLong(0x3FC82650,30,1);
ephD1NAVSF2->addUnsignedLong(0x1B1C17A5,30,1);
ephD1NAVSF2->addUnsignedLong(0x070E0107,30,1);
ephD1NAVSF2->addUnsignedLong(0x0B05EE24,30,1);
ephD1NAVSF2->addUnsignedLong(0x1AD7F7D4,30,1);
ephD1NAVSF2->addUnsignedLong(0x033CAE4C,30,1);
ephD1NAVSF2->addUnsignedLong(0x3B50A9BC,30,1);
ephD1NAVSF2->trimsize();


ephD1NAVSF3ct = gpstk::BDSWeekSecond(810,345626);
ephD1NAVSF3 = std::make_shared<gpstk::PackedNavBits>(ephD1NAVSid,oidD1NAV,
                                                     ephD1NAVSF3ct);
ephD1NAVSF3->setNavID(gpstk::NavType::BeiDou_D1);
ephD1NAVSF3->addUnsignedLong(0x3890354C,30,1);
ephD1NAVSF3->addUnsignedLong(0x18314682,30,1);
ephD1NAVSF3->addUnsignedLong(0x004D0280,30,1);
ephD1NAVSF3->addUnsignedLong(0x3C6D0016,30,1);
ephD1NAVSF3->addUnsignedLong(0x0557FF9F,30,1);
ephD1NAVSF3->addUnsignedLong(0x190DFFB2,30,1);
ephD1NAVSF3->addUnsignedLong(0x36E264FC,30,1);
ephD1NAVSF3->addUnsignedLong(0x363DCF4D,30,1);
ephD1NAVSF3->addUnsignedLong(0x3AB53A34,30,1);
ephD1NAVSF3->addUnsignedLong(0x145AB4D0,30,1);
ephD1NAVSF3->trimsize();


almD1NAVSF4p1ct = gpstk::BDSWeekSecond(810,345632);
almD1NAVSF4p1 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF4p1ct);
almD1NAVSF4p1->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p1->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p1->addUnsignedLong(0x184807A9,30,1);
almD1NAVSF4p1->addUnsignedLong(0x0AEB5395,30,1);
almD1NAVSF4p1->addUnsignedLong(0x0056D7D1,30,1);
almD1NAVSF4p1->addUnsignedLong(0x0DEA78AE,30,1);
almD1NAVSF4p1->addUnsignedLong(0x1028182E,30,1);
almD1NAVSF4p1->addUnsignedLong(0x063C2808,30,1);
almD1NAVSF4p1->addUnsignedLong(0x00196052,30,1);
almD1NAVSF4p1->addUnsignedLong(0x003FCA9F,30,1);
almD1NAVSF4p1->addUnsignedLong(0x0A09A1B8,30,1);
almD1NAVSF4p1->trimsize();


almD1NAVSF5p1ct = gpstk::BDSWeekSecond(810,345638);
almD1NAVSF5p1 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF5p1ct);
almD1NAVSF5p1->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p1->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p1->addUnsignedLong(0x1860065A,30,1);
almD1NAVSF5p1->addUnsignedLong(0x25147FB3,30,1);
almD1NAVSF5p1->addUnsignedLong(0x00145E6A,30,1);
almD1NAVSF5p1->addUnsignedLong(0x37AE7BD2,30,1);
almD1NAVSF5p1->addUnsignedLong(0x1017A0BF,30,1);
almD1NAVSF5p1->addUnsignedLong(0x0A4A290F,30,1);
almD1NAVSF5p1->addUnsignedLong(0x3F6500F6,30,1);
almD1NAVSF5p1->addUnsignedLong(0x2411BB10,30,1);
almD1NAVSF5p1->addUnsignedLong(0x0A5EAD64,30,1);
almD1NAVSF5p1->trimsize();


almD1NAVSF4p7ct = gpstk::BDSWeekSecond(810,345812);
almD1NAVSF4p7 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF4p7ct);
almD1NAVSF4p7->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p7->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p7->addUnsignedLong(0x1B181FA7,30,1);
almD1NAVSF4p7->addUnsignedLong(0x0AEFC59D,30,1);
almD1NAVSF4p7->addUnsignedLong(0x3FA88B20,30,1);
almD1NAVSF4p7->addUnsignedLong(0x00D17B89,30,1);
almD1NAVSF4p7->addUnsignedLong(0x1238AE35,30,1);
almD1NAVSF4p7->addUnsignedLong(0x385A2352,30,1);
almD1NAVSF4p7->addUnsignedLong(0x3FD3E60B,30,1);
almD1NAVSF4p7->addUnsignedLong(0x0DB2684B,30,1);
almD1NAVSF4p7->addUnsignedLong(0x12966991,30,1);
almD1NAVSF4p7->trimsize();


almD1NAVSF5p7ct = gpstk::BDSWeekSecond(810,345818);
almD1NAVSF5p7 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF5p7ct);
almD1NAVSF5p7->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p7->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p7->addUnsignedLong(0x1B301C52,30,1);
almD1NAVSF5p7->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p7->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p7->addUnsignedLong(0x002000C0,30,1);
almD1NAVSF5p7->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p7->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p7->addUnsignedLong(0x0000FF04,30,1);
almD1NAVSF5p7->addUnsignedLong(0x00080739,30,1);
almD1NAVSF5p7->addUnsignedLong(0x3E2C2ADE,30,1);
almD1NAVSF5p7->trimsize();


almD1NAVSF4p8ct = gpstk::BDSWeekSecond(810,345842);
almD1NAVSF4p8 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF4p8ct);
almD1NAVSF4p8->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p8->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p8->addUnsignedLong(0x1B9023CF,30,1);
almD1NAVSF4p8->addUnsignedLong(0x0AE68AA7,30,1);
almD1NAVSF4p8->addUnsignedLong(0x3FFC4DFA,30,1);
almD1NAVSF4p8->addUnsignedLong(0x16DBD4F2,30,1);
almD1NAVSF4p8->addUnsignedLong(0x115DF296,30,1);
almD1NAVSF4p8->addUnsignedLong(0x04102982,30,1);
almD1NAVSF4p8->addUnsignedLong(0x3FC7646A,30,1);
almD1NAVSF4p8->addUnsignedLong(0x2617356B,30,1);
almD1NAVSF4p8->addUnsignedLong(0x206101EC,30,1);
almD1NAVSF4p8->trimsize();


almD1NAVSF5p8ct = gpstk::BDSWeekSecond(810,345848);
almD1NAVSF5p8 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF5p8ct);
almD1NAVSF5p8->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p8->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p8->addUnsignedLong(0x1BA8205A,30,1);
almD1NAVSF5p8->addUnsignedLong(0x2C160BEA,30,1);
almD1NAVSF5p8->addUnsignedLong(0x0160B0D1,30,1);
almD1NAVSF5p8->addUnsignedLong(0x160B057D,30,1);
almD1NAVSF5p8->addUnsignedLong(0x20B05869,30,1);
almD1NAVSF5p8->addUnsignedLong(0x0B054227,30,1);
almD1NAVSF5p8->addUnsignedLong(0x22AAAA44,30,1);
almD1NAVSF5p8->addUnsignedLong(0x2AAAAAB4,30,1);
almD1NAVSF5p8->addUnsignedLong(0x2AAAAAB4,30,1);
almD1NAVSF5p8->trimsize();


almD1NAVSF4p9ct = gpstk::BDSWeekSecond(810,345872);
almD1NAVSF4p9 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF4p9ct);
almD1NAVSF4p9->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p9->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p9->addUnsignedLong(0x1C0827F3,30,1);
almD1NAVSF4p9->addUnsignedLong(0x0AED4A9C,30,1);
almD1NAVSF4p9->addUnsignedLong(0x3FC19064,30,1);
almD1NAVSF4p9->addUnsignedLong(0x2CEC4694,30,1);
almD1NAVSF4p9->addUnsignedLong(0x321DE065,30,1);
almD1NAVSF4p9->addUnsignedLong(0x09B629A6,30,1);
almD1NAVSF4p9->addUnsignedLong(0x3FD9A830,30,1);
almD1NAVSF4p9->addUnsignedLong(0x0F17FEAC,30,1);
almD1NAVSF4p9->addUnsignedLong(0x075245E5,30,1);
almD1NAVSF4p9->trimsize();

almD1NAVSF5p9ct = gpstk::BDSWeekSecond(810,345878);
almD1NAVSF5p9 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                       almD1NAVSF5p9ct);
almD1NAVSF5p9->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p9->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p9->addUnsignedLong(0x1C202406,30,1);
almD1NAVSF5p9->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p9->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p9->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p9->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p9->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p9->addUnsignedLong(0x0015556B,30,1);
almD1NAVSF5p9->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p9->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p9->trimsize();


almD1NAVSF4p10ct = gpstk::BDSWeekSecond(810,345902);
almD1NAVSF4p10 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                        almD1NAVSF4p10ct);
almD1NAVSF4p10->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p10->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p10->addUnsignedLong(0x1C802B94,30,1);
almD1NAVSF4p10->addUnsignedLong(0x0AE86A90,30,1);
almD1NAVSF4p10->addUnsignedLong(0x000FEB36,30,1);
almD1NAVSF4p10->addUnsignedLong(0x00C033EA,30,1);
almD1NAVSF4p10->addUnsignedLong(0x11F45631,30,1);
almD1NAVSF4p10->addUnsignedLong(0x3AAC296B,30,1);
almD1NAVSF4p10->addUnsignedLong(0x3FD2A603,30,1);
almD1NAVSF4p10->addUnsignedLong(0x06FA09E5,30,1);
almD1NAVSF4p10->addUnsignedLong(0x2E05E9FD,30,1);
almD1NAVSF4p10->trimsize();

almD1NAVSF5p10ct = gpstk::BDSWeekSecond(810,345908);
almD1NAVSF5p10 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                        almD1NAVSF5p10ct);
almD1NAVSF5p10->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p10->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p10->addUnsignedLong(0x1C9828C1,30,1);
almD1NAVSF5p10->addUnsignedLong(0x04043DE2,30,1);
almD1NAVSF5p10->addUnsignedLong(0x3FFFFFFF,30,1);
almD1NAVSF5p10->addUnsignedLong(0x3F9000B0,30,1);
almD1NAVSF5p10->addUnsignedLong(0x0000190D,30,1);
almD1NAVSF5p10->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p10->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p10->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p10->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p10->trimsize();


almD1NAVSF4p11ct = gpstk::BDSWeekSecond(810,345932);
almD1NAVSF4p11 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                        almD1NAVSF4p11ct);
almD1NAVSF4p11->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p11->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p11->addUnsignedLong(0x1CF82EBB,30,1);
almD1NAVSF4p11->addUnsignedLong(0x251489BF,30,1);
almD1NAVSF4p11->addUnsignedLong(0x0031DAAE,30,1);
almD1NAVSF4p11->addUnsignedLong(0x0D746EA5,30,1);
almD1NAVSF4p11->addUnsignedLong(0x107988FA,30,1);
almD1NAVSF4p11->addUnsignedLong(0x3E5C29CB,30,1);
almD1NAVSF4p11->addUnsignedLong(0x3F6F6ACB,30,1);
almD1NAVSF4p11->addUnsignedLong(0x3DBABC3B,30,1);
almD1NAVSF4p11->addUnsignedLong(0x0E4E1DE5,30,1);
almD1NAVSF4p11->trimsize();


almD1NAVSF5p11ct = gpstk::BDSWeekSecond(810,345938);
almD1NAVSF5p11 = std::make_shared<gpstk::PackedNavBits>(almD1NAVSid,oidD1NAV,
                                                        almD1NAVSF5p11ct);
almD1NAVSF5p11->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p11->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p11->addUnsignedLong(0x1D102C0D,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p11->trimsize();


almD1NAVSF4p24Bct = gpstk::BDSWeekSecond(810,345602);
almD1NAVSF4p24B = std::make_shared<gpstk::PackedNavBits>(almD1NAVSidB,oidD1NAV,
                                                         almD1NAVSF4p24Bct);
almD1NAVSF4p24B->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p24B->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x17D0626B,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x251474BD,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x3FDC2C34,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x37F0603D,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x10194088,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x08A2296F,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x3F64BFFA,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x0FAA6D64,30,1);
almD1NAVSF4p24B->addUnsignedLong(0x2628CBF7,30,1);
almD1NAVSF4p24B->trimsize();


almD1NAVSF5p24Bct = gpstk::BDSWeekSecond(810,345608);
almD1NAVSF5p24B = std::make_shared<gpstk::PackedNavBits>(almD1NAVSidB,oidD1NAV,
                                                         almD1NAVSF5p24Bct);
almD1NAVSF5p24B->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p24B->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x17E861FE,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x145555EB,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p24B->addUnsignedLong(0x1555554B,30,1);
almD1NAVSF5p24B->trimsize();


almD1NAVSF4p8Bct = gpstk::BDSWeekSecond(810,345848);
almD1NAVSF4p8B = std::make_shared<gpstk::PackedNavBits>(almD1NAVSidB,oidD1NAV,
                                                        almD1NAVSF4p8Bct);
almD1NAVSF4p8B->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p8B->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x1B9023CF,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x0AE68AA7,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x3FFC4DFA,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x16DBD4F2,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x115DF296,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x04102982,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x3FC7646A,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x2617356B,30,1);
almD1NAVSF4p8B->addUnsignedLong(0x206103EA,30,1);
almD1NAVSF4p8B->trimsize();


almD1NAVSF5p8Bct = gpstk::BDSWeekSecond(810,345848);
almD1NAVSF5p8B = std::make_shared<gpstk::PackedNavBits>(almD1NAVSidB,oidD1NAV,
                                                         almD1NAVSF5p8Bct);
almD1NAVSF5p8B->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p8B->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x1BA8205A,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x2C160BEA,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x0160B0D1,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x160B057D,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x20B05869,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x0B054227,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x20000090,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p8B->addUnsignedLong(0x00000000,30,1);
almD1NAVSF5p8B->trimsize();


almD1NAVSF4p11Bct = gpstk::BDSWeekSecond(810,345932);
almD1NAVSF4p11B = std::make_shared<gpstk::PackedNavBits>(almD1NAVSidB,oidD1NAV,
                                                         almD1NAVSF4p11Bct);
almD1NAVSF4p11B->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF4p11B->addUnsignedLong(0x38904547,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x1CF82EBB,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x251489BF,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x0031DAAE,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x0D746EA5,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x107988FA,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x3E5C29CB,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x3F6F6ACB,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x3DBABC3B,30,1);
almD1NAVSF4p11B->addUnsignedLong(0x0E4E1FE3,30,1);
almD1NAVSF4p11B->trimsize();


almD1NAVSF5p11Bct = gpstk::BDSWeekSecond(810,345938);
almD1NAVSF5p11B = std::make_shared<gpstk::PackedNavBits>(almD1NAVSidB,oidD1NAV,
                                                         almD1NAVSF5p11Bct);
almD1NAVSF5p11B->setNavID(gpstk::NavType::BeiDou_D1);
almD1NAVSF5p11B->addUnsignedLong(0x38905548,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x1D102F08,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x0AE644AF,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x3FD0F105,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x33DF1228,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x21036F3D,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x3BECB17B,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x3FCA2154,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x21B581CB,30,1);
almD1NAVSF5p11B->addUnsignedLong(0x28328DC7,30,1);
almD1NAVSF5p11B->trimsize();
