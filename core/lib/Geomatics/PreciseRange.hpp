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

/** @file PreciseRange.hpp
    Include file defining class PreciseRange: computation of range and
    associated quantities from NavLibrary, given receiver position and time. */

//------------------------------------------------------------------------------------
#ifndef PRECISE_EPHEMERIS_RANGE_INCLUDE
#define PRECISE_EPHEMERIS_RANGE_INCLUDE

//------------------------------------------------------------------------------------
// GNSSTk
#include "EllipsoidModel.hpp"
#include "GPSEllipsoid.hpp"
#include "CommonTime.hpp"
#include "Matrix.hpp"
#include "Position.hpp"
#include "SatID.hpp"
#include "NavLibrary.hpp"

// geomatics
#include "AntexData.hpp"
#include "SolarSystem.hpp"

//------------------------------------------------------------------------------------
namespace gnsstk
{
   /// @ingroup ephemcalc
   //@{

      /**
       class PreciseRange. Compute the corrected range from receiver
       at position Rx, to the GPS satellite given by SatID sat, as well as
       azimuth, elevation, etc., given a nominal timetag (either received or
       transmitted time) and a NavLibrary.
      */
   class PreciseRange
   {
   public:
         /// Default constructor.
      PreciseRange() {}

         /**
          Compute the corrected range at transmit time from ephemeris is the
          given NavLibrary, from receiver at position Rx with measured pseudorange
          pr and time tag nomRecTime, to the GPS satellite given by SatID sat,
          as well as all the CER quantities.
          @param nomRecTime  nominal receive time
          @param pr          measured pseudorange at this time
          @param rxPos          receiver position
          @param sat         satellite
          @param antenna     satellite antenna data;
          @param freq1,freq2 ANTEX frequencies to evaluate PCO/Vs eg 'G01'
                 if freq2 is zero e.g. 'G00', compute single freq (freq1) PCO/Vs
          @param solSys      SolarSystem object, to get SatelliteAttitude()
                 if any of above 4 not valid, PCO/V correction is NOT done (silently)
          @param eph        Ephemeris store
          @param isCOM          if true, Eph is Center-of-mass,
                                        else antenna-phase-center, default
                                        false.
          @param ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          @return corrected raw range
          @throw Exception if ephemeris is not found
         */
      double ComputeAtTransmitTime(const CommonTime& nomRecTime,
                                   const double pr,
                                   const Position& rxPos,
                                   const SatID sat,
                                   const AntexData& antenna,
                                   const std::string& freq1,
                                   const std::string& freq2,
                                   SolarSystem& solSys,
                                   NavLibrary& eph,
                                   bool isCOM = false,
                                   const EllipsoidModel& ellipsoid = GPSEllipsoid());

         /**
          Version with no antenna, and therefore no Attitude and no SolarSystem;
          cf. doc for other version for details.
          @throw Exception
         */
      double ComputeAtTransmitTime(const CommonTime& nomRecTime,
                                   const double pr,
                                   const Position& rxPos,
                                   const SatID sat,
                                   NavLibrary& eph,
                                   const EllipsoidModel& ellipsoid = GPSEllipsoid())
      {
         // ant will be invalid, so antenna computations will be skipped;
         // thus satellite attitude will not be needed.
         AntexData ant;
         SolarSystem ss;
         std::string s;
         return ComputeAtTransmitTime(nomRecTime, pr, rxPos, sat, ant, s, s, ss,
                                      eph, false, ellipsoid);
      }

         /**
          The computed raw (geometric) range in meters, with NO corrections
          applied; to correct it, use rawrange -=
          satclkbias+relativity+relativity2-satLOSPCO-satLOSPCV.
         */
      double rawrange;

         /// The relativity correction in meters, and high precision correction
      double relativity, relativity2;

         /// The satellite position (m) and velocity (m/s) in ECEF coordinates
      Position SatR, SatV;

         /**
          The satellite clock bias (m) and drift (m/s) at transmit time, from
          NavLibrary
         */
      double satclkbias, satclkdrift;

         /**
          The satellite elevation (spheroidal), as seen at the receiver, in
          degrees.
         */
      double elevation;

         /**
          The satellite azimuth (spheroidal), as seen at the receiver, in
          degrees.
         */
      double azimuth;

         /**
          The satellite elevation (geodetic), as seen at the receiver, in
          degrees.
         */
      double elevationGeodetic;

         /// The satellite azimuth (geodetic), as seen at the receiver, in degrees.
      double azimuthGeodetic;

         /// The computed transmit time of the signal.
      CommonTime transmit;

         /// The direction cosines of the satellite, as seen at the receiver (XYZ).
      Triple cosines;

         /**
          The net line-of-sight offset, in the direction from sat to rx,
          of the antenna PCO and PCVs, meters
         */
      double satLOSPCO, satLOSPCV;

         /// The Satellite PCO vector, in ECEF XYZ, meters (from COM to PC)
      Vector<double> SatPCOXYZ;

         /// Net time delay due to Sagnac effect in seconds
      double Sagnac;

   }; // end class PreciseRange

   //@}

} // namespace gnsstk

#endif // PRECISE_EPHEMERIS_RANGE_INCLUDE
