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
/** @file CNav2TestDataDecl.hpp This file contains data declarations
 * for testing GPS and QZSS CNAV2 processing in the NavFactory code. */

gpstk::ObsID oidCNAV2GPS;
gpstk::SatID ephCNAV2GPSsid;
gpstk::CommonTime sf2CNAV2GPSct, sf3p1CNAV2GPSct, sf3p2CNAV2GPSct, sf3p2fakeCNAV2GPSct, sf3p4CNAV2GPSct;
gpstk::PackedNavBitsPtr sf2CNAV2GPS, sf3p1CNAV2GPS, sf3p2CNAV2GPS, sf3p2fakeCNAV2GPS, sf3p4CNAV2GPS;

gpstk::ObsID oidCNAV2QZSS;
gpstk::SatID ephCNAV2QZSSsid;
gpstk::CommonTime sf2CNAV2QZSSct, sf3p1CNAV2QZSSct, sf3p2CNAV2QZSSct, sf3p4CNAV2QZSSct;
gpstk::PackedNavBitsPtr sf2CNAV2QZSS, sf3p1CNAV2QZSS, sf3p2CNAV2QZSS, sf3p4CNAV2QZSS;
