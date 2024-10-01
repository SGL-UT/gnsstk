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

#include "Vector.hpp"
#include "TestUtil.hpp"
#include <iostream>


class Vector_T
{
   public:

   /* Tests the initialization of Vector objects */
   int initializationTest(void)
   {
      gnsstk::TestUtil testFramework("Vector","initializationTest",__FILE__,__LINE__);
      std::string failMesg;

      int badCount=0;

      gnsstk::Vector<double> v1(250,1.0);
      gnsstk::Vector<double> v2(1000,5.0);
      gnsstk::Vector<double> st(250); //Initialize without values
      gnsstk::Vector<double> Compare(v1); //Copy Constructor

      std::valarray<double> valarray1(10.0, 250);
      gnsstk::Vector<double> val(valarray1);

      int i = 0;

      for(i=0; i<v1.size(); i++)
      {
      failMesg = "Were the array values set to expectation?";
         if (1.0 != v1[i]) {badCount++;}
         if (1.0 != v1(i)) {badCount++;}
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);
      badCount = 0; // Reset error counter

      failMesg = "Was the size set to expectation?";
      testFramework.assert(i == v1.size(), failMesg, __LINE__);

      for(i=0; i<v2.size(); i++)
      {
      failMesg = "Were the array values set to expectation?";
         if (5.0 != v2[i]) {badCount++;}
         if (5.0 != v2(i)) {badCount++;}
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);
      badCount = 0; // Reset error counter

      failMesg = "Was the size set to expectation?";
      testFramework.assert(i == v2.size(), failMesg, __LINE__);

      failMesg = "Was the size set to expectation?";
      testFramework.assert(250 == st.size(), failMesg, __LINE__);

      for(i=0; i<Compare.size(); i++)
      {
      failMesg = "Were the array values set to expectation?";
         if (1.0 != Compare[i]) {badCount++;}
         if (1.0 != Compare(i)) {badCount++;}
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);
      badCount = 0; // Reset error counter

      failMesg = "Was the size set to expectation?";
      testFramework.assert(i == Compare.size(), failMesg, __LINE__);

      for(i=0; i<val.size(); i++)
      {
      failMesg = "Were the array values set to expectation?";
         if (10.0 != val[i]) {badCount++;}
         if (10.0 != val(i)) {badCount++;}
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);
      badCount = 0; // Reset error counter

      failMesg = "Was the size set to expectation?";
      testFramework.assert(i == val.size(), failMesg, __LINE__);

//===============================================================
// Add test for VectorBase and subvector constructors
//===============================================================

      return testFramework.countFails();
   }

   /* Tests addition and subtraction between vector objects */
   int operatorTest(void)
   {
      gnsstk::TestUtil testFramework( "Vector", "== Operator", __FILE__, __LINE__);
      std::string failMesg;

      gnsstk::Vector<double> v1(3,1.0);
      gnsstk::Vector<double> v2(12,3.0);
      gnsstk::Vector<double> v3 = v2;
      gnsstk::Vector<double> v4 = -v1;

      int badCount = 0;

      for(int i = 0; i < v2.size(); i++)
      {
      failMesg = "Are equivalent objects equivalent?";
         if (v2[i] != v3[i]) {badCount++;}
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);

      for(int i = 0; i < v1.size(); i++)
      {
      failMesg = "Are equivalent objects equivalent?";
         if (-v1[i] != v4[i]) {badCount++;}
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);

      testFramework.changeSourceMethod("+= Operator");
      v2 += v3; // 6 6 6 ...
      v2 += 2; // 8 8 8 ...

      for(int i = 0; i < v2.size(); i++)
      {
      failMesg = "Were the previous addition operators successful?";
         if (8. != v2[i]) {badCount++;}
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);

      testFramework.changeSourceMethod("+= Operator");
      v2 -= v3; // 5 5 5 ...
      v2 -= 4; // 1 1 1 ...

      for(int i = 0; i < v1.size(); i++)
      {
      failMesg = "Were the previous subtraction operators successful?";
         if (v1[i] != v2[i]) {badCount++;} //sizes mismatch, check till v1 ends
      }
      testFramework.assert(badCount==0, failMesg, __LINE__);

      v2 += 2; // 3 3 3 ...

      v1 = v1&&v2; // 3 3 3 3 3 3 ...

      failMesg = "Was the previous && operators successful in joining similar vectors?";
      testFramework.assert((size_t)15==v1.size(), failMesg, __LINE__);

      v1 = v1&&v3;

      failMesg = "Was the previous && operators successful in joining different vectors?";
      testFramework.assert((size_t)27==v1.size(), failMesg, __LINE__);

      return testFramework.countFails();
   }

