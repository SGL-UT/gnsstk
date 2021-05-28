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
#ifdef __GNUG__
#include <cxxabi.h>
#endif
#include "NavData.hpp"
#include "TimeString.hpp"

// required to demangle G++ class names
static std::string demangle(const char* name)
{
      // yanked from stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
#ifdef __GNUG__
   int status = -4;
   std::unique_ptr<char, void(*)(void*)> res {
      abi::__cxa_demangle(name, NULL, NULL, &status),
      std::free
   };
   return (status == 0) ? res.get() : name;
#else
      // do nothing if not G++
   return name;
#endif
}

namespace gpstk
{
   const std::string NavData :: dumpTimeFmt("  %6.0g   %3a-%w   %3j   %5.0s   %02m/%02d/%04Y   %02H:%02M:%02S");

   NavData ::
   NavData()
         : msgLenSec(0),
           weekFmt("%4F(%4G)")
   {
   }


   void NavData ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      static const std::string timeFmt("%4Y/%02m/%02d %03j %02H:%02M:%02S");
      s << printTime(timeStamp, timeFmt) << " " << signal << std::endl;
   }


   std::string NavData ::
   getClassName() const
   {
      return demangle(typeid(*this).name());
   }
}

