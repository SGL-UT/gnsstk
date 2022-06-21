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
/** @file D1NavTestDataDecl.hpp This file contains data declarations
 * for testing BeiDou D1Nav processing in the NavFactory code. */

gnsstk::ObsID oidD1NAV;
gnsstk::PackedNavBitsPtr ephD1NAVSF1, ephD1NAVSF2, ephD1NAVSF3,
   almD1NAVSF4p1, almD1NAVSF5p1,
   almD1NAVSF4p7, almD1NAVSF5p7,
   almD1NAVSF4p8, almD1NAVSF5p8,
   almD1NAVSF4p9, almD1NAVSF5p9,
// note that these two are synthesized and wouldn't pass a parity test
   almD1NAVSF5p9A0, almD1NAVSF5p9A1,
   almD1NAVSF4p10, almD1NAVSF5p10,
// note that these two are synthesized and wouldn't pass a parity test
   almD1NAVSF5p10ZZ, almD1NAVSF5p10A1,
   almD1NAVSF4p11, almD1NAVSF5p11,
   almD1NAVSF4p24B, almD1NAVSF5p24B,
   almD1NAVSF4p8B, almD1NAVSF5p8B,
   almD1NAVSF4p11B, almD1NAVSF5p11B,
   almD1NAVSF4p12B, almD1NAVSF5p12B,
   almD1NAVSF4p7B, almD1NAVSF5p7B,
   almD1NAVSF4p24B2, almD1NAVSF5p24B2,
   almD1Default1, almD1Default2, almD1Default3;
gnsstk::CommonTime ephD1NAVSF1ct, ephD1NAVSF2ct, ephD1NAVSF3ct,
   almD1NAVSF4p1ct, almD1NAVSF5p1ct,
   almD1NAVSF4p7ct, almD1NAVSF5p7ct,
   almD1NAVSF4p8ct, almD1NAVSF5p8ct,
   almD1NAVSF4p9ct, almD1NAVSF5p9ct,
   almD1NAVSF4p10ct, almD1NAVSF5p10ct,
   almD1NAVSF4p11ct, almD1NAVSF5p11ct,
   almD1NAVSF4p24Bct, almD1NAVSF5p24Bct,
   almD1NAVSF4p8Bct, almD1NAVSF5p8Bct,
   almD1NAVSF4p11Bct, almD1NAVSF5p11Bct,
   almD1NAVSF4p12Bct, almD1NAVSF5p12Bct,
   almD1NAVSF4p7Bct, almD1NAVSF5p7Bct,
   almD1NAVSF4p24B2ct, almD1NAVSF5p24B2ct,
   almD1Default123ct;
gnsstk::SatID ephD1NAVSid, almD1NAVSid, almD1NAVSidB;
