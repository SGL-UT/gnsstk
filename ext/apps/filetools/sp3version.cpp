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
 * - \subpage sp3version - Convert SP3 files between format versions
 * \page sp3version
 * \tableofcontents
 *
 * \section sp3version_name NAME
 * sp3version - Convert SP3 files between format versions
 *
 * \section sp3version_synopsis SYNOPSIS
 * <b>sp3version</b>  <b>-h</b> <br/>
 * <b>sp3version</b> <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[\--in</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--out</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--outputC</b><b>]</b> <b>[\--msg</b>&nbsp;\argarg{ARG}<b>]</b> <b>[</b>\argarg{ARG}<b>]</b> <b>[</b>...<b>]</b>
 *
 * \section sp3version_description DESCRIPTION
 * This application reads an SP3 file (either a or c format) and
 * writes it to another file (also either in a or c format).
 *
 * \dictionary
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{\--in=\argarg{ARG}}
 * \dicdef{Read the input file(s)}
 * \dicterm{\--out=\argarg{ARG}}
 * \dicdef{Name the output file (default=sp3.out)}
 * \dicterm{\--outputC}
 * \dicdef{Output SP3 version c (default=a, random correlations are generated for a->c translation)}
 * \dicterm{\--msg=\argarg{ARG}}
 * \dicdef{Add a comment to the output header}
 * \enddictionary
 *
 * \section sp3version_examples EXAMPLES
 * \cmdex{sp3version sp3version --in data/test_input_SP3ae.sp3}
 * 
 * Generates an SP3c version of data/test_input_SP3ae.sp3 in sp3.out.
 *
 * \section sp3version_support SUPPORT
 * sp3version is not part of the gpstk core and thus testing and support
 * are limited to non-existent.
 *
 * \section sp3version_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred}
 * \enddictable
 */

/**
 * @file sp3version.cpp
 * Read an SP3 format file (any version) and write the data out to
 * another, specifying the SP3 version. This code is intended for
 * testing and as a pattern for SP3 I/O and analysis programs;
 * currently in converting SP3a to SP3c it inserts random data.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "SP3Stream.hpp"
#include "SP3Header.hpp"
#include "SP3Data.hpp"
#include "CommonTime.hpp"
#include "SatID.hpp"
#include "BasicFramework.hpp"

using namespace std;
using namespace gpstk;

class SP3Version : public BasicFramework
{
public:
   SP3Version(const string& applName);
   void process() override;
   double unitrand();
   CommandOptionWithAnyArg inFileOpt;
   CommandOptionWithAnyArg outFileOpt;
   CommandOptionNoArg sp3cOpt;
   CommandOptionWithAnyArg msgOpt;
      /** The original implementation allowed either --in or trailing
       * arguments to indicate an input file name, so we do the
       * same... */
   CommandOptionRest inFile2Opt;
      /// Make sure one and only one of inFileOpt and/or inFile2Opt is used
   CommandOptionMutex inFileMutex;
};


SP3Version ::
SP3Version(const string& applName)
      : BasicFramework(applName, "Read an SP3 file (either a or c format) and"
                       " write it to another"),
        inFileOpt(0, "in", "Read the input file(s)"),
        inFile2Opt("[SP3 file] ..."),
        outFileOpt(0, "out", "Name the output file (default=sp3.out)"),
        sp3cOpt(0, "outputC", "Output SP3 version c (default=a, random"
                " correlations are generated for a->c translation)"),
        msgOpt(0, "msg", "Add a comment to the output header"),
        inFileMutex(true)
{
   inFileOpt.setMaxCount(1);
   outFileOpt.setMaxCount(1);
   inFileMutex.addOption(&inFileOpt);
   inFileMutex.addOption(&inFile2Opt);
}


