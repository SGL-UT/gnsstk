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

/**
 * @file ObsRngDev.hpp
 * Observed range deviation computation & storage.
 */

#ifndef OBSRNGDEV_HPP
#define OBSRNGDEV_HPP

#include <ostream>

#include "CommonTime.hpp"
#include "CarrierBand.hpp"
#include "NavLibrary.hpp"
#include "Exception.hpp"
#include "GPSEllipsoid.hpp"
#include "IonoModelStore.hpp"
#include "TropModel.hpp"
#include "NBTropModel.hpp"
#include "ValidType.hpp"
#include "SatID.hpp"
#include "GNSSconstants.hpp"
#include "gnsstk_export.h"

namespace gnsstk
{
      /// @ingroup ClockModel
      //@{

      /**
       * A single (one observation from one sv), Observed Range
       * Deviation (ORD).  It contains all of the parameters that
       * define an ORD and includes metadata on ORD computation such
       * as SV position and health.
       */
   class ObsRngDev
   {
   public:

         /**
          * default constructor.
          * Creates an empty, useless object to facilitate STL
          * containers of this object.
          */
      ObsRngDev() noexcept
      : obstime(CommonTime::END_OF_TIME), wonky(0)
      {}

         /**
          * Creates an ORD, with no ionospheric correction and a default
          * trop correction.
          * @param[in] prange The observed pseudorange
          * @param[in] svid The SV being observed
          * @param[in] time The time of the observation
          * @param[in] rxpos The earth-centered, earth-fixed receiver position
          * @param[in] navLib A store of either broadcast or precise ephemerides
          * @param[in] em An EllipsoidModel for performing range calculations
          * @param[in] svTime True if prange is in SV time, false for RX time.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          */ 
      ObsRngDev(const double prange,
                const SatID& svid,
                const CommonTime& time,
                const Position& rxpos,
                NavLibrary& navLib,
                EllipsoidModel& em,
                bool svTime = false,
                NavSearchOrder order = NavSearchOrder::User,
                SVHealth xmitHealth = SVHealth::Any,
                NavValidityType valid = NavValidityType::ValidOnly);

         /**
          * Creates an ORD, applies a single-frequency nav-message based
          * ionospheric correction and a default trop correction.
          * @param[in] prange The observed pseudorange
          * @param[in] svid The PRN number of the observed SV
          * @param[in] time The time of the observation
          * @param[in] rxpos The earth-centered, earth-fixed receiver position
          * @param[in] navLib A store of either broadcast or precise ephemerides
          * @param[in] em An EllipsoidModel for performing range calculations
          * @param[in] ion A store of nav based ionospheric models
          * @param[in] fq The GPS band (L1, L2, L5) from which the obs was made
          * @param[in] svTime True if prange is in SV time, false for RX time.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          */ 
      ObsRngDev(const double prange,
                const SatID& svid,
                const CommonTime& time,
                const Position& rxpos,
                NavLibrary& navLib,
                EllipsoidModel& em,
                const IonoModelStore& ion,
                CarrierBand band,
                bool svTime = false,
                NavSearchOrder order = NavSearchOrder::User,
                SVHealth xmitHealth = SVHealth::Any,
                NavValidityType valid = NavValidityType::ValidOnly);

         /**
          * Creates an ORD, applies no ionospheric correction and
          * a user-specified trop correction.
          * @param[in] prange The observed pseudorange
          * @param[in] svid The PRN number of the observed SV
          * @param[in] time The time of the observation
          * @param[in] rxpos The earth-centered, earth-fixed receiver position
          * @param[in] navLib A store of either broadcast or precise ephemerides
          * @param[in] em An EllipsoidModel for performing range calculations
          * @param[in] tm A TropModel for performing trop calculation
          * @param[in] svTime True if prange is in SV time, false for RX time.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          */ 
      ObsRngDev(const double prange,
                const SatID& svid,
                const CommonTime& time,
                const Position& rxpos,
                NavLibrary& navLib,
                EllipsoidModel& em,
                const TropModel& tm,
                bool svTime = false,
                NavSearchOrder order = NavSearchOrder::User,
                SVHealth xmitHealth = SVHealth::Any,
                NavValidityType valid = NavValidityType::ValidOnly);

         /**
          * Creates an ORD, applies a single-frequency nav-message based
          * ionospheric correction and a user-specified trop correction.
          * @param[in] prange The observed pseudorange
          * @param[in] svid The PRN number of the observed SV
          * @param[in] time The time of the observation
          * @param[in] rxpos The earth-centered, earth-fixed receiver position
          * @param[in] navLib A store of either broadcast or precise ephemerides
          * @param[in] em An EllipsoidModel for performing range calculations
          * @param[in] tm A TropModel for performing trop calculation
          * @param[in] ion A store of nav based ionospheric models
          * @param[in] fq The GPS band (L1, L2, L5) from which the obs was made
          * @param[in] svTime True if prange is in SV time, false for RX time.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          */ 
      ObsRngDev(const double prange,
                const SatID& svid,
                const CommonTime& time,
                const Position& rxpos,
                NavLibrary& navLib,
                EllipsoidModel& em,
                const TropModel& tm,
                const IonoModelStore& ion,
                CarrierBand band,
                bool svTime = false,
                NavSearchOrder order = NavSearchOrder::User,
                SVHealth xmitHealth = SVHealth::Any,
                NavValidityType valid = NavValidityType::ValidOnly);
   
