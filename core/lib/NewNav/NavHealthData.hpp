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
#ifndef GNSSTK_NAVHEALTHDATA_HPP
#define GNSSTK_NAVHEALTHDATA_HPP

#include "NavData.hpp"
#include "SVHealth.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Abstract base class for classes that provide satellite
       * health information. */
   class NavHealthData : public NavData
   {
   public:
         /// Set our message type to health so we don't have to in every leaf.
      NavHealthData()
      { signal.messageType = NavMessageType::Health; }

         /** Returns the time for the data to be used when searching
          * in "Nearest" mode. */
      CommonTime getNearTime() const override
      { return timeStamp; }

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat). */
      virtual SVHealth getHealth() const = 0;

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

#endif // GNSSTK_NAVHEALTHDATA_HPP
