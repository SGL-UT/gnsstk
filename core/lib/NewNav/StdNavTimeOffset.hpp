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
#ifndef GNSSTK_STDNAVTIMEOFFSET_HPP
#define GNSSTK_STDNAVTIMEOFFSET_HPP

#include "TimeOffsetData.hpp"
#include "TimeSystem.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines a base class that provide the ability to convert
       * between time systems, using data extracted from GNSS
       * navigation messages.
       * The algorithm and parameters are largely the same across
       * systems, so this is basically the TimeOffset equivalent of
       * OrbitDataKepler, in that it contains data and algorithms to
       * use it.
       *
       * Section 20.3.3.5.2.4, paragraph b of IS-GPS-200 goes into
       * details about how the future leap seconds
       * (&Delta;t<sub>LSF</sub>) are handled.  This text makes
       * assumptions about the tying of time representation (seconds
       * of day) and the time system (UTC) which are not true for
       * gnsstk.  Specifically, the IS allows for the leap second to be
       * represented as 86400+ seconds of day, which the gnsstk
       * currently rejects as an error.  These leap seconds are
       * currently not representable in the gnsstk, but the offset can
       * still be used the rest of the time.  This means that an
       * attempt to display UTC time during the period of a leap
       * second will be incorrect, but for now we're chosing to ignore
       * this inconsistency rather than completely refactor the time
       * classes again for 1 second every few years in UTC only. */
   class StdNavTimeOffset : public TimeOffsetData
   {
   public:
         /// Initialize all data to 0.
      StdNavTimeOffset();

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Get the offset, in seconds, to apply to times when
          * converting them from fromSys to toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] when The time being converted, in the same time
          *   system as fromSys.
          * @param[out] offset The offset in seconds where
          *   when(toSys)=when(fromSys)-offset.
          * @throw AssertionFailure if when's time system is not fromSys.
          * @return true if an offset is available, false if not. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, double& offset)
         const override;

         /** The set of time system conversions this class is capable of making.
          * @return a set of supported time system conversion to/from pairs. */
      TimeCvtSet getConversions() const override;

         /** Return the number of seconds prior to the effectivity
          * time during which the standard delta t formula is used. */
      virtual double effStart() const
      { return 6 * 3600; }
         /** Return the number of seconds after the effectivity
          * time during which the standard delta t formula is used. */
      virtual double effEnd() const
      { return 6 * 3600; }

         // These terms are referenced in the ICDs for multiple
         // systems, but you can find their definition in places like
         // IS-GPS-200 30.3.3.6.2.  They occasionally have different
         // names like wnot, t0t, wn0t, etc., but the terms originate
         // from the nav message, though some may be zero (unused).
         // They are meant to be set by the NavDataFactory classes,
         // but are made available for the user to read.  The user
         // generally should not be changing these values.

      TimeSystem src;     ///< Source time system.
      TimeSystem tgt;     ///< Target time system.
      double a0;          ///< Bias coefficient of source time scale.
      double a1;          ///< Drift coefficient of source time scale.
      double a2;          ///< Drift rate coefficient of source time scale.
      double deltatLS;    ///< Current or past leap second count (UTC only).
      CommonTime refTime; ///< Reference time for computation.
      CommonTime effTime; ///< Effectivity time (wnLSF,dn).
      double tot;         ///< Time data reference time of week (storage only).
      unsigned wnot;      ///< Time data reference week number (storage only).
         // These terms are not used in computing an offset, they're
         // more of a warning of an upcoming change in the offset.  We
         // keep them here for user convenience.
      unsigned wnLSF;   ///< Leap second reference week number (UTC only).
      unsigned dn;      ///< Leap second reference day number (UTC only).
      double deltatLSF; ///< Current or future leap second count (UTC only).
   };

      //@}

}

#endif // GNSSTK_STDNAVTIMEOFFSET_HPP
