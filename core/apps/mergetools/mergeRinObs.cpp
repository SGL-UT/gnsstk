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

/** \page apps
 * - \subpage mergeRinObs - Merge the contents of two or more RINEX OBS files
 * \page mergeRinObs
 * \tableofcontents
 *
 * \section mergeRinObs_name NAME
 * mergeRinObs - Merge the contents of two or more RINEX OBS files
 *
 * \section mergeRinObs_synopsis SYNOPSIS
 * \b mergeRinObs [\argarg{OPTION}] ... \argarg{FILE} ...
 *
 * \section mergeRinObs_description DESCRIPTION
 * Sorts and merges input RINEX Obs files into a single file. The
 * output will be sorted by time. This program assumes all the input
 * files are from the same station.
 *
 * \dictionary
 * \dicterm{-o, \--output}
 * \dicdef{Name for the merged output RINEX Obs file. Any existing file with that name will be overwritten.}
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \enddictionary
 *
 * \note mergeRinObs will load the entire contents of all input files into
 * memory before writing.
 *
 * \section mergeRinObs_examples EXAMPLES
 *
 * \cmdex{mergeRinObs -o merged.15o data/arlm200a.15o data/arlm200b.15o}
 *
 * Creates the file merged.15o containing the combined contents of
 * data/arlm200a.15o and data/arlm200b.15o.
 *
 * \section mergeRinObs_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred\, e.g. invalid input files}
 * \enddictable
 *
 * \section mergeRinObs_see_also SEE ALSO
 * \ref RinEdit, \ref mergeRinNav, \ref mergeRinMet
 */

// mergeRinObs
// Merge and sort rinex observation files

#include "RinexObsStream.hpp"
#include "RinexObsHeader.hpp"
#include "RinexObsData.hpp"
#include "RinexObsFilterOperators.hpp"
#include "FileFilterFrameWithHeader.hpp"
#include "SystemTime.hpp"
#include "CivilTime.hpp"

#include "MergeFrame.hpp"

using namespace std;
using namespace gpstk;

class MergeRinObs : public MergeFrame
{
public:
   MergeRinObs(char* arg0)
      : MergeFrame(arg0, 
                   std::string("RINEX Obs"),
                   std::string("The output will be sorted by time. This program assumes all the input files are from the same station."))
   {}

protected:
   virtual void process();
};

void MergeRinObs::process()
{
   std::vector<std::string> files = inputFileOption.getValue();

      // FFF will sort and merge the obs data using
      // a simple time check
   FileFilterFrameWithHeader<RinexObsStream, RinexObsData, RinexObsHeader> 
      fff(files);

      // get the header data
   RinexObsHeaderTouchHeaderMerge merged;
   fff.touchHeader(merged);

      // sort and filter the data using the obs set from the merged header
   fff.sort(RinexObsDataOperatorLessThanFull(merged.obsSet));
   fff.unique(RinexObsDataOperatorEqualsSimple());
   
      // set the time of first obs in the header
   merged.theHeader.firstObs = fff.front().time;

      // set the pgm/runby/date field
   merged.theHeader.fileProgram = std::string("mergeRinObs");
   merged.theHeader.fileAgency = std::string("gpstk");
   merged.theHeader.date = CivilTime(SystemTime()).asString();

      // write the file
   std::string outputFile = outputFileOption.getValue().front();
   fff.writeFile(outputFile, merged.theHeader);
}

int main(int argc, char* argv[])
{
   try
   {
      MergeRinObs m(argv[0]);
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
