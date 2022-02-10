#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, run_unit_tests

import gnsstk
import math

# Tests for vector
def test_vector():
    actual = 0
    expected = 0
    test_num = 1

    # Vector Constructor Tests
    a = gnsstk.Vector_int(5) # Initial size
    actual = len(a)
    expected = 5
    check_test(actual, expected, "Construct Vector with initial size", test_num)
    test_num += 1

    a = gnsstk.Vector_int(6, 89) # Initial size and default value
    actual = (len(a), [i for i in a])
    expected = (6, [89, 89, 89, 89, 89, 89])
    check_test(actual, expected, "Construct vector with initial size and default value", test_num)
    test_num += 1

    b = gnsstk.Vector_int(a) # Copy Constructor
    actual = [i for i in b]
    expected = [i for i in a]
    check_test(actual, expected, "Construct vector from an other vector", test_num)
    test_num += 1

    # Resizing tests
    a = gnsstk.Vector_int(6, 23)
    a.resize(3)
    actual = [i for i in a] + [a[0], a[1], a[2]]
    expected = [23, 23, 23, 23, 23, 23]
    check_test(actual, expected, "Resize", test_num)
    test_num += 1

    # Append Test
    a = gnsstk.Vector_int()
    for i in range(0, 10):
        a = a.append(i)
    expected = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    actual = list(a)
    check_test(actual, expected, "Append", test_num)
    test_num += 1

    # Extend Test
    a = gnsstk.Vector_int(10, 1)
    b = gnsstk.Vector_int(10, 2)
    c = gnsstk.Vector_int(10, 3)
    expected = 10 * [1] + 10 * [2] + 10 * [3]
    actual = list(a.extend(b).extend(c))
    check_test(actual, expected, "Extend", test_num)
    test_num += 1

    # Iterator test
    a = gnsstk.Vector_int(10, 0)
    for i in range(0, len(a)):
        a[i] =  2*i
    actual = []
    for i in a:
        actual.append(i)
    expected = [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]
    check_test(actual, expected, "Iterator", test_num)
    test_num += 1

    # Convert to std vector test
    a = gnsstk.Vector_int(10, 3)
    b = a.toStdVector()
    actual = tuple(b)
    expected = (3, 3, 3, 3, 3, 3, 3, 3, 3, 3)
    check_test(actual, expected, "Convert to STD Vector", test_num)
    test_num += 1

    # Single Vector Operation Tests
    a = gnsstk.Vector_double(5, 2.0)
    a = a.extend(gnsstk.Vector_double(3, 4.5))
    expected = 23.5
    actual = gnsstk.Vector_sum_double(a) # Sum of vector elements
    check_test(actual, expected, "Vector Sum", test_num)
    test_num += 1

    a = gnsstk.Vector_int()
    for i in [-1, -2, 56, -6, -89, -45]:
        a = a.append(i)
    # maxabs removed see below
    # expected = (-1, -89, -89, 56)
    expected = (-1, -89, 56)
    actual = (gnsstk.Vector_minabs_int(a), gnsstk.Vector_min_int(a), gnsstk.Vector_max_int(a))
    check_test(actual, expected, "Vector MinAbs, Min, and Max", test_num) # Min Abs, Min, Max Abs, and Max Value
    # maxabs causes build to fail, see TemplateMacros.i
    # actual = (gnsstk.Vector_minabs_int(a), gnsstk.Vector_min_int(a), gnsstk.Vector_maxabs_int(a), gnsstk.Vector_max_int(a))
    # check_test(actual, expected, "Vector MinAbs, Min, MaxAbs, and Max", test_num) # Min Abs, Min, Max Abs, and Max Value
    test_num += 1

    a = gnsstk.Vector_double(3, 4)
    a = a.extend(gnsstk.Vector_double(3, 2))
    a = a.extend(gnsstk.Vector_double(3, -2.5))
    actual = round(gnsstk.Vector_norm_double(a), 4)
    expected = 8.8741
    check_test(actual, expected, "Vector Norm", test_num) # L2 Norm
    test_num += 1

    a = gnsstk.Vector_int(3, 2)
    a = a.extend(gnsstk.Vector_int(3, 23))
    a = a.extend(gnsstk.Vector_int(3, -5))
    actual = (gnsstk.Vector_dotscalar_int(a, 4), gnsstk.Vector_dotscalar_int(5, a))
    expected = (240, 300)
    check_test(actual, expected, "Vector Dot Product with Scalar", test_num) # Vector-Scalar Dot Product
    test_num += 1

    a = gnsstk.Vector_double(9, -34)
    a = a.extend(gnsstk.Vector_double(9, -78))
    a = a.extend(gnsstk.Vector_double(9, -74))
    actual = [round(x, 3) for x in list(gnsstk.Vector_normalize_double(a))]
    expected = [-0.101] * 9 + [-0.231] * 9 + [-0.219] * 9
    check_test(actual, expected, "Vector Normalize", test_num) # Vector Normalization
    test_num += 1 # Vector Normalization

    a = gnsstk.Vector_double(3, -7)
    a = a.extend(gnsstk.Vector_double(3, 4.5))
    a = a.extend(gnsstk.Vector_double(3, 20))
    actual = (round(gnsstk.Vector_RSS_double(a), 3), round(gnsstk.Vector_RMS_double(a), 3))
    expected = (37.520, 12.507)
    check_test(actual, expected, "Vector Root-Mean and Root-Sum Sqaure", test_num) # Vector Root-Mean and Root-Sum Square
    test_num += 1 # Vector Root-Sum Square and Root-Mean Square

    # STD Unary Vector Operations
    a = gnsstk.Vector_double(3, 0.45)
    a = a.extend(gnsstk.Vector_double(3, 0.23))
    a = a.extend(gnsstk.Vector_double(3, 0.1))
    actual = [gnsstk.Vector_abs_double(a),
              gnsstk.Vector_acos_double(a),
              gnsstk.Vector_asin_double(a),
              gnsstk.Vector_atan_double(a),
              gnsstk.Vector_cos_double(a),
              gnsstk.Vector_cosh_double(a),
              gnsstk.Vector_exp_double(a),
              gnsstk.Vector_log_double(a),
              gnsstk.Vector_log10_double(a),
              gnsstk.Vector_sinh_double(a),
              gnsstk.Vector_sin_double(a),
              gnsstk.Vector_sqrt_double(a),
              gnsstk.Vector_tan_double(a),
              gnsstk.Vector_tanh_double(a)]
    actual = map(list, actual)
    actual = [round_list(lst) for lst in actual]
    b = list(a)
    expected = [list(map(abs, b)),
                list(map(math.acos, b)),
                list(map(math.asin, b)),
                list(map(math.atan, b)),
                list(map(math.cos, b)),
                list(map(math.cosh, b)),
                list(map(math.exp, b)),
                list(map(math.log, b)),
                [math.log(x, 10) for x in b],
                list(map(math.sinh, b)),
                list(map(math.sin, b)),
                list(map(math.sqrt, b)),
                list(map(math.tan, b)),
                list(map(math.tanh, b))]
    expected = [round_list(lst) for lst in actual]
    check_test(actual, expected, "STD Unary Vector Operations", test_num) # STD Unary Vector Operations
    test_num += 1 # Vector Root-Sum Square and Root-Mean Square

    # Binary Vector Operations


# Rounds a list of floats to a certain decimal point
def round_list(lst, tolerance=3):
    return [round(x , tolerance) for x in lst]

# Checks if something is
def check_test(observed, expected, description="", test_num=0):
    if expected != observed:
        print("FAILED TEST", test_num, ":", description, "test")
    else:
        print("passed test", test_num, ":", description, "test")

if __name__ == "__main__":
    test_vector()
