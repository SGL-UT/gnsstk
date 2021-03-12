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
#ifndef GPSTK_GPSLNAVTIMEOFFSET_HPP
#define GPSTK_GPSLNAVTIMEOFFSET_HPP

#include "TimeOffsetData.hpp"
#include "TimeSystem.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the class that provides the ability to convert
       * between GPS and UTC, using data extracted from GPS legacy
       * navigation messages. */
   class GPSLNavTimeOffset : public TimeOffsetData
   {
   public:
         /// Initialize all data to 0.
      GPSLNavTimeOffset();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

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
          *   system, "as estimated by the user after correcting
          *   t<sub>SV</sub> for factors described in paragraph
          *   20.3.3.3.3 as well as for SA effects"
          * @param[out] offset The offset when converting fromSys->toSys.
          * @return true if an offset is available, false if not. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, double& offset)
         const override;

         /** The set of time system conversions this class is capable of making.
          * @return a set of supported time system conversion to/from pairs. */
      TimeCvtSet getConversions() const override;

         // these terms are defined in IS-GPS-200, 20.3.3.5.1.7 & 20.3.3.5.2.4

      double deltatLS;  ///< &Delta;t<sub>LS</sub> term
      double a0;        ///< A<sub>0</sub> term
      double a1;        ///< A<sub>1</sub> term
      double tot;       ///< t<sub>ot</sub> term
      unsigned wnt;     ///< UTC reference week
         // These terms are not used in computing an offset, they're
         // more of a warning of an upcoming change in the offset.  We
         // keep them here for user convenience.
      unsigned wnLSF;   ///< Week number for future scheduled increment.
      unsigned dn;      ///< Day number for future leap second.
      double deltatLSF; ///< Scheduled future time increment due to leap sec.
   };

      //@}

}

#endif // GPSTK_GPSLNAVTIMEOFFSET_HPP
