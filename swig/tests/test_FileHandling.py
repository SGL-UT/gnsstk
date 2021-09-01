#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests
import gpstk as gpstk


class Test_Binex(unittest.TestCase):
    def test_BinexData(self):
        a = gpstk.BinexData()
        b = gpstk.UBNXI()
        c = gpstk.MGFZI()
        self.assertTrue(isinstance(a, gpstk.BinexData))
        self.assertTrue(isinstance(b, gpstk.UBNXI))
        self.assertTrue(isinstance(c, gpstk.MGFZI))
        self.assertEqual(a.getMessageLength(), 0)
        self.assertEqual(b.MAX_VALUE, 536870911)
        self.assertEqual(1, c.getSize())

    def test_BinexFilterOperators(self):
        a = gpstk.BinexDataOperatorEquals()
        self.assertTrue(isinstance(a, gpstk.BinexDataOperatorEquals))

    def test_BinexStream(self):
        a = gpstk.BinexStream()
        self.assertTrue(isinstance(a, gpstk.BinexStream))

class Test_RinexClock(unittest.TestCase):
    def test_RinexClockBase(self):
        a = gpstk.RCDouble(14)
        b = gpstk.RinexClkType()
        self.assertTrue(isinstance(a, gpstk.RCDouble))
        self.assertTrue(isinstance(b, gpstk.RinexClkType))

    def test_RinexClockData(self):
        a = gpstk.RinexClockData()
        self.assertTrue(isinstance(a, gpstk.RinexClockData))
        self.assertTrue(isinstance(a.name, str))

    def test_RinexClockHeader(self):
        a = gpstk.RinexClockHeader()
        self.assertTrue(a, gpstk.RinexClockHeader)
        b = gpstk.RefClk()
        self.assertTrue(b, gpstk.RefClk)
        c = gpstk.RefClkRecord()
        self.assertTrue(c, gpstk.RefClkRecord)
        d = gpstk.SolnSta()
        self.assertTrue(a, gpstk.SolnSta)
        self.assertEqual(a.versionString, "RINEX VERSION / TYPE")

    def test_RinexClockStream(self):
        a = gpstk.RinexClockStream()
        self.assertTrue(a, gpstk.RinexClockStream)
        self.assertTrue(not a.headerRead)


class Test_RinexMet(unittest.TestCase):

    # TODO: The filter operators are there but did not have time to test since no default constructor
    # def test_MetFilterOperators(self):
    #     a = gpstk.RinexMetDataOperatorLessThanFull()
    #     b = gpstk.RinexMetDataOperatorLessThanSimple()
    #     c = gpstk.RinexMetDataOperatorEqualsSimple()
    #     d = gpstk.RinexMetHeaderTouchHeaderMerge()
    #     e = gpstk.RinexMetDataFilterTime()

    def test_RinexMetData(self):
        a = gpstk.RinexMetData()
        self.assertTrue(isinstance(a, gpstk.RinexMetData))
        self.assertTrue(a.isData())

    def test_RinexMetStream(self):
        a = gpstk.RinexMetStream()
        self.assertTrue(isinstance(a, gpstk.RinexMetStream))
        self.assertTrue(not a.headerRead)

    def test_RinexMetHeader(self):
        a = gpstk.RinexMetHeader()
        self.assertTrue(isinstance(a, gpstk.RinexMetHeader))


class Test_RinexNav(unittest.TestCase):
    def test_RinexNavData(self):
        a = gpstk.RinexNavData()
        self.assertTrue(isinstance(a, gpstk.RinexNavData))
        self.assertTrue(isinstance(a.getHOWTime(), gpstk.CommonTime))

    # TODO: The filter operators are there but did not have time to test since no default constructor

    def test_RinexNavHeader(self):
        a = gpstk.RinexNavHeader()
        self.assertTrue(isinstance(a, gpstk.RinexNavHeader))
        self.assertTrue(a.isHeader())

    def test_RinexNavStream(self):
        a = gpstk.RinexNavStream()
        self.assertTrue(isinstance(a, gpstk.RinexNavStream))
        self.assertTrue(not a.headerRead)

    def test_RinexObsData(self):
        a = gpstk.RinexObsData()
        self.assertTrue(isinstance(a, gpstk.RinexObsData))
        self.assertEqual(a.isData(), True)

    def test_RinexObsHeader(self):
        a = gpstk.RinexObsHeader()
        self.assertTrue(isinstance(a, gpstk.RinexObsHeader))
        self.assertEqual(a.isHeader(), True)

    def test_RinexObsStream(self):
        a = gpstk.RinexObsStream()
        self.assertTrue(isinstance(a, gpstk.RinexObsStream))
        self.assertEqual(a.headerRead, False)

# TODO: Write tests for RinexUtilities
# TODO: Write tests for Sinex when wrapped

class Test_SP3(unittest.TestCase):
    def test_SP3Data(self):
        a = gpstk.SP3Data()
        self.assertTrue(isinstance(a, gpstk.SP3Data))
        self.assertTrue(a.isData())
        self.assertTrue(isinstance(a.sat, gpstk.SatID))

    def test_SP3Header(self):
        a = gpstk.SP3Header()
        self.assertTrue(isinstance(a, gpstk.SP3Header))
        self.assertTrue(isinstance(a.timeSystemString(), str))

    def test_SP3Stream(self):
        a = gpstk.SP3Stream()
        self.assertTrue(isinstance(a, gpstk.SP3Stream))
        self.assertTrue(isinstance(a.currentEpoch, gpstk.CommonTime))

if __name__ == '__main__':
    run_unit_tests()
