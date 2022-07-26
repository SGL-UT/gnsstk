#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class PythonTestCallback(gnsstk.NavDataFactoryCallback):
    # Define Python class 'constructor'
    def __init__(self, *args, **kwargs):
        gnsstk.NavDataFactoryCallback.__init__(self, *args, **kwargs)
        self.count = 0

    # Override C++ method
    def process(self, navOut):
        print("foo");
        return True

class TestNavDataFactoryCallback(unittest.TestCase):
    def test_callback(self):
        fact = gnsstk.MultiFormatNavDataFactory()
        uut = PythonTestCallback()
        fact.process(args.input_dir+
                     '/test_input_rinex_nav_RinexNavExample.99n',
                     uut)

if __name__ == '__main__':
    run_unit_tests()
