#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestMultiFormatNavDataFactory(unittest.TestCase):
    def test_addDataSource(self):
        ndf = gpstk.MultiFormatNavDataFactory()
        ndf.addDataSource(args.input_dir+'/test_input_yuma377.txt')
        ndf.addDataSource(args.input_dir+
                          '/test_input_rinex_nav_RinexNavExample.99n')
        ndf.addDataSource(args.input_dir+'/test_input_SP3c.sp3')
        ndf.addDataSource(args.input_dir+'/test_input_sem387.txt')
        self.assertEqual(1629, ndf.size())
        ndf.clear()
        self.assertEqual(0, ndf.size())

if __name__ == '__main__':
    run_unit_tests()
