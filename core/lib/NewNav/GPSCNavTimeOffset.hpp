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
#ifndef GPSTK_GPSCNAVTIMEOFFSET_HPP
#define GPSTK_GPSCNAVTIMEOFFSET_HPP

#include "TimeOffsetData.hpp"
#include "TimeSystem.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the class that provide the ability to convert
       * between GPS and a supported time system (UTC, GLONASS,
       * Galileo), using data extracted from GPS civil navigation
       * messages.
       * @note This class is used to encapsulate the data from message
       *   types 33 and 35.  Message type 33 is for UTC conversion and
       *   has additional factors not present in message type 35 (see
       *   the data fields).  Additionally, the parameter subscripts
       *   are different between the two, e.g. A<sub>0-n</sub> in
       *   message type 33 vs A<sub>0GGTO</sub> in message type 35,
       *   however mathematically they are identical terms and generic
       *   names are used in this class to represent the data. */
   class GPSCNavTimeOffset : public TimeOffsetData
   {
   public:
         /// Initialize all data to 0.
      GPSCNavTimeOffset();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 12s or 6s, depending on whether
          *   this is CNAV on L5 or on L2.
          */
      CommonTime getUserTime() const override;

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, Detail dl) const override;

         /** Get the offset, in seconds, to apply to times when
          * converting them from fromSys to toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] when The time being converted, in the GPS time
          *   system.
          * @param[out] offset The offset in seconds where
          *   when(toSys)=when(fromSys)-offset.
          * @return true if an offset is available, false if not. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, double& offset)
         const override;

         /** The set of time system conversions this class is capable of making.
          * @return a set of supported time system conversion to/from pairs. */
      TimeCvtSet getConversions() const override;

         // these terms are defined in IS-GPS-200, 30.3.3.6.2 and 30.3.3.8.2

      TimeSystem tgt;   ///< Target time system (UTC, GLONASS, Galileo).
      double a0;        ///< Bias coefficient of GPS time scale
      double a1;        ///< Drift coefficient of GPS time scale
      double a2;        ///< Drift rate coefficient of GPS time scale
      double deltatLS;  ///< Current or past leap second count (UTC only)
      double tot;       ///< Time data reference time of week
      unsigned wnot;    ///< Time data reference week number
         // These terms are not used in computing an offset, they're
         // more of a warning of an upcoming change in the offset.  We
         // keep them here for user convenience.
      unsigned wnLSF;   ///< Leap second reference week number (UTC only)
      unsigned dn;      ///< Leap second reference day number (UTC only)
      double deltatLSF; ///< Current or future leap second count (UTC only)
   };

      //@}

}

#endif // GPSTK_GPSCNAVUTCTIMEOFFSET_HPP
