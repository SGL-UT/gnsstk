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

#include "Matrix_T.hpp"
#include <iostream>

using namespace std;

/* Test the add/subtract and set operators */
int Matrix_T::operatorTest(void)
{
    gnsstk::TestUtil testFramework("Matrix", "Operators", __FILE__, __LINE__);
    std::string failMesg;
    int badCount = 0;

    gnsstk::Vector<int> v1 = gnsstk::Vector<int>(16);
    for (int i = 0; i < 16; i++)
    	v1[i] = i + 1;
    gnsstk::Vector<int> v2 = gnsstk::Vector<int>(16);
    for (int i = 0; i < 16; i++)
    	v2[i] = 16 - i;
    gnsstk::Vector<int> v3 = gnsstk::Vector<int>(4);
    for (int i = 0; i < 4; i++)
    	v3[i] = i + 1;
    gnsstk::Vector<int> v4 = gnsstk::Vector<int>(8);
    for (int i = 0; i < 4; i++)
    {
    	v4[i] = i + 1 + 4;
    	v4[i + 4] = i + 1 + 4;
    }

    gnsstk::Matrix<int> a(2, 2, 1);
    gnsstk::Matrix<int> b(8, 2, 3);
    gnsstk::Matrix<int> c(4, 2, 5);
    gnsstk::Matrix<int> d(4, 4, 7);
    gnsstk::Matrix<int> e(8, 2, v1);
    gnsstk::Matrix<int> f(4, 4, v2);
    gnsstk::Matrix<int> g(2, 2, v3);
    gnsstk::Matrix<int> h(4, 2, v4);

    //--------------Matrix_operatorTest_1 - Does 2x2 additon function as expected?
    a += g; // 2x2 addition
    for (int i = 0; i < a.rows(); i++)
    	for (int j = 0; j < a.cols(); j++)
    		if (v3(i * g.cols() + j) + 1 != a(i, j))
    		{
    			badCount++;
    		}
    failDescriptionStream << "Check to see if addition between two 2x2 gnsstk::Matrix objects works. " << badCount << " of the elements are incorrect.";
    failDescriptionString = failDescriptionStream.str();
    failDescriptionStream.str("");
    testFramework.assert(badCount == 0, failDescriptionString, __LINE__);
    badCount = 0; // Reset error counter;
    a -= g;		  // 2x2 reset a

    //--------------Matrix_operatorTest_2 - Does 8x2 subtraction work as expected?
    b -= e; // 8x2 subtraction
    for (int i = 0; i < b.rows(); i++)
    	for (int j = 0; j < b.cols(); j++)
    		//cout << b(i,j) << " " << 3-(1+i+j*8) << endl;
    		if (3 - v1(i * e.cols() + j) != b(i, j))
    		{
    			badCount++;
    		}
    failDescriptionStream << "Check to see if subtraction between two 8x2 gnsstk::Matrix objects works. " << badCount << " of the elements are incorrect.";
    failDescriptionString = failDescriptionStream.str();
    failDescriptionStream.str("");
    testFramework.assert(badCount == 0, failDescriptionString, __LINE__);
    badCount = 0; // Reset error counter;
    b += e;		  // 8x2 reset b

    //--------------Matrix_operatorTest_3 - Does 4x2 subtraction work as expected?
    c -= h; // 4x2 subtraction
    for (int i = 0; i < c.rows(); i++)
    	for (int j = 0; j < c.cols(); j++)
    		//cout << c(i,j) << " " << 5-(5.+i) << endl;
    		if (5 - v4(i * h.cols() + j) != c(i, j))
    		{
    			badCount++;
    		}
    failDescriptionStream << "Check to see if subtraction between two 4x2 gnsstk::Matrix objects works. " << badCount << " of the elements are incorrect.";
    failDescriptionString = failDescriptionStream.str();
    failDescriptionStream.str("");
    testFramework.assert(badCount == 0, failDescriptionString, __LINE__);
    badCount = 0; // Reset error counter;
    c += h;		  // 4x2 reset c

    //--------------Matrix_operatorTest_4 - Does 4x4 addition work as expected?
    d += f; // 4x4 addition
    for (int i = 0; i < d.rows(); i++)
    	for (int j = 0; j < d.cols(); j++)
    		//cout << d(i,j) << " " << 16-i-4*j+7 << endl;
    		if (v2(i * f.cols() + j) + 7 != d(i, j))
    		{
    			badCount++;
    		}
    failDescriptionStream << "Check to see if addition between two 4x4 gnsstk::Matrix objects works. " << badCount << " of the elements are incorrect.";
    failDescriptionString = failDescriptionStream.str();
    failDescriptionStream.str("");
    testFramework.assert(badCount == 0, failDescriptionString, __LINE__);
    badCount = 0; // Reset error counter
    d -= f;		  // 4x4 reset d

    return testFramework.countFails();
}

