#!/usr/bin/env python

"""
An example reading a RINEX obs, nav and met file and using
PRSolutionLegacy to computer a receiver position and compare
this to the position in the obs header.
"""
import gnsstk
import sys

obsfn = gnsstk.getPathData() + '/arlm200z.15o'
navfn = gnsstk.getPathData() + '/arlm200z.15n'
metfn = gnsstk.getPathData() + '/arlm200z.15m'

navHeader, navData = gnsstk.readRinex3Nav(navfn)
ephStore = gnsstk.gnsstk.Rinex3EphemerisStore()
for navDataObj in navData:
    ephStore.addEphemeris(navDataObj)

tropModel = gnsstk.GGTropModel()
metHeader, metData = gnsstk.readRinexMet(metfn)

obsHeader, obsData = gnsstk.readRinex3Obs(obsfn)

indexP1 = obsHeader.getObsIndex('C1W')
indexP2 = obsHeader.getObsIndex('C2W')
raimSolver = gnsstk.PRSolutionLegacy()

for obsObj in obsData:
    for metObj in metData:
        if metObj.time >= obsObj.time:
            break
        else:
            metDataDict = metObj.getData()
            temp = metDataDict[gnsstk.RinexMetHeader.TD]
            pressure = metDataDict[gnsstk.RinexMetHeader.PR]
            humidity = metDataDict[gnsstk.RinexMetHeader.HR]
            tropModel.setWeather(temp, pressure, humidity)

    if obsObj.epochFlag == 0 or obsObj.epochFlag == 1:
        # Note that we use lists here, but we will need types backed
        # by C++ std::vectors later. We'll just keep it easy and use
        # gnsstk.seqToVector to convert them. If there was a speed
        # bottleneck we could use gnsstk.cpp.vector_SatID and
        # gnsstk.cpp.vector_double though.
        prnList = []
        rangeList = []

        # This part gets the PRN numbers and ionosphere-corrected
        # pseudoranges for the current epoch. They are correspondly fed
        # into "prnList" and "rangeList"; "obs" is a public attribute of
        # Rinex3ObsData to get the map of observations
        for satID, datumList in obsObj.obs.iteritems():
            # The RINEX file may have P1 observations, but the current
             # satellite may not have them.
            P1 = 0.0
            try:
                P1 = obsObj.getObs(satID, indexP1).data
            except gnsstk.exceptions.Exception:
                continue  # Ignore this satellite if P1 is not found

            ionocorr = 0.0

              # If there are P2 observations, let's try to apply the
              # ionospheric corrections
            if indexP2 >= 0:
                # The RINEX file may have P2 observations, but the
                # current satellite may not have them.
                P2 = 0.0
                try:
                    P2 = obsObj.getObs(satID, indexP2).data
                except gnsstk.exceptions.Exception:
                    continue  # Ignore this satellite if P1 is not found
                # list 'vecList' contains RinexDatum, whose public
                # attribute "data" indeed holds the actual data point
                ionocorr = 1.0 / (1.0 - gnsstk.GAMMA_GPS) * ( P1 - P2 )

            # Now, we include the current PRN number in the first part
            # of "it" iterator into the list holding the satellites.
            # All satellites in view at this epoch that have P1 or P1+P2
            # observations will be included.
            prnList.append(satID)

            # The same is done for the list of doubles holding the
            # corrected ranges
            rangeList.append(P1 - ionocorr)
            # WARNING: Please note that so far no further correction
            # is done on data: Relativistic effects, tropospheric
            # correction, instrumental delays, etc

        # The default constructor for PRSolutionLegacy objects (like
        # "raimSolver") is to set a RMSLimit of 6.5. We change that
        # here. With this value of 3e6 the solution will have a lot
        # more dispersion.
        raimSolver.RMSLimit = 3e6


        # In order to compute positions we need the current time, the
        # vector of visible satellites, the vector of corresponding
        # ranges, the object containing satellite ephemerides, and a
        # pointer to the tropospheric model to be applied

        time = obsObj.time

        # the RAIMComputer method of PRSolutionLegacy accepts a vector<SatID> as its
        # 2nd argument, but the list is of RinexSatID, which is a subclass of SatID.
        # Since C++ containers are NOT covariant, it is neccessary to change the
        # output to a vector or SatID's rather thta a vector of RinexSatID's.
        satVector = gnsstk.seqToVector(prnList, outtype='vector_SatID')
        rangeVector = gnsstk.seqToVector(rangeList)
        raimSolver.RAIMCompute(time, satVector, rangeVector, ephStore, tropModel)

        # Note: Given that the default constructor sets public
        # attribute "Algebraic" to FALSE, a linearized least squares
        # algorithm will be used to get the solutions.
        # Also, the default constructor sets ResidualCriterion to true,
        # so the rejection criterion is based on RMS residual of fit,
        # instead of RMS distance from an a priori position.


        if raimSolver.isValid():
            # Vector "Solution" holds the coordinates, expressed in
            # meters in an Earth Centered, Earth Fixed (ECEF) reference
            # frame. The order is x, y, z  (as all ECEF objects)
            pos = gnsstk.Triple(raimSolver.Solution[0],  raimSolver.Solution[1], raimSolver.Solution[2])
            err = obsHeader.antennaPosition - pos
            print err
