#!/usr/bin/env python

"""
An example of reading a rinex file and extracting the
pseudrange obs and computing a biased multipath observation.
"""

# We recommend only using
#     'import gnsstk',
# but if you need constants fairly often, then importing them specifically at once
# may be easier than referring to them by gnsstk.L1_FREQ_GPS.
from gnsstk import C_MPS, GAMMA_GPS, L1_FREQ_GPS
import gnsstk


rfn = gnsstk.getPathData() + '/test_input_rinex2_obs_RinexObsFile.06o'

# Make a GNSSTk SatID used to find a specific satellite in the data
svid = gnsstk.RinexSatID(5, gnsstk.SatelliteSystem.GPS)

try:
    header, data = gnsstk.readRinex3Obs(rfn, strict=True)
    print header

    # Loop through all the epochs and process the data for each one
    for d in data:
        # Note that d is now a Rinex3ObsData

        # Check if the PRN is in view (by searching for it)
        if d.obs.find(svid) == d.obs.end():
            print gnsstk.CivilTime(d.time), 'svid', svid, 'not in view'
        else:
            P1 = d.getObs(svid, "C1W", header).data
            P2 = d.getObs(svid, "C2W", header).data
            L1 = d.getObs(svid, "L1C", header).data
            mu = P1 - L1*(C_MPS/L1_FREQ_GPS) - 2*(P1-P2)/(1-GAMMA_GPS)
            print gnsstk.CivilTime(d.time), svid, 'biased multipath', mu

# We can catch any custom gnsstk exception like this:
except gnsstk.InvalidRequest as e:
    print "InvalidRequest:",e
except gnsstk.Exception as e:
    print e

