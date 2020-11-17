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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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

#ifndef GPSTK_SATID_HPP
#define GPSTK_SATID_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include "gps_constants.hpp"
#include "SatelliteSystem.hpp"

/**
 * @file SatID.hpp
 * gpstk::SatID - navigation system-independent representation of a satellite.
 */

namespace gpstk
{
      // forward declarations
   class SatID;
//   std::istream& operator<<(std::istream& s, SatID& p);

      /// @ingroup GNSSEph
      //@{

      /** Class used to uniquely identify the transmitter of a signal
       * being broadcast, either by satellite or by ground station
       * (sometimes referred to as a "pseudolite").  The meaning of
       * the identifier \a id depends on the context of the system, as
       * does the \a extra field.  The meanings are as follows:
       *
       * system  | id           | extra
       * ------- | ------------ | ----------------
       * GPS     | PRN ID       | n/a
       * Galileo | ?            | n/a
       * Glonass | Slot Number  | frequency offset
       * Geosync | PRN ID - 100 | n/a
       * LEO     | ?            | n/a
       * Transit | ?            | n/a
       * BeiDou  | ?            | n/a
       * QZSS    | PRN ID       | n/a
       * IRNSS   | ?            | n/a
       *
       * For equality and ordering, any of the three fields may be
       * treated as a wildcard, which means that a wildcard value for
       * a given field will match any other field value.  The system
       * and id fields use boolean flags (wildSys and wildId,
       * respectively), and extra uses a bitmask where only the bits
       * set in extraMask are compared between two SatID values.
       *
       * For example, if you want to match the GLONASS frequency
       * offset, you might set id=2, extra=-3 and extraMask=-1.  This
       * would only match id=2,extra=-3 (slot 2, freq offset -3).  On
       * the other hand, if you only care about the slot number, you
       * would set id=2, extra=don't-care and extraMask=0.
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
         /** Constructor that allows for the specification of extra
          * information to help uniquely identify a transmitter (see
          * SatID).  Typically used to specify GLONASS frequency
          * offset.
          * @param[in] p The satellite number of the transmitter.
          * @param[in] s The satellite system of the transmitter.
          * @param[in] x The "extra" information for the transmitter
          *   (e.g. frequency offset).
          * @param[in] xm The bitmask used for matching extra.  The
          *   default value is -1 which means that all bits will be
          *   compared.
          */
      SatID(int p, SatelliteSystem s, int64_t x, int64_t xm = -1);
         /** Initialize with a set number and a wild system (weird).
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

         /// operator < for SatID : order by system, id, extra
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
          * @note this is not used internally in the gpstk library */
      bool isValid() const;

      int id;                   ///< Satellite identifier, e.g. PRN
      bool wildId;              ///< If true, any satellite matches.
      int64_t extra;            ///< Extra data required to uniquely ID signal.
      int64_t extraMask;        ///< Bitmask for matching extra.
      SatelliteSystem system;   ///< System for this satellite
      bool wildSys;             ///< If true, any system matches.
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

} // namespace gpstk

#endif
