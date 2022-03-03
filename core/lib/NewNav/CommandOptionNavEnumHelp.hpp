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

#ifndef GNSSTK_COMMANDOPTIONNAVENUMHELP_HPP
#define GNSSTK_COMMANDOPTIONNAVENUMHELP_HPP

#include <iostream>
#include "CommandOption.hpp"

namespace gnsstk
{
      /** Command-line optino to provide help information for
       * specifying codes, carrier frequencies and other common
       * enumerations used throughout the NewNav code. */
   class CommandOptionNavEnumHelp : public CommandOptionHelp
   {
   public:
       /** Constructor.
          * @param[in] shOpt The one character command line option.
          *   Set to 0 if unused.
          * @param[in] loOpt The long command option.  Set to
          *   std::string() if unused.
          * @param[in] desc A string describing what this option does.
          */ 
      CommandOptionNavEnumHelp(const char shOpt,
                               const std::string& loOpt,
                               const std::string& desc = std::string(
                                  "Get help for enums, specify one of system,"
                                  " obstype, carrier, range, antenna, navtype,"
                                  " navmsgtype, health, validity, order,"
                                  " detail"));

         /** Print the requested help information.
          * @param[in] out The stream to which the help text will be printed.
          * @param[in] pretty Unused in this child class. */
      void printHelp(std::ostream& out, bool pretty = true) override;
   };
}

#endif // GNSSTK_COMMANDOPTIONNAVENUMHELP_HPP
