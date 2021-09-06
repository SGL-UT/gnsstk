#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestSEMNavDataFactory(unittest.TestCase):
    def test_addDataSource(self):
        ndf = gpstk.SEMNavDataFactory()
        ndf.addDataSource(args.input_dir+'/test_input_sem387.txt')
        self.assertEqual(60, ndf.size())
        ndf.clear()
        self.assertEqual(0, ndf.size())

if __name__ == '__main__':
    run_unit_tests()
