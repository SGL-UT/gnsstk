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
 * - \subpage bc2sp3 - Generate a GPS calendar in text
 * \page bc2sp3
 * \tableofcontents
 *
 * \section bc2sp3_name NAME
 * bc2sp3 - Display time stamps in a variety of formats
 *
 * \section bc2sp3_synopsis SYNOPSIS
 * <b>bc2sp3</b>  <b>-h</b> <br/>
 * <b>bc2sp3</b> <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[\--in</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--out</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--tb</b>&nbsp;\argarg{TIME}<b>]</b> <b>[\--te</b>&nbsp;\argarg{TIME}<b>]</b> <b>[\--cs</b>&nbsp;\argarg{NUM}<b>]</b> <b>[\--outputC</b><b>]</b> <b>[\--msg</b>&nbsp;\argarg{ARG}<b>]</b> <b>[</b>\argarg{ARG}<b>]</b> <b>[</b>...<b>]</b>
 *
 * \section bc2sp3_description DESCRIPTION
 * This application reads RINEX navigation file(s) and writes to SP3
 * (a or c) file(s).
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
 * \dicterm{\--tb=\argarg{TIME}}
 * \dicdef{Output beginning epoch (week,sec-of-week)}
 * \dicterm{\--te=\argarg{TIME}}
 * \dicdef{Output ending epoch (week,sec-of-week)}
 * \dicterm{\--cs=\argarg{NUM}}
 * \dicdef{Cadence of epochs in seconds (default=300s)}
 * \dicterm{\--outputC}
 * \dicdef{Output SP3 version c (no correlation, default=a)}
 * \dicterm{\--msg=\argarg{ARG}}
 * \dicdef{Add a comment to the output header}
 * \enddictionary
 *
 * \subsection bc2sp3_example_merge Merge and Convert
 *
 * \cmdex{bc2sp3 \--in s121001a.00n \--in s121002a.00n \--out bc2sp3.out}
 *
 * Generate an SP3a file in bc2sp3.out on a 5-minute interval for the
 * satellites available in the two source files.  In this case, for
 * the span from January 1, 2000 00:00:00 through January 3, 2000
 * 02:00:00.
 *
 * \section bc2sp3_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred}
 * \enddictable
 */

   /**
    * @file bc2sp3.cpp
    * Read RINEX format navigation file(s) and write the data out to an SP3 format file.
    * Potential problems related to discontinuities at change of BCE are ignored.
    */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "RinexNavStream.hpp"
#include "RinexNavHeader.hpp"
#include "RinexNavData.hpp"
#include "XvtStore.hpp"
#include "GPSEphemerisStore.hpp"
#include "SP3Stream.hpp"
#include "SP3Header.hpp"
#include "SP3Data.hpp"
#include "CommonTime.hpp"
#include "SatID.hpp"
#include "StringUtils.hpp"
#include "TimeString.hpp"
#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "CommandOptionWithCommonTimeArg.hpp"

using namespace std;
using namespace gpstk;

class BC2SP3 : public BasicFramework
{
public:
   BC2SP3(const string& applName);
   void process() override;
   CommandOptionWithAnyArg inFileOpt;
   CommandOptionWithAnyArg outFileOpt;
   CommandOptionWithCommonTimeArg beginOpt;
   CommandOptionWithCommonTimeArg endOpt;
   CommandOptionWithNumberArg cadenceOpt;
   CommandOptionNoArg sp3cOpt;
   CommandOptionWithAnyArg msgOpt;
      /** The original implementation allowed either --in or trailing
       * arguments to indicate an input file name, so we do the
       * same... */
   CommandOptionRest inFile2Opt;
      /// Make sure at least one of inFileOpt and/or inFile2Opt is used
   CommandOptionOneOf inFileOneOf;
};


BC2SP3 ::
BC2SP3(const string& applName)
      : BasicFramework(applName, "Read RINEX nav file(s) and write to SP3(a or"
                       " c) file."),
        inFileOpt(0, "in", "Read the input file(s)"),
        inFile2Opt("[RINEX nav file] ..."),
        outFileOpt(0, "out", "Name the output file (default=sp3.out)"),
        beginOpt(0, "tb", "%F,%g", "Output beginning epoch (week,sec-of-week)"),
        endOpt(0, "te", "%F,%g", "Output ending epoch (week,sec-of-week)"),
        cadenceOpt(0, "cs", "Cadence of epochs in seconds (default=300s)"),
        sp3cOpt(0, "outputC", "Output SP3 version c (no correlation,"
                " default=a)"),
        msgOpt(0, "msg", "Add a comment to the output header")
{
   outFileOpt.setMaxCount(1);
   beginOpt.setMaxCount(1);
   endOpt.setMaxCount(1);
   cadenceOpt.setMaxCount(1);
   inFileOneOf.addOption(&inFileOpt);
   inFileOneOf.addOption(&inFile2Opt);
}


