//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================

#include <typeinfo>
#include <vector>
#include "ord.hpp"
#include "GPSEllipsoid.hpp"
#include "GNSSconstants.hpp"

using std::vector;
using std::cout;

namespace gnsstk {
namespace ord {

// When calculating range with the receiver's clock, the rotation of the earth
// during the time between transmission and receipt must be included. This
// updates svPosVel to account for that rotation.
//
Xvt rotateEarth(const Position& Rx, const Xvt& svPosVel,
        const EllipsoidModel& ellipsoid) {
    Xvt revisedXvt(svPosVel);

    double tof = RSS(svPosVel.x[0] - Rx.X(), svPosVel.x[1] - Rx.Y(),
            svPosVel.x[2] - Rx.Z()) / ellipsoid.c();
    double wt = ellipsoid.angVelocity() * tof;
    double sx = ::cos(wt) * svPosVel.x[0] + ::sin(wt) * svPosVel.x[1];
    double sy = -::sin(wt) * svPosVel.x[0] + ::cos(wt) * svPosVel.x[1];
    revisedXvt.x[0] = sx;
    revisedXvt.x[1] = sy;
    sx = ::cos(wt) * svPosVel.v[0] + ::sin(wt) * svPosVel.v[1];
    sy = -::sin(wt) * svPosVel.v[0] + ::cos(wt) * svPosVel.v[1];
    revisedXvt.v[0] = sx;
    revisedXvt.v[1] = sy;

    return revisedXvt;
}

double IonosphereFreeRange(const std::vector<double>& frequencies,
        const std::vector<double>& pseudoranges) {
    // Check vectors are same length
    if (frequencies.size() != pseudoranges.size()) {
        gnsstk::Exception exc(
            "Mismatch between frequency and pseudorange array size");
        GNSSTK_THROW(exc)
    }

    // Check vectors are at least two
    if (frequencies.size() < 2) {
        gnsstk::Exception exc(
            "Multiple frequency and range values are required.");
        GNSSTK_THROW(exc)
    }

    // Check vectors aren't greater than two
    if (frequencies.size() > 2) {
        gnsstk::Exception exc(
            "Only dual-frequency ionosphere correction is supported.");
        GNSSTK_THROW(exc)
    }

    // TODO(someone): Add proper gamma calculation for arbitrary
    //                number of frequencies.
    const double gamma = (frequencies[0]/frequencies[1]) *
                         (frequencies[0]/frequencies[1]);

    // for dual-frequency see IS-GPS-200, section 20.3.3.3.3.3
    double icpr = (pseudoranges[1] - gamma * pseudoranges[0])/(1-gamma);

    return icpr;
}

double IonosphereModelCorrection(const gnsstk::IonoModelStore& ionoModel,
        const gnsstk::CommonTime& time, CarrierBand band,
        const gnsstk::Position& rxLoc, const gnsstk::Xvt& svXvt) {
    Position trx(rxLoc);
    Position svPos(svXvt);

    double elevation = trx.elevation(svPos);
    double azimuth = trx.azimuth(svPos);

    double iono = ionoModel.getCorrection(time, trx, elevation, azimuth, band);
    return -iono;
}

gnsstk::Xvt getSvXvt(const gnsstk::SatID& satId, const gnsstk::CommonTime& time,
        NavLibrary& ephemeris) {
   Xvt rv;
      /** @todo getXvt was expected to throw an exception on failure
       * in the past.  This assert more or less mimics that behavior.
       * Refactoring is needed.  */
   GNSSTK_ASSERT(ephemeris.getXvt(NavSatelliteID(satId), time, rv));
   return rv;
}

double RawRange1(const gnsstk::Position& rxLoc, const gnsstk::SatID& satId,
        const gnsstk::CommonTime& timeReceived,
        NavLibrary& ephemeris, gnsstk::Xvt& svXvt) {
    try {
        int nit;
        double tof, tof_old, rawrange;
        GPSEllipsoid ellipsoid;

        CommonTime transmit(timeReceived);
        Xvt svPosVel;     // Initialize to zero

        nit = 0;
        tof = 0.07;       // Initial guess 70ms
        do {
            // best estimate of transmit time
            transmit = timeReceived;
            transmit -= tof;
            tof_old = tof;
            // get SV position
            try {
                  /** @todo getXvt was expected to throw an exception on
                   * failure in the past.  This assert more or less mimics
                   * that behavior.  Refactoring is needed.  */
               GNSSTK_ASSERT(ephemeris.getXvt(NavSatelliteID(satId), transmit,
                                              svPosVel));
            } catch (InvalidRequest& e) {
                GNSSTK_RETHROW(e);
            }

            svPosVel = rotateEarth(rxLoc, svPosVel, ellipsoid);
            // update raw range and time of flight
            rawrange = RSS(svPosVel.x[0] - rxLoc.X(), svPosVel.x[1] - rxLoc.Y(),
                    svPosVel.x[2] - rxLoc.Z());
            tof = rawrange / ellipsoid.c();
        } while (ABS(tof-tof_old) > 1.e-13 && ++nit < 5);

        svXvt = svPosVel;

        return rawrange;
    } catch (gnsstk::Exception& e) {
        GNSSTK_RETHROW(e);
    }
}

double RawRange2(double pseudorange, const gnsstk::Position& rxLoc,
        const gnsstk::SatID& satId, const gnsstk::CommonTime& time,
        NavLibrary& ephemeris, gnsstk::Xvt& svXvt) {
    try {
        CommonTime tt, transmit;
        Xvt svPosVel;     // Initialize to zero
        double rawrange;
        GPSEllipsoid ellipsoid;

        // 0-th order estimate of transmit time = receiver - pseudorange/c
        transmit = time;
        transmit -= pseudorange / C_MPS;
        tt = transmit;

        // correct for SV clock
        for (int i = 0; i < 2; i++) {
            // get SV position
            try {
                  /** @todo getXvt was expected to throw an exception on
                   * failure in the past.  This assert more or less mimics
                   * that behavior.  Refactoring is needed.  */
               GNSSTK_ASSERT(ephemeris.getXvt(NavSatelliteID(satId), tt,
                                              svPosVel));
            } catch (InvalidRequest& e) {
                GNSSTK_RETHROW(e);
            }
            tt = transmit;
            // remove clock bias and relativity correction
            tt -= (svPosVel.clkbias + svPosVel.relcorr);
        }

        svPosVel = rotateEarth(rxLoc, svPosVel, ellipsoid);

        // raw range
        rawrange = RSS(svPosVel.x[0] - rxLoc.X(),
                       svPosVel.x[1] - rxLoc.Y(),
                       svPosVel.x[2] - rxLoc.Z());

        svXvt = svPosVel;

        return rawrange;
    } catch (gnsstk::Exception& e) {
        GNSSTK_RETHROW(e);
    }
}

double RawRange3(double pseudorange, const gnsstk::Position& rxLoc,
        const gnsstk::SatID& satId, const gnsstk::CommonTime& time,
        NavLibrary& ephemeris, gnsstk::Xvt& svXvt) {
    Position trx(rxLoc);
    trx.asECEF();

    Xvt svPosVel;
       /** @todo getXvt was expected to throw an exception on
        * failure in the past.  This assert more or less mimics
        * that behavior.  Refactoring is needed.  */
    GNSSTK_ASSERT(ephemeris.getXvt(NavSatelliteID(satId), time,
                                   svPosVel));

    // compute rotation angle in the time of signal transit

    // While this is quite similiar to rotateEarth, its not the same
    // and jcl doesn't know which is really correct
    // BWT this uses the measured pseudorange, corrected for SV clock and
    // relativity, to compute the time of flight; rotateEarth uses the value
    // computed from the receiver position and the ephemeris. They should be
    // very nearly the same, and multiplying by angVel/c should make the angle
    // of rotation very nearly identical.
    GPSEllipsoid ell;
    double range(pseudorange/ell.c() - svPosVel.clkbias - svPosVel.relcorr);
    double rotation_angle = -ell.angVelocity() * range;
    svPosVel.x[0] = svPosVel.x[0] - svPosVel.x[1] * rotation_angle;
    svPosVel.x[1] = svPosVel.x[1] + svPosVel.x[0] * rotation_angle;
    // svPosVel.x[2] = svPosVel.x[2];  // ?? Reassign for readability ??

    double rawrange = trx.slantRange(svPosVel.x);

    svXvt = svPosVel;
    return rawrange;
}

double RawRange4(const gnsstk::Position& rxLoc, const gnsstk::SatID& satId,
        const gnsstk::CommonTime& time,
        NavLibrary& ephemeris, gnsstk::Xvt& svXvt) {
    try {
       gnsstk::GPSEllipsoid gm;
       Xvt svPosVel;
          /** @todo getXvt was expected to throw an exception on
           * failure in the past.  This assert more or less mimics
           * that behavior.  Refactoring is needed.  */
       GNSSTK_ASSERT(ephemeris.getXvt(NavSatelliteID(satId), time,
                                      svPosVel));
       double pr = svPosVel.preciseRho(rxLoc, gm);
       return RawRange2(pr, rxLoc, satId, time, ephemeris, svXvt);
    }
    catch(gnsstk::Exception& e) {
       GNSSTK_RETHROW(e);
    }
}

double SvClockBiasCorrection(const gnsstk::Xvt& svXvt) {
    double svclkbias = svXvt.clkbias * C_MPS;
    double svclkdrift = svXvt.clkdrift * C_MPS;
    return -svclkbias;
}

double SvRelativityCorrection(gnsstk::Xvt& svXvt) {
    double relativity = svXvt.computeRelativityCorrection() * C_MPS;
    return -relativity;
}

double TroposphereCorrection(const gnsstk::TropModel& tropModel,
        const gnsstk::Position& rxLoc, const gnsstk::Xvt& svXvt) {
    Position trx(rxLoc);
    Position svPos(svXvt);

    double elevation = trx.elevation(svPos);

    double trop = tropModel.correction(elevation);

    return trop;
}

/*
 * Example not fully fleshed-out.  If dual-band data given, for example,
 * then the last IonosphereModelCorrection call must not be made.
 * Users should construct an ORD algorithm based on their data and use case.
 *
double calculate_ord(const std::vector<CarrierBand>& bands,
                     const std::vector<double>& pseudoranges,
                     const gnsstk::Position& rx_loc,
                     const gnsstk::SatID& sat_id,
                     const gnsstk::CommonTime& transmit_time,
                     const gnsstk::CommonTime& receive_time,
                     const gnsstk::IonoModelStore& iono_model,
                     const gnsstk::TropModel& trop_model,
                     NavLibrary& ephemeris,
                     int range_method) {
    double ps_range = IonosphereFreeRange(bands, pseudoranges);

    gnsstk::Xvt sv_xvt;
    // find raw_range
    double range = 0;
    switch (range_method) {
        case 1:
            range = RawRange1(rx_loc, sat_id, receive_time, ephemeris, sv_xvt);
            break;
        case 2:
            range = RawRange2(ps_range, rx_loc, sat_id, receive_time, ephemeris,
                              sv_xvt);
            break;
        case 3:
            range = RawRange3(ps_range, rx_loc, sat_id, transmit_time, ephemeris,
                              sv_xvt);
            break;
        case 4:
            range = RawRange4(rx_loc, sat_id, receive_time, ephemeris, sv_xvt);
            break;
    }

    // apply sv relativity correction
    range += SvRelativityCorrection(sv_xvt);

    // apply sv clock bias correction
    range += SvClockBiasCorrection(sv_xvt);

    // apply troposphere model correction
    range += TroposphereCorrection(trop_model, rx_loc, sv_xvt);

    // apply ionosphere model correction -- GPS only (this is Klobuchar)
    range += IonosphereModelCorrection(iono_model,
                                       receive_time, bands[0],
                                       rx_loc, sv_xvt);

    return ps_range - range;
}
 */

}  // namespace ord
}  // namespace gnsstk
