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
 * @file GPSZcount.cpp
 * gnsstk::GPSZcount - encapsulates time according to full GPS Week and Z-count.
 */

#include <limits>

#include "GPSZcount.hpp"
#include "StringUtils.hpp"

using namespace std;
using gnsstk::StringUtils::asString;

namespace gnsstk
{
   const long GPSZcount::ZCOUNT_MINUTE = 40;
   const long GPSZcount::ZCOUNT_HOUR = 2400;
   const long GPSZcount::ZCOUNT_DAY = 57600;
   const long GPSZcount::ZCOUNT_WEEK = 403200;

   GPSZcount::GPSZcount(short inWeek,
                        long inZcount)
   {
      try
      {
         setWeek(inWeek);
         setZcount(inZcount);
      }
      catch(gnsstk::InvalidParameter& ip)
      {
         GNSSTK_RETHROW(ip);
      }
   }

   GPSZcount::GPSZcount(long inFullZcount)
   {
      try
      {
         setFullZcount(inFullZcount);
      }
      catch(gnsstk::InvalidParameter& ip)
      {
         GNSSTK_RETHROW(ip);
      }
   }

   GPSZcount::GPSZcount(const GPSZcount& right)
   {
      operator=(right);
   }

   GPSZcount& GPSZcount::setWeek(short inWeek)
   {
      if( inWeek < 0 )
      {
         gnsstk::InvalidParameter ip("GPS Week invalid: " +
                                    asString<short>(inWeek)) ;
         GNSSTK_THROW(ip);
      }
      week = inWeek;
      return *this;
   }

   GPSZcount& GPSZcount::setZcount(long inZcount)
   {
      if(validZcount(inZcount) != 0)
      {
         gnsstk::InvalidParameter ip("GPS Z-count invalid: " +
                                    asString<long>(inZcount));
         GNSSTK_THROW(ip);
      }
      zcount = inZcount;
      return *this;
   }

   GPSZcount& GPSZcount::setFullZcount(long inZcount)
   {
      try
      {
         setZcount(inZcount & 0x7FFFFL);        // 19-bit mask

         // A 10-bit value will always be within constraints for a GPS week, so
         // there's no need to test it.
         setWeek((inZcount >> 19) & 0x3FFL);   // 10-bit mask

      }
      catch(gnsstk::InvalidParameter& ip)
      {
         ip.addText("GPS Full Z-count invalid: " + asString<long>(inZcount)) ;
         GNSSTK_RETHROW(ip);
      }
      return *this;
   }

   GPSZcount& GPSZcount::addWeeks(short inWeeks)
   {
      if (inWeeks == 0)
      {
         return *this;
      }

      try
      {
         return setWeek(week + inWeeks);
      }
      catch(gnsstk::InvalidParameter& ip)
      {
         gnsstk::InvalidRequest ir(ip) ;
         ir.addText("Addition of " + asString(inWeeks) +
                    " weeks renders this object invalid.");
         GNSSTK_THROW(ir);
      }
   }

   GPSZcount& GPSZcount::addZcounts(long inZcounts)
   {
      if (inZcounts == 0)
      {
         return *this;
      }

      short originalWeek(week);
      long originalZcount(zcount);

      try
      {
            // First, do week modifications.
         addWeeks(inZcounts / ZCOUNT_WEEK);

            // Now, take care of Z-counts.
         long tmp = zcount + (inZcounts % ZCOUNT_WEEK);

         if (tmp < 0)
         {
            addWeeks(-1);
            tmp += ZCOUNT_WEEK;
         }
         else if (tmp >= ZCOUNT_WEEK)
         {
            addWeeks(1);
            tmp -= ZCOUNT_WEEK;
         }

         setZcount(tmp);
         return *this;

      }
      catch(gnsstk::InvalidRequest& ir)
      {
         setWeek(originalWeek);
         setZcount(originalZcount);
         ir.addText("Did not add " + asString(inZcounts) + " Z-counts.") ;
         GNSSTK_RETHROW(ir);

      }
      catch(gnsstk::InvalidParameter& ip)
      {
         setWeek(originalWeek);
         setZcount(originalZcount);
         gnsstk::InvalidRequest ir(ip);
         ir.addText("Did not add " + asString(inZcounts) + " Z-counts.") ;
         GNSSTK_THROW(ir);
      }
   }

