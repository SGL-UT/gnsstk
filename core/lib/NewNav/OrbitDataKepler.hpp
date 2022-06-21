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
#ifndef GNSSTK_ORBITDATAKEPLER_HPP
#define GNSSTK_ORBITDATAKEPLER_HPP

#include "OrbitData.hpp"
#include "SVHealth.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Base class for orbit information that uses Keplerian parameters.
   class OrbitDataKepler : public OrbitData
   {
   public:
         /// Precision used when printing floating point numbers
      static const size_t precision = 8;
         /// Field width of floating point numbers (precision + 8).
      static const size_t fw = 16;

         /// Initialize all data members to 0.
      OrbitDataKepler();

         /** Returns the time for the data to be used when searching
          * in "Nearest" mode. */
      CommonTime getNearTime() const override
      { return Toe; }

         /** Print the contents of this NavData object in a
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Dump SV status information (e.g. health).  Nothing to do
          * at this level, all the work is in derived classes.
          * @param[in,out] s The stream to write the data to. */
      virtual void dumpSVStatus(std::ostream& s) const
      {}
         /** Dump the clock parameters.
          * @param[in,out] s The stream to write the data to. */
      virtual void dumpClock(std::ostream& s) const;
         /** Dump the orbit parameters section of a full detail dump.
          * @param[in,out] s The stream to write the data to. */
      virtual void dumpOrbit(std::ostream& s) const;
         /** Dump the harmonic corrections section of a full detail dump.
          * @param[in,out] s The stream to write the data to. */
      virtual void dumpHarmonics(std::ostream& s) const;
         /** This is just a method for making the dump output say
          * "almanac" vs "ephemeris" when appropriate.  Yes it's
          * perfectly legitimate to alternate between Ephemeris and
          * Almanac based on the message type, as no other message
          * types would be inheriting from OrbitDataKepler. */
      virtual std::string getDataType() const
      {
         return (signal.messageType == NavMessageType::Ephemeris
                 ? "Ephemeris" : "Almanac");
      }

         /** Compute the satellites position and velocity at a time.
          * @note Defaults to using the GPS ellipsoid parameters.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @param[in] oid Ignored at this level, only used in derived classes.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt,
                  const ObsID& oid = ObsID()) override = 0;

         /** Compute satellite relativity correction (sec) at the given time.
          * @note Each child class must implement this method to call
          *   svRelativity(CommonTime,EllipsoidModel) with the
          *   appropriate EllipsoidModel.
          * @param[in] when The time at which to get the relativity correction.
          * @return the relativity correction in seconds.
          */
      virtual double svRelativity(const CommonTime& when) const = 0;

         /** Compute the satellite clock bias (sec) at the given time
          * @param[in] when The time at which to get the satellite clock bias.
          * @return the satellite's clock bias in seconds.
          */
      double svClockBias(const CommonTime& when) const;

         /** Compute the satellite clock drift (sec/sec) at the given time
          * @param[in] when The time at which to get the satellite clock drift.
          * @return the satellite's clock drift in seconds per second.
          */
      double svClockDrift(const CommonTime& when) const;

         /** Compute the satellites position and velocity at a time.
          * @param[in] when The time at which to compute the xvt.
          * @param[in] ell The ellipsoid used in computing the Xvt
          *   (specifically EllipsoidModel::gm() and
          *   EllipsoidModel::angVelocity()).
          * @param[out] xvt The resulting computed position/velocity.
          * @param[in] oid Ignored at this level, only used in derived classes.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, const EllipsoidModel& ell, Xvt& xvt,
                  const ObsID& oid = ObsID());

         /** Compute satellite relativity correction (sec) at the given time.
          * @param[in] ell The ellipsoid used in computing the Xvt
          *   (specifically EllipsoidModel::gm()).
          * @param[in] when The time at which to get the relativity correction.
          * @return the relativity correction in seconds.
          */
      virtual double svRelativity(const CommonTime& when,
                                  const EllipsoidModel& ell)
         const;

         /** Returns true if this two objects are 
          *   1. same concrete type, and
          *   2. same data contents.
          * This is intended as a "data uniqueness test" to allow
          * detection of successive transmissions of same data
          * and avoid duplicate storage.  The exact rules for 
          * uniqueness will vary by descendent class.
          * @note This method assumes that no tweaking of values has
          *   been made, i.e. it checks all potentially relevant
          *   parameters, not just those that are specific to the type
          *   (e.g. Cuc etc. are checked even for almanacs, which
          *   should be fine as long as they remain in their initial
          *   states). */
      bool isSameData(const NavDataPtr& right) const override;
         /// @copydoc NavData::compare
      std::list<std::string> compare(const NavDataPtr& right)
         const override;

      CommonTime xmitTime; ///< Time of transmission of the start of the data.
      CommonTime Toe;      ///< Orbit epoch
      CommonTime Toc;      ///< Clock epoch
      SVHealth health;     ///< SV health status.

      double Cuc;          ///< Cosine latitude (rad)
      double Cus;          ///< Sine latitude (rad)
      double Crc;          ///< Cosine radius (m)
      double Crs;          ///< Sine radius (m)
      double Cic;          ///< Cosine inclination (rad)
      double Cis;          ///< Sine inclination (rad)

      double M0;           ///< Mean anomaly (rad)
      double dn;           ///< Correction to mean motion (rad/sec)
      double dndot;	   ///< Rate of correction to mean motion (rad/sec/sec)
      double ecc;          ///< Eccentricity
      double A;            ///< Semi-major axis (m)
      double Ahalf;        ///< Square Root of semi-major axis (m**.5)
      double Adot;         ///< Rate of semi-major axis (m/sec)
      double OMEGA0;       ///< Longitude of ascending node at weekly epoch (rad)
      double i0;           ///< Inclination (rad)
      double w;            ///< Argument of perigee (rad)
      double OMEGAdot;     ///< Rate of Rt ascension (rad/sec)
      double idot;         ///< Rate of inclination angle (rad/sec)

      double af0;          ///< SV clock error (sec)
      double af1;          ///< SV clock drift (sec/sec)
      double af2;          ///< SV clock drift rate (sec/sec**2)

      CommonTime beginFit; ///< Time at beginning of fit interval
      CommonTime endFit;   ///< Time at end of fit interval

      ReferenceFrame frame;///< Reference frame of produced Xvt data.
   };

      //@}

}

#endif // GNSSTK_ORBITDATAKEPLER_HPP