void BC2SP3 ::
process()
{
   try
   {
         //char version_out='a';
      SP3Header::Version versionOut(SP3Header::SP3a);
      int i,j;
      size_t k,nfile;
      string fileout("sp3.out");
      vector<string> inputFiles;
      vector<string> comments;
      map<SatID,long> IODEmap;
      CommonTime begTime=CommonTime::BEGINNING_OF_TIME;
      CommonTime endTime=CommonTime::END_OF_TIME;
      CommonTime tt;
      GPSEphemerisStore BCEph;
      SP3Header sp3header;
      SP3Data sp3data;
      double cadence = 300.0;       // Cadence of epochs.  Default to 5 minutes.

      if (sp3cOpt)
      {
         versionOut = SP3Header::SP3c;   //'c';
         if (verboseLevel)
            cout << " Output version c\n";
      }
      if (inFileOpt.getCount())
      {
         inputFiles = inFileOpt.getValue();
      }
      if (inFile2Opt.getCount())
      {
         const std::vector<std::string>& if2o(inFileOpt.getValue());
         inputFiles.insert(inputFiles.end(), if2o.begin(), if2o.end());
      }
      if (verboseLevel)
      {
         for (unsigned i = 0; i < inputFiles.size(); i++)
         {
            cout << " Input file name " << inputFiles[i] << endl;
         }
      }
      if (outFileOpt.getCount())
      {
         fileout = outFileOpt.getValue()[0];
         if (verboseLevel)
            cout << " Output file name " << fileout << endl;
      }
      if (beginOpt.getCount())
      {
         begTime = beginOpt.getTime()[0];
         if (verboseLevel)
         {
            cout << " Begin time "
                 << printTime(begTime,
                              "%Y/%02m/%02d %2H:%02M:%06.3f = %F/%10.3g")
                 << endl;
         }
      }
      if (endOpt.getCount())
      {
         endTime = endOpt.getTime()[0];
         if (verboseLevel)
         {
            cout << " End time   "
                 << printTime(endTime,
                              "%Y/%02m/%02d %2H:%02M:%06.3f = %F/%10.3g")
                 << endl;
         }
      }
      if (cadenceOpt.getCount())
      {
         cadence = StringUtils::asDouble(cadenceOpt.getValue()[0]);
         if (verboseLevel)
            cout << " Cadence    " << cadence << "s " << endl;
      }
      if (msgOpt.getCount())
      {
         comments = msgOpt.getValue();
         if (verboseLevel)
         {
            for (unsigned i = 0; i < comments.size(); i++)
            {
               cout << " Add comment " << comments[i] << endl;
            }
         }
      }

      bool existPass = true;
      for (nfile=0; nfile<inputFiles.size(); nfile++)
      {
         RinexNavStream rns(inputFiles[nfile].c_str());
         if (!rns)
         {
            cerr << "File " << inputFiles[nfile]
                 << " cannot be opened for input." << endl;
            existPass =false;
         }
      }
      if (!existPass)
      {
         exitCode = BasicFramework::EXIST_ERROR;
         return;
      }

         // open the output SP3 file
      SP3Stream outstrm(fileout.c_str(),ios::out);
      outstrm.exceptions(ifstream::failbit);

      for (nfile=0; nfile<inputFiles.size(); nfile++)
      {
         RinexNavHeader rnh;
         RinexNavData rnd;

         RinexNavStream rns(inputFiles[nfile].c_str());
         rns.exceptions(ifstream::failbit);

         if (verboseLevel)
            cout << "Reading file " << inputFiles[nfile] << endl;

         rns >> rnh;
         if (verboseLevel)
         {
            cout << "Input";
            rnh.dump(cout);
         }

         while (rns >> rnd)
         {
            if (rnd.health == 0)
            {
               BCEph.addEphemeris(rnd);
            }
         }
      }

      if (verboseLevel)
      {
         cout << "Number of ephemerides loaded: " << BCEph.size() << endl
              << " Initial time: " << printTime(BCEph.getInitialTime(),
                                                "%03j.%02H:%02M:%02S, %P")
              << endl
              << "   Final time: " << printTime(BCEph.getFinalTime(),
                                                "%03j.%02H:%02M:%02S, %P")
              << endl;
      }

         // time limits, if not given by user
      if (begTime == CommonTime::BEGINNING_OF_TIME)
         begTime = BCEph.getInitialTime();
      if (endTime == CommonTime::END_OF_TIME)
         endTime = BCEph.getFinalTime();

         // define the data version and the header info
      if (versionOut == SP3Header::SP3c)
      {
            // data and header must have the correct version
            //sp3data.version =
         sp3header.version = SP3Header::SP3c;

         sp3header.system = SP3SatID();
         sp3header.timeSystem = TimeSystem::GPS;
         sp3header.basePV = 0.0;
         sp3header.baseClk = 0.0;
      }
      else
      {
            //sp3data.version =
         sp3header.version = SP3Header::SP3a; //'a';
      }

         // fill the header
         //sp3header.pvFlag = 'V';
      sp3header.containsVelocity = true;
      sp3header.time = CommonTime::END_OF_TIME;
      sp3header.epochInterval = cadence; 
      sp3header.dataUsed = "BCE";
      sp3header.coordSystem = "WGS84";
      sp3header.orbitType = "   ";
      sp3header.agency = "ARL";

         // determine which SVs, with accuracy, start time, epoch interval,
         // number of epochs, for header
         // this is a pain....
      sp3header.numberOfEpochs = 0;
      tt = begTime;
      while (tt <= endTime)
      {
         bool foundSome = false;
         for (i=1; i<33; i++)              // for each PRN ...
         {
            SatID sat(i,SatelliteSystem::GPS);
            try
            {
               GPSEphemeris ee = BCEph.findEphemeris(sat, tt);
            }
            catch(InvalidRequest& nef)
            {
               continue;
            }

            if (sp3header.satList.find(sat) == sp3header.satList.end())
            {
               sp3header.satList[sat] = 0;        // sat accuracy = ?
               IODEmap[sat] = -1;
            }

            if (!foundSome)
            {
               sp3header.numberOfEpochs++;
               foundSome = true;
               if (tt < sp3header.time)
                  sp3header.time = tt;
            }
         }
         tt += sp3header.epochInterval;
      }

         // add comments
      if (comments.size() > 0)
      {
            // try to keep existing comments
         for (k=0; k<comments.size(); k++)
         {
            if (k > 3)
            {
               cout << "Warning - only 4 comments are allowed in SP3 header.\n";
               break;
            }
            sp3header.comments.push_back(comments[k]);
         }
      }

         // dump the SP3 header
      if (verboseLevel)
         sp3header.dump(cout);

         // write the header
      outstrm << sp3header;

         // sigmas to output (version c)
      for (j=0; j<4; j++)
         sp3data.sig[j]=0;   // sigma = ?

      tt = begTime;
      tt.setTimeSystem(TimeSystem::Any);
      while (tt <= endTime)
      {
         bool epochOut=false;

         for (i=1; i<33; i++)
         {
            long iode;
            SatID sat(i,SatelliteSystem::GPS);
            Xvt xvt;
            GPSEphemeris ee;

            try
            {
               ee = BCEph.findEphemeris(sat, tt);
            }
            catch(InvalidRequest& nef)
            {
               continue;
            }

            sp3data.sat = sat;
            xvt = BCEph.getXvt(sat, tt);

               // epoch
            if (!epochOut)
            {
               sp3data.time = tt;
               sp3data.RecType = '*';
               outstrm << sp3data;
               if (verboseLevel)
                  sp3data.dump(cout);
               epochOut = true;
            }

               // Position
            sp3data.RecType = 'P';
            for (j=0; j<3; j++)
               sp3data.x[j] = xvt.x[j]/1000.0;       // km
            sp3data.clk = xvt.clkbias * 1.0e6;    // microseconds

               //if (versionOut == 'c') for (j=0; j<4; j++) sp3data.sig[j]=...
            iode = ee.IODE;
            if (IODEmap[sat] == -1)
               IODEmap[sat] = iode;
            if (IODEmap[sat] != iode)
            {
               sp3data.orbitManeuverFlag = true;
               IODEmap[sat] = iode;
            }
            else
            {
               sp3data.orbitManeuverFlag = false;
            }

            outstrm << sp3data;
            if (verboseLevel)
               sp3data.dump(cout);

               // Velocity
            sp3data.RecType = 'V';
            for (j=0; j<3; j++)
               sp3data.x[j] = xvt.v[j] * 10.0;         // dm/s
            sp3data.clk = xvt.clkdrift * 1.0e10;                  // 10**-4 us/s
               //if (versionOut == 'c') for (j=0; j<4; j++) sp3data.sig[j]=...

            outstrm << sp3data;
            if (verboseLevel)
               sp3data.dump(cout);
         }

         tt += sp3header.epochInterval;
      }
         // don't forget this
         //outstrm << "EOF" << endl;

      outstrm.close();

      if (verboseLevel)
         cout << "Wrote " << sp3header.numberOfEpochs << " records" << endl;
   }
   catch (Exception& e)
   {
      GPSTK_RETHROW(e);
   }
}


int main(int argc, char* argv[])
{
   try
   {
      BC2SP3 app(argv[0]);
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
