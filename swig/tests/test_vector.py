#!/usr/env python

import math
import os
import sys
import unittest

sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, assertSequenceAlmostEqual, run_unit_tests

import gnsstk


class TestVector(unittest.TestCase):

    def test_constructor(self):
        # Construct vector with initial size
        a = gnsstk.Vector_int(5) # Initial size
        self.assertEqual(5, len(a), "Vector must initialize with provided size")

        # Construct vector with initial size and default value
        a = gnsstk.Vector_int(6, 89)
        self.assertSequenceEqual([89, 89, 89, 89, 89, 89], list(a), "Vector must initialize with provided size and default value")

        # Use copy constructor
        a = gnsstk.Vector_int(6, 42)
        b = gnsstk.Vector_int(a)
        self.assertSequenceEqual([42, 42, 42, 42, 42, 42], list(b))
        self.assertIsNot(a, b, "Copy constructor must make a new instance")

    def test_resize(self):
        a = gnsstk.Vector_int(6, 23)
        self.assertSequenceEqual([23, 23, 23, 23, 23, 23], list(a))

        a.resize(3)
        self.assertSequenceEqual([23, 23, 23], list(a), "Vector resize must reduce the list")

        # Resize with a default value for the entire vector
        # NOTE: this is different, but seems to be expected, from the C++ standard library vector
        a.resize(5, 42)
        self.assertSequenceEqual([42, 42, 42, 42, 42], list(a), "Vector resize must increase size with provided default value")

        # Really? The vector destroys itself if the resize is larger than the current size and no default value.
        # a.resize(6)
        # self.assertSequenceEqual([42, 42, 42, 42, 42, 0], list(a), "Vector resize must use value initialization if no default given")

    def test_append(self):
        a = gnsstk.Vector_int()
        self.assertEqual(0, len(a))

        for i in range(0, 10):
            a = a.append(i)

        self.assertSequenceEqual(list(range(10)), list(a))

    def test_extend(self):
        a = gnsstk.Vector_int(2, 1)
        b = gnsstk.Vector_int(2, 2)
        c = gnsstk.Vector_int(2, 3)

        # NOTE: this is different behavior from the standard Python list `extend()`
        #   The python list `extend` modifies the list and returns None, whereas,
        #   this method for Vector<int> returns a *new* instance with the modifications.
        d = a.extend(b).extend(c)
        self.assertSequenceEqual([1, 1, 2, 2, 3, 3], list(d))

    def test_iterator(self):
        a = gnsstk.Vector_int(10, 0)
        for i in range(len(a)):
            a[i] = 2*i

        # Get iterator without failure and return the first few elements
        a_iter = iter(a)
        self.assertEqual(0, next(a_iter))
        self.assertEqual(2, next(a_iter))
        self.assertEqual(4, next(a_iter))

        # Exercise the iterator via list comprehension
        a_dump = [v for v in a]
        self.assertSequenceEqual([0, 2, 4, 6, 8, 10, 12, 14, 16, 18], a_dump)

    def test_to_std_vector(self):
        a = gnsstk.Vector_int(10, 3)
        self.assertIsInstance(a, gnsstk.Vector_int)

        b = a.toStdVector()
        self.assertIsInstance(b, gnsstk.std_vector_int)

        self.assertSequenceEqual([3, 3, 3, 3, 3, 3, 3, 3, 3, 3], list(a))
        self.assertSequenceEqual([3, 3, 3, 3, 3, 3, 3, 3, 3, 3], list(b))

    def test_vector_sum(self):
        # Rather clunky syntax for creating a vector of [2, 2, 2, 2, 2, 4.5, 4.5, 4.5]
        a = gnsstk.Vector_double(5, 2.0).extend(gnsstk.Vector_double(3, 4.5))
        self.assertAlmostEqual(23.5, gnsstk.Vector_sum_double(a))


