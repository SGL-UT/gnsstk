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
#ifndef GNSSTK_IONODATA_HPP
#define GNSSTK_IONODATA_HPP

#include "NavData.hpp"
#include "Position.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the interface for classes that provide the ability
       * to compute ionospheric delay, using data extracted from GNSS
       * navigation messages. */
   class IonoNavData : public NavData
   {
   public:
         /// Set the messageType
      IonoNavData()
      { signal.messageType = NavMessageType::Iono; }

         /// Obligatory virtual destructor.
      virtual ~IonoNavData()
      {}

         /** Get the ionospheric correction in meters.
          * @param[in] when The time of the observation to correct.
          * @param[in] rxgeo The receiver's geodetic position.
          * @param[in] svgeo The observed satellite's geodetic position.
          * @param[in] band The carrier band of the signal being corrected.
          * @return The ionospheric delay, in meters, on band. */
      virtual double getIonoCorr(const CommonTime& when,
                                 const Position& rxgeo,
                                 const Position& svgeo,
                                 CarrierBand band) const = 0;

         /// @copydoc NavData::isSameData
      bool isSameData(const NavDataPtr& right) const override
      {
         Exception exc("Unimplemented function");
         GNSSTK_THROW(exc);
      }
         /// @copydoc NavData::compare
      std::list<std::string> compare(const NavDataPtr& right)
         const override
      {
         Exception exc("Unimplemented function");
         GNSSTK_THROW(exc);
      }
   };

      //@}

}

#endif // GNSSTK_IONODATA_HPP
