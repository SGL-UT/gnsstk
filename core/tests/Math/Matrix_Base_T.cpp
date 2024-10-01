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
//  Copyright 2004-2024, The Board of Regents of The University of Texas System
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
#include "TestUtil.hpp"
#include <iostream>

using namespace gnsstk;
using namespace std;

int Matrix_T::isUTTest(void)
{
    TUDEF("MatrixBase", "isUT");
    gnsstk::Matrix<int> notSquare(3, 4, 1);
    TUASSERT(!notSquare.isUT());
    gnsstk::Matrix<int> a(3, 3, 1);
    TUASSERT(!a.isUT());
    int data[9] = {1, 1, 1, 0, 1, 1, 0, 0, 1};
    a = data;
    TUASSERT(a.isUT());
    TURETURN();
}

int Matrix_T::isLTTest(void)
{
    TUDEF("MatrixBase", "isLT");
    gnsstk::Matrix<int> notSquare(3, 4, 1);
    TUASSERT(!notSquare.isLT());
    gnsstk::Matrix<int> a(3, 3, 1);
    TUASSERT(!a.isLT());
    int data[9] = {1, 0, 0, 1, 1, 0, 1, 1, 1};
    a = data;
    TUASSERT(a.isLT());
    TURETURN();
}

int Matrix_T::isDiagonalTest(void)
{
    TUDEF("MatrixBase", "isDiagonal");
    gnsstk::Matrix<int> notSquare(3, 4, 1);
    TUASSERT(!notSquare.isDiagonal());
    gnsstk::Matrix<int> a(3, 3, 1);
    TUASSERT(!a.isDiagonal());
    int data[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    a = data;
    TUASSERT(a.isDiagonal());
    TURETURN();
}

int Matrix_T::isSymmetricTest(void)
{
    TUDEF("MatrixBase", "isSymmetric");
    gnsstk::Matrix<int> notSquare(3, 4, 1);
    TUASSERT(!notSquare.isSymmetric());
    gnsstk::Matrix<int> a(3, 3, 1);
    TUASSERT(a.isSymmetric());
    int data[9] = {1, 1, 1, 0, 0, 0, 0, 0, 0};
    a = data;
    TUASSERT(!a.isSymmetric());
    TURETURN();
}

int Matrix_T::rowCopyTest(void)
{
    TUDEF("MatrixBase", "rowCopy");
    // bad args case
    gnsstk::Matrix<int> a(3, 3, 1);
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    a = data;
// the rangecheck is guarded by an ifdef so we'll only test this in that case.
#ifdef RANGECHECK
    // if rows or cols is too high, die.
    TUTHROW(a.rowCopy(4, 0));
    TUTHROW(a.rowCopy(0, 4));
#endif
    // copy first row, whole row.
    gnsstk::Vector<int> result = a.rowCopy(0, 0);
    TUASSERTE(int, result[0], 1);
    TUASSERTE(int, result[1], 2);
    TUASSERTE(int, result[2], 3);
    TURETURN();
}

int Matrix_T::diagCopyTest(void)
{
    TUDEF("MatrixBase", "diagCopy");
    gnsstk::Matrix<int> a(3, 3, 1);
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    a = data;
    gnsstk::Vector<int> result = a.diagCopy();
    TUASSERTE(int, result[0], 1);
    TUASSERTE(int, result[1], 5);
    TUASSERTE(int, result[2], 9);
    TURETURN();
}

// RefMatrixBase
int Matrix_T::zeroizeTest(void)
{
    TUDEF("MatrixBase", "zeroize");
    int i, j;
    gnsstk::Matrix<double> a(2, 2, 1e-16);
    a.zeroize();
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(double, 0.0, a.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::zeroizeRowTest(void)
{
    TUDEF("MatrixBase", "zeroizeRow");
    int i, j;
    gnsstk::Matrix<double> a(2, 2, 1e-16);
    a = a.zeroizeRow(0);
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		if (i == 0)
    		{
    			TUASSERTE(double, 0.0, a.row(i)[j]);
    		}
    		else
    		{
    			TUASSERT(a.row(i)[j] != 0.0);
    		}
    	}
    }
    TURETURN();
}

int Matrix_T::zeroizeColTest(void)
{
    TUDEF("MatrixBase", "zeroizeCol");
    int i, j;
    gnsstk::Matrix<double> a(2, 2, 1e-16);
    a = a.zeroizeCol(0);
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		if (j == 0)
    		{
    			TUASSERTE(double, 0.0, a.row(i)[j]);
    		}
    		else
    		{
    			TUASSERT(a.row(i)[j] != 0.0);
    		}
    	}
    }
    TURETURN();
}