int Matrix_T::andOpTest(void)
{
    TUDEF("MatrixOperators", "operator&&");
    // matrix matrix valid
    // remember constructor is rows, cols, init
    gnsstk::Matrix<int> a(2, 2, 1);
    gnsstk::Matrix<int> b(4, 2, 1);
    gnsstk::Matrix<int> c = a && b;
    TUASSERTE(int, c.rows(), 6);
    TUASSERTE(int, c.cols(), 2);
    // matrix matrix error
    gnsstk::Matrix<int> d(4, 3, 1);
    TUTHROW(b && d);
    // matrix vector, and the reverse direction
    gnsstk::Vector<int> e(2);
    c = a && e;
    TUASSERTE(int, c.rows(), 3);
    TUASSERTE(int, c.cols(), 2);
    c = e && a;
    TUASSERTE(int, c.rows(), 3);
    TUASSERTE(int, c.cols(), 2);
    // matrix vector error and the reverse direction
    gnsstk::Vector<int> f(3);
    TUTHROW(a && f);
    TUTHROW(f && a);
    TURETURN();
}

int Matrix_T::orOpTest(void)
{
    TUDEF("MatrixOperators", "operator||");
    // matrix matrix valid
    // remember constructor is rows, cols, init
    gnsstk::Matrix<int> a(2, 2, 1);
    gnsstk::Matrix<int> b(2, 4, 1);
    gnsstk::Matrix<int> c = a || b;
    TUASSERTE(int, c.cols(), 6);
    TUASSERTE(int, c.rows(), 2);
    // matrix matrix error
    gnsstk::Matrix<int> d(3, 4, 1);
    TUTHROW(b || d);
    // matrix vector, and the reverse direction
    gnsstk::Vector<int> e(2);
    c = a || e;
    TUASSERTE(int, c.cols(), 3);
    TUASSERTE(int, c.rows(), 2);
    c = e || a;
    TUASSERTE(int, c.cols(), 3);
    TUASSERTE(int, c.rows(), 2);
    // matrix vector error and the reverse direction
    gnsstk::Vector<int> f(3);
    TUTHROW(a || f);
    TUTHROW(f || a);
    // vector vector
    gnsstk::Vector<int> g(5);
    gnsstk::Vector<int> h(5);
    gnsstk::Matrix<int> i = g || h;
    TUASSERTE(int, i.cols(), 2)
    TUASSERTE(int, i.rows(), 5)
    TURETURN();
}

