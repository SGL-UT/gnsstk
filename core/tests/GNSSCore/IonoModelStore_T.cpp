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
#include "IonoModelStore.hpp"
#include "YDSTime.hpp"
#include <iostream>

   /// Test the IonoModelStore class
class IonoModelStoreTest
{
public: 

      /// Default Constructor
   IonoModelStoreTest();

      /// Default Destructor
   ~IonoModelStoreTest()
   { delete store; }

   int testConstructDestruct();
   int testAdd();
   int testGet();
   int testEdit();
   int testClear();

protected:

   const gnsstk::YDSTime t1;
   const gnsstk::YDSTime t2;
   const gnsstk::YDSTime t3;
   const gnsstk::YDSTime t4;

   const double a[4] = {1.,2.,3.,4.};
   const double b[4] = {5.,6.,7.,8.};

   gnsstk::IonoModelStore *store = nullptr;
};


IonoModelStoreTest :: IonoModelStoreTest()
   : t1(2021, 123,   0.0),
     t2(2021, 123, 300.0),
     t3(2021, 123, 900.0),
     t4(2021, 124,   0.0)
{
}


int IonoModelStoreTest :: testConstructDestruct()
{
   TUDEF( "IonoModelStore", "constructor/destructor" );

   TUCATCH( store = new gnsstk::IonoModelStore() );

   TUASSERTE( size_t, 0, store->size() );
   TUASSERT( true == store->empty() );

   TUCATCH( delete store );

   TURETURN( );
}


int IonoModelStoreTest :: testAdd()
{
   TUDEF( "IonoModelStore", "addIonoModel" );

   store = new gnsstk::IonoModelStore();

   const gnsstk::IonoModel emptyModel;
   const gnsstk::IonoModel validModel1(a, b);
   const gnsstk::IonoModel validModel2(b, a);

      // Attempt to add an invalid IonoModel - should fail
   TUASSERT( false == store->addIonoModel(t1, emptyModel) );
   TUASSERTE( size_t, 0, store->size() );
   TUASSERT( gnsstk::CommonTime::END_OF_TIME == store->getInitialTime() );
   TUASSERT( gnsstk::CommonTime::BEGINNING_OF_TIME == store->getFinalTime() );
      // Post: { }

      // Add an initial, valid IonoModel - should succeed
   TUASSERT( true == store->addIonoModel(t1, validModel1) );
   TUASSERTE( size_t, 1, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t1 == store->getFinalTime() );
      // Post: { (t1,m1) }

      // Add another, unique, valid IonoModel - should succeed
   TUASSERT( true == store->addIonoModel(t3, validModel2) );
   TUASSERTE( size_t, 2, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t3 == store->getFinalTime() );
      // Post: { (t1,m1), (t3,m2) }

      // Attempt to add a non-unique IonoModel - should fail
   TUASSERT( false == store->addIonoModel(t2, validModel1) );
   TUASSERTE( size_t, 2, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t3 == store->getFinalTime() );
      // Post: { (t1,m1), (t3,m2) }

      // Attempt to add another non-unique IonoModel - should fail
   TUASSERT( false == store->addIonoModel(t4, validModel2) );
   TUASSERTE( size_t, 2, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t3 == store->getFinalTime() );
      // Post: { (t1,m1), (t3,m2) }

      // Add another, unique, valid IonoModel - should succeed
   TUASSERT( true == store->addIonoModel(t2, validModel2) );
   TUASSERTE( size_t, 3, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t3 == store->getFinalTime() );
      // Post: { (t1,m1), (t2,m2), (t3,m2) }

      // Add another, unique, valid IonoModel - should succeed
   TUASSERT( true == store->addIonoModel(t4, validModel1) );
   TUASSERTE( size_t, 4, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t4 == store->getFinalTime() );
      // Post: { (t1,m1), (t2,m2), (t3,m2), (t4,m1) }

      // Replace an existing model at t3 - should succeed
   TUASSERT( true == store->addIonoModel(t3, validModel1) );
   TUASSERTE( size_t, 4, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t4 == store->getFinalTime() );
      // Post: { (t1,m1), (t2,m2), (t3,m1), (t4,m1) }

      // Replace an existing model at t3 - should succeed
   TUASSERT( true == store->addIonoModel(t3, validModel2) );
   TUASSERTE( size_t, 4, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t4 == store->getFinalTime() );
      // Post: { (t1,m1), (t2,m2), (t3,m2), (t4,m1) }

   store->dump();

   TURETURN( );
}


