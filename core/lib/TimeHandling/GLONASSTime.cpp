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

#include <math.h>
#include "GLONASSTime.hpp"
#include "TimeConverters.hpp"

namespace gnsstk
{
   GLONASSTime ::
   GLONASSTime(unsigned leapCount,
               unsigned days,
               double instant,
               TimeSystem ts)
         : TimeTag(ts),
           epoch(leapCount),
           day(days),
           sod(instant)
   {
   }


   CommonTime GLONASSTime ::
   convertToCommonTime() const
   {
      try
      {
            // day of year within the current year
         unsigned doy;
            // current year number in the four year interval, see ICD A.3.1.3
         unsigned J;
            // epoch (N4) 27 corresponds to year 2100 which is not a leap year
         unsigned lastDay = (epoch == 27 ? 365 : 366);
         if (day <= lastDay)
         {
            J = 1;
            doy = day;
         }
         else if (day <= (lastDay + 365))
         {
            J = 2;
            doy = day - lastDay;
         }
         else if (day <= (lastDay + 730))
         {
            J = 3;
            doy = day - (lastDay + 365);
         }
         else
         {
            J = 4;
            doy = day - (lastDay + 730);
         }
         unsigned year = 1996 + 4*(epoch-1) + (J - 1);
         long jday = convertCalendarToJD(year, 1, 1) + doy - 1;
            /** @note We don't add the C1 or C2 constants because we
             * don't have any way of determining what year it is
             * independently of the spec.  This won't matter until
             * 2119 at the earliest though, and I'll be long dead by
             * then. */
         double tmp = 1461.0 * (epoch-1.0) + day + GLO_EPOCH_JD
            - ((day - 3.0)/25.0);
         CommonTime ct;
         return ct.set(jday, sod, timeSystem);
      }
      catch (InvalidParameter& ip)
      {
         InvalidRequest ir(ip);
         GNSSTK_THROW(ir);
      }
   }


   void GLONASSTime ::
   convertFromCommonTime(const CommonTime& ct)
   {
      long jday = 0, secDay = 0;
      double fsecDay = 0.0;
      ct.get(jday, secDay, fsecDay, timeSystem);
      sod = static_cast<double>(secDay) + fsecDay;

      int cvtYear = 0, cvtMonth = 0, cvtDay = 0;
      convertJDtoCalendar(jday, cvtYear, cvtMonth, cvtDay);
      if (cvtYear < 1996)
      {
         InvalidRequest ir("Unable to represent years < 1996 in GLONASS time");
         GNSSTK_THROW(ir);
      }
      epoch = 1 + ((cvtYear - 1996) >> 2); // integer divide by 4.
         // current year within the four year interval, starting from
         // 0 instead of 1 as per usual.
      unsigned J = ((cvtYear-1996) % 4);
      day = J * 365;
      if (J > 0)
      {
         unsigned leapYear = cvtYear - (cvtYear % 4);
         unsigned leapDay = (((leapYear % 400) == 0) ||
                             ((leapYear % 100) != 0));
            // First day of the current year.  Possibly adding 1 to
            // account for leap year (J=0).
         day = leapDay + (J * 365);
      }
      day += jday - convertCalendarToJD(cvtYear, 1, 1) + 1;
   }


   bool GLONASSTime ::
   operator==(const GLONASSTime& right) const
   {
      TimeTag::checkTimeSystem(timeSystem, right.timeSystem);
      return ((epoch == right.epoch) &&
              (day == right.day) &&
              (fabs(sod - right.sod) < CommonTime::eps));
   }


   bool GLONASSTime ::
   operator!=(const GLONASSTime& right) const
   {
      return !operator==(right);
   }


   bool GLONASSTime ::
   operator<(const GLONASSTime& right) const
   {
      TimeTag::checkTimeSystem(timeSystem, right.timeSystem);
      if (epoch < right.epoch) return true;
      if (right.epoch < epoch) return false;
      if (day < right.day) return true;
      if (right.day < day) return false;
      if (sod < right.sod) return true;
      return false;
   }


   bool GLONASSTime ::
   operator>(const GLONASSTime& right) const
   {
      TimeTag::checkTimeSystem(timeSystem, right.timeSystem);
      if (right.epoch < epoch) return true;
      if (epoch < right.epoch) return false;
      if (right.day < day) return true;
      if (day < right.day) return false;
      if (right.sod < sod) return true;
      return false;
   }


   bool GLONASSTime ::
   operator<=(const GLONASSTime& right) const
   {
      TimeTag::checkTimeSystem(timeSystem, right.timeSystem);
      if (epoch < right.epoch) return true;
      if (right.epoch < epoch) return false;
      if (day < right.day) return true;
      if (right.day < day) return false;
      if (sod < right.sod) return true;
      if (right.sod < sod) return false;
      return true;
   }


   bool GLONASSTime ::
   operator>=(const GLONASSTime& right) const
   {
      TimeTag::checkTimeSystem(timeSystem, right.timeSystem);
      if (right.epoch < epoch) return true;
      if (epoch < right.epoch) return false;
      if (right.day < day) return true;
      if (day < right.day) return false;
      if (right.sod < sod) return true;
      if (sod < right.sod) return false;
      return true;
   }


   bool GLONASSTime ::
   isValid() const
   {
         // I think this is right, 1461 = 4 years.
      return ((day <= 1461) && (sod >= 0.0) && (sod < 86400.0));
   }

   void GLONASSTime ::
   reset()
   {
      epoch = 0;
      day = 0;
      sod = 0;
      timeSystem = TimeSystem::GLO;
   }

} // namespace gnsstk
