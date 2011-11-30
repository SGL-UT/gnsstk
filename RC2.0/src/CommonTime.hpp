#pragma ident "$Id$"

#ifndef GPSTK_COMMONTIME_HPP
#define GPSTK_COMMONTIME_HPP

//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//  
//  Copyright 2004, The University of Texas at Austin
//
//============================================================================

//============================================================================
//
//This software developed by Applied Research Laboratories at the University of
//Texas at Austin, under contract to an agency or agencies within the U.S. 
//Department of Defense. The U.S. Government retains all rights to use,
//duplicate, distribute, disclose, or release this software. 
//
//Pursuant to DoD Directive 523024 
//
// DISTRIBUTION STATEMENT A: This software has been approved for public 
//                           release, distribution is unlimited.
//
//=============================================================================

#include "Exception.hpp"
#include "TimeConstants.hpp"
#include "TimeSystem.hpp"

using namespace std;

namespace gpstk
{
      /** 
       * This is the common time format that all time formats convert to when
       * converting between themselves.  This allows the decoupling of 
       * inter-format conversions.
       *
       * The interface is based on three quantites: days, seconds of day, and
       * fractional seconds of day.  The internal representation, however, is
       * slightly different.  It consists of a day, milliseconds of day, and
       * fractional seconds of day.  Their valid ranges are shown below:
       * 
       *  Quantity   >=     <
       *  --------   ---   ---
       *   day        0    2^31
       *   msod       0    86400000
       *   fsod       0    0.001
       * 
       * The above is somewhat difficult to grasp at first, but the reason for 
       * keeping the fractional part of time in units of seconds is due to the
       * fact that the time formats usually break at seconds and partial seconds
       * not at milliseconds and partial milliseconds.  By keeping the value in
       * seconds, we save ourselves additional work and loss of precision
       * through conversion of fractional seconds to fractional milliseconds.
       */
   class CommonTime
   {
     public:
         /**
          * @defgroup ctc CommonTime Constants
          */
         //@{

         /// 'julian day' of earliest epoch expressible by CommonTime:
         /// 1/1/4713 B.C.
      static const long BEGIN_LIMIT_JDAY;
         /// 'julian day' of latest epoch expressible by CommonTime:
         /// 1/1/4713 A.D.
      static const long END_LIMIT_JDAY;

         /// earliest representable CommonTime
      static const CommonTime BEGINNING_OF_TIME;
         /// latest representable CommonTime
      static const CommonTime END_OF_TIME;

         /// Default tolerance for time equality in days.
      static const double eps;
         //@}

         /**
          * @defgroup ctbo CommonTime Basic Operations
          * Default and Copy Constructors, Assignment Operator and Destructor.
          */
         //@{

         /**
          * Default Constructor.
          * All numerical elements default to zero, "Unknown" for time frame.
          * @see CommonTime::set()
          */
      CommonTime( long day = 0, 
                  long sod = 0,
                  double fsod = 0.0,
                  TimeSystem timeSystem = TimeSystem::Unknown )
         throw( InvalidParameter )
      { set( day, sod, fsod, timeSystem ); }

         /**
          * Constructor that accepts days and second of day.
          * All elements default to zero and Unknown.
          */
      CommonTime( long day,
                  double sod,
                  TimeSystem timeSystem )
         throw( InvalidParameter )
      { set( day, sod, timeSystem ); }
     

         /** 
          * Constructor that accepts days only.
          * All elements default to zero and Unknown.
          */
      CommonTime( double day,
                  TimeSystem timeSystem )
         throw( InvalidParameter )
      { set( day, timeSystem ); }
      
         /**
          * Copy Constructor.
          * @param right a const reference to the CommonTime object to copy.
          */
      CommonTime( const CommonTime& right )
         throw();

         /**
          * Assignment Operator.
          * @param right a const reference to the CommonTime object to copy.
          * @return a reference to this CommonTime object.
          */
      CommonTime& operator=( const CommonTime& right )
         throw();
      
         /// Destructor.
      virtual ~CommonTime()
         throw()
      {}

         //@}