int Matrix_T::minorMatrixTest(void)
{
    TUDEF("MatrixOperators", "minorMatrixTest");
    int i, j;
    //  1  4  7
    //  3  0  5
    // -1  9 11
    int data[9] = {1, 4, 7, 3, 0, 5, -1, 9, 11};
    gnsstk::Matrix<int> a(3, 3);
    a = data;
    // invalid row exception for too high or a row or column
    TUTHROW(minorMatrix(a, 1, 9));
    TUTHROW(minorMatrix(a, 9, 1));
    // this thing has corner cases so basically, we need to do all 9 combinations
    gnsstk::Matrix<int> result = minorMatrix(a, 0, 0);
    gnsstk::Matrix<int> expected(2, 2);
    int *expectedData = new int[4]{0, 5, 9, 11};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 0, 1);
    expectedData = new int[4]{3, 5, -1, 11};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 0, 2);
    delete[] expectedData;
    expectedData = new int[4]{3, 0, -1, 9};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 1, 0);
    delete[] expectedData;
    expectedData = new int[4]{4, 7, 9, 11};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 1, 1);
    delete[] expectedData;
    expectedData = new int[4]{1, 7, -1, 11};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 1, 2);
    delete[] expectedData;
    expectedData = new int[4]{1, 4, -1, 9};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 2, 0);
    delete[] expectedData;
    expectedData = new int[4]{4, 7, 0, 5};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 2, 1);
    delete[] expectedData;
    expectedData = new int[4]{1, 7, 3, 5};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    result = minorMatrix(a, 2, 2);
    delete[] expectedData;
    expectedData = new int[4]{1, 4, 3, 0};
    expected = expectedData;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, expected.row(i)[j], result.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::condNumTest(void)
{
    TUDEF("MatrixOperators", "condNum");
    gnsstk::Matrix<double> a(3, 3, 1);
    double data[9] = {0.10308804, 0.96986452, 0.91268777, 0.38505298, 0.33877576, 0.62561471, 0.08571227, 0.65236093, 0.09723488};
    a = data;
    TUASSERTFEPS(6.791972189793003, condNum(a), .0000001);
    TURETURN();
}

int Matrix_T::diagTest(void)
{
    TUDEF("MatrixOperators", "diag");
    int i, j;
    gnsstk::Matrix<int> a(3, 3, 1);
    gnsstk::Matrix<int> b = diag(a);
    TUASSERTE(int, 3, b.rows());
    TUASSERTE(int, 3, b.cols());
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		if (i != j)
    		{
    			TUASSERTE(int, 0, b.row(i)[j]);
    		}
    		else
    		{
    			TUASSERTE(int, 1, b.row(i)[j]);
    		}
    	}
    }
    TURETURN();
}

int Matrix_T::blkdiagTest(void)
{
    TUDEF("MatrixOperators", "blkdiag");
    int i, j;
    gnsstk::Matrix<int> a(2, 2, 1);
    gnsstk::Matrix<int> b(2, 2, 2);
    gnsstk::Matrix<int> c(2, 2, 3);
    gnsstk::Matrix<int> d(2, 2, 4);
    // resulting thing should  be 5x5
    gnsstk::Matrix<int> e = blkdiag(a, b, c, d);
    for (i = 0; i < 8; i++)
    {
    	for (j = 0; j < 8; j++)
    	{
    		if (i < 2 && j < 2)
    		{
    			TUASSERTE(int, 1, e.row(i)[j]);
    		}
    		else if (i >= 2 && j >= 2 && i < 4 && j < 4)
    		{
    			TUASSERTE(int, 2, e.row(i)[j]);
    		}
    		else if (i >= 4 && j >= 4 && i < 6 && j < 6)
    		{
    			TUASSERTE(int, 3, e.row(i)[j]);
    		}
    		else if (i >= 6 && j >= 6)
    		{
    			TUASSERTE(int, 4, e.row(i)[j]);
    		}
    		else
    		{
    			TUASSERTE(int, 0, e.row(i)[j]);
    		}
    	}
    }
    TURETURN();
}

