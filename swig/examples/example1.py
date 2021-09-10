#!/usr/bin/env python

"""
A GNSSTk example with some simple time classes and functions.

This is a port of the older C++ example1.cpp.

Usage:
  python example1.py

"""


import gnsstk


def main():
    # In the GNSSTk there are multiple classes to manage time, depending
    # on the specific operation that we want to carry out. This modular
    # approach eases handling the many different time systems used in the
    # modern Global Navigation Satellite Systems.

    # Note, however, that in the GNSSTk the unifying class to do time
    # Computations is the 'CommonTime' class.

    # Read current time from system clock
    systime = gnsstk.SystemTime()

    # Convert to 'CommonTime', the standard way to handle time at GNSSTk
    comtime = systime.toCommonTime()

    # This is the typical way to handle civil time
    civtime = gnsstk.CivilTime(comtime)

    # The YDSTime class is very useful for common GNSS tasks
    ydstime = gnsstk.YDSTime(comtime)

    # This is a typical class to handle time in GPS system
    gpstime = gnsstk.GPSWeekSecond(comtime)

    # Class to handle Modified Julian Date
    mjd = gnsstk.MJD(comtime)

    print "Hello world!"
    print "   The current civil time is", civtime
    print "   The current year is", ydstime.year
    print "   The current day of year is", ydstime.doy
    print "   The current second of day is", ydstime.sod
    print "   The current full GPS week is", gpstime.week
    print "   The current short GPS week is", gpstime.getModWeek()
    print "   The current day of GPS week is", gpstime.getDayOfWeek()
    print "   The current second of GPS week is", gpstime.sow
    print "   The current Modified Julian Date is", mjd


if __name__ == '__main__':
    main()
