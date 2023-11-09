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

#ifndef GNSSTK_SATID_HPP
#define GNSSTK_SATID_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include "gps_constants.hpp"
#include "SatelliteSystem.hpp"

/**
 * @file SatID.hpp
 * gnsstk::SatID - navigation system-independent representation of a satellite.
 */

namespace gnsstk
{
      // forward declarations
   class SatID;
//   std::istream& operator<<(std::istream& s, SatID& p);

      /// @ingroup GNSSEph
      //@{

      /** Class used to uniquely identify the transmitter of a signal
       * being broadcast, either by satellite or by ground station
       * (sometimes referred to as a "pseudolite").  The meaning of
       * the identifier \a id depends on the context of the system.
       * The meanings are as follows:
       *
       * system  | id
       * ------- | ------------
       * GPS     | PRN ID
       * Galileo | ?
       * Glonass | Slot Number
       * Geosync | PRN ID - 100
       * LEO     | ?
       * Transit | ?
       * BeiDou  | ?
       * QZSS    | PRN ID
       * IRNSS   | ?
       *
       * For equality and ordering, any of the fields may be
       * treated as a wildcard, which means that a wildcard value for
       * a given field will match any other field value.  The system
       * and id fields use boolean flags (wildSys and wildId,
       * respectively).
       *
       * @todo Update the above table for the proper nomenclature for
       * identification.
       */
   class SatID
   {
   public:
         /// Initialize with invalid data with no wildcards.
      SatID();

         /// explicit constructor, no defaults
         /// @note if s is given a default value here,
         /// some compilers will silently cast int to SatID.
      SatID(int p, SatelliteSystem s);
         /** Initialize with a set id number and a wild system (weird).
          * @note explicit keyword to prevent assignment with
          *   unexpected results.
          * @param[in] p The satellite number to specifically match.
          */
      explicit SatID(int p);
         /** Initialize with a set system and a wild satellite number.
          * @note explicit keyword to prevent assignment with
          *   unexpected results.
          * @param[in] s The satellite system to specifically match.
          */
      explicit SatID(SatelliteSystem s);

         /** Set all values to wildcard values, meaning that this
          * SatID will equal any other SatID object.  This method
          * attempts to obviate knowledge about the details of
          * wildcard implementation. */
      void makeWild();
         /// return true if any of the fields are set to match wildcards.
      bool isWild() const;

         // operator=, copy constructor and destructor built by compiler


         /// Convenience output method.
      void dump(std::ostream& s) const;

         /// operator == for SatID
      bool operator==(const SatID& right) const;

         /// operator != for SatID
      bool operator!=(const SatID& right) const
      { return !(operator==(right)); }

         /// operator < for SatID : order by system, id
      bool operator<(const SatID& right) const;

         /// operator > for SatID
      bool operator>(const SatID& right) const
      {  return (!operator<(right) && !operator==(right)); }

         /// operator <= for SatID
      bool operator<=(const SatID& right) const
      { return (operator<(right) || operator==(right)); }

         /// operator >= for SatID
      bool operator>=(const SatID& right) const
      { return !(operator<(right)); }

         /** return true if this is a valid SatID
          * @note assumes all id's are positive and less than 100;
          *     plus GPS id's are less than or equal to MAX_PRN (32).
          * @note this is not used internally in the gnsstk library */
      bool isValid() const;

         /** Assign a NORAD identifier for this satellite.
          * A convienence method for setting the norad ID and #hasNorad field.
          * See #norad for more info. */
      void setNorad(unsigned long n);

      int id;                   ///< Satellite identifier, e.g. PRN
      bool wildId;              ///< If true, any satellite matches.
      SatelliteSystem system;   ///< System for this satellite
      bool wildSys;             ///< If true, any system matches.

         /** NORAD assigned identifier for this satellite.
          * @note This value is only used as additional metadata.  It
          *   is intentionally not taken into account for sorting or
          *   comparing to avoid breaking code that doesn't actively
          *   support it. 
          * @todo replace norad/hasNorad with `std::optional` when 
          *   C++ 17 is used.*/
      unsigned long norad;
         /// Set to True if #norad is set.
         /// It is the user's responsibility to keep this variable valid.
      bool hasNorad = false;

   }; // class SatID

      /// stream output for SatID
   inline std::ostream& operator<<(std::ostream& s, const SatID& p)
   {
      p.dump(s);
      return s;
   }

      //@}

   namespace StringUtils
   {
         /// @ingroup StringUtils
         //@{

         /// SatID as a string
      inline std::string asString(const SatID& p)
      {
         std::ostringstream oss;
         p.dump(oss);
         return oss.str();
      }
   }

} // namespace gnsstk

#endif
