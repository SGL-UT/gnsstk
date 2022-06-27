#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, assertSequenceAlmostEqual, run_unit_tests, vec_to_list

import gnsstk


class TestTriple(unittest.TestCase):

    def test_constructor(self):
        a = gnsstk.Triple(1, 2, 3)
        self.assertSequenceEqual([1, 2, 3], vec_to_list(a))
        
    def test_to_vector(self):
        a = gnsstk.Triple(2, 4, 6)

        a_vector = a.toVector()
        self.assertSequenceEqual([2, 4, 6], vec_to_list(a_vector))

        a_std_vector = a.toStdVector()
        self.assertSequenceEqual([2, 4, 6], vec_to_list(a_std_vector))

    def test_products(self):
        a = gnsstk.Triple(1.5, 2.3, 3.6)
        b = gnsstk.Triple(3.4, 6.8, 4.5)
        
        dot_product = a.dot(b)
        self.assertAlmostEqual(36.94, dot_product)

        cross_product = a.cross(b)
        assertSequenceAlmostEqual(self, [-14.13, 5.49, 2.38], vec_to_list(cross_product))

    def test_magnitude(self):
        a = gnsstk.Triple(1.5, 2.3, 3.6)
        self.assertAlmostEqual(4.52769256, a.mag())

    def test_two_vector_computations(self):
        a = gnsstk.Triple(1.5, 2.3, 3.6)
        b = gnsstk.Triple(3.4, 6.8, 4.5)
        
        self.assertAlmostEqual(0.92349278, a.cosVector(b))
        self.assertAlmostEqual(4.96689037, a.slantRange(b))
        self.assertAlmostEqual(46.97353420, a.elvAngle(b))
        self.assertAlmostEqual(165.18222367, a.azAngle(b))

    def test_rotation(self):
        a = gnsstk.Triple(22.5, -56, 30)
        
        # 35 deg about the x-axis
        a = a.R1(35)  
        assertSequenceAlmostEqual(self, [22.5, -28.66522138, 56.69484176], vec_to_list(a))

        # 24 deg about the y-axis
        a = a.R2(24)
        assertSequenceAlmostEqual(self, [-2.50509682, -28.66522138, 60.94488963], vec_to_list(a))
        
        # -89 deg about the z-axis 
        a = a.R3(-89)  
        assertSequenceAlmostEqual(self, [28.61713556, -3.00499237, 60.94488963], vec_to_list(a))

    def test_elementwise_ops(self):
        a = gnsstk.Triple(2.3, 4.5, 3.6)
        b = gnsstk.Triple(-9.0, 34.4, 23.2)
        
        c = a - b
        assertSequenceAlmostEqual(self, [11.3, -29.9, -19.6], vec_to_list(c))

        c = c + b
        assertSequenceAlmostEqual(self, [2.3, 4.5, 3.6], vec_to_list(c))
        
        # The `==` is element wise comparison
        self.assertFalse(gnsstk.Triple(1, 2, 3) == gnsstk.Triple(3, 2, 1))
        self.assertTrue(gnsstk.Triple(1, 2, 3) == gnsstk.Triple(1, 2, 3))
        
    def test_scale(self):
        a = gnsstk.Triple(2.3, 4.5, 3.6)
        a = a.scale(4.5)
        
        assertSequenceAlmostEqual(self, [10.35, 20.25, 16.2], vec_to_list(a))

    def test_size(self):
        a = gnsstk.Triple(2.3, 4.5, 3.6)
        
        # Triple should always be size 3
        self.assertEqual(3, a.size())
        self.assertEqual(3, len(a))


if __name__ == "__main__":
    run_unit_tests()

