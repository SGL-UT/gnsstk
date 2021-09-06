#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestSP3NavDataFactory(unittest.TestCase):
    def test_addDataSource(self):
        ndf = gpstk.SP3NavDataFactory()
        ndf.addDataSource(args.input_dir+'/test_input_SP3c.sp3')
        self.assertEqual(1500, ndf.size())
        ndf.clear()
        self.assertEqual(0, ndf.size())

if __name__ == '__main__':
    run_unit_tests()
