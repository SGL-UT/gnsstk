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
/** @file GLOCNavTestDataDecl.hpp This file contains data declarations
 * for testing GLONASS CDMA NAV processing in the NavFactory code. */

gnsstk::ObsID oidGLOC;
gnsstk::PackedNavBitsPtr navGLOCStr10a,
   navGLOCStr11a,
   navGLOCStr12a,
   navGLOCStr20,
   navGLOCStr25,
   navGLOCStr31,
   navGLOCStr32,
   navGLOCStr10b,
   navGLOCStr11b,
   navGLOCStr12b;
gnsstk::CommonTime navGLOCStr10act,
   navGLOCStr11act,
   navGLOCStr12act,
   navGLOCStr20ct,
   navGLOCStr25ct,
   navGLOCStr31ct,
   navGLOCStr32ct,
   navGLOCStr10bct,
   navGLOCStr11bct,
   navGLOCStr12bct;
gnsstk::SatID navGLOCSid;
