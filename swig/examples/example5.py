#!/usr/bin/env python

"""
An example of how to walk a RINEX data set and use SatID and ObsID,
building an obsEpochMap of the data along the way.
"""

import gnsstk

fn = gnsstk.getPathData() + '/arlm200z.15o'

# read in header and get a generator for the data
header, data = gnsstk.readRinex3Obs(fn)
print header

oem = gnsstk.ObsEpochMap()
for d in data:
    print gnsstk.CivilTime(d.time)
    oe=gnsstk.ObsEpoch()
    oe.time = d.time
    for sv in d.obs.keys():
        # sv is an SatID object
        print sv,
        epoch = d.obs[sv]
        soe = gnsstk.SvObsEpoch()
        soe.svid = sv
        for i in range(len(epoch)):
            rinex2_obs_type = header.R2ObsTypes[i]
            oid = header.mapObsTypes['G'][i]
            print "{}({})={}".format(str(oid), rinex2_obs_type, epoch[i].data),
            soe[oid] = epoch[i].data
        oe[sv] = soe
        print
    oem[d.time] = oe
