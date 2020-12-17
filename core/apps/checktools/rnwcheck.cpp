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

/** \page apps
 * - \subpage rnwcheck - Determine if a file is valid RINEX NAV
 * \page rnwcheck
 * \tableofcontents
 *
 * \section rnwcheck_name NAME
 * rnwcheck - Determine if a file is valid RINEX NAV
 *
 * \section rnwcheck_synopsis SYNOPSIS
 * \b rnwcheck [\argarg{OPTION}] ... [\argarg{FILE}] ...
 *
 * \section rnwcheck_description DESCRIPTION
 * Attempt to read files as RINEX NAV data, to determine if they are
 * correctly formatted.
 *
 * \dictionary
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-1, \--quit-on-first-error}
 * \dicdef{Quit on the first error encountered (default = no).}
 * \dicterm{-t, \--time=\argarg{TIME}}
 * \dicdef{Start of time range to compare (default = "beginning of time")}
 * \dicterm{-e, \--end-time=\argarg{TIME}}
 * \dicdef{End of time range to compare (default = "end of time")}
 * \enddictionary
 *
 * Time may be specified in one of three formats:
 * - month/day/year
 * - year day-of-year
 * - year day-of-year seconds-of-day
 *
 * \section rnwcheck_examples EXAMPLES
 *
 * \cmdex{rnwcheck data/arlm200a.15n data/arlm200z.15n}
 *
 * Will process the two files and return 0 on success.
 *
 * \todo Add an example or two using the time options.
 *
 * \section rnwcheck_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred\, input files are valid}
 * \dictentry{1,A C++ exception occurred\, or one or more files were invalid}
 * \enddictable
 *
 * \section rnwcheck_see_also SEE ALSO
 * \ref rmwcheck, \ref rowcheck
 */

#include "CheckFrame.hpp"

#include "Rinex3NavStream.hpp"
#include "Rinex3NavData.hpp"

using namespace std;
using namespace gpstk;

int main(int argc, char* argv[])
{
   try
   {
      CheckFrame<Rinex3NavStream, Rinex3NavData> cf(argv[0],
                                                  std::string("Rinex Nav"));
      
      if (!cf.initialize(argc, argv))
         return cf.exitCode;
      if (!cf.run())
         return cf.exitCode;
      
      return cf.exitCode;   
   }
   catch(gpstk::Exception& e)
   {
      cout << e << endl;
   }
   catch(std::exception& e)
   {
      cout << e.what() << endl;
   }
   catch(...)
   {
      cout << "unknown error" << endl;
   }
   return BasicFramework::EXCEPTION_ERROR;;
}