   GPSZcount GPSZcount::operator++(int)
   {
      GPSZcount temp = *this;
      ++(*this);
      return temp;
   }

   GPSZcount& GPSZcount::operator++()
   {
      return addZcounts(1);
   }

   GPSZcount GPSZcount::operator--(int)
   {
      GPSZcount temp = *this;
      --(*this);
      return temp ;
   }

   GPSZcount& GPSZcount::operator--()
   {
      return addZcounts(-1);
   }

   GPSZcount GPSZcount::operator+(long inZcounts) const
   {
      return GPSZcount(*this).addZcounts(inZcounts);
   }

   GPSZcount GPSZcount::operator-(long inZcounts) const
   {
      return operator+(-inZcounts);
   }

   double GPSZcount::operator-(const GPSZcount& right) const
   {
      return (double(week) - right.week) * ZCOUNT_WEEK
         + (zcount - right.zcount) ;
   }

   long GPSZcount::operator%(const long right) const
   {
      return zcount % right;
   }

   GPSZcount& GPSZcount::operator+=(long inZcounts)
   {
      return addZcounts(inZcounts);
   }

   GPSZcount& GPSZcount::operator-=(long inZcounts)
   {
      return addZcounts(-inZcounts);
   }

   GPSZcount& GPSZcount::operator=(const GPSZcount& right)
   {
      week = right.week;
      zcount = right.zcount;
      return *this;
   }

   bool GPSZcount::operator<(const GPSZcount& right) const
   {
      if (week < right.week)
      {
         return true;
      }
      if (week == right.week &&
          zcount < right.zcount)
      {
         return true;
      }
      return false;
   }

   bool GPSZcount::operator>(const GPSZcount& right) const
   {
      if (week > right.week)
      {
         return true;
      }
      if (week == right.week &&
          zcount > right.zcount)
      {
         return true;
      }
      return false;
   }

   bool GPSZcount::operator==(const GPSZcount& right) const
   {
      if (week == right.week &&
          zcount == right.zcount)
      {
         return true;
      }
      return false;
   }

   bool GPSZcount::operator!=(const GPSZcount& right) const
   {
      return (! operator==(right));
   }

   bool GPSZcount::operator<=(const GPSZcount& right) const
   {
      return (! operator>(right));
   }

   bool GPSZcount::operator>=(const GPSZcount& right) const
   {
      return (! operator<(right));
   }

   GPSZcount::operator std::string() const
   {
      return asString<short>(week) + "w" + asString<long>(zcount) + "z";
   }

   bool GPSZcount::inSameTimeBlock(const GPSZcount& other,
                                   unsigned long inZcountBlock,
                                   unsigned long inZcountOffset)
   {
      if ((long)inZcountBlock < ZCOUNT_WEEK)
      {
            // Make sure that we're in the same week, and then check to see if
            // we're in the same time block
         if ( (getWeek() == other.getWeek()) &&
              (((getZcount() - inZcountOffset) / inZcountBlock) ==
               ((other.getZcount() - inZcountOffset) / inZcountBlock)) )
         {
            return true;
         }
      }
      else // inZcountBlock >= ZCOUNT_WEEK
      {
            // Compare using the total number of Z-counts.
         if (long((getTotalZcounts() - inZcountOffset) / inZcountBlock) ==
             long((other.getTotalZcounts() - inZcountOffset) / inZcountBlock))
         {
            return true;
         }
      }

      return false;
   }

   void GPSZcount::dump(std::ostream& out,
                        short level) const
   {
      switch(level)
      {
         case 0:
            out << week << "w" << zcount << "z" << flush;
            break;
         case 1:
         default:
            out << "GPS Full Week: " << setw(6) << week << endl
                << "GPS Z-count:   " << setw(6) << zcount << endl;
            break;
      }
   }

   long GPSZcount::validZcount(long z)
   {
      if (z < 0)
      {
         return z;
      }
      if (z >= GPSZcount::ZCOUNT_WEEK)
      {
         return (z - (GPSZcount::ZCOUNT_WEEK - 1));
      }
      return 0; // z is OK
   }

   std::ostream& operator<<(std::ostream& s,
                            const gnsstk::GPSZcount& z)
   {
      z.dump(s, 0);
      return s;
   }

} // namespace gnsstk