int Matrix_T::assignFromTest(void)
{
    TUDEF("MatrixBase", "assignFrom");
    // doing valarray assignFrom
    int i, j;
    gnsstk::Matrix<int> a(2, 2, 1);
    int data[4] = {5, 5, 5, 5};
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 1, a.row(i)[j]);
    	}
    }
    a.assignFrom(data);
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 5, a.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::addTest(void)
{
    TUDEF("MatrixBase", "operator+=");
    int i, j;
    gnsstk::Matrix<int> a(2, 2, 0);
    gnsstk::Vector<int> b(4);
    b[0] = 0;
    b[1] = 1;
    b[2] = 2;
    b[3] = 3;
    // vector
    a += b;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, i * 2 + j, a.row(i)[j]);
    	}
    }
    // add to each element each element of an array
    int c[4] = {0, 1, 2, 3};
    a += c;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 2 * (i * 2 + j), a.row(i)[j]);
    	}
    }
    // add to each element each element of stuff on a pointer (can just reuse array)
    int *d = c;
    a += d;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 3 * (i * 2 + j), a.row(i)[j]);
    	}
    }
    // add to each element a constant
    a += 1;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 3 * (i * 2 + j) + 1, a.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::subtractTest(void)
{
    TUDEF("MatrixBase", "operator-=");
    int i, j;
    gnsstk::Matrix<int> a(2, 2, 0);
    gnsstk::Vector<int> b(4);
    b[0] = 0;
    b[1] = -1;
    b[2] = -2;
    b[3] = -3;
    // add to each element of the matrix every element in the vector
    a -= b;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, i * 2 + j, a.row(i)[j]);
    	}
    }
    // add to each element each element of an array
    int c[4] = {0, -1, -2, -3};
    a -= c;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 2 * (i * 2 + j), a.row(i)[j]);
    	}
    }
    // add to each element each element of stuff on a pointer (can just reuse array)
    int *d = c;
    a -= d;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 3 * (i * 2 + j), a.row(i)[j]);
    	}
    }
    // add to each element a constant
    a -= -1;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 3 * (i * 2 + j) + 1, a.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::multiplyTest(void)
{
    TUDEF("MatrixBase", "operator*=");
    int i, j;
    gnsstk::Matrix<int> a(2, 2, 1);
    a *= 2;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 2, a.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::divideTest(void)
{
    TUDEF("MatrixBase", "operator/=");
    int i, j;
    gnsstk::Matrix<int> a(2, 2, 2);
    a /= 2;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, 1, a.row(i)[j]);
    	}
    }
    TURETURN();
}

int Matrix_T::unaryNegativeTest(void)
{
    TUDEF("MatrixBase", "operator-");
    int i, j;
    gnsstk::Matrix<int> a(2, 2, 2);
    a = -a;
    for (i = 0; i < 2; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    		TUASSERTE(int, -2, a.row(i)[j]);
    	}
    }
    TURETURN();
}

int main(void)
{
    int errorCounter = 0;
    Matrix_T testClass;

    errorCounter += testClass.isUTTest();
    errorCounter += testClass.isLTTest();
    errorCounter += testClass.isDiagonalTest();
    errorCounter += testClass.isSymmetricTest();
    errorCounter += testClass.rowCopyTest();
    errorCounter += testClass.diagCopyTest();
    errorCounter += testClass.zeroizeTest();
    errorCounter += testClass.zeroizeRowTest();
    errorCounter += testClass.zeroizeColTest();
    errorCounter += testClass.assignFromTest();
    errorCounter += testClass.addTest();
    errorCounter += testClass.subtractTest();
    errorCounter += testClass.multiplyTest();
    errorCounter += testClass.divideTest();
    errorCounter += testClass.unaryNegativeTest();

    std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter << std::endl;

    return errorCounter; //Return the total number of errors
}