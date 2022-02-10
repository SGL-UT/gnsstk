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
/** @file CNavTestDataDecl.hpp This file contains data declarations
 * for testing GPS CNav processing in the NavFactory code. */

gnsstk::ObsID oidCNAVGPSL2;
gnsstk::SatID ephCNAVGPSL2sid;
gnsstk::CommonTime msg10CNAVGPSL2ct, msg11CNAVGPSL2ct, msg30CNAVGPSL2ct, msg32CNAVGPSL2ct, msg33CNAVGPSL2ct;
gnsstk::PackedNavBitsPtr msg10CNAVGPSL2, msg11CNAVGPSL2, msg30CNAVGPSL2, msg32CNAVGPSL2, msg33CNAVGPSL2;

gnsstk::ObsID oidCNAVQZSSL5;
gnsstk::SatID ephCNAVQZSSL5sid;
gnsstk::CommonTime msg10CNAVQZSSL5ct, msg11CNAVQZSSL5ct, msg12CNAVQZSSL5ct, msg30CNAVQZSSL5ct, msg31CNAVQZSSL5ct, msg32CNAVQZSSL5ct, msg33CNAVQZSSL5ct, msg35CNAVQZSSL5ct, msg37CNAVQZSSL5ct;
gnsstk::PackedNavBitsPtr msg10CNAVQZSSL5, msg11CNAVQZSSL5, msg12CNAVQZSSL5, msg30CNAVQZSSL5, msg31CNAVQZSSL5, msg32CNAVQZSSL5, msg33CNAVQZSSL5, msg35CNAVQZSSL5, msg37CNAVQZSSL5;
