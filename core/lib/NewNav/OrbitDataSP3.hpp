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
#ifndef GNSSTK_ORBITDATASP3_HPP
#define GNSSTK_ORBITDATASP3_HPP

#include <vector>
#include "OrbitData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class for orbit information using SP3 data tables.
   class OrbitDataSP3 : public OrbitData
   {
   public:
         /// Set the vector sizes and initialize everything to 0.
      OrbitDataSP3();
         /// Initialize all values to the given one (e.g. NaN)
      explicit OrbitDataSP3(double val);
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<OrbitDataSP3>(*this); }

         /** Copy only the position, velocity and acceleration data.
          * @param[in] right The OrbitDataSP3 to copy XV data from.
          */
      void copyXV(const OrbitDataSP3& right);
         /** Copy only the clock correction data.
          * @param[in] right The OrbitDataSP3 to copy clock data from.
          */
      void copyT(const OrbitDataSP3& right);
         /** Checks the contents of this message.
          * @todo Determine and implement validity criteria.
          * @return true if this data is valid according to some criteria.
          */
      bool validate() const override
      { return true; }
         /** Print the contents of this NavData object in a
          * human-readable format.  Brief detail includes P/V.  Full
          * detail includes P/V/A and sigma.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;
         /** Compute the satellites position and velocity at a time.
          * @note Defaults to using the GPS ellipsoid parameters.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @param[in] oid Value is ignored - SP3 has no such information.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt,
                  const ObsID& oid = ObsID()) override;

      Triple pos;      ///< ECEF position (km) of satellite at time.
      Triple posSig;   ///< Standard deviation of position.
      Triple vel;      ///< ECEF velocity (dm/s) of satellite at time.
      Triple velSig;   ///< Standard deviation of velocity.
      Triple acc;      ///< Acceleration (m/s/s) of satellite at time.
      Triple accSig;   ///< Standard deviation of acceleration.
      double clkBias;  ///< SV clock bias in microseconds.
      double biasSig;  ///< SV clock bias std deviation in microseconds.
      double clkDrift; ///< SV clock drift in s/s.
      double driftSig; ///< SV clock drift std deviation in microseconds/sec.
      double clkDrRate;///< SV clock drift rate in s/s**2.
      double drRateSig;///< SV clock drift rate std deviation.
         /// Copy of SP3Header::coordSystem since it might not translate.
      std::string coordSystem;
         /// Translation of coordSystem into an enum, if possible.
      RefFrame frame;
   };

      //@}

}

#endif // GNSSTK_ORBITDATASP3_HPP