         /** 
          * @defgroup ctam CommonTime Accessor Methods
          */
         //@{
         /** 
          * Set method that accepts values for day, seconds of day and
          * fractional seconds of day.  It also checks the validity of the 
          * arguments and throws an exception if any values are out of bounds.
          */
      CommonTime& set( long day, 
                       long sod,
                       double fsod = 0.0,
                       TimeSystem timeSystem = TimeSystem::Unknown )
         throw( InvalidParameter );

         /**
          * Set method that accepts values for day and seconds of day.  It also
          * checks the validity of the arguments and throws an exception if any
          * values are out of bounds.
          */
      CommonTime& set( long day,
                       double sod = 0.0,
                       TimeSystem timeSystem = TimeSystem::Unknown )
         throw( InvalidParameter );
      
         /**
          * Set method that accepts a value for day.  It also checks the
          * validity of the argument and throws an exception if its value is
          * out of bounds.
          */
      CommonTime& set( double day,
                       TimeSystem timeSys = TimeSystem::Unknown )
         throw( InvalidParameter );

         /**
          * Set internal values method.  Set the data members of this object
          * directly.  Checks the validity of the given time representation
          * and throws an exception if any values are out of bounds.
          */
      CommonTime& setInternal( long day = 0,
                               long msod = 0,
                               double fsod = 0.0,
                               TimeSystem timeSys = TimeSystem::Unknown )
         throw( InvalidParameter );

         /**
          * Set method for internal variable m_timeSystem.
          */
      void setTimeSystem( const TimeSystem& timeSystem )
         throw()
      { m_timeSystem = timeSystem; }

         /**
          * Set the object's time using POSIX structures.
          * This uses the POSIX timeval struct to set the time.  This
          * structure represents the time with a resolution of
          * microseconds.
          * @param t time to set
          * @param f Time frame (see #TimeFrame)
          * @return a reference to this object.
          */
      CommonTime& setUnix(const struct timeval& t,
                       TimeSystem)
         throw(Exception);

         /**
          * Set the object's time using ANSI structures.  This uses
          * the ANSI C/C++ time_t struct to set the time.  This
          * structure represents the time with a resolution of
          * seconds.
          * @param t time to set
          * @param f Time frame (see #TimeFrame)
          * @return a reference to this object.
          */
      CommonTime& setANSI(const time_t& t,
                       TimeSystem)
         throw(Exception);

    CommonTime& setMJD(long double mjd,
                             TimeSystem)
       throw(Exception);   

         /**
          * Get method.  Obtain values in days, second of day and fractional
          * second of day, plus the time frame.
          */
      void get( long& day, 
                long& sod,
                double& fsod,
                TimeSystem& timeSystem ) const
         throw();

         /**
          * Get method.  Obtain values in days, second of day and fractional
          * second of day.
          */
      void get( long& day, 
                long& sod,
                double& fsod ) const
         throw();

         /**
          * Get method through which one may obtain values for day and
          * second of day which includes the fractional second of day,
          * plus the time frame.
          */
      void get( long& day,
                double& sod,
                TimeSystem& timeSystem ) const
         throw();

         /**
          * Get method through which one may obtain values for day and
          * second of day which includes the fractional second of day.
          */
      void get( long& day,
                double& sod ) const
         throw();

         /**
          * Get method through which one may obtain a value for day which
          * includes the fraction of a day, plus the time frame.
          */
      void get( double& day,
                TimeSystem& timeSystem ) const
         throw();

         /**
          * Get method through which one may obtain a value for day which
          * includes the fraction of a day.
          */
      void get( double& day ) const
         throw();

         /** 
          * Get internal values method.  Obtain the values stored within this
          * object.
          */
      void getInternal( long& day,
                        long& msod,
                        double& fsod,
                        TimeSystem& timeSystem ) const
         throw()
      { day = m_day; msod = m_msod; fsod = m_fsod; timeSystem = m_timeSystem; }

         /** 
          * Get internal values method.  Obtain the values stored within this
          * object.
          */
      void getInternal( long& day,
                        long& msod,
                        double& fsod ) const
         throw()
      { day = m_day; msod = m_msod; fsod = m_fsod; }

         /// Obtain the time, in days, including the fraction of a day.
      double getDays() const 
         throw();

