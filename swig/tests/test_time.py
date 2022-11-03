#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, run_unit_tests

import time
from gnsstk import CommonTime, SystemTime, CivilTime, BDSWeekSecond, GPSWeekSecond, GPSWeekZcount, TimeSystem
from gnsstk import JulianDate, MJD, UnixTime, YDSTime
import gnsstk

class TestTimeHandling(unittest.TestCase):

    def test_TimeConstants(self):
        a = gnsstk.MJD_TO_JD
        b = gnsstk.FULLWEEK
        c = gnsstk.SEC_PER_DAY
        self.assertIsNotNone(a)
        self.assertIsNotNone(b)
        self.assertIsNotNone(c)

    def test_CommonTime(self):
        a = gnsstk.CommonTime(gnsstk.TimeSystem.GPS)
        self.assertIsInstance(a, gnsstk.CommonTime)
        self.assertEqual(str(a.getTimeSystem()), "GPS")

    def test_ANSITime(self):
        a = gnsstk.ANSITime()
        self.assertIsInstance(a, gnsstk.ANSITime)

class TestSystemTime(unittest.TestCase):
    def test_SystemTime(self):
        t0 = SystemTime()
        self.assertIsInstance(t0, gnsstk.SystemTime)

class TestTimeTags(unittest.TestCase):
    def test_CivilTime(self):
        t0 = CivilTime(1980, 1, 6, 0, 0, 0, TimeSystem.GPS)
        self.assertEqual(str(t0), '01/06/1980 00:00:00 GPS')
        ct = t0.toCommonTime()
        self.assertEqual(str(ct), '2444245 00000000 0.000000000000000 GPS')
        t1 = CivilTime(ct)
        self.assertEqual(str(t1), str(t0))

    def test_BDSWeekSecond(self):
        t0 = BDSWeekSecond( 0, 0)
        self.assertEqual(str(t0), "0 0.000000 BDT")
        t1 = CivilTime(t0.toCommonTime())
        self.assertEqual(str(t1), "01/01/2006 00:00:00 BDT")
        t2 = BDSWeekSecond(t1.toCommonTime())
        self.assertEqual(str(t2), str(t0))

    def test_GPSWeekSecond(self):
        t0 = GPSWeekSecond(1023, 86400*7-1)
        self.assertEqual(str(t0), "1023 604799.000000 GPS")
        t1 = GPSWeekSecond(t0.toCommonTime())
        self.assertEqual(str(t1), "1023 604799.000000 GPS")
        t3 = GPSWeekSecond(t0.toCommonTime()+1)
        self.assertEqual(str(t3), "1024 0.000000 GPS")
        t4 = CivilTime(t3.toCommonTime())
        self.assertEqual(str(t4), "08/22/1999 00:00:00 GPS")

    def test_GPSWeekZcount(self):
        t0 = GPSWeekZcount(1023, (2*86400//3)*7-1)
        self.assertEqual(str(t0), "1023 403199 GPS")
        t1 = GPSWeekZcount(t0.toCommonTime())
        self.assertEqual(str(t1), "1023 403199 GPS")
        t2 = GPSWeekZcount(t0.toCommonTime()+1.5)
        self.assertEqual(str(t2), "1024 000000 GPS")
        t3 = CivilTime(t2.toCommonTime())
        self.assertEqual(str(t3), "08/22/1999 00:00:00 GPS")

    def test_JulianDate(self):
        t0 = JulianDate(0.0)
        self.assertEqual(str(t0), "0.000000 UNK")
        t1 = CivilTime(t0.toCommonTime())
        # See http://aa.usno.navy.mil/data/docs/JulianDate.php
        self.assertEqual(str(t1), "01/01/-4713 12:00:00 UNK")
        t2 = JulianDate(2444244.5)
        t3 = CivilTime(t2.toCommonTime())
        self.assertEqual(str(t3), "01/06/1980 00:00:00 UNK")

    def test_MJD(self):
        t0 = MJD(50449)
        t1 = CivilTime(t0.toCommonTime())
        self.assertEqual(str(t1), "01/01/1997 00:00:00 UNK")

    def test_UnixTime(self):
        t0 = UnixTime(315964800)
        t1 = CivilTime(t0.toCommonTime())
        self.assertEqual(str(t1), "01/06/1980 00:00:00 UNK")
        t2 = CivilTime(2038, 1, 19, 3, 14, 7.999999)
        t3 = UnixTime(t2.toCommonTime())
        self.assertEqual(str(t3), "2147483647 999999 UNK")

    def test_YDSTime(self):
        t0 = YDSTime(1980, 6, 0.0)
        t1 = CivilTime(t0.toCommonTime())
        self.assertEqual(str(t1), "01/06/1980 00:00:00 UNK")

class CommonTime_Tests(unittest.TestCase):
    def test(self):
        a = gnsstk.CommonTime()
        a.addDays(1234)
        b = gnsstk.CommonTime(a)
        b.addSeconds(123.4)
        c = b - a
        self.assertAlmostEqual(1234.0, a.getDays())
        self.assertEqual('0001234 00000000 0.000000000000000 UNK', str(a))
        self.assertAlmostEqual(1234.0014282407408, b.getDays())
        self.assertEqual('0001234 00123400 0.000000000000000 UNK', str(b))
        self.assertAlmostEqual(123.4, c)
        self.assertEqual(gnsstk.TimeSystem.Unknown, b.getTimeSystem())
        self.assertEqual(str(b.getTimeSystem()), 'Unknown', msg='default time system should be Unknown')

    def test_exception(self):
        # subtracting 2 CommonTimes throws an InvalidRequest
        a = gnsstk.CommonTime(gnsstk.TimeSystem.GPS)
        b = gnsstk.CommonTime(gnsstk.TimeSystem.GLO)
        self.assertRaises(gnsstk.InvalidRequest, a.__sub__, b)
        self.assertEqual(str(a.getTimeSystem()), 'GPS', msg='TimeSystem enum should properly cast to string.')
        self.assertEqual(str(b.getTimeSystem()), 'GLO', msg='TimeSystem enum should properly cast to string')

    def test_times_gen(self):
        start = gnsstk.CommonTime()
        start.addSeconds(100.0)
        end = gnsstk.CommonTime()
        end.addSeconds(900.0)
        times = gnsstk.times(start, end, seconds=200.0)
        self.assertEqual(100.0, next(times).getSecondOfDay())
        self.assertEqual(300.0, next(times).getSecondOfDay())
        self.assertEqual(500.0, next(times).getSecondOfDay())
        self.assertEqual(700.0, next(times).getSecondOfDay())
        self.assertEqual(900.0, next(times).getSecondOfDay())
        if sys.version_info[0] < 3:
            self.assertRaises(StopIteration, times.next)
        else:
            self.assertRaises(StopIteration, times.__next__)

    def test_times_list(self):
        start = gnsstk.CommonTime()
        start.addSeconds(100.0)
        end = gnsstk.CommonTime()
        end.addSeconds(900.0)
        times = list(gnsstk.times(start, end, seconds=200.0))
        self.assertEqual(100.0, times[0].getSecondOfDay())
        self.assertEqual(300.0, times[1].getSecondOfDay())
        self.assertEqual(500.0, times[2].getSecondOfDay())
        self.assertEqual(700.0, times[3].getSecondOfDay())
        self.assertEqual(900.0, times[4].getSecondOfDay())

        times = list(gnsstk.times(start, end))
        self.assertEqual(2, len(times))
        self.assertEqual(times[0], start)
        self.assertEqual(times[1], end)


class ScanTimes(unittest.TestCase):
    def test_scanTime(self):
        def test(instr, fmt, expected='', raises=None):
            if raises is not None:
                self.assertRaises(raises, gnsstk.scanTime, instr, fmt)
            else:
                self.assertEqual(expected, str(gnsstk.scanTime(instr, fmt)))

        # ANSI
        test('10000', '%K', '2440588 10000000 0.000000000000000 UNK')
        test('100000000', '%x', raises=gnsstk.InvalidRequest)  # bad format

if __name__ == '__main__':
    run_unit_tests()
