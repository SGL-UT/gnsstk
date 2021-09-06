#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args, run_unit_tests

import gpstk
import math

# Tests for vector
def test_matrix():
    actual = 0
    expected = 0
    test_num = 1

    # Matrix Constructor tests
    a = gpstk.Matrix_double()
    actual = (a.size(), a.empty(), a.rows(), a.cols())
    expected = (0, True, 0, 0)
    check_test(actual, expected, "Default Matrix Constructor", test_num)
    test_num += 1

    a = gpstk.Matrix_int(34, 67)
    actual = (a.size(), a.empty(), a.rows(), a.cols())
    expected = (2278, False, 34, 67)
    check_test(actual, expected, "Matrix Constructor with initial size", test_num)
    test_num += 1

    a = gpstk.Matrix_int(34, 67, 5)
    actual = (a.size(), a.empty(), a.rows(), a.cols(), a.get_value(1, 2), a.get_value(23, 34))
    expected = (2278, False, 34, 67, 5, 5)
    check_test(actual, expected, "Matrix Constructor with initial size and value", test_num)
    test_num += 1

    # Size, Num Rows, Num Cols, Empty, Max Size tests
    a = gpstk.Matrix_double()
    actual = []
    actual.append(a.empty())
    a = gpstk.Matrix_double(12, 23, 5)
    actual.extend([a.rows(), a.cols(), a.empty(), a.size(), a.max_size()])
    expected = [True, 12, 23, False, 276, 276]
    check_test(actual, expected, "Size, Num Rows, Num Cols, Empty, Max Size", test_num)
    test_num += 1

    # TODO:
    # rowRef, colRef, row, col tests

    # Resize
    a = gpstk.Matrix_double(12, 34, 5)
    actual = [a.rows(), a.cols(), a.size()]
    b = a.resize(34, 56)
    actual.extend([a.rows(), a.cols(), a.size(), b.rows(), b.cols(), b.size()])
    expected = [12, 34, 408, 34, 56, 34*56, 34, 56, 34*56]
    check_test(actual, expected, "Matrix Resize", test_num)
    test_num += 1

    # zeroize, zeroizeRow, zeroizeCol
    a = gpstk.Matrix_double(12, 34, 10**-9)
    a.zeroizeRow(7)
    actual = [slice_is_value(a[7])]
    a = gpstk.Matrix_double(12, 34, 10**-11)
    a.zeroizeRow(7)
    actual.append(slice_is_value(a[7]))

    a_col_slice = gpstk.MatrixColSlice_double(a, 30)
    actual.append(slice_is_value(a_col_slice))
    a.zeroizeCol(30)
    actual.append(slice_is_value(a_col_slice))

    actual.append(matrices_are_equal(a, gpstk.Matrix_double(12, 34, 0)))
    a.zeroize()
    actual.append(matrices_are_equal(a, gpstk.Matrix_double(12, 34, 0)))

    expected = [False, True, False, True, False, True]
    check_test(actual, expected, "Matrix zeroize zeroizeRow zeroizeCol", test_num)
    test_num += 1

    # Upper Triangular, Lower Triangular, Square, Diagonal, Symmetric
    a = gpstk.Matrix_double(45, 45, 1)
    b = gpstk.Matrix_double(45, 33, 0)
    c = gpstk.Matrix_double(45, 45, 0)
    actual = [a.isSquare(), b.isSquare(), c.isSquare(), a.isUT(), b.isUT(), c.isUT(), a.isLT(), b.isLT(), c.isLT(),
              a.isDiagonal(), b.isDiagonal(), c.isDiagonal(), a.isSymmetric(), b.isSymmetric(), c.isSymmetric()]
    expected = [True, False, True, False, False, True, False, False, True, False, False, True, True, False, True]
    check_test(actual, expected, "UT LT Square Diagonal Symmetric", test_num)
    test_num += 1

    # ColCopy, rowCopy, diagCopy
    a = gpstk.Matrix_double(34, 12, 1)
    vec1 = a.colCopy(5, 23)
    vec2 = a.rowCopy(33, 7)
    vec3 = a.diagCopy()
    actual = [vec1.size(), vec2.size(), vec3.size()]
    expected = [34-23, 12-7, 12]
    check_test(actual, expected, "Colcopy rowCopy diagCopy", test_num)
    test_num += 1

    # Swap rows and columns
    a = gpstk.Matrix_double(34, 34)
    for i in range(a.rows()):
        for j in range(a.cols()):
            a[i][j] = i
    actual = [slice_is_value(a[1], 1), slice_is_value(a[20], 20)]
    a.swapRows(1, 20)
    actual.extend([slice_is_value(a[1], 20), slice_is_value(a[20], 1)])

    for i in range(a.rows()):
        for j in range(a.cols()):
            a[i][j] = j
    col1 = gpstk.MatrixColSlice_double(a, 1)
    col20 = gpstk.MatrixColSlice_double(a, 20)
    actual.extend([slice_is_value(col1, 1), slice_is_value(col20, 20)])
    a.swapCols(1, 20)
    actual.extend([slice_is_value(col1, 20), slice_is_value(col20, 1)])
    expected = [True, True, True, True, True, True, True, True]
    check_test(actual, expected, "Swap rows and columns", test_num)
    test_num += 1

    # AssignFrom
    a = gpstk.Matrix_double(5, 5)
    b = gpstk.Matrix_double(5, 5, 10)
    a.assignFromMatrix(b) # AssignFrom matrix
    b[2][3] = -3
    actual = [matrices_are_equal(a, gpstk.Matrix_double(5, 5, 10)), matrices_are_equal(b, gpstk.Matrix_double(5, 5, 10))]

    b = gpstk.Vector_double(25, -78)
    a.assignFromVector(b) # AssignFrom GPSTK vector
    b[2] = 30
    actual.extend([matrices_are_equal(a, gpstk.Matrix_double(5, 5, -78)), slice_is_value(b, -78)])
    # This test is the only place that array_double is used, and the
    # array_double declarations cause downstream errors and have been
    # commented out.  See gpstk.i
    # double_array = gpstk.array_double(25)
    # for i in range(25):
    #     double_array[i] = 4
    # a.assignFrom(double_array) # AssignFrom array
    # actual.append(matrices_are_equal(a, gpstk.Matrix_double(5, 5, 4)))

    a.assignFrom(5) # AssignFrom value
    actual.append(matrices_are_equal(a, gpstk.Matrix_double(5, 5, 5)))
    expected = [True, False, True, False, True, True]
    check_test(actual, expected, "AssignFrom", test_num)
    test_num += 1

    # Operators +=, -=,  *=, /=, and unary negation
    # @todo actually do some tests...
    a = gpstk.Matrix_double(5, 5, 3)
    b = gpstk.Matrix_double(5, 5, 10)
    vec = gpstk.Vector_double(25, -8)

    # ident, trace, normF, normCol, slowDet


# Rounds a list of floats to a certain decimal point
def round_list(lst, tolerance=3):
    return [round(x , tolerance) for x in lst]

def slice_is_value(row, value=0):
    for i in range(row.size()):
        if row[i] != value:
            return False
    return True

def matrices_are_equal(a, b):
    # Check if the number of columns and rows are equal
    if a.rows() != b.rows() or a.cols() != b.cols():
        return False
    # Check if each element equals each other
    for i in range(a.rows()):
        for j in range(a.cols()):
            if a.get_value(i, j) != b.get_value(i, j):
                return False
    return True

# Checks if something is
def check_test(observed, expected, description="", test_num=0):
    if expected != observed:
        print("FAILED TEST", test_num, ":", description, "test")
    else:
        print("passed test", test_num, ":", description, "test")

if __name__ == "__main__":
    test_matrix()
#     # a = gpstk.Vector_double(5, 10)
#     # b = gpstk.Vector_double(5, -9)
#     # a = gpstk.WtdStats_double(a, b)
#     print(a)

