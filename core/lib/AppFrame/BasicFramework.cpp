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

/**
 * @file BasicFramework.cpp
 * Basic framework for programs in the GPS toolkit
 */


#include "Exception.hpp"
#include "BasicFramework.hpp"
#include "StringUtils.hpp"


namespace gnsstk
{

   using namespace std;


   BasicFramework :: BasicFramework( const std::string& applName,
                                     const std::string& applDesc )
      noexcept
         : debugLevel(0),
           verboseLevel(0),
           exitCode(0),
           argv0(applName),
           appDesc(applDesc),
           debugOption('d', "debug", "Increase debug level"),
           verboseOption('v', "verbose", "Increase verbosity"),
           helpOption('h', "help", "Print help usage")
   {} // End of constructor 'BasicFramework::BasicFramework()'



   bool BasicFramework :: initialize( int argc,
                                      char *argv[],
                                      bool pretty )
      noexcept
   {

         // Creating the parser here ensures that all the subclasses'
         // option objects are constructed.
      try
      {
         CommandOptionParser cop(appDesc);

         cop.parseOptions(argc, argv);

         if (cop.helpRequested())
         {
            cop.printHelp(cerr, pretty);
            return false;
         }

         if (cop.hasErrors())
         {
            cop.dumpErrors(cerr);
            cop.displayUsage(cerr, pretty);
            exitCode = OPTION_ERROR;
            return false;
         }

         debugLevel = debugOption.getCount();
         verboseLevel = verboseOption.getCount();
      }
      catch (gnsstk::Exception &exc)
      {
         cerr << exc << endl;
         exitCode=OPTION_ERROR;
         return false;
      }
      catch (std::exception &exc)
      {
         cerr << "BasicFramework::initialize caught " << exc.what() << endl;
         exitCode=OPTION_ERROR;
         return false;
      }
      catch (...)
      {
         cerr << "BasicFramework::initialize caught unknown exception" << endl;
         exitCode=OPTION_ERROR;
         return false;
      }

      return true;

   }  // End of method 'BasicFramework::initialize()'


   bool BasicFramework :: run()
      noexcept
   {

      try
      {
         completeProcessing();
      }
      catch (Exception& exc)
      {
         cerr << exc;
         exitCode = EXCEPTION_ERROR;
         return false;
      }
      catch (...)
      {
         cerr << "Caught unknown exception" << endl;
         exitCode = EXCEPTION_ERROR;
         return false;
      }

      shutDown();

      return true;

   }  // End of method 'BasicFramework::run()'



   void BasicFramework :: completeProcessing()
   {
      additionalSetup();

      spinUp();

      process();

   }  // End of method 'BasicFramework::completeProcessing()'


}  // End of namespace gnsstk
