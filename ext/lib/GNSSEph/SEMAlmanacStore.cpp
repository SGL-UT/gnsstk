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
 * @file SEMAlmanacStore.cpp
 * Store SEM almanac information, and access by satellite and time
 */

#include "SEMAlmanacStore.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   //----------------------------------------------------------------
   void SEMAlmanacStore::loadFile(const std::string& filename)
   {
      try
      {
         SEMStream strm(filename.c_str());
         if (!strm)
         {
            FileMissingException e("File " + filename + " could not be opened.");
            GNSSTK_THROW(e);
         }

         SEMHeader header;
         strm >> header;

            // If the user has indcated a time of interest and
            // the reference week number is less than 10 bits long,
            // assume the almanac must be within 511 weeks of the
            // time of interest
            // If necessary, adjust the GPS week number
            //
            // NOTE: According to the SEM format documetation on the
            // USCG NAVCEN website, the week in the header should be
            // 0-1023; however there is anecdotal evidence that some
            // organizations have used the full GPS week number.
            // This is an attempt to "do the right thing" in the
            // broadest number of cases.
         if (timeOfInterest>gnsstk::CommonTime::BEGINNING_OF_TIME &&
             header.week < 1024)
         {
            short diff = static_cast<GPSWeekSecond>(timeOfInterest).week - header.week;
            short nEpochs = (diff+512) / 1024;
            header.week += nEpochs * 1024;
         }
         addFile(filename, header);

         SEMData rec;
         while(strm >> rec)
	      {
               //This is a fix to get the header and the data to share Toa and week.
	         rec.Toa = header.Toa;
	         rec.week = header.week;

            OrbAlmGen oag = OrbAlmGen(rec);
            addOrbAlm(&(oag));
         }
      }
      catch (Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }

   //----------------------------------------------------------------
   bool SEMAlmanacStore::addAlmanac(const SEMData& sAlmData)
   {
      unsigned short retVal = OrbAlmStore::ADD_NEITHER;
      try
      {
         OrbAlmGen oag = OrbAlmGen(sAlmData);
         retVal = addOrbAlm(&oag);
      }
      catch (InvalidParameter ip)
      {
         GNSSTK_RETHROW(ip);
      }
      catch (Exception exc)
      {
         GNSSTK_RETHROW(exc);
      }
      if (retVal==OrbAlmStore::ADD_NEITHER)
         return false;
      return true;
   }

}
