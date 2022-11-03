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

#ifndef GLONASSTIME_HPP
#define GLONASSTIME_HPP

#include "TimeTag.hpp"

namespace gnsstk
{
      /// @ingroup TimeHandling
      //@{

      /** This class handles the peculiar representation used in
       * GLONASS nav messages.
       * @note The methods for printing (getPrintChars,
       *   getDefaultFormat, printf, printError, setFromInfo) are as
       *   yet unimplemented.  In order for these to be implemented,
       *   format tokens would need to be added for the
       *   GLONASS-specific representation and there is no immediate
       *   call for this (99.999% of the time the time will be
       *   displayed in civil format), and the unclaimed tokens are
       *   few. */
   class GLONASSTime : public TimeTag
   {
   public:

         /** Constructor.
          * @param[in] leapCount The number of leap years since 1996 (N4).
          * @param[in] days The number of days since the January 1 of
          *   the most recent leap year (N<sub>T</sub> or
          *   N<sub>A</sub>).
          * @param[in] instant The time of the day (t<sub>b</sub>). */
      GLONASSTime(unsigned leapCount = 0,
                  unsigned days = 0,
                  double instant = 0.,
                  TimeSystem ts = TimeSystem::GLO);

         /** Copy Constructor.
          * @param[in] right the GLONASSTime object to copy
          */
      GLONASSTime(const GLONASSTime& right)
            : TimeTag(right), epoch(right.epoch), day(right.day), sod(right.sod)
      {
      }

         /**
          * Alternate Copy Constructor.  Takes a const TimeTag
          * reference and copies its contents via conversion to
          * CommonTime.
          * @param[in] right the TimeTag-based object to copy
          * @throw InvalidRequest on over-/under-flow
          */
      GLONASSTime(const TimeTag& right)
      {
         convertFromCommonTime(right.convertToCommonTime());
      }

         /**
          * Alternate Copy Constructor.  Takes a const CommonTime
          * reference and copies its contents via the
          * convertFromCommonTime method.
          * @param[in] right the CommonTime object to copy.
          * @throw InvalidRequest on over-/under-flow
          */
      GLONASSTime(const CommonTime& right)
      {
         convertFromCommonTime(right);
      }

         /// Destructor (nothing to do).
      ~GLONASSTime()
      {}

         /// As it says, convert to a CommonTime object.
      CommonTime convertToCommonTime() const override;

         /// As it says, convert from a CommonTime object.
      void convertFromCommonTime(const CommonTime& ct) override;

         /** @name WeekSecond Comparison Operators
          * All comparison operators have a parameter "right" which
          * corresponds to the GLONASSTime object to the right of
          * the symbol.  All comparison operators are const and return
          * true on success and false on failure. */
         //@{
      bool operator==(const GLONASSTime& right) const;
      bool operator!=(const GLONASSTime& right) const;
      bool operator<(const GLONASSTime& right) const;
      bool operator>(const GLONASSTime& right) const;
      bool operator<=(const GLONASSTime& right) const;
      bool operator>=(const GLONASSTime& right) const;
         //@}

         /// Return a string containing the characters that this class
         /// understands when printing times.
      std::string getPrintChars() const override
      { return "P"; }

      
         /// Return a string containing the default format to use in printing.
      std::string getDefaultFormat() const override
      { return "GLONASSTime-has-no-printability-yet"; }

         /// This function formats this time to a string.  The exceptions
         /// thrown would only be due to problems parsing the fmt string.
      std::string printf(const std::string& fmt) const override
      { return "GLONASSTime-has-no-printability-yet"; }

         /// This function works similarly to printf.  Instead of filling
         /// the format with data, it fills with error messages.
      std::string printError(const std::string& fmt) const override
      { return "GLONASSTime-has-no-printability-yet"; }

         /// Set this object using the information provided in \a info.
         /// @param info the IdToValue object to which this object shall be set.
         /// @return true if this object was successfully set using the
         ///  data in \a info, false if not.
      bool setFromInfo(const IdToValue& info)
      { return false; }

         /// Returns true if this object's members are valid, false otherwise.
      bool isValid() const override;

         /// Reset this object to the default state.
      void reset() override;

         /// Number of leap years since 1996 (aka N4).
      unsigned epoch;
         /** Days since Jan 1 of the current epoch (aka N<sub>T</sub>
          * or N<sub>A</sub>. */
      unsigned day;
         /// Time of day (seconds of day, aka t<sub>b</sub>).
      double sod;

   }; // end class GLONASSTime

      //@}

} // namespace

#endif // GLONASSTIME_HPP