int Matrix_T::rotationTest(void)
{
    TUDEF("MatrixOperators", "rotation");
    int i, j;
    TUTHROW(gnsstk::rotation(180.0, 0));
    gnsstk::Matrix<double> a = gnsstk::rotation(180.0, 1);
    double data[9] = {1.0, 0.0, 0.0, 0.0, -0.59846, -0.801153, 0.0, 0.801153, -0.59846};
    gnsstk::Matrix<double> expected(3, 3, 0.0);
    expected = data;
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		TUASSERTFEPS(expected.row(i)[j], a.row(i)[j], .00001);
    	}
    }
    TURETURN();
}

int Matrix_T::inverseLUDTest(void)
{
    int i, j;
    TUDEF("MatrixOperators", "inverseLUD");
    // error case, nonsquare matrix
    gnsstk::Matrix<double> a(3, 4, 1);
    TUTHROW(gnsstk::inverseLUD(a));
    gnsstk::Matrix<double> b(3, 3);
    double data[9] = {-1.0, 2.0, 3.0, 4.0, -5.0, 6.0, 7.0, 8.0, -9.0};
    b = data;
    // valid case, normal full rank matrix.
    gnsstk::Matrix<double> result = gnsstk::inverseLUD(b);
    double expected[9] = {-0.00833333, 0.11666667, 0.075, 0.21666667, -0.03333333, 0.05, 0.18611111, 0.06111111, -0.00833333};
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		TUASSERTFEPS(expected[j + i * 3], result.row(i)[j], .000001);
    	}
    }
    // error case but with determinant call
    double det = 0;
    double &detRef = det;
    TUTHROW(gnsstk::inverseLUD(a, det));
    // valid case but with determinant call
    result = gnsstk::inverseLUD(b, det);
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		TUASSERTFEPS(expected[j + i * 3], result.row(i)[j], .000001);
    	}
    }
    TURETURN();
}

int Matrix_T::inverseSVDTest(void)
{
    int i, j;
    TUDEF("MatrixOperators", "inverseSVD");
    // non square error
    gnsstk::Matrix<double> a(3, 4, 1);
    TUTHROW(gnsstk::inverseSVD(a));
    // zero matrix error
    gnsstk::Matrix<double> b(3, 3, 0.0);
    TUTHROW(gnsstk::inverseSVD(b));
    // valid
    double data[9] = {-1.0, 2.0, 3.0, 4.0, -5.0, 6.0, 7.0, 8.0, -9.0};
    gnsstk::Matrix<double> c(3, 3, 0.0);
    c = data;
    gnsstk::Matrix<double> result = gnsstk::inverseSVD(c);
    double expected[9] = {-0.00833333, 0.11666667, 0.075, 0.21666667, -0.03333333, 0.05, 0.18611111, 0.06111111, -0.00833333};
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		TUASSERTFEPS(expected[j + i * 3], result.row(i)[j], .000001);
    	}
    }
    // do the same tests but with the big and small values of the diagonal matrix returned.
    double big, small;
    double &bigRef = big;
    double &smallRef = small;
    double svdDiagExpected[3] = {15.00794407, 6.90641434, 3.47319101};
    TUTHROW(gnsstk::inverseSVD(a, bigRef, smallRef));
    TUTHROW(gnsstk::inverseSVD(b, bigRef, smallRef));
    result = gnsstk::inverseSVD(c, bigRef, smallRef);
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		TUASSERTFEPS(expected[j + i * 3], result.row(i)[j], .000001);
    	}
    }
    TUASSERTFEPS(big, svdDiagExpected[0], .000001);
    TUASSERTFEPS(small, svdDiagExpected[2], .000001);
    // one more time, but we need to get the entire diag vector
    gnsstk::Vector<double> v(3);
    gnsstk::Vector<double> &vRef = v;
    TUTHROW(gnsstk::inverseSVD(a, vRef));
    TUTHROW(gnsstk::inverseSVD(b, vRef));
    result = gnsstk::inverseSVD(c, vRef);
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		TUASSERTFEPS(expected[j + i * 3], result.row(i)[j], .000001);
    	}
    }
    for (i = 0; i < 3; i++)
    {
    	TUASSERTFEPS(vRef[i], svdDiagExpected[i], .000001);
    }
    TURETURN();
}

