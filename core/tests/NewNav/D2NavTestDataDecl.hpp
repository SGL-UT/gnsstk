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
/** @file D2NavTestDataDecl.hpp This file contains data declarations
 * for testing BeiDou D2Nav processing in the NavFactory code. */

gnsstk::ObsID oidD2NAV;
gnsstk::PackedNavBitsPtr navD2SF1p001, navD2SF2p001, navD2SF3p001,
   navD2SF4p001, navD2SF5p001, navD2SF1p002, navD2SF2p002,
   navD2SF3p002, navD2SF4p002, navD2SF5p002, navD2SF1p003,
   navD2SF2p003, navD2SF3p003, navD2SF4p003, navD2SF5p003,
   navD2SF1p004, navD2SF2p004, navD2SF3p004, navD2SF4p004,
   navD2SF5p004, navD2SF1p005, navD2SF2p005, navD2SF3p005,
   navD2SF4p005, navD2SF5p005, navD2SF1p006, navD2SF2p006,
   navD2SF3p006, navD2SF4p006, navD2SF5p006, navD2SF1p007,
   navD2SF5p007, navD2SF1p008, navD2SF5p008, navD2SF1p009,
   navD2SF5p009, navD2SF1p010, navD2SF5p010, navD2SF5p011,
   navD2SF5p012, navD2SF5p013, navD2SF5p014, navD2SF5p015,
   navD2SF5p016, navD2SF5p017, navD2SF5p018, navD2SF5p019,
   navD2SF5p020, navD2SF5p021, navD2SF5p022, navD2SF5p023,
   navD2SF5p024, navD2SF5p025, navD2SF5p026, navD2SF5p027,
   navD2SF5p028, navD2SF5p029, navD2SF5p030, navD2SF5p031,
   navD2SF5p032, navD2SF5p033, navD2SF5p034, navD2SF5p035,
   navD2SF5p036, navD2SF5p037, navD2SF5p038, navD2SF5p039,
   navD2SF5p040, navD2SF5p041, navD2SF5p042, navD2SF5p043,
   navD2SF5p044, navD2SF5p045, navD2SF5p046, navD2SF5p047,
   navD2SF5p048, navD2SF5p049, navD2SF5p050, navD2SF5p051,
   navD2SF5p052, navD2SF5p053, navD2SF5p054, navD2SF5p055,
   navD2SF5p056, navD2SF5p057, navD2SF5p058, navD2SF5p059,
   navD2SF5p060, navD2SF5p061, navD2SF5p062, navD2SF5p063,
   navD2SF5p064, navD2SF5p065, navD2SF5p066, navD2SF5p067,
   navD2SF5p068, navD2SF5p069, navD2SF5p070, navD2SF5p071,
   navD2SF5p072, navD2SF5p073, navD2SF5p074, navD2SF5p075,
   navD2SF5p076, navD2SF5p077, navD2SF5p078, navD2SF5p079,
   navD2SF5p080, navD2SF5p081, navD2SF5p082, navD2SF5p083,
   navD2SF5p084, navD2SF5p085, navD2SF5p086, navD2SF5p087,
   navD2SF5p088, navD2SF5p089, navD2SF5p090, navD2SF5p091,
   navD2SF5p092, navD2SF5p093, navD2SF5p094, navD2SF5p095,
   navD2SF5p096, navD2SF5p097, navD2SF5p098, navD2SF5p099,
   navD2SF5p100, navD2SF5p101, navD2SF5p102, navD2SF5p103,
   navD2SF5p104, navD2SF5p105, navD2SF5p106, navD2SF5p107,
   navD2SF5p108, navD2SF5p109, navD2SF5p110, navD2SF5p111,
   navD2SF5p112, navD2SF5p113, navD2SF5p114, navD2SF5p115,
   navD2SF5p116, navD2SF5p117, navD2SF5p118, navD2SF5p119,
   navD2SF5p120;
