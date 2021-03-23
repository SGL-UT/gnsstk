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

#include "BasicFramework.hpp"
#include "SatMetaDataStore.hpp"

using namespace std;
using namespace gpstk;

/** Implement an application that checks a CSV file to make sure it is
 * valid satellite metadata. */
class SatMetaDataStoreCheck : public BasicFramework
{
public:
      /** Initialize command-line options.
       * @param[in] applName Application file name.
       */
   SatMetaDataStoreCheck(const string& applName);

      /** Load all specified input files.
       * @copydetails BasicFramework::initialize()
       */
   bool initialize(int argc, char* argv[], bool pretty=true) throw() override;

      /// Print out a message if no errors were found.
   void shutDown() override;

      /// command option for specifying the location of the satellite metadata.
   CommandOptionWithAnyArg satMetaOpt;
      /// Storage for PRN<->SVN translation.
   SatMetaDataStore satMetaDataStore;
};


SatMetaDataStoreCheck ::
SatMetaDataStoreCheck(const string& applName)
      : BasicFramework(applName, "Perform basic sanity checks on a"
                       " SatMetaDataStore CSV file"),
        satMetaOpt('M', "svconfig", "File containing satellite configuration"
                   " information for mapping SVN<->PRN", true)
{
}


bool SatMetaDataStoreCheck ::
initialize(int argc, char* argv[], bool pretty) throw()
{
   if (!BasicFramework::initialize(argc, argv))
      return false;

   for (unsigned i = 0; i < satMetaOpt.getCount(); i++)
   {
      if (!satMetaDataStore.loadData(satMetaOpt.getValue()[i]))
      {
         cerr << "Failed to load \"" << satMetaOpt.getValue()[i]
              << "\"" << endl;
         exitCode = 2;
         return false;
      }
   }
   return true;
}


void SatMetaDataStoreCheck ::
shutDown()
{
   if (exitCode == 0)
      cout << "Success" << endl;
}

int main(int argc, char *argv[])
{
   try
   {
      SatMetaDataStoreCheck app(argv[0]);
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
