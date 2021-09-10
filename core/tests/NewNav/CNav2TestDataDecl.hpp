//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

gnsstk::ObsID oidCNAV2GPS;
gnsstk::SatID ephCNAV2GPSsid;
gnsstk::CommonTime sf123p1CNAV2GPSct,  sf123p2CNAV2GPSct,  sf123p4CNAV2GPSct, sf2CNAV2GPSct, sf2CNAV2GPS2ct, sf3p1CNAV2GPSct, sf3p2CNAV2GPSct, sf3p2fakeCNAV2GPSct, sf3p4CNAV2GPSct;
gnsstk::PackedNavBitsPtr sf123p1CNAV2GPS,  sf123p2CNAV2GPS,  sf123p4CNAV2GPS, sf2CNAV2GPS, sf2CNAV2GPS2, sf3p1CNAV2GPS, sf3p2CNAV2GPS, sf3p2fakeCNAV2GPS, sf3p4CNAV2GPS;

gnsstk::ObsID oidCNAV2QZSS;
gnsstk::SatID ephCNAV2QZSSsid;
gnsstk::CommonTime sf2CNAV2QZSSct, sf3p1CNAV2QZSSct, sf3p2CNAV2QZSSct, sf3p4CNAV2QZSSct;
gnsstk::PackedNavBitsPtr sf2CNAV2QZSS, sf3p1CNAV2QZSS, sf3p2CNAV2QZSS, sf3p4CNAV2QZSS;
