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

/// @file JulianDate.cpp

#include <cmath>
#include "JulianDate.hpp"
#include "TimeConstants.hpp"

namespace gnsstk
{
   JulianDate& JulianDate::operator=( const JulianDate& right )
   {
      jd = right.jd;
      timeSystem = right.timeSystem;
      return *this;
   }

   CommonTime JulianDate::convertToCommonTime() const
   {
      try
      {
         long double temp_jd( jd + 0.5 );
         long jday( static_cast<long>( temp_jd ) );
         long double sod =
            ( temp_jd - static_cast<long double>( jday ) ) * SEC_PER_DAY;

         CommonTime ct;
         return ct.set( jday,
                        static_cast<long>( sod ),
                        static_cast<double>( sod - static_cast<long>( sod ) ),
                        timeSystem );
      }
      catch (InvalidParameter& ip)
      {
         InvalidRequest ir(ip);
         GNSSTK_THROW(ir);
      }
   }

   void JulianDate::convertFromCommonTime( const CommonTime& ct )
   {
      long jday, sod;
      double fsod;
      ct.get( jday, sod, fsod, timeSystem );

      jd =   static_cast<long double>( jday ) +
           (   static_cast<long double>( sod )
             + static_cast<long double>( fsod ) ) * DAY_PER_SEC
           - 0.5;
   }

   std::string JulianDate::printf( const std::string& fmt ) const
   {
      try
      {
         using gnsstk::StringUtils::formattedPrint;
         std::string rv( fmt );

         rv = formattedPrint( rv, getFormatPrefixFloat() + "J",
                              "JLf", jd );
         rv = formattedPrint( rv, getFormatPrefixInt() + "P",
                              "Ps", StringUtils::asString(timeSystem).c_str() );
         return rv;
      }
      catch( gnsstk::StringUtils::StringException& se )
      {
         GNSSTK_RETHROW( se );
      }
   }

   std::string JulianDate::printError( const std::string& fmt ) const
   {
      try
      {
         using gnsstk::StringUtils::formattedPrint;
         std::string rv( fmt );

         rv = formattedPrint( rv, getFormatPrefixFloat() + "J",
                              "Js", getError().c_str() );
         rv = formattedPrint( rv, getFormatPrefixInt() + "P",
                              "Ps", getError().c_str() );
         return rv;
      }
      catch( gnsstk::StringUtils::StringException& se )
      {
         GNSSTK_RETHROW( se );
      }
   }

   bool JulianDate::setFromInfo( const IdToValue& info )
   {
      using namespace gnsstk::StringUtils;

      for( IdToValue::const_iterator i = info.begin(); i != info.end(); i++ )
      {
         switch( i->first )
         {
            case 'J':
               jd = asLongDouble( i->second );
               break;

            case 'P':
               timeSystem = gnsstk::StringUtils::asTimeSystem(i->second);
               break;

            default:
                  // do nothing
               break;
         };
      }

      return true;
   }

   bool JulianDate::isValid() const
   {
      JulianDate temp;
      temp.convertFromCommonTime( convertToCommonTime() );
      if( *this == temp )
      {
         return true;
      }
      return false;
   }

   void JulianDate::reset()
   {
      jd = 0.0;
      timeSystem = TimeSystem::Unknown;
   }

   bool JulianDate::operator==( const JulianDate& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
         return false;

      if( std::abs(jd - right.jd) < CommonTime::eps )
      {
         return true;
      }
      return false;
   }

   bool JulianDate::operator!=( const JulianDate& right ) const
   {
      return ( !operator==( right ) );
   }

   bool JulianDate::operator<( const JulianDate& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
      {
         gnsstk::InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
         GNSSTK_THROW(ir);
      }

      if( jd < right.jd )
      {
         return true;
      }
      return false;
   }

   bool JulianDate::operator>( const JulianDate& right ) const
   {
      return ( !operator<=( right ) );
   }

   bool JulianDate::operator<=( const JulianDate& right ) const
   {
      return ( operator<( right ) ||
               operator==( right ) );
   }

   bool JulianDate::operator>=( const JulianDate& right ) const
   {
      return ( !operator<( right ) );
   }

} // namespace
