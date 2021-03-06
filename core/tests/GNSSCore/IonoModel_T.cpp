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

#include "TestUtil.hpp"
#include "IonoModel.hpp"

using namespace gnsstk;
using namespace std;

class IonoModel_T
{
    public:
        IonoModel_T(){}
        ~IonoModel_T(){}
        int getSetTest( void );
        int equalityTest( void );
        int nonEqualityTest( void );
        int validTest( void );
        int exceptionTest( void );
    protected:
    private:
};


//------------------------------------------------------------
//------------------------------------------------------------
int IonoModel_T :: getSetTest( void )
{
   TUDEF( "IonoModel", "get and set" );

   const double a[4] = {1.,2.,3.,4.};
   const double b[4] = {5.,6.,7.,8.};
   gnsstk::IonoModel model;

   double gotA[4] = {0.,0.,0.,0.};
   double gotB[4] = {0.,0.,0.,0.};

   // Try to get data from an uninitialized model - should fail
   // becuase the model isn't yet valid.
   bool unsetResult = model.getModel(gotA, gotB);
   TUASSERT( unsetResult == false );

   // Attempt to set and then get a valid model - should succeed
   model.setModel(a, b);
   //model.dump();
   bool setResult = model.getModel(gotA, gotB);
   TUASSERT( setResult == true );

   bool aEqual = (a[0] == gotA[0]) && (a[1] == gotA[1]) && (a[2] == gotA[2]) && (a[3] == gotA[3]);
   TUASSERT( aEqual == true );

   bool bEqual = (b[0] == gotB[0]) && (b[1] == gotB[1]) && (b[2] == gotB[2]) && (b[3] == gotB[3]);
   TUASSERT( bEqual == true );

   TURETURN();
}


//------------------------------------------------------------
// Assert quality of class IonoModel operator ==
//------------------------------------------------------------

int IonoModel_T :: equalityTest( void )
{
   TestUtil test1( "IonoModel", "operator ==", __FILE__, __LINE__ );
   std::string test_desc = "IonoModel objects are created and compared to test operator == precision";
   std::string test_fail_equals = "These should be equal but they are not.";
   std::string test_false_equals = "These should NOT be equal but they are.";

   //Create many alpha and beta arrays which define the Ionospheric model
   double a[4] = {1.,2.,3.,4.};
   double b[4] = {4.,3.,2.,1.};
   double c[4] = {1.,2.,3.,4.};
   double d[4] = {4.,3.,2.,1.};
   double e[4] = {0.,0.,0.,0.};
   gnsstk::IonoModel Model1(a,b);
   gnsstk::IonoModel Model2(c,d);
   gnsstk::IonoModel Model3(a,e);

   test1.assert(   Model1 == Model2,  test_desc + test_fail_equals,  __LINE__  );
   test1.assert( !(Model1 == Model3), test_desc + test_false_equals, __LINE__  );

   return( test1.countFails() );
}


//------------------------------------------------------------
// Assert quality of class IonoModel operator !=
//------------------------------------------------------------
int IonoModel_T :: nonEqualityTest( void )
{
   TestUtil test2( "IonoModel", "operator !=", __FILE__, __LINE__ );
   std::string test_desc = "IonoModel objects are created and compared to test operator != precision";
   std::string test_fail_notequal = "These should be [not equal] but they are not [not equal].";
   std::string test_false_notequal = "These should NOT be [not equal] but they are [not equal].";

   //Create many alpha and beta arrays which define the Ionospheric model
   double a[4] = {1.,2.,3.,4.};
   double b[4] = {4.,3.,2.,1.};
   double c[4] = {1.,2.,3.,4.};
   double d[4] = {4.,3.,2.,1.};
   double e[4] = {0.,0.,0.,0.};
   gnsstk::IonoModel Model1(a,b);
   gnsstk::IonoModel Model2(c,d);
   gnsstk::IonoModel Model3(a,e);

   test2.assert( ( Model1 != Model2 )==false, test_desc + test_fail_notequal,  __LINE__  );
   test2.assert( ( Model1 != Model3 )==true,  test_desc + test_false_notequal, __LINE__  );

   return( test2.countFails() );
}


