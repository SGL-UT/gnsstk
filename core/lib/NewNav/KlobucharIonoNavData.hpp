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
#ifndef GNSSTK_KLOBUCHARIONODATA_HPP
#define GNSSTK_KLOBUCHARIONODATA_HPP

#include "IonoNavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to the Klobuchar iono model.
   class KlobucharIonoNavData : public IonoNavData
   {
   public:
         /// Sets the nav message type.
      KlobucharIonoNavData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override
      { return true; }

         /** Print the contents of this KlobucharIonoNavData object in a
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Get the ionospheric correction in meters.
          * @param[in] when The time of the observation to correct.
          * @param[in] rxgeo The receiver's geodetic position.
          * @param[in] svgeo The observed satellite's geodetic position.
          * @param[in] band The carrier band of the signal being corrected.
          * @return The ionospheric delay, in meters, on band. */
      double getIonoCorr(const CommonTime& when,
                         const Position& rxgeo,
                         const Position& svgeo,
                         CarrierBand band) const override;

         // alpha/beta terms are seconds, seconds/semi-circle,
         // seconds/semi-circle**2, seconds/semi-circle**3.  Refer to
         // IS-GPS-200 Table 20-X.
      double alpha[4]; ///< alpha terms of Klobuchar model, using semi-circles.
      double beta[4];  ///< beta terms of Klobuchar model, using semi-circles.
   };

      //@}

}

#endif // GNSSTK_KLOBUCHARIONODATA_HPP
