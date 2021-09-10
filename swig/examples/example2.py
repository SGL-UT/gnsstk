#!/usr/bin/env python

"""
A GNSSTk example of reading a RINEX obs file.

This is a semi-port of the older C++ example2.cpp.
Usage:
  python example2.py

"""

import gnsstk


def main():
    rfn = gnsstk.getPathData() + '/test_input_rinex2_obs_RinexObsFile.06o'
    header, data = gnsstk.readRinex3Obs(rfn, strict=True)

    # Let's pretend we want to change something in the header
    # (otherwise this would be a two-line example!)
    header.receiverOffset = 47

    # Now let's find the earliest and latest observations
    # function for how to compare Rinex3ObsData objects for min/max functions:
    timeFunction = lambda self: self.time
    earliest = min(data, key=timeFunction)
    latest = max(data, key=timeFunction)

    print 'Earliest time found:', gnsstk.CivilTime(earliest.time)
    print 'Latest time found:  ', gnsstk.CivilTime(latest.time)

    # Now let's write it all back to a different file
    gnsstk.writeRinex3Obs( 'rinex3obs_data.txt.new', header, data)

if __name__ == '__main__':
    main()
