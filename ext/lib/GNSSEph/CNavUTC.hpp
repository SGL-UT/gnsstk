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
 * @file CNavUTC.hpp
 * Designed to support loading CNAV UTC data
 * (Message Type 33)
 */

#ifndef GNSSTK_CNAVUTC_HPP
#define GNSSTK_CNAVUTC_HPP

#include <string>
#include <iostream>

#include "CNavDataElement.hpp"
#include "PackedNavBits.hpp"

namespace gnsstk
{
   class CNavUTC : public CNavDataElement
   {
   public:
         /// Default constructor
      CNavUTC();

         /**
          * @throw InvalidParameter
          */
      CNavUTC(const PackedNavBits& message33);

         /// Destructor
      virtual ~CNavUTC() {}

         /// Clone method
      virtual CNavUTC* clone() const;

      virtual bool isSameData(const CNavDataElement* right) const;

         /**
          * Store the contents of message type 33 in this object.
          * @param message33 - 300 bits of Message Type 33
          * @throw InvalidParameter if message data is invalid
          */
      void loadData(const PackedNavBits& message33);

      virtual std::string getName() const
      {
         return "CNavUTC";
      }

      virtual std::string getNameLong() const
      {
         return "Civilian Navigation (CNAV) UTC Parameters";
      }

         /**
          * @throw InvalidRequest
          */
      virtual void dumpBody(std::ostream& s = std::cout) const;

      double A0;
      double A1;
      double A2;
      double deltaTls;
      long   Tot;         // Note: These are stored for completeness,
      int    WNot;        // The epoch time variable provides a CommonTime representation,
      int    WNlsf;
      int    DN;
      double deltaTlsf;
   }; // end class CNavUTC

   std::ostream& operator<<(std::ostream& s,
                                    const CNavUTC& eph);
} // end namespace

#endif // GNSSTK_CNAVUTC_HPP
