#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, run_unit_tests
import gnsstk

class Test_Exceptions(unittest.TestCase):

    def test_AntexData_raises_exception(self):
        """
        AntexData SWIG bindings must allow exceptions to be raised through python.
        """
        ad = gnsstk.AntexData()
        with self.assertRaises(RuntimeError) as e:
            ad.getPhaseCenterVariation("invalid", 0, 0)

        ad_exception = e.exception
        self.assertIn("GNSSTk exception", str(ad_exception))
        self.assertIn("text 0:Invalid AntexData object", str(ad_exception))


if __name__ == '__main__':
    run_unit_tests()
