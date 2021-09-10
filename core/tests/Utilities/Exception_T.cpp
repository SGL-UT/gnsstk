//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

#include "TestUtil.hpp"
#include "Exception.hpp"
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;

// part of the test for this macro
NEW_EXCEPTION_CLASS(TestExceptionClass, gnsstk::InvalidParameter);

class Exception_T
{
public:
      /** Test instantiation and throwing of the globally-defined
       * exception classes */
   int testThrowTypes();
      /** Check error id and severity preservation.  Tests the
       * following methods:
       * getErrorId()
       * isRecoverable()
       * getLocationCount()
       * getTextCount()
       * addText()
       * addLocation()
       * getLocation()
       * ExceptionLocation::getLineNumber()
       * ExceptionLocation::getFileName()
       * ExceptionLocation::getFunctionName()
       * setErrorId()
       * setSeverity()
       *
       * Macros:
       * GNSSTK_THROW()
       */
   int testErrorSeverity();
      /** Test macro function:
       * GNSSTK_RETHROW()
       * GNSSTK_ASSERT()
       * NEW_EXCEPTION_CLASS()
       */
   int testMacros();

private:
      /// Template function used by throwTypes()
   template <class EXC>
   int throwType();

      /// Throw an exception for testMacros
   void throwSomething(gnsstk::TestUtil& testFramework);
      /// Throw an exception for testMacros
   void rethrowSomething(gnsstk::TestUtil& testFramework);

   int cowLine1, cowLine2;
};


int Exception_T ::
testThrowTypes()
{
   int total = 0;
   total += throwType<gnsstk::Exception>();
   total += throwType<gnsstk::InvalidParameter>();
   total += throwType<gnsstk::InvalidRequest>();
   total += throwType<gnsstk::AssertionFailure>();
   total += throwType<gnsstk::AccessError>();
   total += throwType<gnsstk::IndexOutOfBoundsException>();
   total += throwType<gnsstk::InvalidArgumentException>();
   total += throwType<gnsstk::ConfigurationException>();
   total += throwType<gnsstk::FileMissingException>();
   total += throwType<gnsstk::SystemSemaphoreException>();
   total += throwType<gnsstk::SystemPipeException>();
   total += throwType<gnsstk::SystemQueueException>();
   total += throwType<gnsstk::OutOfMemory>();
   total += throwType<gnsstk::ObjectNotFound>();
   total += throwType<gnsstk::NullPointerException>();
   total += throwType<gnsstk::UnimplementedException>();
   return total;
}


int Exception_T ::
testErrorSeverity()
{
   TUDEF("Exception", "getErrorId()");
   unsigned loc1Line = 0, loc2Line = 0;
   int locFails = 0;
   std::string fn("testErrorSeverity");
   try
   {
      gnsstk::Exception exc("fail", 1234, gnsstk::Exception::recoverable);
         // do not separate these statements
      loc1Line = __LINE__; GNSSTK_THROW(exc);
      testFramework.changeSourceMethod("GNSSTK_THROW");
      TUFAIL("Did not throw an exception when expected");
   }
   catch (gnsstk::Exception &exc)
   {
      testFramework.changeSourceMethod("GNSSTK_THROW");
      TUPASS("GNSSTK_THROW");
      testFramework.changeSourceMethod("getErrorId");
      TUASSERTE(unsigned long, 1234, exc.getErrorId());
      testFramework.changeSourceMethod("isRecoverable");
      testFramework.assert(exc.isRecoverable(), "Incorrect severity", __LINE__);
      testFramework.changeSourceMethod("getLocationCount");
      TUASSERTE(size_t, 1, exc.getLocationCount());
      testFramework.changeSourceMethod("getTextCount");
      TUASSERTE(size_t, 1, exc.getTextCount());
      testFramework.changeSourceMethod("addText");
      exc.addText("another text");
      TUASSERTE(size_t, 2, exc.getTextCount());
      testFramework.changeSourceMethod("addLocation");
         // do not separate these statements
      loc2Line = __LINE__; gnsstk::ExceptionLocation here(__FILE__,fn,__LINE__);
      exc.addLocation(here);
      TUASSERTE(size_t, 2, exc.getLocationCount());
      if (exc.getLocationCount() == 2)
      {
         gnsstk::TestUtil test2("ExceptionLocation", "", __FILE__, __LINE__);
         gnsstk::ExceptionLocation loc1 = exc.getLocation();
         gnsstk::ExceptionLocation loc2 = exc.getLocation(1);
            // can't change class in gnsstk::TestUtil, oh well
         testFramework.changeSourceMethod("getLineNumber");
         TUASSERTE(unsigned long, loc1Line, loc1.getLineNumber());
         TUASSERTE(unsigned long, loc2Line, loc2.getLineNumber());
         testFramework.changeSourceMethod("getFileName");
         TUASSERTE(std::string, std::string(__FILE__), loc1.getFileName());
         TUASSERTE(std::string, std::string(__FILE__), loc2.getFileName());
         testFramework.changeSourceMethod("getFunctionName");
#if defined ( __FUNCTION__ )
         TUASSERTE(std::string, std::string(__FUNCTION__), loc1.getFunctionName());
#else
         TUASSERTE(std::string, std::string(""), loc1.getFunctionName());
#endif
         TUASSERTE(std::string, fn, loc2.getFunctionName());
      }
   }
   catch (...)
   {
      testFramework.changeSourceMethod("GNSSTK_THROW");
      TUFAIL("Threw an unexpected exception type");
   }
   try
   {
         // Initialize error id as 5678 then change it using
         // setErrorId to make sure that method works.  Likewise with
         // setSeverity()
      gnsstk::Exception exc("fail", 5678, gnsstk::Exception::recoverable);
      exc.setErrorId(9012);
      exc.setSeverity(gnsstk::Exception::unrecoverable);
      GNSSTK_THROW(exc);
      testFramework.changeSourceMethod("GNSSTK_THROW");
      TUFAIL("Did not throw an exception when expected");
   }
   catch (gnsstk::Exception &exc)
   {
      testFramework.changeSourceMethod("GNSSTK_THROW");
      TUPASS("GNSSTK_THROW");
      testFramework.changeSourceMethod("getErrorId");
      TUASSERTE(unsigned long, 9012, exc.getErrorId());
      testFramework.changeSourceMethod("isRecoverable");
      testFramework.assert(!exc.isRecoverable(), "Incorrect severity", __LINE__);
   }
   catch (...)
   {
      testFramework.changeSourceMethod("GNSSTK_THROW");
      TUFAIL("Threw an unexpected exception type");
   }
   return testFramework.countFails();
}


