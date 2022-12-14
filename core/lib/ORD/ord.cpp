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
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
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

#include <tuple>
#include <typeinfo>
#include <vector>
#include "ord.hpp"
#include "GPSEllipsoid.hpp"
#include "GNSSconstants.hpp"
#include "RawRange.hpp"
#include "Position.hpp"
#include "SatID.hpp"
#include "CommonTime.hpp"
#include "NavLibrary.hpp"
#include "Xvt.hpp"
#include "NavSatelliteID.hpp"
#include "GPSEllipsoid.hpp"

using std::vector;
using std::cout;

namespace gnsstk {
namespace ord {

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

double RawRange1(
   const Position& rxLoc,
   const SatID& satId,
   const CommonTime& timeReceived,
   NavLibrary& ephemeris,
   Xvt& svXvt
)
{
   bool success;
   double range;
   GPSEllipsoid ellipsoid;
   std::tie(success, range, svXvt) =
      RawRange::fromReceive(rxLoc, timeReceived, ephemeris,
                            NavSatelliteID(satId), ellipsoid);
   GNSSTK_ASSERT(success);
   return range;
}


double RawRange2(double pseudorange, const Position& rxLoc,
        const SatID& satId, const CommonTime& time,
        NavLibrary& ephemeris, Xvt& svXvt) {

   bool success;
   double range;
   GPSEllipsoid ellipsoid;
   std::tie(success, range, svXvt) =
      RawRange::fromNominalReceiveWithObs(rxLoc, time, pseudorange, ephemeris,
                                          NavSatelliteID(satId), ellipsoid);
   GNSSTK_ASSERT(success);
   return range;
}


double RawRange3(double pseudorange, const gnsstk::Position& rxLoc,
        const gnsstk::SatID& satId, const gnsstk::CommonTime& time,
        NavLibrary& ephemeris, gnsstk::Xvt& svXvt) {
   bool success;
   double range;
   GPSEllipsoid ellipsoid;
   std::tie(success, range, svXvt) =
      RawRange::fromSvTransmitWithObs(rxLoc, pseudorange, ephemeris,
                                      NavSatelliteID(satId), time, ellipsoid,
                                      true);
   GNSSTK_ASSERT(success);
   return range;
}

double RawRange4(const gnsstk::Position& rxLoc, const gnsstk::SatID& satId,
        const gnsstk::CommonTime& time,
        NavLibrary& ephemeris, gnsstk::Xvt& svXvt) {

   GNSSTK_ASSERT(ephemeris.getXvt(NavSatelliteID(satId), time, svXvt));

      // Compute initial time of flight estimate using the
      // geometric range at transmit time.  This fails to account
      // for the rotation of the earth, but should be good to
      // within about 40 m
   GPSEllipsoid ellipsoid;
   double tofEstimate = rxLoc.slantRange(svXvt.x) / ellipsoid.c();

   bool success;
   double range;
   Xvt rotatedSvXvt;

      // First estimate the range by using the receiveNominal as the
      // transmit time.
   std::tie(success, range, rotatedSvXvt) =
      RawRange::fromSvTransmit(rxLoc, ephemeris, NavSatelliteID(satId), time,
                               ellipsoid, true, SVHealth::Any,
                               NavValidityType::ValidOnly, NavSearchOrder::User,
                               tofEstimate, 0, 2);
   GNSSTK_ASSERT(success);

      // Create a mock pseudorange using the estimated range and SV clock offsets
   double fakePsuedorange = range -
      (rotatedSvXvt.clkbias + rotatedSvXvt.relcorr) * ellipsoid.c();

   std::tie(success, range, rotatedSvXvt) =
      RawRange::fromNominalReceiveWithObs(rxLoc, time, fakePsuedorange,
                                          ephemeris, NavSatelliteID(satId),
                                          ellipsoid, false);
   GNSSTK_ASSERT(success);
   svXvt = rotatedSvXvt;
   return range;
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
