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
 * - \subpage rmwdiff - Difference the contents of two RINEX MET files
 * \page rmwdiff
 * \tableofcontents
 *
 * \section rmwdiff_name NAME
 * rmwdiff - Difference the contents of two RINEX MET files
 *
 * \section rmwdiff_synopsis SYNOPSIS
 * \b rmwdiff [\argarg{OPTION}] ... file1 file2
 *
 * \section rmwdiff_description DESCRIPTION
 * Perform a simple difference between two RINEX MET files.
 *
 * \dictionary
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
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
 * \note rmwdiff will load the entire contents of both files into
 * memory before comparing.  This ignores any ordering differences.
 *
 * \section rmwdiff_examples EXAMPLES
 *
 * \cmdex{rmwdiff data/arlm2000.15m data/arlm2001.15m}
 *
 * Will show differences in individual records.  Records that appear
 * in the first file will start with "<" and records that appear in the
 * second file will start with ">".
 *
 * \todo Add an example or two using the time options.
 *
 * \section rmwdiff_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred\, or differences were encountered}
 * \dictentry{2,One or both of the input files does not exist}
 * \enddictable
 *
 * \section rmwdiff_see_also SEE ALSO
 * \ref rnwdiff, \ref rowdiff
 */

#include "FileFilterFrameWithHeader.hpp"

#include "RinexMetData.hpp"
#include "RinexMetStream.hpp"
#include "RinexMetFilterOperators.hpp"

#include "DiffFrame.hpp"
#include "YDSTime.hpp"

using namespace std;
using namespace gpstk;

class RMWDiff : public DiffFrame
{
public:
      /// Input file does not exist exit code
   static const int EXIST_ERROR = 2;
      /// Differences found in input files
   static const int DIFFS_CODE = 1;
   RMWDiff(char* arg0)
         : DiffFrame(arg0, 
                     std::string("RINEX Met"))
   {}

protected:
   virtual void process();
};


void RMWDiff::process()
{
   try
   {
      FileFilterFrameWithHeader<RinexMetStream, RinexMetData, RinexMetHeader>
         ff1(inputFileOption.getValue()[0]),
         ff2(inputFileOption.getValue()[1]);

         // find the obs data intersection
      RinexMetHeaderTouchHeaderMerge merged;

         // no data?  FIX make this program faster.. if one file
         // doesn't exist, there's little point in reading any.
      if (ff1.emptyHeader())
         cerr << "No header information for " << inputFileOption.getValue()[0]
              << endl;
      if (ff2.emptyHeader())
         cerr << "No header information for " << inputFileOption.getValue()[1]
              << endl;
      if (ff1.emptyHeader() || ff2.emptyHeader())
      {
         cerr << "Check that files exist." << endl;
         cerr << "diff failed." << endl;
         exitCode = EXIST_ERROR;
         return;
      }

      merged(ff1.frontHeader());
      merged(ff2.frontHeader());

      set<RinexMetHeader::RinexMetType> intersection = merged.obsSet;

      cout << "Comparing the following fields (other header data is ignored):"
           << endl;
      set<RinexMetHeader::RinexMetType>::iterator m = intersection.begin();
      while (m != intersection.end())
      {
         cout << RinexMetHeader::convertObsType(*m) << ' ';
         m++;
      }
      cout << endl;

      if (timeOptions.getCount())
      {
         ff1.filter(RinexMetDataFilterTime(startTime, endTime));
         ff2.filter(RinexMetDataFilterTime(startTime, endTime));
      }

      ff1.sort(RinexMetDataOperatorLessThanFull(intersection));
      ff2.sort(RinexMetDataOperatorLessThanFull(intersection));

      pair< list<RinexMetData>, list<RinexMetData> > difflist = 
         ff1.diff(ff2, RinexMetDataOperatorLessThanFull(intersection));

      if (difflist.first.empty() && difflist.second.empty())
      {
            // no differences
         exitCode = 0;
         return;
      }

         // differences found
      exitCode = DIFFS_CODE;

      list<RinexMetData>::iterator firstitr = difflist.first.begin();
      while (firstitr != difflist.first.end())
      {
         bool matched = false;
         list<RinexMetData>::iterator seconditr = difflist.second.begin();
         while ((!matched) && (seconditr != difflist.second.end()))
         {
            if (firstitr->time == seconditr->time)
            {
               YDSTime recTime(firstitr->time);
               cout << setw(3) << recTime.doy << ' ' 
                    << setw(10) << setprecision(0) << fixed
                    << recTime.sod << ' ' 
                    << ff1.frontHeader().markerName << ' '
                    << ff2.frontHeader().markerName << ' ';

               for (m = intersection.begin(); m != intersection.end(); m++)
               {
                  double diff = firstitr->data[*m];
                  diff -= seconditr->data[*m];

                  cout << setw(7) << setprecision(1) << fixed << diff << ' '
                       << RinexMetHeader::convertObsType(*m) << ' ';

               }
               cout << endl;

               firstitr = difflist.first.erase(firstitr);
               seconditr = difflist.second.erase(seconditr);
               matched = true;
            }
            else
               seconditr++;
         }

         if (!matched)
            firstitr++;
      }

      list<RinexMetData>::iterator itr = difflist.first.begin();
      while (itr != difflist.first.end())
      {
         cout << '<' << itr->stableText();
         itr++;
      }

      cout << endl;

      itr = difflist.second.begin();
      while (itr != difflist.second.end())
      {
         cout << '>' << itr->stableText();
         itr++;
      }

   }
   catch(Exception& e)
   {
      exitCode = BasicFramework::EXCEPTION_ERROR;
      cout << e << endl
           << endl
           << "Terminating.." << endl;
   }
   catch(std::exception& e)
   {
      exitCode = BasicFramework::EXCEPTION_ERROR;
      cout << e.what() << endl
           << endl
           << "Terminating.." << endl;
   }
   catch(...)
   {
      exitCode = BasicFramework::EXCEPTION_ERROR;
      cout << "Unknown exception... terminating..." << endl;
   }
}


int main(int argc, char* argv[])
{
   try
   {
      RMWDiff m(argv[0]);
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
