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

int Matrix_T::matrixSliceConstructorTest(void)
{
    TUDEF("MatrixSlice", "constructors");
    // default constructor
    TUCATCH(MatrixSlice<int>());
    // makes a slice of a whole matrix
    gnsstk::Matrix<int> a(3, 3);
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    a = data;
    gnsstk::MatrixSlice<int> result1(a);
    TUASSERTE(int, 9, result1.size());
    // partial slice of a matrix, get the first row.
    gnsstk::MatrixSlice<int> result2(a, std::slice(0, 3, 1), std::slice(0, 1, 3));
    TUASSERTE(int, 3, result2.size());
    // while i have it, why don'twe test the operator() alone
    TUASSERTE(int, 1, result2(0, 0));
    TURETURN();
}

int Matrix_T::constMatrixSliceTest(void)
{
    TUDEF("MatrixSlice", "constMatrixSlice");
    // default constructor
    TUCATCH(ConstMatrixSlice<int>());
    // makes a slice of a whole matrix
    gnsstk::Matrix<int> a(3, 3);
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    a = data;
    gnsstk::ConstMatrixSlice<int> result1(a);
    TUASSERTE(int, 9, result1.size());
    // partial slice of a matrix, get the first row.
    gnsstk::ConstMatrixSlice<int> result2(a, std::slice(0, 3, 1), std::slice(0, 1, 3));
    TUASSERTE(int, 3, result2.size());
    // submatrix slice
    gnsstk::ConstMatrixSlice<int> result3(a, 1, 1, 2, 2);
    TUASSERTE(int, 4, result3.size());
    // exercise the functions
    TUASSERTE(int, 3, result1.cols());
    TUASSERTE(int, 3, result1.rows());
    TUASSERTE(int, 1, result1(0, 0));
    TUASSERTE(int, result2.rowSize(), 3);
    TUASSERTE(int, result2.rowStart(), 0);
    TUASSERTE(int, result2.rowStride(), 1);
    TUASSERTE(int, result2.colSize(), 1);
    TUASSERTE(int, result2.colStart(), 0);
    TUASSERTE(int, result2.colStride(), 3);
    TURETURN();
}

int Matrix_T::constMatrixRowSliceTest(void)
{
    TUDEF("MatrixSlice", "constMatrixRowSlice");
    TUCATCH(ConstMatrixRowSlice<int>());
    gnsstk::Matrix<int> a(3, 3);
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    a = data;
    // first row
    ConstMatrixRowSlice<int> result1(a, 0);
    TUASSERTE(int, result1.size(), 3);
    // second row, first and third element
    ConstMatrixRowSlice<int> result2(a, 0, std::slice(0, 2, 2));
    TUASSERTE(int, result2.size(), 2);
    // methods
    TUASSERTE(int, result1.rows(), 1);
    TUASSERTE(int, result1.cols(), 3);
    TUASSERTE(int, result1.rowSize(), 1);
    TUASSERTE(int, result1.rowStart(), 0);
    TUASSERTE(int, result1.rowStride(), 1);
    TUASSERTE(int, result2.colSize(), 2);
    TUASSERTE(int, result2.colStart(), 0);
    TUASSERTE(int, result2.colStride(), 2);
    TURETURN();
}

int Matrix_T::constMatrixColSliceTest(void)
{
    TUDEF("MatrixSlice", "constMatrixColSlice");
    // default
    TUCATCH(ConstMatrixColSlice<int>());
    gnsstk::Matrix<int> a(3, 3);
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    a = data;
    // first col 
    ConstMatrixColSlice<int> result1(a, 0);
    TUASSERTE(int, result1.size(), 3);
    // second col, first and third element
    ConstMatrixColSlice<int> result2(a, 0, std::slice(0, 2, 2));
    TUASSERTE(int, result2.size(), 2);
    // methods
    TUASSERTE(int, result1.cols(), 1);
    TUASSERTE(int, result1.rows(), 3);
    TUASSERTE(int, result1.colSize(), 1);
    TUASSERTE(int, result1.colStart(), 0);
    TUASSERTE(int, result1.colStride(), 1);
    TUASSERTE(int, result2.rowSize(), 2);
    TUASSERTE(int, result2.rowStart(), 0);
    TUASSERTE(int, result2.rowStride(), 2);
    TURETURN();
}

int main(void)
{
    int errorCounter = 0;
    Matrix_T testClass;

    errorCounter += testClass.matrixSliceConstructorTest();
    errorCounter += testClass.constMatrixSliceTest();
    errorCounter += testClass.constMatrixColSliceTest();
    errorCounter += testClass.constMatrixRowSliceTest();

    std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter << std::endl;

    return errorCounter; //Return the total number of errors
}