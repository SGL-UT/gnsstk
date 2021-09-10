#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, run_unit_tests

import gnsstk
import math
import numpy as np

def test_misc_math():
    # Lagrange Interpolation
    test_num = 1


def test_triple():
    test_num = 1
    # Constructing and indexing a Triple
    a = gnsstk.Triple(1, 2, 3)
    expected = ([1, 2, 3])
    actual = ([a[0], a[1], a[2]])
    check_test(actual, expected, "Triple Contructor and indexing check", test_num)
    test_num += 1
    # Conversion to stdvector and GNSSTK vector
    a = gnsstk.Triple(2, 4, 6)
    expected = ([2, 4, 6], [2, 4, 6])
    actual = (list(a.toVector()), list(a.toStdVector())) # Type is not quite correct
    check_test(actual, expected, "Conversion to stdvector and gnsstk vector", test_num)
    test_num+=1
    # Cross and dot product
    a = gnsstk.Triple(1.5, 2.3, 3.6)
    b = gnsstk.Triple(3.4, 6.8, 4.5)
    np_a = np.array(gnsstk.makeTuple(a))
    np_b = np.array(gnsstk.makeTuple(b))
    expected = (np.dot(np_a, np_b), tuple(np.cross(np_a, np_b)))
    actual = (a.dot(b), gnsstk.makeTuple(a.cross(b)))
    check_test(actual, expected, "Dot and Cross Product", test_num)
    test_num += 1
    # Magnitude Test
    a = gnsstk.Triple(1.5, 2.3, 3.6)
    expected = np.linalg.norm(np.array(gnsstk.makeTuple(a)))
    actual = a.mag()
    check_test(actual, expected, "Magnitude", test_num)
    test_num += 1
    # Cosine, slant range, elevation angle, azimuth angle between two triples
    a = gnsstk.Triple(1.5, 2.3, 3.6)
    b = gnsstk.Triple(3.4, 6.8, 4.5)
    expected = round_list([0.923493, 4.96689, 46.9735, 165.182])
    actual = round_list([a.cosVector(b), a.slantRange(b), a.elvAngle(b), a.azAngle(b)])
    check_test(actual, expected, "Dot and Cross Product", test_num)
    test_num += 1
    # Rotations about x, y, and z axis
    a = gnsstk.Triple(22.5, -56, 30)
    rangle_x = 35
    rangle_y = 24
    rangle_z = -89
    expected = round_list([22.5, -28.6652, 56.6948, -2.5051, -28.6652, 60.9449, 28.6171, -3.00499, 60.9449])
    actual = []
    a = a.R1(rangle_x)
    actual.extend([a[0], a[1], a[2]])
    a = a.R2(rangle_x)
    actual.extend([a[0], a[1], a[2]])
    a = a.R3(rangle_x)
    actual.extend([a[0], a[1], a[2]])
    actual = round_list(expected)
    check_test(actual, expected, "Rotations about x-y-z axes", test_num)
    test_num += 1
    # Elementwise addition, subtraction, and 'equal to'
    a = gnsstk.Triple(2.3, 4.5, 3.6)
    b = gnsstk.Triple(-9.0, 34.4, 23.2)
    actual = []
    c = a - b
    actual.extend(round_list([c[0], c[1], c[2]]))
    actual.append(c == a)
    c = c + b
    actual.extend(round_list([c[0], c[1], c[2]]))
    actual.append(a == gnsstk.Triple(2.3, 4.5, 3.6))
    expected = [11.3, -29.9, -19.6, False, 2.3, 4.5, 3.6, True]
    check_test(actual, expected, "Elementwise addition, subtraction, and 'equal to'", test_num)
    test_num += 1
    # Scale
    a = gnsstk.Triple(2.3, 4.5, 3.6)
    a = a.scale(4.5)
    actual = round_list([a[0], a[1], a[2]])
    expected = round_list([10.35, 20.25, 16.2])
    check_test(actual, expected, "Triple scale", test_num)
    test_num += 1
    # Triple size
    a = gnsstk.Triple(2.3, 4.5, 3.6)
    expected = (3, 3)
    actual = (len(a), a.size())
    check_test(actual, expected, "Tripe size", test_num)
    test_num += 1


# Rounds a list of floats to a certain number of decimal places
def round_list(lst, tolerance=3):
    return [round(x , tolerance) for x in lst]

# Checks if something is
def check_test(observed, expected, description="", test_num=0):
    if expected != observed:
        print("FAILED TEST", test_num, ":", description, "test")
    else:
        print("passed test", test_num, ":", description, "test")

if __name__ == "__main__":
    test_triple()