   int baseOperatorTest() 
   {
      TUDEF("Vector", "VectorBaseOperators");
      gnsstk::Vector<double> a(3); 
      double data[3] = {1.0, 2.0, 3.0};
      a = data;
      std::stringstream ss;
      ss << a;
      std::cout << ss.str() << std::endl;
      TUASSERT(ss.str().find("2") != std::string::npos);
      // sum the vector
      TUASSERTE(double, sum(a), 6.0);
      // empty vector exception checks
      gnsstk::Vector<double> empty(0);
      TUTHROW(minabs(empty));
      TUTHROW(min(empty));
      TUTHROW(maxabs(empty));
      TUTHROW(max(empty));
      // non empty checks
      TUASSERTE(double, 1.0, minabs(a));
      TUASSERTE(double, 1.0, min(a));
      TUASSERTE(double, 3.0, maxabs(a));
      TUASSERTE(double, 3.0, max(a));
      // dot product of two vectors
      TUASSERTE(double, dot(a, a), 14.0);
      // norm of a vector
      TUASSERTFEPS(norm(a), SQRT(14.0), 1e-8);
      // Minkowski, exception if vector not length of 4
      TUTHROW(Minkowski(a, a);)
      gnsstk::Vector<double> four(4);
      double mink[4] = {1.0, 2.0, 3.0, 4.0};
      four = mink;
      // 1 + 4 + 9 - 16
      TUASSERTFEPS(Minkowski(four, four), -2.0, 1e-8);
      TUASSERTFEPS(cosVec(a, a), 1.0, 1e-8);
      // do all shortline operators
      TUASSERT(eq(a, a));
      TUASSERT(!ne(a, a));
      TUASSERT(!lt(a, a));
      TUASSERT(!gt(a, a));
      TUASSERT(ge(a, a));
      TUASSERT(le(a, a));
      TURETURN();
   }

   int baseNewBinaryOperatorsTest()
   {
      TUDEF("Vector", "VectorBaseBinaryOperator");
      gnsstk::Vector<int> a(3);
      int data1[3] = {1, 2, 3};
      a = data1;
      gnsstk::Vector<int> b(3);
      int data2[3] = {1, 1, 1};
      b = data2;
      gnsstk::Vector<int> c = a * b;
      TUASSERTE(int, 1, c[0]);
      TUASSERTE(int, 2, c[1]);
      TUASSERTE(int, 3, c[2]);
      c = a / b;
      TUASSERTE(int, 1, c[0]);
      TUASSERTE(int, 2, c[1]);
      TUASSERTE(int, 3, c[2]);
      c = a % b;
      TUASSERTE(int, 0, c[0]);
      TUASSERTE(int, 0, c[1]);
      TUASSERTE(int, 0, c[2]);
      c = a + b;
      TUASSERTE(int, 2, c[0]);
      TUASSERTE(int, 3, c[1]);
      TUASSERTE(int, 4, c[2]);
      c = a ^ b;
      TUASSERTE(int, 0, c[0]);
      TUASSERTE(int, 3, c[1]);
      TUASSERTE(int, 2, c[2]);
      c = a & b;
      TUASSERTE(int, 1, c[0]);
      TUASSERTE(int, 0, c[1]);
      TUASSERTE(int, 1, c[2]);
      c = a | b;
      TUASSERTE(int, 1, c[0]);
      TUASSERTE(int, 3, c[1]);
      TUASSERTE(int, 3, c[2]);
      gnsstk::Vector<bool> d = a == b;
      TUASSERT(d[0]);
      TUASSERT(!d[1]);
      TUASSERT(!d[2]);
      d = a < b;
      TUASSERT(!d[0]);
      TUASSERT(!d[1]);
      TUASSERT(!d[2]);
      d = a > b;
      TUASSERT(!d[0]);
      TUASSERT(d[1]);
      TUASSERT(d[2]);
      d = a != b;
      TUASSERT(!d[0]);
      TUASSERT(d[1]);
      TUASSERT(d[2]);
      d = a <= b;
      TUASSERT(d[0]);
      TUASSERT(!d[1]);
      TUASSERT(!d[2]);
      d = a >= b;
      TUASSERT(d[0]);
      TUASSERT(d[1]);
      TUASSERT(d[2]);
      TURETURN();
   }

