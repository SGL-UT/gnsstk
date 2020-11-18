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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
/** @file LNavTestDataDecl.hpp This file contains data declarations
 * for testing GPS LNav processing in the NavFactory code. */

gpstk::ObsID oidLNAVGPS;
gpstk::PackedNavBitsPtr ephLNAVGPSSF1, ephLNAVGPSSF2, ephLNAVGPSSF3, almLNAVGPS25, almLNAVGPS26, pg51LNAVGPS, pg56LNAVGPS, pg63LNAVGPS;
gpstk::CommonTime ephLNAVGPSSF1ct, ephLNAVGPSSF2ct, ephLNAVGPSSF3ct, almLNAVGPS25ct, almLNAVGPS26ct, pg51LNAVGPSct, pg56LNAVGPSct, pg63LNAVGPSct;
gpstk::SatID ephLNAVGPSSid, almLNAVGPSSid;