int Exception_T ::
testMacros()
{
   TUDEF("Exception", "macros");

      // test GNSSTK_ASSERT
   testFramework.changeSourceMethod("GNSSTK_ASSERT");
   try
   {
      GNSSTK_ASSERT(false);
      TUFAIL("Did not throw AssertionFailure exception");
   }
   catch (gnsstk::AssertionFailure &exc)
   {
      TUPASS("GNSSTK_ASSERT");
   }
   catch (...)
   {
      TUFAIL("Threw a different exception from AssertionFailure");
   }

      // test NEW_EXCEPTION_CLASS
   testFramework.changeSourceMethod("NEW_EXCEPTION_CLASS");
   try
   {
      TestExceptionClass exc("moo");
      GNSSTK_THROW(exc);
      TUFAIL("Did not throw TestExceptionClass");
   }
   catch (gnsstk::InvalidParameter)
   {
         // make sure the exception class is a child of InvalidParameter
      TUPASS("NEW_EXCEPTION_CLASS");
   }
   catch (...)
   {
      TUFAIL("Threw a different exception from InvalidParameter");
   }

      // test GNSSTK_RETHROW
   testFramework.changeSourceMethod("GNSSTK_RETHROW");
   try
   {
      rethrowSomething(testFramework);
      TUFAIL("rethrowSomething didn't throw exception");
   }
   catch(gnsstk::InvalidRequest& exc)
   {
      TUASSERTE(size_t, 2, exc.getLocationCount());
      if (exc.getLocationCount() == 2)
      {
         gnsstk::ExceptionLocation loc1 = exc.getLocation();
         gnsstk::ExceptionLocation loc2 = exc.getLocation(1);
         TUASSERTE(unsigned long, cowLine1, loc1.getLineNumber());
         TUASSERTE(unsigned long, cowLine2, loc2.getLineNumber());
      }
   }
   catch (...)
   {
      TUFAIL("rethrowSomething threw unexpected exception type");
   }

   return testFramework.countFails();
}


template <class EXC>
int Exception_T ::
throwType()
{
   TUDEF(typeid(EXC).name(), "GNSSTK_THROW");
   try
   {
      EXC exc("fail");
      GNSSTK_THROW(exc);
      TUFAIL("Did not throw an exception when expected");
   }
   catch (gnsstk::Exception &exc)
   {
         // all Exception classes are expected to be children of
         // gnsstk::Exception
      TUPASS("GNSSTK_THROW");
         // Removed - clearly not working and probably not being used.
         // Should the interface be removed from the Exception
         // implementation as well?
         //testFramework.changeSourceMethod("getName");
         //TUASSERTE(std::string, std::string(typeid(EXC).name()), exc.getName());
   }
   catch (...)
   {
      TUFAIL("Threw an unexpected exception type");
   }
   return testFramework.countFails();
}


void Exception_T ::
throwSomething(gnsstk::TestUtil& testFramework)
{
   gnsstk::InvalidRequest exc("cow");
   cowLine1 = __LINE__; GNSSTK_THROW(exc);
}


void Exception_T ::
rethrowSomething(gnsstk::TestUtil& testFramework)
{
   try
   {
      throwSomething(testFramework);
      TUFAIL("throwSomething didn't throw exception");
   }
   catch (gnsstk::InvalidRequest &exc)
   {
      TUPASS("throwSomething");
      cowLine2 = __LINE__; GNSSTK_RETHROW(exc);
   }
   catch (...)
   {
      TUFAIL("throwSomething threw unexpected exception type");
   }
}


int main(int argc, char *argv[])
{
   int errorTotal = 0;
   Exception_T testClass;

   errorTotal += testClass.testThrowTypes();
   errorTotal += testClass.testErrorSeverity();
   errorTotal += testClass.testMacros();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return errorTotal;
}
