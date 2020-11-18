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
#ifndef GPSTK_SVHEALTH_HPP
#define GPSTK_SVHEALTH_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Identify different types of SV health states.
   enum class SVHealth
   {
      Unknown,    ///< Health is not known or is uninitialized.
      Any,        ///< Use in searches when you don't care about the SV health.
      Healthy,    ///< Satellite is in a healthy and useable state.
      Unhealthy,  ///< Satellite is unhealthy and should not be used.
      Degraded,   ///< Satellite is in a degraded state. Use at your own risk.
      Last        ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (SVHealth i : SVHealthIterator()) */
   typedef EnumIterator<SVHealth, SVHealth::Unknown, SVHealth::Last> SVHealthIterator;

   namespace StringUtils
   {
         /// Convert a SVHealth to a whitespace-free string name.
      std::string asString(SVHealth e) throw();
         /// Convert a string name to an SVHealth
      SVHealth asSVHealth(const std::string& s) throw();
   }

      //@}

}

#endif // GPSTK_SVHEALTH_HPP
