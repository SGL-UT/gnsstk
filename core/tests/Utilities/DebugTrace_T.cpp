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

#include "BasicFramework.hpp"
#include "DebugTrace.hpp"

using namespace std;

/**
 * This test is really just a compile-time test to make sure that
 * DebugTrace compiles on whatever platform it's being built on.
 */
class DebugTrace_T : public gnsstk::BasicFramework
{
public:
   DebugTrace_T(const string& applName)
         : gnsstk::BasicFramework(applName, "Compile-time test of DebugTrace")
   {}
   bool initialize(int argc, char *argv[], bool pretty = true) noexcept
      override
   {
      if (!BasicFramework::initialize(argc, argv, pretty))
         return false;
      if (debugLevel)
      {
         DEBUGTRACE_ENABLE();
      }
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("argc = " << argc);
      DEBUGTRACE("pretty = " << boolalpha << pretty << noboolalpha);
      return true;
   }
   void completeProcessing() override
   {
      DEBUGTRACE_FUNCTION();
      BasicFramework::completeProcessing();
   }
   void additionalSetup() override
   {
      DEBUGTRACE_FUNCTION();
      BasicFramework::additionalSetup();
   }
   void spinUp() override
   {
      DEBUGTRACE_FUNCTION();
      BasicFramework::spinUp();
   }
   void process() override
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE_DISABLE(); // if we don't want BasicFramework::process debug
      BasicFramework::process();
      if (debugLevel)
      {
            // reenable if desired
         DEBUGTRACE_ENABLE();
      }
      doSomething();
   }
   void shutDown() override
   {
      DEBUGTRACE_FUNCTION();
      BasicFramework::shutDown();
   }
   void doSomething()
   {
      DEBUGTRACE_FUNCTION();
      int foo;
      DEBUGTRACE("foo = " << foo);
   }
};

int main(int argc, char *argv[])
{
   try
   {
      DebugTrace_T app(argv[0]);
      if (!app.initialize(argc, argv))
         return app.exitCode;
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