int IonoModelStoreTest :: testGet()
{
   TUDEF( "IonoModelStore", "getCorrection" );

      // This depends on successful execution of testAdd() above
      // to populate store.
      //
      // Pre: { (t1,m1), (t2,m2), (t3,m2), (t4,m1) }

   const gnsstk::Position pos(-740290.0, -5457071.7, 3207245.6);

   double az = 123.0;
   double el = 45.0;

   double correction;

   gnsstk::CommonTime t = t1;

      // Attempt to get a correction before any data exists - should throw.
   t -= 900.0;
   TUTHROW( correction = store->getCorrection(t, pos, el, az) );

      // Get a correction at the first available time - should succeed.
   t = t1;
   TUCATCH( correction = store->getCorrection(t, pos, el, az) );

      // Get a correction at another valid time - should succeed.
   t += 30.0;
   TUCATCH( correction = store->getCorrection(t, pos, el, az) );

      // Get a correction at another valid time - should succeed.
   t = t3;
   TUCATCH( correction = store->getCorrection(t, pos, el, az) );

      // Get a correction past the last stored time - should succeed.
   t = t4;
   t += 900.0;
   TUCATCH( correction = store->getCorrection(t, pos, el, az) );

      // Get a correction with unusual elevation - should succeed.
   el = -87.6;
   TUCATCH( correction = store->getCorrection(t, pos, el, az) );

      // Get a correction with unusual azimuth- should succeed.
   el = 45.0;
   az = -456.7;
   TUCATCH( correction = store->getCorrection(t, pos, el, az) );

   TURETURN( );
}


int IonoModelStoreTest :: testEdit()
{
   TUDEF( "IonoModelStore", "edit" );

      // This depends on successful execution of testAdd() above
      // to populate store.
      //
      // Pre: { (t1,m1), (t2,m2), (t3,m2), (t4,m1) }

   gnsstk::CommonTime first = t1;
   gnsstk::CommonTime last  = t4;

      // Call edit with a range encompassing all data - nothing should be removed.
   first -= 900.0;
   last  += 900.0;
   TUCATCH( store->edit(first, last) );
   TUASSERTE( size_t, 4, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t4 == store->getFinalTime() );

      // Call edit with a range whose lower bound is between t1 and t2 - nothing should be removed.
   first += 930.0;
   TUCATCH( store->edit(first, last) );
   TUASSERTE( size_t, 4, store->size() );
   TUASSERT( t1 == store->getInitialTime() );
   TUASSERT( t4 == store->getFinalTime() );
      // Post: { (t1,m1), (t2,m2), (t3,m2), (t4,m1) }

      // Call edit with a range whose lower bound is exactly t2 - the first model should be removed.
   first = t2;
   TUCATCH( store->edit(first, last) );
   TUASSERTE( size_t, 3, store->size() );
   TUASSERT( t2 == store->getInitialTime() );
   TUASSERT( t4 == store->getFinalTime() );
      // Post: { (t2,m2), (t3,m2), (t4,m1) }

      // Call edit with a range whose upper bound is exactly t4 - nothing should be removed.
   last = t4;
   TUCATCH( store->edit(first, last) );
   TUASSERTE( size_t, 3, store->size() );
   TUASSERT( t2 == store->getInitialTime() );
   TUASSERT( t4 == store->getFinalTime() );
      // Post: { (t2,m2), (t3,m2), (t4,m1) }

      // Call edit with a range whose upper bound is just before t4 - the last model should be removed.
   last -= 1.0;
   TUCATCH( store->edit(first, last) );
   TUASSERTE( size_t, 2, store->size() );
   TUASSERT( t2 == store->getInitialTime() );
   TUASSERT( t3 == store->getFinalTime() );
      // Post: { (t2,m2), (t3,m2) }

   TURETURN( );
}

int IonoModelStoreTest :: testClear()
{
   TUDEF( "IonoModelStore", "clear" );

      // This depends on successful execution of testEdit() above
      // to populate store.
      //
      // Pre: { (t2,m2), (t3,m2) }

   TUASSERT( false == store->empty() );
   TUCATCH( store->clear() );
   TUASSERTE( size_t, 0, store->size() );
   TUASSERT( true == store->empty() );
   TUASSERT( gnsstk::CommonTime::END_OF_TIME == store->getInitialTime() );
   TUASSERT( gnsstk::CommonTime::BEGINNING_OF_TIME == store->getFinalTime() );
      // Post: { }

   TURETURN( );
}


   /// Main function to initialize and run the program
int main()
{
   IonoModelStoreTest tester;

   int errorCount = 0;  // Total number of errors

   errorCount += tester.testConstructDestruct();
   errorCount += tester.testAdd();
   errorCount += tester.testGet();
   errorCount += tester.testEdit();
   errorCount += tester.testClear();

   std::cout << "Total Failures for " << __FILE__ << ": "
             << errorCount << std::endl;

   return errorCount;
}
