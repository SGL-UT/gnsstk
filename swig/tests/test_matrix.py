#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, assertSequenceAlmostEqual, run_unit_tests, vec_to_list

import gnsstk
import math

# TODO: need additional tests for:
#   * rowRef, colRef, row, col tests
#   * Operators +=, -=,  *=, /=, and unary negation
#   * ident, trace, normF, normCol, slowDet


def assertMatricesEqual(test_case, a, b):
    """
    Naming scheme attempts to match assertions names from unittest module.
    """
    # Check if the number of columns and rows are equal
    test_case.assertEqual(a.rows(), b.rows())
    test_case.assertEqual(a.cols(), b.cols())

    # Check if each element equals each other
    for i in range(a.rows()):
        for j in range(a.cols()):
            test_case.assertEqual(a.get_value(i, j), b.get_value(i, j))



class TestMatrix(unittest.TestCase):

    def test_constructor(self):
        # Matrix Constructor tests
        a = gnsstk.Matrix_double()
        self.assertEqual(0, a.size())
        self.assertTrue(a.empty())
        self.assertEqual(0, a.rows())
        self.assertEqual(0, a.cols())

        # Constructor with rows/columns
        a = gnsstk.Matrix_int(34, 67)
        self.assertEqual(2278, a.size())
        self.assertFalse(a.empty())
        self.assertEqual(34, a.rows())
        self.assertEqual(67, a.cols())

        # Constructor with rows/columns and a default fill value
        a = gnsstk.Matrix_int(34, 67, 5)
        self.assertEqual(2278, a.size())
        self.assertFalse(a.empty())
        self.assertEqual(34, a.rows())
        self.assertEqual(67, a.cols())
        self.assertEqual(5, a.get_value(23, 34))

    def test_accessors(self):
        # Testing accessors of size, num rows, num cols, empty, and max_size
        a = gnsstk.Matrix_double()
        self.assertTrue(a.empty())

        a = gnsstk.Matrix_double(12, 23, 5)
        self.assertFalse(a.empty())
        self.assertEqual(12, a.rows())
        self.assertEqual(23, a.cols())
        self.assertEqual(276, a.size())
        self.assertEqual(276, a.max_size())

    def test_resize(self):
        a = gnsstk.Matrix_double(12, 34, 5)
        self.assertEqual(12, a.rows())
        self.assertEqual(34, a.cols())
        self.assertEqual(408, a.size())

        # This "works" but all of the data is scrapped in the resize due to the implementation of Vector.resize()
        _ = a.resize(34, 56)
        self.assertEqual(34, a.rows())
        self.assertEqual(56, a.cols())
        self.assertEqual(1904, a.size())

        # Resize and replace all values with given default value
        _ = a.resize(1, 1, 13)
        self.assertEqual(1, a.rows())
        self.assertEqual(1, a.cols())
        self.assertEqual(1, a.size())
        self.assertEqual(13, a.get_value(0, 0))

    def test_zeroize(self):
        # Zeroize results depends on the value of RefVectorBaseHelper::zeroTolerance
        # Zeroize will set a value to zero if it is less than RefVectorBaseHelper::zeroTolerance

        # Attempt to zeroize, but nothing is less than RefVectorBaseHelper::zeroTolerance
        a = gnsstk.Matrix_double(12, 34, 10**-9)
        a.zeroizeRow(7)
        assertSequenceAlmostEqual(self, [10**-9] * 34, vec_to_list(a[7]))

        # Zeroize and all values in row are below RefVectorBaseHelper::zeroTolerance
        a = gnsstk.Matrix_double(12, 34, 10**-11)
        a.zeroizeRow(7)
        self.assertSequenceEqual([0] * 34, vec_to_list(a[7]))

        a = gnsstk.Matrix_double(12, 34, 10**-11)
        a.zeroizeCol(30)
        self.assertSequenceEqual([0] * 12, vec_to_list(gnsstk.MatrixColSlice_double(a, 30)))

        a = gnsstk.Matrix_double(12, 34, 10**-11)
        a.zeroize()
        ref = gnsstk.Matrix_double(12, 34, 0)
        assertMatricesEqual(self, ref, a)

    def test_matrix_properties(self):
        # TODO: should these tests be expanded on, or is it sufficient to simply exercise the swig calls?
        a = gnsstk.Matrix_double(45, 45, 1)
        b = gnsstk.Matrix_double(45, 33, 0)
        c = gnsstk.Matrix_double(45, 45, 0)

        self.assertTrue(a.isSquare())
        self.assertFalse(b.isSquare())
        self.assertTrue(c.isSquare())

        self.assertFalse(a.isUT())
        self.assertFalse(b.isUT())
        self.assertTrue(c.isUT())

        self.assertFalse(a.isLT())
        self.assertFalse(b.isLT())
        self.assertTrue(c.isLT())

        self.assertFalse(a.isDiagonal())
        self.assertFalse(b.isDiagonal())
        self.assertTrue(c.isDiagonal())

        self.assertTrue(a.isSymmetric())
        self.assertFalse(b.isSymmetric())
        self.assertTrue(c.isSymmetric())

    def test_slice_copy(self):
        # ColCopy, rowCopy, diagCopy
        a = gnsstk.Matrix_double(34, 12, 1)
        vec1 = a.colCopy(5, 23)
        self.assertEqual(11, vec1.size())

        vec2 = a.rowCopy(33, 7)
        self.assertEqual(5, vec2.size())

        vec3 = a.diagCopy()
        self.assertEqual(12, vec3.size())

    def test_swap_rows(self):
        a = gnsstk.Matrix_double(34, 34)

        # Setting values of matrix to their row number
        for i in range(a.rows()):
            for j in range(a.cols()):
                a[i][j] = i

        # Asserting current state before swapping rows
        self.assertSequenceEqual([1] * 34, vec_to_list(a[1]))
        self.assertSequenceEqual([20] * 34, vec_to_list(a[20]))

        a.swapRows(1, 20)

        # Rows should be swapped
        self.assertSequenceEqual([20] * 34, vec_to_list(a[1]))
        self.assertSequenceEqual([1] * 34, vec_to_list(a[20]))

    def test_swap_cols(self):
        a = gnsstk.Matrix_double(34, 34)

        # Settings values of matrix to their col number for e
        for i in range(a.rows()):
            for j in range(a.cols()):
                a[i][j] = j

        # Asserting current state before swapping cols
        self.assertSequenceEqual([1] * 34, vec_to_list(gnsstk.MatrixColSlice_double(a, 1)))
        self.assertSequenceEqual([20] * 34, vec_to_list(gnsstk.MatrixColSlice_double(a, 20)))
        
        a.swapCols(1, 20)
        
        # Cols should be swapped
        self.assertSequenceEqual([20] * 34, vec_to_list(gnsstk.MatrixColSlice_double(a, 1)))
        self.assertSequenceEqual([1] * 34, vec_to_list(gnsstk.MatrixColSlice_double(a, 20)))

    def test_swap_rows_and_cols(self):
        a = gnsstk.Matrix_int(5, 5)
        for i in range(a.rows()):
            for j in range(a.cols()):
                a[i][j] = i + j

        a.swapRows(0, 4)
        a.swapCols(1, 2)

        self.assertSequenceEqual([6, 3, 4, 5, 2], vec_to_list(gnsstk.MatrixColSlice_int(a, 1)))
        self.assertSequenceEqual([0, 2, 1, 3, 4], vec_to_list(a[4]))

    def test_assign_from(self):
        # AssignFrom
        a = gnsstk.Matrix_double(5, 5)
        b = gnsstk.Matrix_double(5, 5, 10)
        ref = gnsstk.Matrix_double(5, 5, 10)
        a.assignFromMatrix(b)

        assertMatricesEqual(self, a, ref)

        # a and b should not be linked in any way. Changing b should not result in a change of a
        b[2][3] = -3
        assertMatricesEqual(self, a, ref)

        # The vector represents all elements of the matrix. i.e. it is a flattened matrix 
        a = gnsstk.Matrix_int(5, 5, 0)
        ref = gnsstk.Matrix_int(5, 5, -78)
        v = gnsstk.Vector_int(25, -78)
        
        a.assignFromVector(v)
        v[2] = 30  # A change to `v` should not propogate to `a`
        assertMatricesEqual(self, a, ref)
        
        # This test is the only place that array_double is used, and the
        # array_double declarations cause downstream errors and have been
        # commented out.  See gnsstk.i
        # double_array = gnsstk.array_double(25)
        # for i in range(25):
        #     double_array[i] = 4
        # a.assignFrom(double_array) # AssignFrom array
        # actual.append(matrices_are_equal(a, gnsstk.Matrix_double(5, 5, 4)))

        a = gnsstk.Matrix_int(5, 5, 0)
        ref = gnsstk.Matrix_int(5, 5, 42)

        a.assignFrom(42)
        assertMatricesEqual(self, a, ref)


if __name__ == "__main__":
    run_unit_tests()
