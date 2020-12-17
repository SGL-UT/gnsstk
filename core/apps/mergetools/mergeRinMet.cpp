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
 * - \subpage mergeRinMet - Merge the contents of two or more RINEX MET files
 * \page mergeRinMet
 * \tableofcontents
 *
 * \section mergeRinMet_name NAME
 * mergeRinMet - Merge the contents of two or more RINEX MET files
 *
 * \section mergeRinMet_synopsis SYNOPSIS
 * \b mergeRinMet [\argarg{OPTION}] ... \argarg{FILE} ...
 *
 * \section mergeRinMet_description DESCRIPTION
 * Sorts and merges input RINEX Met files into a single file. The
 * output will be sorted by time. This program assumes all the input
 * files are from the same station.
 *
 * \dictionary
 * \dicterm{-o, \--output}
 * \dicdef{Name for the merged output RINEX Met file. Any existing file with that name will be overwritten.}
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \enddictionary
 *
 * \note mergeRinMet will load the entire contents of all input files into
 * memory before writing.
 *
 * \section mergeRinMet_examples EXAMPLES
 *
 * \cmdex{mergeRinMet -o merged.15m data/arlm2000.15m data/arlm2001.15m}
 *
 * Creates the file merged.15m containing the combined contents of
 * data/arlm2000.15m and data/arlm2001.15m.
 *
 * \section mergeRinMet_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred\, e.g. invalid input files}
 * \enddictable
 *
 * \section mergeRinMet_see_also SEE ALSO
 * \ref mergeRinObs, \ref mergeRinNav
 */

// mergeRinMet
// Merge and sort rinex metrological files

#include "RinexMetStream.hpp"
#include "RinexMetHeader.hpp"
#include "RinexMetData.hpp"
#include "RinexMetFilterOperators.hpp"
#include "FileFilterFrameWithHeader.hpp"
#include "CivilTime.hpp"
#include "SystemTime.hpp"

#include "MergeFrame.hpp"

using namespace std;
using namespace gpstk;

class MergeRinMet : public MergeFrame
{
public:
   MergeRinMet(char* arg0)
      : MergeFrame(arg0, 
                   std::string("RINEX Met"),
                   std::string("The output will be sorted by time. This program assumes all the input files are from the same station."))
   {}
   
protected:
   virtual void process();
};

void MergeRinMet::process()
{
   std::vector<std::string> files = inputFileOption.getValue();

      // FFF will sort and merge the data using
      // a simple time check
   FileFilterFrameWithHeader<RinexMetStream, RinexMetData, RinexMetHeader> 
      fff(files);

      // get the header data
   RinexMetHeaderTouchHeaderMerge merged;
   fff.touchHeader(merged);

      // sort and filter the data
   fff.sort(RinexMetDataOperatorLessThanFull(merged.obsSet));
   fff.unique(RinexMetDataOperatorEqualsSimple());
   
      // set the pgm/runby/date field
   merged.theHeader.fileProgram = std::string("mergeRinMet");
   merged.theHeader.fileAgency = std::string("gpstk");
   merged.theHeader.date = CivilTime(SystemTime()).asString();

      // write the header
   std::string outputFile = outputFileOption.getValue().front();
   fff.writeFile(outputFile, merged.theHeader);
}

int main(int argc, char* argv[])
{
   try
   {
      MergeRinMet m(argv[0]);
      if (!m.initialize(argc, argv))
         return m.exitCode;
      if (!m.run())
         return m.exitCode;
      
      return m.exitCode;
   }
   catch(Exception& e)
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
      // only reach this point if an exception was caught
   return BasicFramework::EXCEPTION_ERROR;
}