void SP3Version ::
process()
{
   try
   {
      SP3Header::Version versionIn, versionOut;
      int i,n;
      string filein,fileout("sp3.out");
      CommonTime currentTime=CommonTime::BEGINNING_OF_TIME;
      SP3Header sp3header;
      SP3Data sp3data;
      vector<string> comments;

      if (sp3cOpt)
      {
         versionOut = SP3Header::SP3c;   //'c';
      }
      if (inFileOpt.getCount())
      {
         filein = inFileOpt.getValue()[0];
      }
      else if (inFile2Opt.getCount())
      {
         filein = inFile2Opt.getValue()[0];
      }
      if (outFileOpt.getCount())
      {
         fileout = outFileOpt.getValue()[0];
      }
      if (msgOpt.getCount())
      {
         comments = msgOpt.getValue();
      }

      if (verboseLevel)
         cout << "Reading file " << filein << endl;

      SP3Stream instrm(filein.c_str());
      instrm.exceptions(ifstream::failbit);

      SP3Stream outstrm(fileout.c_str(),ios::out);
      outstrm.exceptions(ifstream::failbit);

         // read the header
      instrm >> sp3header;
      if (verboseLevel)
      {
         cout << "Input ";
         sp3header.dump(cout);
         cout << endl;
      }
      versionIn = sp3header.version;

         // add comments
      if (comments.size() > 0)
      {
            // try to keep existing comments
         for (i=0; i<4-int(comments.size()); i++)
            comments.push_back(sp3header.comments[i]);
         sp3header.comments.clear();
         for (i=0; i<int(comments.size()); i++)
         {
            sp3header.comments.push_back(comments[i]);
         }
      }

         // prepare to write the header
      if (versionOut == SP3Header::SP3c)
      {
         sp3header.version = SP3Header::SP3c; //'c';
         sp3header.system = SP3SatID();
         sp3header.timeSystem = TimeSystem::GPS;
            // make these up ... a real app would
         sp3header.basePV = 1.25;
            // assign them based on what SP3Data will hold
         sp3header.baseClk = 1.025;
      }
      if (verboseLevel)
      {
         cout << "Output ";
         sp3header.dump(cout);
         cout << endl;
      }

         // write the header
      outstrm << sp3header;

         // for reading and writing, sp3data MUST have the version of
         // the header; this is crucial for version 'c'
         //sp3data.version = versionIn;          // for input

      n = 0;     // count records
      while (instrm >> sp3data)
      {
            // data has now been read in
            // ...handle the data
         if (verboseLevel)
         {
            cout << "Input:\n";
            sp3data.dump(cout);
         }
            // if correlationFlag has been set, there is new correlation data
         if (sp3data.correlationFlag)
         {
            cout << "Input sdev";
            for (i=0; i<4; i++)
               cout << " " << sp3data.sdev[i];
            cout << endl;
            cout << "Input correl";
            for (i=0; i<6; i++)
               cout << " " << sp3data.correlation[i];
            cout << endl;
         }

            // output
            // write the epoch record
         if (sp3data.time > currentTime)
         {
            char saveRecType = sp3data.RecType;
            sp3data.RecType = '*';
               //outstrm << sp3data;
            sp3data.RecType = saveRecType;
            currentTime = sp3data.time;
         }

            // make up some data...a real app would have this data
         if (versionIn == SP3Header::SP3a && versionOut == SP3Header::SP3c)
         {
               // sigmas on the P|V rec
            for (i=0; i<4; i++)
               sp3data.sig[i] = int(99*unitrand());
               // RecType on the P line
            if (sp3data.RecType == 'P')
            {
               sp3data.clockEventFlag = (unitrand() > 0.5);
               sp3data.clockPredFlag = (unitrand() > 0.5);
               sp3data.orbitManeuverFlag = (unitrand() > 0.5);
               sp3data.orbitPredFlag = (unitrand() > 0.5);
            }
               // write out the correlation records ... maybe
            if (unitrand() > 0.5)
            {
                  // set the RecType for output
               sp3data.correlationFlag = true;
               for (i=0; i<4; i++)
                  sp3data.sdev[i] = int(9999*unitrand());
               for (i=0; i<6; i++)
                  sp3data.correlation[i] = int(99999999*unitrand());
               cout << "Output sdev";
               for (i=0; i<4; i++)
                  cout << " " << sp3data.sdev[i];
               cout << endl;
               cout << "Output correl";
               for (i=0; i<6; i++)
                  cout << " " << sp3data.correlation[i];
               cout << endl;
            }
            else
            {
               sp3data.correlationFlag = false;
            }
         }

            // write the data P|V record, and if correlationFlag, the
            // EP|EV record
         if (verboseLevel)
         {
            cout << "Output:\n";
            sp3data.dump(cout);
         }
         outstrm << sp3data;

            // count records
         n++;

            // prepare for the next read
            //sp3data.version = versionIn;
            // must reset before input, since same sp3data is for
            // input and output
         sp3data.correlationFlag = false;
      }

         // don't forget this
         //outstrm << "EOF" << endl;

      instrm.close();
      outstrm.close();

      if (verboseLevel)
         cout << "Read " << n << " records" << endl;
   }
   catch (Exception& e)
   {
      GPSTK_RETHROW(e);
   }
}


double SP3Version ::
unitrand()
{
   return double(rand())/RAND_MAX;
}


int main(int argc, char *argv[])
{
   try
   {
      SP3Version app(argv[0]);
      if (!app.initialize(argc, argv))
         return app.exitCode;
      app.run();
      return app.exitCode;
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
