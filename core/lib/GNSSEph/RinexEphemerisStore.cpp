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

/**
 * @file RinexEphemerisStore.cpp
 * Read & store Rinex formated ephemeris data
 */

#include "RinexEphemerisStore.hpp"
#include "MiscMath.hpp"
#include "GNSSconstants.hpp"
#include <iostream>
#include <fstream>

using namespace gnsstk::StringUtils;
using namespace std;
namespace gnsstk
{
   //-----------------------------------------------------------------------------
   //-----------------------------------------------------------------------------
   void RinexEphemerisStore::loadFile(const std::string& filename)
   {
      try
      {
         RinexNavStream strm(filename.c_str());
         if (!strm)
         {
            FileMissingException e("File " + filename + " could not be opened.");
            GNSSTK_THROW(e);
         }

         RinexNavHeader header;
         strm >> header;

         addFile(filename, header);

         RinexNavData rec;
         while(strm >> rec)
            addEphemeris(rec);
      }
      catch (gnsstk::Exception& e)
      {
	GNSSTK_RETHROW(e);
      }
   }  // end RinexEphemerisStore::load


   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   void RinexEphemerisStore::dump(std::ostream& s, short detail)
      const throw()
   {
      s << "Dump of RinexEphemerisStore:" << std::endl;
      std::vector<std::string> fileNames = getFileNames();
      std::vector<std::string>::const_iterator f=fileNames.begin();
      for (f=fileNames.begin(); f!=fileNames.end(); f++)
         s << *f << std::endl;

      GPSEphemerisStore::dump(s, detail);

   } // end of RinexEphemerisStore::dump

}  // namespace gnsstk
