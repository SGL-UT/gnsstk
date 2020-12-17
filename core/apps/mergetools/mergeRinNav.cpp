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
 * - \subpage mergeRinNav - Merge the contents of two or more RINEX NAV files
 * \page mergeRinNav
 * \tableofcontents
 *
 * \section mergeRinNav_name NAME
 * mergeRinNav - Merge the contents of two or more RINEX NAV files
 *
 * \section mergeRinNav_synopsis SYNOPSIS
 * \b mergeRinNav [\argarg{OPTION}] ... \argarg{FILE} ...
 *
 * \section mergeRinNav_description DESCRIPTION
 * Sorts and merges input RINEX Nav files into a single file. The
 * output will be sorted by time. This program assumes all the input
 * files are from the same station.
 *
 * \dictionary
 * \dicterm{-o, \--output}
 * \dicdef{Name for the merged output RINEX Nav file. Any existing file with that name will be overwritten.}
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \enddictionary
 *
 * \note mergeRinNav will load the entire contents of all input files into
 * memory before writing.
 *
 * \section mergeRinNav_examples EXAMPLES
 *
 * \cmdex{mergeRinNav -o merged.15n data/arlm2000.15n data/arlm2001.15n}
 *
 * Creates the file merged.15n containing the combined contents of
 * data/arlm2000.15n and data/arlm2001.15n.
 *
 * \section mergeRinNav_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred\, e.g. invalid input files}
 * \enddictable
 *
 * \section mergeRinNav_see_also SEE ALSO
 * \ref mergeRinObs, \ref mergeRinMet
 */

#include "Rinex3NavStream.hpp"
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavData.hpp"
#include "Rinex3NavFilterOperators.hpp"

#include "FileFilterFrameWithHeader.hpp"
#include "SystemTime.hpp"
#include "CivilTime.hpp"

#include "MergeFrame.hpp"

using namespace std;
using namespace gpstk;

class MergeRinNav : public MergeFrame
{
public:
   MergeRinNav(char* arg0)
      : MergeFrame(arg0, 
                   std::string("RINEX Nav"),
                   std::string("Only unique nav subframes will be output and they will be sorted by time."))
   {}

protected:
   virtual void process();
};

void MergeRinNav::process()
{
   std::vector<std::string> files = inputFileOption.getValue();

      // FFF will sort and merge the obs data using
      // a simple time check
   FileFilterFrameWithHeader<Rinex3NavStream, Rinex3NavData, Rinex3NavHeader> fff(files);

      // get the header data
   Rinex3NavHeaderTouchHeaderMerge merged;

   fff.touchHeader(merged);

      // sort and filter the data
   fff.sort(Rinex3NavDataOperatorLessThanFull());
   fff.unique(Rinex3NavDataOperatorEqualsFull());
   
      // set the pgm/runby/date field
   merged.theHeader.fileType = string("NAVIGATION");
   merged.theHeader.fileProgram = std::string("mergeRinNav");
   merged.theHeader.fileAgency = std::string("gpstk");
   merged.theHeader.date = CivilTime(SystemTime()).asString();
   merged.theHeader.version = 2.1;
   merged.theHeader.valid |= gpstk::Rinex3NavHeader::validVersion;
   merged.theHeader.valid |= gpstk::Rinex3NavHeader::validRunBy;
   merged.theHeader.valid |= gpstk::Rinex3NavHeader::validComment;
   merged.theHeader.valid |= gpstk::Rinex3NavHeader::validEoH;

      // write the header
   std::string outputFile = outputFileOption.getValue().front();
   fff.writeFile(outputFile, merged.theHeader);
}

int main(int argc, char* argv[])
{
   try
   {
      MergeRinNav m(argv[0]);
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
