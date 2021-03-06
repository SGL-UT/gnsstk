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
 * @file CommandOptionWithTimeArg.cpp
 * Command line options with time (class CommonTime) arguments
 */

#include "StringUtils.hpp"
#include "TimeString.hpp"
#include "CommandOptionWithTimeArg.hpp"

using namespace std;

namespace gnsstk
{
   string CommandOptionWithTimeArg :: checkArguments()
   {
      string errstr = CommandOptionWithAnyArg::checkArguments();

      if (errstr != string())
         return errstr;

      vector<string>::size_type vecindex;
      for(vecindex = 0; vecindex < value.size(); vecindex++)
      {
         string thisTimeSpec = getTimeSpec(vecindex);
         if (thisTimeSpec != string())
         {
            try {
               CommonTime t;
               scanTime(t, value[vecindex], thisTimeSpec);
               times.push_back(t);
            }
            catch (...)
            {
               errstr += "\"" + value[vecindex] + "\" is not a valid time.";
            }
         }
         else
            errstr += "\"" + value[vecindex] + "\" is not a valid time.";
      }

      return errstr;
   }

   string CommandOptionWithSimpleTimeArg :: getTimeSpec
   (vector<string>::size_type index) const
   {
      int numwords = gnsstk::StringUtils::numWords(value[index]);
      string thisTimeSpec;
      switch (numwords)
      {
         case 1:
            thisTimeSpec = "%m/%d/%Y";
            break;
         case 2:
            thisTimeSpec = "%Y %j";
            break;
         case 3:
            thisTimeSpec = "%Y %j %s";
            break;
      }

      return thisTimeSpec;
   }

} // namespace gnsstk