         /**
          * Creates an ORD, applies a dual-frequency ionospheric correction
          * and a default trop correction.
          * @param[in] prange1 The observed pseudorange on the first carrier
          * @param[in] prange2 The observed pseudorange on the second carrier
          * @param[in] svid The PRN number of the observed SV
          * @param[in] time The time of the observation
          * @param[in] rxpos The earth-centered, earth-fixed receiver position
          * @param[in] navLib A store of either broadcast or precise ephemerides
          * @param[in] em An EllipsoidModel for performing range calculations
          * @param[in] svTime True if prange is in SV time, false for RX time.
          * @param[in] gamma The value of gamma: \f$\gamma_{12} = (f_{L1}/f_{L2})^2\f$
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          */ 
      ObsRngDev(const double prange1,
                const double prange2,
                const SatID& svid,
                const CommonTime& time,
                const Position& rxpos,
                NavLibrary& navLib,
                EllipsoidModel& em,
                bool svTime = false,
                double gamma = GAMMA_GPS,
                NavSearchOrder order = NavSearchOrder::User,
                SVHealth xmitHealth = SVHealth::Any,
                NavValidityType valid = NavValidityType::ValidOnly);
   
         /**
          * Creates an ORD, applies a dual-frequency ionospheric correction
          * and a user-specified trop correction.
          * @param[in] prange1 The observed pseudorange on the first carrier
          * @param[in] prange2 The observed pseudorange on the second carrier
          * @param[in] svid The PRN number of the observed SV
          * @param[in] time The time of the observation
          * @param[in] rxpos The earth-centered, earth-fixed receiver position
          * @param[in] navLib A store of either broadcast or precise ephemerides
          * @param[in] em An EllipsoidModel for performing range calculations
          * @param[in] tm A TropModel for performing trop calculations
          * @param[in] svTime True if prange is in SV time, false for RX time.
          * @param[in] gamma The value of gamma: \f$\gamma_{12} = (f_{L1}/f_{L2})^2\f$
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          */ 
      ObsRngDev(const double prange1,
                const double prange2,
                const SatID& svid,
                const CommonTime& time,
                const Position& rxpos,
                NavLibrary& navLib,
                const EllipsoidModel& em,
                const TropModel& tm,
                bool svTime = false,
                double gamma = GAMMA_GPS,
                NavSearchOrder order = NavSearchOrder::User,
                SVHealth xmitHealth = SVHealth::Any,
                NavValidityType valid = NavValidityType::ValidOnly);
   
         /// destructor
      virtual ~ObsRngDev() noexcept
      {}

         // get accessor methods ----------------------------------------------
         /**
          * returns the time of the SV observation
          * \return time of SV observation
          */
      const CommonTime& getTime() const noexcept
      { return obstime; }

         /**
          * returns the observed SV's identifier
          * \return svid
          */
      SatID getSvID() const noexcept
      { return svid; }

         /**
          * returns the SV azimuth angle (in degrees) in relation to the rx
          * \return SV azimuth angle
          */
      vfloat getAzimuth() const noexcept
      { return azimuth; }

         /**
          * returns elevation (in degrees) of the SV in relation to the rx
          * \return SV elevation angle
          */
      vfloat getElevation() const noexcept
      { return elevation; }

         /**
          * returns the 6-bit SV health bitfield from epehemeris, subframe 1
          * \return SV health bitfield
          */
      vshort getHealth() const noexcept
      { return health; }

         /**
          * returns the Issue Of Data, Clock (IODC) from ephemeris, subframe 1
          * \return ephemeris IODC
          */
      vshort getIODC() const noexcept
      { return iodc; }

         /**
          * returns the observed range deviation (ORD) (in meters)
          * \returns ORD
          */
      double getORD() const noexcept
      { return ord; }

         /**
          * returns the ionospheric offset (in meters)
          * \returns ionospheric offset
          */
      vdouble getIono() const noexcept
      { return iono; }

         /**
          * returns the tropospheric offset (in meters)
          * \returns tropospheric offset
          */
      vdouble getTrop() const noexcept
      { return trop; }

      friend std::ostream& operator<<(std::ostream& s,
                                      const ObsRngDev& r) noexcept;

      void applyClockOffset(double clockOffset)
      { ord -= clockOffset; }

      GNSSTK_EXPORT static bool debug;

   private:
      void computeOrd(double obs,
                      const Position& rxpos,
                      NavLibrary& navLib,
                      const EllipsoidModel& em,
                      bool svTime,
                      NavSearchOrder order,
                      SVHealth xmitHealth,
                      NavValidityType valid)
      {
         if (svTime) 
            computeOrdTx(obs, rxpos, navLib, em, order, xmitHealth, valid);
         else 
            computeOrdRx(obs, rxpos, navLib, em, order, xmitHealth, valid);
         return;
      }



      void computeOrdTx(double obs,
                        const Position& rxpos,
                        NavLibrary& navLib,
                        const EllipsoidModel& em,
                        NavSearchOrder order,
                        SVHealth xmitHealth,
                        NavValidityType valid);
   
      void computeOrdRx(double obs,
                        const Position& rxpos,
                        NavLibrary& navLib,
                        const EllipsoidModel& em,
                        NavSearchOrder order,
                        SVHealth xmitHealth,
                        NavValidityType valid);

      void computeTrop(const TropModel& tm);

   public:
      CommonTime obstime;        ///< time of SV observation
      SatID svid;                ///< PRN number of observed SV
      double ord;                ///< difference between expected and observed range
      unsigned wonky;            ///< A bitmask defined by the application to flag questionable data

      vfloat azimuth;            ///< SV azimuth
      vfloat elevation;          ///< SV elevation
      vshort health;             ///< SV health bitfield
      vshort iodc;               ///< ephemeris IODC
      vdouble rho;               ///< expected geometric range
      vdouble iono;              ///< iono correction (meters)
      vdouble trop;              ///< trop correction (meters)
   };

      //@}

}
#endif