//------------------------------------------------------------
// Assert quality of class IonoModel method isValid()
//------------------------------------------------------------
int IonoModel_T :: validTest( void )
{
   TestUtil test3( "IonoModel", "isValid", __FILE__, __LINE__ );
   std::string test_desc = "";
   std::string test_fail = "";

   //Instantiate a blank almanac
   gnsstk::EngAlmanac blankAlmanac;

   //Create an alpha and a beta array which define the Ionospheric model
   double a[4] = {1.,2.,3.,4.};
   double b[4] = {4.,3.,2.,1.};

   //---------------------------------
   //Test to see if various IonoModel instantiations are valid
   //---------------------------------

   // Construct with no inputs
   test_desc = "IonoModel object created with no input parameters";
   test_fail = "should result in an invalid model but did not";
   gnsstk::IonoModel model_withNoParam;
   test3.assert( !model_withNoParam.isValid(), test_desc + test_fail, __LINE__ );

   // Construct with multiple inputs
   test_desc = "IonoModel object created with multiple inputs";
   test_fail = "should result in a valid model but did not";
   gnsstk::IonoModel model_withArray(a,b);
   test3.assert( model_withArray.isValid(), test_desc + test_fail, __LINE__ );

   // Construct with blank Alamanac as input
   test_desc = "IonoModel object created with a blank EngAlamanac";
   test_fail = "should result in an invalid model but did no";
   gnsstk::IonoModel model_withblankAlmanac( blankAlmanac );
   test3.assert( !model_withblankAlmanac.isValid(), test_desc + test_fail, __LINE__   );

   return( test3.countFails() );
}


