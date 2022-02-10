#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk


class Test_Binex(unittest.TestCase):
    def test_BinexData(self):
        a = gnsstk.BinexData()
        b = gnsstk.UBNXI()
        c = gnsstk.MGFZI()
        self.assertTrue(isinstance(a, gnsstk.BinexData))
        self.assertTrue(isinstance(b, gnsstk.UBNXI))
        self.assertTrue(isinstance(c, gnsstk.MGFZI))
        self.assertEqual(a.getMessageLength(), 0)
        self.assertEqual(b.MAX_VALUE, 536870911)
        self.assertEqual(1, c.getSize())

    def test_BinexFilterOperators(self):
        a = gnsstk.BinexDataOperatorEquals()
        self.assertTrue(isinstance(a, gnsstk.BinexDataOperatorEquals))

    def test_BinexStream(self):
        a = gnsstk.BinexStream()
        self.assertTrue(isinstance(a, gnsstk.BinexStream))

class Test_RinexClock(unittest.TestCase):
    def test_RinexClockBase(self):
        a = gnsstk.RCDouble(14)
        b = gnsstk.RinexClkType()
        self.assertTrue(isinstance(a, gnsstk.RCDouble))
        self.assertTrue(isinstance(b, gnsstk.RinexClkType))

    def test_RinexClockData(self):
        a = gnsstk.RinexClockData()
        self.assertTrue(isinstance(a, gnsstk.RinexClockData))
        self.assertTrue(isinstance(a.name, str))

    def test_RinexClockHeader(self):
        a = gnsstk.RinexClockHeader()
        self.assertTrue(a, gnsstk.RinexClockHeader)
        b = gnsstk.RefClk()
        self.assertTrue(b, gnsstk.RefClk)
        c = gnsstk.RefClkRecord()
        self.assertTrue(c, gnsstk.RefClkRecord)
        d = gnsstk.SolnSta()
        self.assertTrue(a, gnsstk.SolnSta)
        self.assertEqual(a.versionString, "RINEX VERSION / TYPE")

    def test_RinexClockStream(self):
        a = gnsstk.RinexClockStream()
        self.assertTrue(a, gnsstk.RinexClockStream)
        self.assertTrue(not a.headerRead)


class Test_RinexMet(unittest.TestCase):

    # TODO: The filter operators are there but did not have time to test since no default constructor
    # def test_MetFilterOperators(self):
    #     a = gnsstk.RinexMetDataOperatorLessThanFull()
    #     b = gnsstk.RinexMetDataOperatorLessThanSimple()
    #     c = gnsstk.RinexMetDataOperatorEqualsSimple()
    #     d = gnsstk.RinexMetHeaderTouchHeaderMerge()
    #     e = gnsstk.RinexMetDataFilterTime()

    def test_RinexMetData(self):
        a = gnsstk.RinexMetData()
        self.assertTrue(isinstance(a, gnsstk.RinexMetData))
        self.assertTrue(a.isData())

    def test_RinexMetStream(self):
        a = gnsstk.RinexMetStream()
        self.assertTrue(isinstance(a, gnsstk.RinexMetStream))
        self.assertTrue(not a.headerRead)

    def test_RinexMetHeader(self):
        a = gnsstk.RinexMetHeader()
        self.assertTrue(isinstance(a, gnsstk.RinexMetHeader))


class Test_RinexNav(unittest.TestCase):
    def test_RinexNavData(self):
        a = gnsstk.RinexNavData()
        self.assertTrue(isinstance(a, gnsstk.RinexNavData))
        self.assertTrue(isinstance(a.getHOWTime(), gnsstk.CommonTime))

    # TODO: The filter operators are there but did not have time to test since no default constructor

    def test_RinexNavHeader(self):
        a = gnsstk.RinexNavHeader()
        self.assertTrue(isinstance(a, gnsstk.RinexNavHeader))
        self.assertTrue(a.isHeader())

    def test_RinexNavStream(self):
        a = gnsstk.RinexNavStream()
        self.assertTrue(isinstance(a, gnsstk.RinexNavStream))
        self.assertTrue(not a.headerRead)

    def test_RinexObsData(self):
        a = gnsstk.RinexObsData()
        self.assertTrue(isinstance(a, gnsstk.RinexObsData))
        self.assertEqual(a.isData(), True)

    def test_RinexObsHeader(self):
        a = gnsstk.RinexObsHeader()
        self.assertTrue(isinstance(a, gnsstk.RinexObsHeader))
        self.assertEqual(a.isHeader(), True)

    def test_RinexObsStream(self):
        a = gnsstk.RinexObsStream()
        self.assertTrue(isinstance(a, gnsstk.RinexObsStream))
        self.assertEqual(a.headerRead, False)

# TODO: Write tests for RinexUtilities
# TODO: Write tests for Sinex when wrapped

class Test_SP3(unittest.TestCase):
    def test_SP3Data(self):
        a = gnsstk.SP3Data()
        self.assertTrue(isinstance(a, gnsstk.SP3Data))
        self.assertTrue(a.isData())
        self.assertTrue(isinstance(a.sat, gnsstk.SatID))

    def test_SP3Header(self):
        a = gnsstk.SP3Header()
        self.assertTrue(isinstance(a, gnsstk.SP3Header))
        self.assertTrue(isinstance(a.timeSystemString(), str))

    def test_SP3Stream(self):
        a = gnsstk.SP3Stream()
        self.assertTrue(isinstance(a, gnsstk.SP3Stream))
        self.assertTrue(isinstance(a.currentEpoch, gnsstk.CommonTime))

if __name__ == '__main__':
    run_unit_tests()