         /// Obtain the seconds of day (ignoring the day).
      double getSecondOfDay() const
         throw();

         /// Obtain time system info (enum).
      TimeSystem getTimeSystem() const
         throw();

         //@}

         /**
          * @defgroup ctao CommonTime Arithmetic Operations
          */
         //@{
         /** 
          * Difference two Common Time objects.
          * @param right CommonTime to subtract from this one
          * @param reutrn difference in seconds
          */
      double operator-( const CommonTime& right ) const
         throw( InvalidRequest );

         /**
          * Add seconds to a copy of this CommonTime.
          * @param sec the number of seconds to add to a copy of this CommonTime
          * @return the new CommonTime object
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime operator+( double seconds ) const
         throw();

         /**
          * Subtract seconds from a copy of this CommonTime.
          * @param sec the number of seconds to subtract from a copy of this
          *  CommonTime
          * @return the new CommonTime object
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime operator-( double seconds ) const
         throw();

         /** 
          * Add seconds to this CommonTime.
          * @param sec the number of seconds to add to this CommonTime.
          * @return a reference to this CommonTime
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime& operator+=( double seconds )
         throw();

         /**
          * Subtract seconds from this CommonTime.
          * @param sec the number of seconds to subtract from this CommonTime
          * @return a reference to this CommonTime object
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime& operator-=( double seconds )
         throw();

         /** 
          * Add seconds to this CommonTime object.
          * This is the workhorse for the addition/subtraction operators.
          * @param sec the number of seconds to add to this CommonTime
          * @return a reference to this CommonTime object
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime& addSeconds( double seconds )
         throw();

         /** 
          * Add integer days to this CommonTime object.
          * param days the number of days to add to this CommonTime
          * @return a reference to this CommonTime object
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime& addDays( long days )
         throw();

         /** 
          * Add integer seconds to this CommonTime object.
          * @param sec the number of seconds to add to this CommonTime
          * @return a reference to this CommonTime object
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime& addSeconds( long seconds )
         throw();

         /** 
          * Add integer milliseconds to this CommonTime object.
          * @param ms the number of milliseconds to add to this CommonTime
          * @return a reference to this CommonTime object
          * @throws InvalidRequest on over-/under-flow
          */
      CommonTime& addMilliseconds( long ms )
         throw();
         //@}

         /**
          * @defgroup ctco CommonTime Comparison Operators
          * All comparison operators have a parameter "right" which corresponds
          *  to the CommonTime object to the right of the symbol.
          * All comparison operators are const and return true on success
          *  and false on failure.
          */
         //@{
      bool operator==( const CommonTime& right ) const
         throw();
      bool operator!=( const CommonTime& right ) const
         throw();
      bool operator<( const CommonTime& right ) const
         throw( InvalidRequest );
      bool operator>( const CommonTime& right ) const
         throw( InvalidRequest );
      bool operator<=( const CommonTime& right ) const
         throw( InvalidRequest );
      bool operator>=( const CommonTime& right ) const
         throw( InvalidRequest );
         //@}

      void reset()
         throw()
      { m_day = m_msod = 0; m_fsod = 0.0; m_timeSystem = TimeSystem::Unknown; }

      std::string asString() const
         throw();

   protected:
      
         /**
          * Add the following quantities to this CommonTime object.
          * @param days the number of days to add
          * @param msod the number of milliseconds to add
          * @param fsod the number of fractional seconds to add
          * @return the result of calling the normalize() function
          */
      bool add( long days,
                long msod,
                double fsod )
         throw();
      
         /// Normalize the values.  This takes out of bounds values and rolls
         /// other values appropriately.
         /// @return true if m_day is valid, false otherwise
      bool normalize()
         throw();
      
      long m_day;     ///< days (as a Julian Day)     0 <= val < 2^31
      long m_msod;    ///< milliseconds-of-day        0 <= val < 86400000
      double m_fsod;  ///< fractional seconds-of-day  0 <= val < 0.001

      TimeSystem m_timeSystem; ///< time frame (system representation) of the data

   }; // end class CommonTime

   std::ostream& operator<<(std::ostream& o, const CommonTime& ct);

} // namespace

#endif // GPSTK_COMMONTIME_HPP
