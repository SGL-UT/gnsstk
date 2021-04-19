//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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
#ifndef GPSTK_STDNAVTIMEOFFSET_HPP
#define GPSTK_STDNAVTIMEOFFSET_HPP

#include "TimeOffsetData.hpp"
#include "TimeSystem.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines a base class that provide the ability to convert
       * between time systems, using data extracted from GNSS
       * navigation messages.
       * The algorithm and parameters are largely the same across
       * systems, so this is basically the TimeOffset equivalent of
       * OrbitDataKepler, in that it contains data and algorithms to
       * use it. */
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

         // These terms are referenced in the ICDs for multiple
         // systems, but you can find their definition in places like
         // IS-GPS-200 Figure 30-6.  They occasionally have different
         // names like wnot, t0t, wn0t, etc., but the terms originate
         // from the nav message, though some may be zero (unused).

      TimeSystem src;     ///< Source time system.
      TimeSystem tgt;     ///< Target time system.
      double a0;          ///< Bias coefficient of source time scale.
      double a1;          ///< Drift coefficient of source time scale.
      double a2;          ///< Drift rate coefficient of source time scale.
      double deltatLS;    ///< Current or past leap second count (UTC only).
      CommonTime refTime; ///< Reference time for computation.
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

#endif // GPSTK_STDNAVTIMEOFFSET_HPP
