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
/** @file GalINavTestDataDecl.hpp This file contains data declarations
 * for testing Galileo I/NAV processing in the NavFactory code. */

gnsstk::ObsID oidINAVGal;
gnsstk::PackedNavBitsPtr ephINAVGalWT1,
   ephINAVGalWT2,
   ephINAVGalWT3,
   ephINAVGalWT4,
   ephINAVGalWT5,
   navINAVGalWT6,
   navINAVGalWT7,
   navINAVGalWT8,
   navINAVGalWT9,
   navINAVGalWT10;
   // Page pair versions
gnsstk::PackedNavBitsPtr ephINAVGalPP1,
   ephINAVGalPP2,
   ephINAVGalPP3,
   ephINAVGalPP4,
   ephINAVGalPP5;
gnsstk::CommonTime ephINAVGalWT1ct,
   ephINAVGalWT2ct,
   ephINAVGalWT3ct,
   ephINAVGalWT4ct,
   ephINAVGalWT5ct,
   navINAVGalWT6ct,
   navINAVGalWT7ct,
   navINAVGalWT8ct,
   navINAVGalWT9ct,
   navINAVGalWT10ct;
gnsstk::SatID ephINAVGalSid;

// week rollover test data
gnsstk::ObsID oidINAVWR;
gnsstk::SatID ephINAVWRsid;
gnsstk::CommonTime navINAVWRWT1ct, navINAVWRWT2ct, navINAVWRWT3ct,
   navINAVWRWT4ct, navINAVWRWT5ct;
gnsstk::PackedNavBitsPtr navINAVWRWT1, navINAVWRWT2, navINAVWRWT3,
   navINAVWRWT4, navINAVWRWT5;