//------------------------------------------------------------
// Test class Ionomodel, verify exceptions are thrown as expected
//------------------------------------------------------------
// Please note:  As of June 29,2006 I have not found a way to get the blankAlmanac
// exception to throw the way I wanted it to.  I have set it to assert fail so I can
// come back at a later date to fix it.
//------------------------------------------------------------
int IonoModel_T :: exceptionTest( void )
{
   TestUtil test4( "IonoModel", "exception", __FILE__, __LINE__ );
   std::string test_desc = "";
   std::string test_fail = "";
   std::string assert_message = "";

   //Default constructer for Almanac will give a blank almanac
   gnsstk::EngAlmanac blankAlmanac;

   //Set DayTime to the current system time
   gnsstk::CommonTime commonTime;

   //Use the default Geodetic constructer
   gnsstk::Position rxgeo;

   //Set el and az to 0 for ease of testing
   double svel = 0;
   double svaz = 0;

   //Easy alpha and beta for Ionospheric testing
   double a[4] = {1.,2.,3.,4.};
   double b[4] = {4.,3.,2.,1.};
   gnsstk::IonoModel Model(blankAlmanac);
   gnsstk::IonoModel goodModel(a,b);

   //----------------------------------------
   // getIon() exception handling test
   //----------------------------------------
   try
   {
      blankAlmanac.getIon(a,b);
      assert_message = "blankAlmanac.getIon(), This test should have thrown an InvalidRequest exception";
      test4.assert( false, assert_message, __LINE__ );
   }
   catch( gnsstk::InvalidRequest& exception_invalid )
   {
      assert_message = "blankAlmanac.getIon(), This test threw an InvalidRequest exception as expected";
      test4.assert( true, assert_message , __LINE__ );
   }
   catch(...)
   {
      assert_message = "blankAlmanac.getIon(), This test should have thrown an InvalidRequest but threw a different type of exception";
      test4.assert( false, assert_message , __LINE__ );
   }


   // //----------------------------------------
   // // What is this? Commenting it out until someone else figures it out.
   // //----------------------------------------
   //
   // try
   // {
   //     //Questioning why this isnt failing auto fail for now
   //     CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_THROW(gnsstk::IonoModel Model(blankAlmanac), gnsstk::Exception) );
   // }
   // catch( gnsstk::Exception& e )
   // {
   //
   // }

   //----------------------------------------
   try
   {
      Model.getCorrection( commonTime, rxgeo,svel, svaz, CarrierBand::L1 );
      assert_message = "getCorrection(), This test should have thrown an InvalidIonoModel exception";
      test4.assert( false, assert_message, __LINE__ );
   }
   catch( gnsstk::IonoModel::InvalidIonoModel& exception_invalid )
   {
      assert_message = "getCorrection(), This test threw an InvalidIonoModel exception as expected";
      test4.assert( true, assert_message, __LINE__ );
   }
   catch(...)
   {
      assert_message = "getCorrection(), This test should have thrown an InvalidRequest but threw a different type of exception";
      test4.assert( false, assert_message, __LINE__ );
   }

   //----------------------------------------
   try
   {
      goodModel.getCorrection( commonTime, rxgeo, svel, svaz, CarrierBand::L1 );
      assert_message = "getCorrection( L1 ), This test should NOT throw an exception";
      test4.assert( true, assert_message, __LINE__ );
   }
   catch(gnsstk::Exception& e)
   {
      assert_message = "getCorrection( L1 ), This test should NOT have thrown any exceptions but threw gnsstk::Exception";
      test4.assert( false, assert_message, __LINE__ );
   }
   catch(...)
   {
      assert_message = "getCorrection( L1 ), This test should NOT have thrown any exceptions but threw one anyway";
      test4.assert( false, assert_message, __LINE__ );
   }


   //----------------------------------------
   try
   {
      goodModel.getCorrection( commonTime, rxgeo, svel, svaz, CarrierBand::L2 );
      assert_message = "getCorrection( L2 ), This test should NOT throw an exception";
      test4.assert( true, assert_message, __LINE__ );
   }
   catch( gnsstk::Exception& e )
   {
      assert_message = "getCorrection( L2 ), This test should NOT have thrown any exceptions but threw gnsstk::Exception";
      test4.assert( false, assert_message, __LINE__ );
   }
   catch(...)
   {
      assert_message = "getCorrection( L2 ), This test should NOT have thrown any exceptions but threw one anyway";
      test4.assert( false, assert_message, __LINE__ );
   }


   //----------------------------------------
   try
   {
      goodModel.getCorrection( commonTime, rxgeo, 72., 45., CarrierBand::L1 );
      assert_message = "getCorrection( commonTime,rxgeo,72.,45.,Model.L1 ), This test should NOT throw an exception";
      test4.assert( true, assert_message, __LINE__ );
   }
   catch( gnsstk::Exception& e )
   {
      assert_message = "getCorrection( commonTime,rxgeo,72.,45.,Model.L1 ), This test should NOT have thrown any exceptions but threw gnsstk::Exception";
      test4.assert( false, assert_message, __LINE__ );
   }
   catch(...)
   {
      assert_message = "getCorrection( commonTime,rxgeo,72.,45.,Model.L1 ), This test should NOT have thrown any exceptions but threw one anyway";
      test4.assert( false, assert_message, __LINE__ );
   }

   return( test4.countFails() );
}


//------------------------------------------------------------
// main()
//------------------------------------------------------------
int main( void )
{
   int check, errorCounter = 0;
   IonoModel_T testClass;

   check = testClass.getSetTest();
   errorCounter += check;

   check = testClass.equalityTest();
   errorCounter += check;

   check = testClass.nonEqualityTest();
   errorCounter += check;

   check = testClass.validTest();
   errorCounter += check;

   check = testClass.exceptionTest();
   errorCounter += check;

   std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter << std::endl;

   return( errorCounter );
}
