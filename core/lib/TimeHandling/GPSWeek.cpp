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

/// @file GPSWeek.cpp

#include "GPSWeek.hpp"
#include "TimeConstants.hpp"

namespace gnsstk
{
   const int GPSWeek::MAX_WEEK=(CommonTime::END_LIMIT_JDAY-MJD_JDAY-GPS_EPOCH_MJD)/7;

   GPSWeek& GPSWeek::operator=(const GPSWeek& right)
   {
      week = right.week;
      timeSystem = right.timeSystem;
      return *this;
   }

   std::string GPSWeek::printf( const std::string& fmt ) const
   {
      try
      {
         using gnsstk::StringUtils::formattedPrint;
         std::string rv = fmt;

         rv = formattedPrint( rv, getFormatPrefixInt() + "E",
                              "Eu", getEpoch() );
         rv = formattedPrint( rv, getFormatPrefixInt() + "F",
                              "Fu", week );
         rv = formattedPrint( rv, getFormatPrefixInt() + "G",
                              "Gu", getWeek10() );
         rv = formattedPrint( rv, getFormatPrefixInt() + "P",
                              "Ps", StringUtils::asString(timeSystem).c_str() );
         return rv;
      }
      catch( gnsstk::StringUtils::StringException& exc )
      {
         GNSSTK_RETHROW( exc );
      }
   }

   std::string GPSWeek::printError( const std::string& fmt ) const
   {
      try
      {
         using gnsstk::StringUtils::formattedPrint;
         std::string rv = fmt;

         rv = formattedPrint( rv, getFormatPrefixInt() + "E",
                              "Es", getError().c_str() );
         rv = formattedPrint( rv, getFormatPrefixInt() + "F",
                              "Fs", getError().c_str() );
         rv = formattedPrint( rv, getFormatPrefixInt() + "G",
                              "Gs", getError().c_str() );
         rv = formattedPrint( rv, getFormatPrefixInt() + "P",
                              "Ps", getError().c_str() );
         return rv;
      }
      catch( gnsstk::StringUtils::StringException& exc )
      {
         GNSSTK_RETHROW( exc );
      }
   }

      /**
       * Set this object using the information provided in \a info.
       * @param info the IdToValue object to which this object shall be set.
       * @return true if this object was successfully set using the
       *  data in \a info, false if not.
       */
   bool GPSWeek::setFromInfo( const IdToValue& info )
   {
      using namespace gnsstk::StringUtils;

      for( IdToValue::const_iterator i = info.begin(); i != info.end(); i++ )
      {
            // based on the character, we know what to do...
         switch ( i->first )
         {
            case 'E':
               setEpoch( asInt( i->second ) );
               break;
            case 'F':
               week = asInt( i->second );
               break;
            case 'G':
               setWeek10( asInt( i->second ) );
               break;
            case 'P':
               timeSystem = gnsstk::StringUtils::asTimeSystem(i->second);
               break;
            default:
                  // do nothing
               break;
         };

      } // end of for loop

      return true;
   }

} // namespace