gnsstk::CommonTime navD2SF1p001ct, navD2SF2p001ct, navD2SF3p001ct,
   navD2SF4p001ct, navD2SF5p001ct, navD2SF1p002ct, navD2SF2p002ct,
   navD2SF3p002ct, navD2SF4p002ct, navD2SF5p002ct, navD2SF1p003ct,
   navD2SF2p003ct, navD2SF3p003ct, navD2SF4p003ct, navD2SF5p003ct,
   navD2SF1p004ct, navD2SF2p004ct, navD2SF3p004ct, navD2SF4p004ct,
   navD2SF5p004ct, navD2SF1p005ct, navD2SF2p005ct, navD2SF3p005ct,
   navD2SF4p005ct, navD2SF5p005ct, navD2SF1p006ct, navD2SF2p006ct,
   navD2SF3p006ct, navD2SF4p006ct, navD2SF5p006ct, navD2SF1p007ct,
   navD2SF5p007ct, navD2SF1p008ct, navD2SF5p008ct, navD2SF1p009ct,
   navD2SF5p009ct, navD2SF1p010ct, navD2SF5p010ct, navD2SF5p011ct,
   navD2SF5p012ct, navD2SF5p013ct, navD2SF5p014ct, navD2SF5p015ct,
   navD2SF5p016ct, navD2SF5p017ct, navD2SF5p018ct, navD2SF5p019ct,
   navD2SF5p020ct, navD2SF5p021ct, navD2SF5p022ct, navD2SF5p023ct,
   navD2SF5p024ct, navD2SF5p025ct, navD2SF5p026ct, navD2SF5p027ct,
   navD2SF5p028ct, navD2SF5p029ct, navD2SF5p030ct, navD2SF5p031ct,
   navD2SF5p032ct, navD2SF5p033ct, navD2SF5p034ct, navD2SF5p035ct,
   navD2SF5p036ct, navD2SF5p037ct, navD2SF5p038ct, navD2SF5p039ct,
   navD2SF5p040ct, navD2SF5p041ct, navD2SF5p042ct, navD2SF5p043ct,
   navD2SF5p044ct, navD2SF5p045ct, navD2SF5p046ct, navD2SF5p047ct,
   navD2SF5p048ct, navD2SF5p049ct, navD2SF5p050ct, navD2SF5p051ct,
   navD2SF5p052ct, navD2SF5p053ct, navD2SF5p054ct, navD2SF5p055ct,
   navD2SF5p056ct, navD2SF5p057ct, navD2SF5p058ct, navD2SF5p059ct,
   navD2SF5p060ct, navD2SF5p061ct, navD2SF5p062ct, navD2SF5p063ct,
   navD2SF5p064ct, navD2SF5p065ct, navD2SF5p066ct, navD2SF5p067ct,
   navD2SF5p068ct, navD2SF5p069ct, navD2SF5p070ct, navD2SF5p071ct,
   navD2SF5p072ct, navD2SF5p073ct, navD2SF5p074ct, navD2SF5p075ct,
   navD2SF5p076ct, navD2SF5p077ct, navD2SF5p078ct, navD2SF5p079ct,
   navD2SF5p080ct, navD2SF5p081ct, navD2SF5p082ct, navD2SF5p083ct,
   navD2SF5p084ct, navD2SF5p085ct, navD2SF5p086ct, navD2SF5p087ct,
   navD2SF5p088ct, navD2SF5p089ct, navD2SF5p090ct, navD2SF5p091ct,
   navD2SF5p092ct, navD2SF5p093ct, navD2SF5p094ct, navD2SF5p095ct,
   navD2SF5p096ct, navD2SF5p097ct, navD2SF5p098ct, navD2SF5p099ct,
   navD2SF5p100ct, navD2SF5p101ct, navD2SF5p102ct, navD2SF5p103ct,
   navD2SF5p104ct, navD2SF5p105ct, navD2SF5p106ct, navD2SF5p107ct,
   navD2SF5p108ct, navD2SF5p109ct, navD2SF5p110ct, navD2SF5p111ct,
   navD2SF5p112ct, navD2SF5p113ct, navD2SF5p114ct, navD2SF5p115ct,
   navD2SF5p116ct, navD2SF5p117ct, navD2SF5p118ct, navD2SF5p119ct,
   navD2SF5p120ct;
gnsstk::SatID navD2Sid;
