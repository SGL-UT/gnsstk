#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestRinexNavDataFactory(unittest.TestCase):
    def test_addDataSource(self):
        ndf = gnsstk.RinexNavDataFactory()
        ndf.addDataSource(args.input_dir+
                          '/test_input_rinex_nav_RinexNavExample.99n')
        self.assertEqual(9, ndf.size())

if __name__ == '__main__':
    run_unit_tests()
