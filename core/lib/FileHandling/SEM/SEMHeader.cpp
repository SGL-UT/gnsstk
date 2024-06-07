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

/**
 * @file SEMHeader.cpp
 * Encapsulate header of SEM file data, including I/O
 */

#include "StringUtils.hpp"
#include "GNSSconstants.hpp"

#include "SEMHeader.hpp"
#include "SEMStream.hpp"


using namespace gnsstk::StringUtils;
using namespace std;

namespace gnsstk
{
   short SEMHeader::nearFullWeek = 0;

   namespace detail
   {
      int32_t getDisAmbiguatedNearbyWeek(int32_t refEpochInWeeks, int32_t almanacWeek)
      {
         // No reference epoch passed. Just return the input almanacWeek.
         if (refEpochInWeeks <= 0)
            return almanacWeek;

         // If already disambiguated, don't bother disambiguating. The user made an effort to 
         // disambiguate using a trusted resource. So, just honor that.
         // The disamgiguation algorithm can afterall yield something other than the input
         // almancWeek.
         if (almanacWeek > GPS_WEEK_PER_EPOCH - 1)
            return almanacWeek;

         // Algorithm:
         // Compute the disambiguated GPS week that is closest (nearby) to the input almanacWeek.
         // Note that the disambiguated GPS week must be centered around both sides of the 
         // refEpochInWeeks and this modulo arithmetic guarantees that.
         int32_t moduloWeekDifference = std::remainder(almanacWeek - refEpochInWeeks, GPS_WEEK_PER_EPOCH);
         almanacWeek = refEpochInWeeks + moduloWeekDifference;

         // As the modulo arithmetic can yield negative numbers for low values of refEpochInWeeks and
         // negative resolved numbers are meaningless, 
         // offset by adding GPS_WEEK_PER_EPOCH as a special case. This will give the same value as
         // input almanacWeek
         if (almanacWeek < 0)
            almanacWeek += GPS_WEEK_PER_EPOCH;

         return almanacWeek;
      }
   }

   void SEMHeader::reallyPutRecord(FFStream& ffs) const
   {
      string line;

      SEMStream& strm = dynamic_cast<SEMStream&>(ffs);

      line = leftJustify(asString<short>(numRecords),2);
      line += " ";
      line += Title;
      strm << line << endl;
      line.erase();

      line = rightJustify(asString<short>(week),4);
      line += " ";
      line += asString<long>(Toa);
      strm << line << endl;
      line.erase();

   }   // end SEMAHeader::reallyPutRecord


   void SEMHeader::reallyGetRecord(FFStream& ffs)
   {
      string line;

      SEMStream& strm = dynamic_cast<SEMStream&>(ffs);

      //Grab the first line
      strm.formattedGetLine(line);
      if (line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ .-+") != std::string::npos ||
          line.length() < 4 || line.length() > 28)
      {
         FFStreamError fe("Invalid data");
         GNSSTK_THROW(fe);
      }

      numRecords = (short) asInt(line.substr(0,2));
      Title = line.substr(3,24);

      //Grab the second line
      strm.formattedGetLine(line);
      if (line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ .-+") != std::string::npos ||
          line.length() < 6 || line.length() > 12)
      {
         FFStreamError fe("Invalid data");
         GNSSTK_THROW(fe);
      }
      vector<std::string> values = split(line,' ');
      if (values.size()!=2)
      {
         FFStreamError fe("Invalid data");
         GNSSTK_THROW(fe);
      }
      week = (short) asInt(values[0]);
      Toa = asInt(values[1]);

      if (nearReferenceEpochInWeeks > 0) 
      {
         week = detail::getDisAmbiguatedNearbyWeek(nearReferenceEpochInWeeks, week);
      }
      else if (nearFullWeek > 0)
      {
         week = detail::getDisAmbiguatedNearbyWeek(nearFullWeek, week);
      }

      strm.header = *this;
      strm.headerRead = true;

   } // end of reallyGetRecord()

   void SEMHeader::dump(ostream& s) const
   {
      std::cout << "numRecords = " << numRecords << std::endl;
      std::cout << "Title = " << Title << std::endl;
      std::cout << "week = " << week << std::endl;
      std::cout << "Toa = " << Toa << std::endl;
   }

} // namespace