class TestVectorOperators(unittest.TestCase):

    def test_vector_min_max_abs(self):
        a = gnsstk.Vector_int()
        for i in [-1, -2, 56, -6, -89, -45]:
            a = a.append(i)

        self.assertEqual(-89, gnsstk.Vector_min_int(a))
        self.assertEqual(-1, gnsstk.Vector_minabs_int(a))
        self.assertEqual(56, gnsstk.Vector_max_int(a))
        # TODO: maxabs causes build to fail, see TemplateMacros.i
        # self.assertEqual(-89, gnsstk.Vector_maxabs_int(a))

    def test_vector_norm(self):
        a = gnsstk.Vector_double(3, 4)
        a = a.extend(gnsstk.Vector_double(3, 2))
        a = a.extend(gnsstk.Vector_double(3, -2.5))

        self.assertAlmostEqual(8.87411967, gnsstk.Vector_norm_double(a))

    def test_vector_dot(self):
        a = gnsstk.Vector_int(3, 2)
        a = a.extend(gnsstk.Vector_int(3, 23))
        a = a.extend(gnsstk.Vector_int(3, -5))

        # There are two "dotscalar" functions that work the same but can take the arguments
        # in either order
        self.assertEqual(240, gnsstk.Vector_dotscalar_int(a, 4))
        self.assertEqual(240, gnsstk.Vector_dotscalar_int(4, a))

    def test_vector_normalize(self):
        a = gnsstk.Vector_double(9, -34)
        a = a.extend(gnsstk.Vector_double(9, -78))
        a = a.extend(gnsstk.Vector_double(9, -74))

        b = gnsstk.Vector_normalize_double(a)
        self.assertIsNot(a, b)
        assertSequenceAlmostEqual(
            self,
            [
                -0.10050378, -0.10050378, -0.10050378, -0.10050378, -0.10050378, -0.10050378, -0.10050378, -0.10050378, -0.10050378,
                -0.23056749, -0.23056749, -0.23056749, -0.23056749, -0.23056749, -0.23056749, -0.23056749, -0.23056749, -0.23056749,
                -0.21874352, -0.21874352, -0.21874352, -0.21874352, -0.21874352, -0.21874352, -0.21874352, -0.21874352, -0.21874352,
            ],
            list(b)
        )

    def test_vector_RSS_RMS(self):
        a = gnsstk.Vector_double(3, -7)
        a = a.extend(gnsstk.Vector_double(3, 4.5))
        a = a.extend(gnsstk.Vector_double(3, 20))

        self.assertAlmostEqual(37.51999466, gnsstk.Vector_RSS_double(a))
        self.assertAlmostEqual(12.50666488, gnsstk.Vector_RMS_double(a))

    def test_vector_unary_operators(self):

        # STD Unary Vector Operations
        a = gnsstk.Vector_double(3, 0.45)
        a = a.extend(gnsstk.Vector_double(3, 0.23))
        a = a.extend(gnsstk.Vector_double(3, 0.1))

        gnsstk_vector_ops = [
            gnsstk.Vector_abs_double,
            gnsstk.Vector_acos_double,
            gnsstk.Vector_asin_double,
            gnsstk.Vector_atan_double,
            gnsstk.Vector_cos_double,
            gnsstk.Vector_cosh_double,
            gnsstk.Vector_exp_double,
            gnsstk.Vector_log_double,
            gnsstk.Vector_log10_double,
            gnsstk.Vector_sinh_double,
            gnsstk.Vector_sin_double,
            gnsstk.Vector_sqrt_double,
            gnsstk.Vector_tan_double,
            gnsstk.Vector_tanh_double,
        ]

        python_math_ops = [
            abs,
            math.acos,
            math.asin,
            math.atan,
            math.cos,
            math.cosh,
            math.exp,
            math.log,
            lambda v: math.log(v, 10),  # Can't use partial() since there are no keywords
            math.sinh,
            math.sin,
            math.sqrt,
            math.tan,
            math.tanh,
        ]

        for gnsstk_op, python_op in zip(gnsstk_vector_ops, python_math_ops):
            # Using `subTest` here will be useful when moving to python 3.4
            # with self.subTest(gnsstk_op.__name__):
            
            # The gnsstk operators are "vectorized" while the python functions are not
            assertSequenceAlmostEqual(self, list(gnsstk_op(a)), list(map(python_op, list(a))))


if __name__ == "__main__":
    run_unit_tests()