int Matrix_T::inverseCholTest(void)
{
    int i, j;
    TUDEF("MatrixOperators", "inverseChol");

    double data[9] = {1, 2, -3, 2, 5, -12, -3, -12, 46};
    gnsstk::Matrix<double> c(3, 3, 0.0);
    c = data;
    gnsstk::Matrix<double> result = gnsstk::inverseChol(c);
    double expected[9] = {86, -56, -9, -56, 37, 6, -9, 6, 1};
    for (i = 0; i < 3; i++)
    {
    	for (j = 0; j < 3; j++)
    	{
    		TUASSERTFEPS(expected[j + i * 3], result.row(i)[j], .000001);
    	}
    }
    TURETURN();
}

int Matrix_T::multiplyOpTest(void)
{
    TUDEF("MatrixOperators", "operator*");
    // vector times matrix
    gnsstk::Matrix<double> a(2, 2, 1);
    // incompatible dims exception
    gnsstk::Vector<double> v1(3);
    TUTHROW(v1 * a)
    // valid case
    gnsstk::Vector<double> v2(2);
    v2[0] = 1;
    v2[1] = 1;
    gnsstk::Vector<double> result = v2 * a;
    TUASSERTE(double, 2, result[0]);
    TUASSERTE(double, 2, result[1]);
    TURETURN();
}

int Matrix_T::addOpTest(void)
{
    int i, j;
    TUDEF("MatrixOperators", "operator+");
    gnsstk::Matrix<double> a(2, 2, 1);
    gnsstk::Matrix<double> b1(2, 3, 1);
    TUTHROW(a + b1);
    gnsstk::Matrix<double> b2(2, 2, 2);
    gnsstk::Matrix<double> result = a + b2;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(double, 3, result.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::subtractOpTest(void)
{
    int i, j;
    TUDEF("MatrixOperators", "operator-");
    gnsstk::Matrix<double> a(2, 2, 1);
    gnsstk::Matrix<double> b1(2, 3, 1);
    TUTHROW(a - b1);
    gnsstk::Matrix<double> b2(2, 2, 2);
    gnsstk::Matrix<double> result = a - b2;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(double, -1, result.row(i)[j]);
    	}
    }
    TURETURN();
    TURETURN();
}

int Matrix_T::constMultiplyTest(void)
{
    int i, j;
    TUDEF("MatrixOperators", "operator* with const");
    gnsstk::Matrix<double> a(2, 2, 2);
    gnsstk::Matrix<double> result = a * 2.0;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(double, 4.0, result.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::constDivideTest(void)
{
    int i, j;
    TUDEF("MatrixOperators", "operator/ with const");
    gnsstk::Matrix<double> a(2, 2, 2);
    gnsstk::Matrix<double> result = a / 2.0;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(double, 1.0, result.row(i)[j]);
    	}
    }
    TURETURN();
}

int main()
{
    int check, errorCounter = 0;
    Matrix_T testClass;

    check = testClass.operatorTest();
    errorCounter += check;
    errorCounter += testClass.andOpTest();
    errorCounter += testClass.orOpTest();
    errorCounter += testClass.minorMatrixTest();
    errorCounter += testClass.condNumTest();
    errorCounter += testClass.diagTest();
    errorCounter += testClass.blkdiagTest();
    errorCounter += testClass.rotationTest();
    errorCounter += testClass.inverseLUDTest();
    errorCounter += testClass.inverseSVDTest();
    errorCounter += testClass.inverseCholTest();
    errorCounter += testClass.multiplyOpTest();
    errorCounter += testClass.addOpTest();
    errorCounter += testClass.subtractOpTest();
    errorCounter += testClass.constMultiplyTest();
    errorCounter += testClass.constDivideTest();

    std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter << std::endl;

    return errorCounter; //Return the total number of errors
}
