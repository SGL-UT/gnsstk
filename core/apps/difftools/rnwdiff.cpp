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
 * - \subpage rnwdiff - Difference the contents of two RINEX NAV files
 * \page rnwdiff
 * \tableofcontents
 *
 * \section rnwdiff_name NAME
 * rnwdiff - Difference the contents of two RINEX NAV files
 *
 * \section rnwdiff_synopsis SYNOPSIS
 * \b rnwdiff [\argarg{OPTION}] ... file1 file2
 *
 * \section rnwdiff_description DESCRIPTION
 * Perform a simple difference between two RINEX NAV files.
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
 * \dicterm{-p, \--precision=\argarg{ARG}}
 * \dicdef{Ignore diffs smaller than (data * (10 ^ -\argarg{ARG}). Default = 13}
 * \enddictionary
 *
 * Time may be specified in one of three formats:
 * - month/day/year
 * - year day-of-year
 * - year day-of-year seconds-of-day
 *
 * \note rnwdiff will load the entire contents of both files into
 * memory before comparing.  This ignores any ordering differences.
 *
 * \section rnwdiff_examples EXAMPLES
 *
 * \cmdex{rnwdiff data/arlm200a.15n data/arlm200z.15n}
 *
 * Will show differences in individual records.  Records that appear
 * in the first file will start with "<" and records that appear in
 * the second file will start with ">".  Records with the same epoch
 * and satellite ID will be printed with a timestamp followed by the
 * differences between the two nav data records.
 *
 * \todo Add an example or two using the time options.
 *
 * \section rnwdiff_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred\, or differences were encountered}
 * \dictentry{2,One or both of the input files does not exist}
 * \enddictable
 *
 * \section rnwdiff_see_also SEE ALSO
 * \ref rmwdiff, \ref rowdiff
 */

#include <RINEX3/Rinex3ObsFilterOperators.hpp>
#include "FileFilterFrameWithHeader.hpp"

#include "Rinex3NavData.hpp"
#include "Rinex3NavStream.hpp"
#include "Rinex3NavFilterOperators.hpp"

#include "DiffFrame.hpp"
#include "YDSTime.hpp"

using namespace std;
using namespace gpstk;

class RNWDiff : public DiffFrame
{
public:
      /// Input file does not exist exit code
   static const int EXIST_ERROR = 2;
      /// Differences found in input files
   static const int DIFFS_CODE = 1;
   RNWDiff(char* arg0)
         : DiffFrame(arg0, 
                     std::string("RINEX Nav")),
           precisionOption('p',"precision", "Ignore diffs smaller than "
              "(data * (10 ^ -ARG). Default = 13")
   {}
   virtual bool initialize(int argc, char* argv[]) throw();

protected:
   virtual void process();
   gpstk::CommandOptionWithAnyArg precisionOption;

private:
   int precision;
   static const int DEFAULT_PRECISION = 13;
};

bool RNWDiff::initialize(int argc, char* argv[]) throw()
{
   if (!DiffFrame::initialize(argc, argv))
   {
      return false;
   }
   if (precisionOption.getCount())
   {
      precision = atoi(precisionOption.getValue()[0].c_str());
   }
   else
   {
      precision = DEFAULT_PRECISION;
   }
   return true;
}
void RNWDiff::process()
{
   try
   {
      FileFilterFrameWithHeader<Rinex3NavStream, Rinex3NavData, Rinex3NavHeader>
         ff1(inputFileOption.getValue()[0]),
         ff2(inputFileOption.getValue()[1]);

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
      Rinex3NavDataOperatorLessThanFull op;
         // Always sort by the default to mantain organization
      op.setPrecision(DEFAULT_PRECISION);
      ff1.sort(op);
      ff2.sort(op);

      //set desired precision for diffs
      op.setPrecision(precision);

      pair< list<Rinex3NavData>, list<Rinex3NavData> > difflist =
         ff1.diff(ff2, op);
      
      if (difflist.first.empty() && difflist.second.empty())
      {
         cout << "no differences were found" << endl;
            // no differences
         exitCode = 0;
         return;
      }

         // differences found
      exitCode = DIFFS_CODE;

      list<Rinex3NavData>::iterator firstitr = difflist.first.begin();
      while (firstitr != difflist.first.end())
      {
         bool matched = false;
         list<Rinex3NavData>::iterator seconditr = difflist.second.begin();
         while ((!matched) && (seconditr != difflist.second.end()))
         {
               // this will match the exact same nav message in both
               // files, not just the same ephemeris broadcast at
               // different times.
            if ((firstitr->time == seconditr->time) &&
                (firstitr->PRNID == seconditr->PRNID) &&
                (firstitr->xmitTime == seconditr->xmitTime) )
            {
               YDSTime recTime(firstitr->time);
               cout << fixed << setw(3) << recTime.doy << ' ' 
                    << setw(10) << setprecision(0)
                    << recTime.sod << ' ' 
                    << setw(19) << setprecision(12) << scientific
                    << (firstitr->af0      - seconditr->af0) << ' '
                    << (firstitr->af1      - seconditr->af1) << ' '
                    << (firstitr->af2      - seconditr->af2) << ' '
                    << (firstitr->IODE     - seconditr->IODE) << ' '
                    << (firstitr->Crs      - seconditr->Crs) << ' '
                    << (firstitr->dn       - seconditr->dn) << ' '
                    << (firstitr->M0       - seconditr->M0) << ' '
                    << (firstitr->Cuc      - seconditr->Cuc) << ' '
                    << (firstitr->ecc      - seconditr->ecc) << ' '
                    << (firstitr->Cus      - seconditr->Cus) << ' '
                    << (firstitr->Ahalf    - seconditr->Ahalf) << ' '
                    << (firstitr->Toe      - seconditr->Toe) << ' '
                    << (firstitr->Cic      - seconditr->Cic) << ' '
                    << (firstitr->OMEGA0   - seconditr->OMEGA0) << ' '
                    << (firstitr->Cis      - seconditr->Cis) << ' '
                    << (firstitr->i0       - seconditr->i0) << ' '
                    << (firstitr->Crc      - seconditr->Crc) << ' '
                    << (firstitr->w        - seconditr->w) << ' '
                    << (firstitr->OMEGAdot - seconditr->OMEGAdot) << ' '
                    << (firstitr->idot     - seconditr->idot) << ' '
                    << (firstitr->codeflgs - seconditr->codeflgs) << ' '
                    << (firstitr->weeknum  - seconditr->weeknum) << ' '
                    << (firstitr->L2Pdata  - seconditr->L2Pdata) << ' '
                    << (firstitr->accuracy - seconditr->accuracy) << ' '
                    << (firstitr->health   - seconditr->health) << ' '
                    << (firstitr->Tgd      - seconditr->Tgd) << ' '
                    << (firstitr->IODC     - seconditr->IODC) << ' '
                    << (firstitr->xmitTime - seconditr->xmitTime) << ' '
                    << (firstitr->fitint   - seconditr->fitint)
                    << endl;

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

      list<Rinex3NavData>::iterator itr = difflist.first.begin();
      while (itr != difflist.first.end())
      {
         cout << '<' << itr->dumpString() << endl;
         itr++;
      }

      cout << endl;

      itr = difflist.second.begin();
      while (itr != difflist.second.end())
      {
         cout << '>' << itr->dumpString() << endl;
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
      RNWDiff m(argv[0]);
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
