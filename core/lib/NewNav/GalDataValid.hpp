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
#ifndef GPSTK_GALDATAVALID_HPP
#define GPSTK_GALDATAVALID_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Identify Galileo Data Validity Status (DVS) states.
   enum class GalDataValid
   {
      Unknown = -1,    ///< Health is not known or is uninitialized.
      Valid = 0,       ///< Navigation data valid.
      NoGuarantee = 1, ///< Working without guarantee.
      Last             ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (GalDataValid i : GalDataValidIterator()) */
   typedef EnumIterator<GalDataValid, GalDataValid::Unknown, GalDataValid::Last> GalDataValidIterator;

   namespace StringUtils
   {
         /// Convert a GalDataValid to a whitespace-free string name.
      std::string asString(GalDataValid e) throw();
         /// Convert a string name to an GalDataValid
      GalDataValid asGalDataValid(const std::string& s) throw();
   }

      //@}

}

#endif // GPSTK_GALDATAVALID_HPP