   int baseNewUnaryOperatorsTest()
   {
      TUDEF("Vector", "VectorBaseNewUnaryOperator");
      gnsstk::Vector<double> a(3);
      double data1[3] = {1, 2, 3};
      a = data1;
      TUCATCH(std::abs(a));
      TUCATCH(std::acos(a));
      TUCATCH(std::asin(a));
      TUCATCH(std::atan(a));
      TUCATCH(std::cos(a));
      TUCATCH(std::cosh(a));
      TUCATCH(std::exp(a));
      TUCATCH(std::log(a));
      TUCATCH(std::log10(a));
      TUCATCH(std::sinh(a));
      TUCATCH(std::sin(a));
      TUCATCH(std::sqrt(a));
      TUCATCH(std::tan(a));
      TUCATCH(std::tanh(a));
      TURETURN();
   }

   int crossVectorTest()
   {
      TUDEF("Vector", "cross");
      gnsstk::Vector<double> bad(4);
      TUTHROW(cross(bad, bad));
      gnsstk::Vector<double> a(3);
      double data1[3] = {1, 2, 3};
      a = data1;
      gnsstk::Vector<double> b(3);
      double data2[3] = {1, 1, 1};
      b = data2;
      gnsstk::Vector<double> result = cross(a, b);
      TUASSERTFEPS(result[0], -1, 1e-8);
      TUASSERTFEPS(result[1], 2, 1e-8);
      TUASSERTFEPS(result[2], -1, 1e-8);
      TURETURN();
   }

   int normalizeVectorTest()
   {
      TUDEF("Vector", "normalize");
      gnsstk::Vector<double> a(3);
      double data1[3] = {1, 2, 3};
      a = data1;
      gnsstk::Vector<double> result = normalize(a); 
      TUASSERTFEPS(result[0], 0.26726124, 1e-8); 
      TUASSERTFEPS(result[1], 0.53452248, 1e-8);
      TUASSERTFEPS(result[2], 0.80178373, 1e-8);
      TURETURN();
   }

   int RMSVectorTest()
   {
      TUDEF("Vector", "RMS");
      gnsstk::Vector<double> a(3);
      double data1[3] = {1, 2, 3};
      a = data1;
      TUASSERTFEPS(2.160246899469287, RMS(a), 1e-8);
      TURETURN();
   }

   int RSSVectorTest()
   {
      TUDEF("Vector", "RSS");
      gnsstk::Vector<double> a(3);
      double data1[3] = {1, 2, 3};
      a = data1;
      TUASSERTFEPS(3.7416573867739413, RSS(a), 1e-8);
      TURETURN();
   }

   int NewBinaryTranscendentalOperatorsTest()
   {
      TUDEF("Vector", "NewBinaryTranscendentalOperators (atan2, pow)");
      gnsstk::Vector<double> a(3);
      double data1[3] = {4, 4, 4};
      a = data1;
      gnsstk::Vector<double> b(3);
      double data2[3] = {-3, -3, -3};
      b = data2;
      gnsstk::Vector<double> result = std::atan2(a, b);
      TUASSERTFEPS(result[0], 2.2142974355881808179, 1e-8);
      TUASSERTFEPS(result[1], 2.2142974355881808179, 1e-8);
      TUASSERTFEPS(result[2], 2.2142974355881808179, 1e-8);
      result = std::pow(a, a);
      // 4**4 is 256
      TUASSERTFEPS(result[0], 256, 1e-8);
      TUASSERTFEPS(result[1], 256, 1e-8);
      TUASSERTFEPS(result[2], 256, 1e-8);
      TURETURN();
   }

};

int main()
{
   int check, errorCounter = 0;
   Vector_T testClass;

   check = testClass.initializationTest();
   errorCounter += check;

   check = testClass.operatorTest();
   errorCounter += check;

   check = testClass.baseOperatorTest();
   errorCounter += check;

   check = testClass.baseNewUnaryOperatorsTest();
   errorCounter += check;

   check = testClass.baseNewBinaryOperatorsTest();
   errorCounter += check;

   check = testClass.crossVectorTest();
   errorCounter += check;

   check = testClass.normalizeVectorTest();
   errorCounter += check;

   check = testClass.RMSVectorTest();
   errorCounter += check;

   check = testClass.RSSVectorTest();
   errorCounter += check;

   check = testClass.NewBinaryTranscendentalOperatorsTest();
   errorCounter += check;

   std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter << std::endl;

   return errorCounter; //Return the total number of errors
}
