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

#include "DebugTrace.hpp"
#include "BasicFramework.hpp"
#include "RinexNavDataFactory.hpp"
#include "NewNavToRinex.hpp"
#include "TimeString.hpp"

using namespace std;
using namespace gnsstk;

/// Simple storage callback for use by NewNavToRinex
class NavDataStore : public NavDataFactoryCallback
{
public:
   bool process(const NavDataPtr& navOut) override
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("signal=" << navOut->signal);
      if (navOut->signal.messageType == NavMessageType::Health)
      {
         DEBUGTRACE(printTime(navOut->timeStamp,
                              "time=%Y/%02m/%02d %02H:%02M:%02S"));
            //navOut->dump(cerr, DumpDetail::Full);
         healthGet.healthMap[navOut->signal][navOut->timeStamp] = navOut;
      }
      ndlist.push_back(navOut);
      return true;
   }

   NavDataPtrList ndlist;
   HealthGetter healthGet;
};

/** This is a unit test for NewNavToRinex.  It is implemented a bit
 * differently than the typical unit tests in that it acts like an
 * application being tested, taking input and producing output.  The
 * validation of the output is separate from this process.  This is so
 * that we can implement a round-trip test. */
class NewNavToRinex_T : public BasicFramework
{
public:
   NewNavToRinex_T(const string& applName);

   bool initialize(int argc, char *argv[], bool pretty=true) throw() override;

   void process() override;

   CommandOptionRest restOpt;
};


NewNavToRinex_T ::
NewNavToRinex_T(const string& applName)
      : BasicFramework(applName, "Execute a basic round-trip conversion using"
                       " NewNavToRinex"),
        restOpt("input-file output-file", true)
{
   restOpt.setMaxCount(2);
}


bool NewNavToRinex_T ::
initialize(int argc, char *argv[], bool pretty) throw()
{
   if (!BasicFramework::initialize(argc, argv, pretty))
   {
      return false;
   }
   if (debugLevel)
   {
      DEBUGTRACE_ENABLE();
   }
   if (restOpt.getCount() != 2)
   {
      cerr << "Must have exactly two file arguments specified." << endl;
      exitCode = BasicFramework::OPTION_ERROR;
      return false;
   }
   return true;
}


void NewNavToRinex_T ::
process()
{
   Rinex3NavStream strm;
   RinexNavDataFactory fact;
   NavDataStore store;
   NewNavToRinex writer;
      // Just use the ones needed for RINEX conversion
   fact.setTypeFilter({NavMessageType::Ephemeris,
                       NavMessageType::Iono,
                       NavMessageType::TimeOffset,
                       NavMessageType::Health});
      // Get the header for re-use in the output.
   strm.open(restOpt.getValue()[0].c_str(), ios::in);
   if (!strm)
   {
      cerr << "Could not open \"" << restOpt.getValue()[0] << "\" for input"
           << endl;
      exitCode = BasicFramework::EXIST_ERROR;
      return;
   }
   strm >> writer.header;
   if (!strm)
   {
      cerr << "Failed to read RINEX header from \"" << restOpt.getValue()[0]
           << endl;
      exitCode = BasicFramework::GENERAL_ERROR;
      return;
   }
   strm.close();
   if (!fact.process(restOpt.getValue()[0], store))
   {
      cerr << "Unable to process RINEX input file \"" << restOpt.getValue()[0]
           << "\"" << endl
           << "Is it valid RINEX?" << endl;
      exitCode = BasicFramework::OPTION_ERROR;
      return;
   }
   cerr << "Stored " << store.ndlist.size() << " NavData objects" << endl;
   if (!writer.translate(store.ndlist, store.healthGet))
   {
      cerr << "Failed to translate data back into RINEX" << endl;
      exitCode = BasicFramework::GENERAL_ERROR;
      return;
   }
   if (!writer.write(restOpt.getValue()[1]))
   {
      cerr << "Failed to write RINEX output file \"" << restOpt.getValue()[1]
           << "\"" << endl;
      exitCode = BasicFramework::GENERAL_ERROR;
      return;
   }
   cout << "Done." << endl;
}


int main(int argc, char *argv[])
{
   try
   {
      NewNavToRinex_T app(argv[0]);
      if (!app.initialize(argc, argv))
      {
         return app.exitCode;
      }
      app.run();
      return app.exitCode;
   }
   catch(gnsstk::Exception& e)
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
   return gnsstk::BasicFramework::EXCEPTION_ERROR;
}
