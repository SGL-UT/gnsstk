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
 * @file IonoModel.cpp
 * Implementation of the IS-GPS-200 Ionosphere model (20.3.3.5.2.5).
 */

#ifndef GNSSTK_IONOMODEL_HPP
#define GNSSTK_IONOMODEL_HPP

#include "CommonTime.hpp"
#include "CarrierBand.hpp"
#include "EngAlmanac.hpp"
#include "Position.hpp"

namespace gnsstk
{
      /// @ingroup GPSsolutions
      //@{

      /** Simple model of the ionosphere ("Klobuchar"), specified in the GPS IS.
       * It is used to compute the satellite signal ionospheric delay seen at
       * the receiver by a single-band user.
       *
       * See ICD-GPS-200, section 20.3.3.5.2.5 and  Figure 20-4.
       *
       * Verbatim from the above section:
       * "It is estimated that the use of this model will provide at least
       * a 50% reduction in the single-frequency user's RMS error due to
       * ionospheric propagation effects. During extended operations, or for
       * the Block IIR SVs in the Autonav mode if the CS is unable to upload
       * the SVs, the use of this model will yield unpredictable results."
       */
   class IonoModel
   {
   public:

         /// Exception, thrown when attempting to use a model for which not all
         /// the necessary parameters have been specified.
         /// @ingroup exceptiongroup
      NEW_EXCEPTION_CLASS(InvalidIonoModel, gnsstk::Exception);

         /// Default constructor, creates an invalid model for lack of parameters.
      IonoModel() noexcept : valid(false) {}

         /// Destructor.
      virtual ~IonoModel() noexcept {}

         /** Ionosphere model constructor.
          * Creates a valid model with satellite transmitted alpha and beta
          * (Klobuchar) parameters provided by the user.
          * @param[in] a An array containing the four alpha terms.
          * @param[in] b An array containing the four beta terms.
          * @param[in] semicircle_units A boolean indicating params are in
          *                         semicircles (T, default) or radians (F).
          * Note that the IS-GPS-200 defines the algorithm and parameters
          * in terms of semi-circles, not radians; but that the GNSSTk for
          * historical reasons extracts parameters from a GPS Nav message
          * in power of inverse radians.  Hence the need for the boolean flag.
          */
      IonoModel(const double a[4], const double b[4],
                const bool semicircle_units = true) noexcept;

         /** EngAlmanac constructor.
          * Creates a valid model from and EngAlmanac object.
          * @param[in] engalm An EngAlmanac object.
          */
      IonoModel(const EngAlmanac& engalm) noexcept;

         /** Method to feed the model with satellite-transmitted alpha and
          * beta parameters from the passed almanac.
          * See the IS-GPS-200, 20.3.3.3.3.2.
          * @param[in] a An array containing the four alpha terms.
          * @param[in] b An array containing the four beta terms.
          */
      void setModel(const double a[4], const double b[4],
                    const bool semicircle_units = true) noexcept;

         /** Return whether the model contains valid data.
          * @return model validity
          */
      bool isValid() const noexcept
      { return valid; }

         /** Get the ionospheric correction value.
          * @param[in] time Time of the observation.
          * @param[in] rxgeo WGS84 geodetic position of the receiver.
          * @param[in] svel Elevation angle between the rx and SV (degrees).
          * @param[in] svaz Azimuth angle between the rx and SV (degrees).
          * @param[in] band GPS frequency band the observation was made from.
          * @return The ionospheric correction (meters).
          * @throw InvalidIonoModel
          */
      double getCorrection(const CommonTime& time,
                           const Position& rxgeo,
                           double svel,
                           double svaz,
                           CarrierBand band = CarrierBand::L1) const;

         /// Equality operator
      bool operator==(const IonoModel& right) const noexcept;

         /// Inequality operator
      bool operator!=(const IonoModel& right) const noexcept;

         /** Get the raw model contents in semicircle units.
          * @param[in,out] a Ionospheric model alpha parameters
          * @param[in,out] b Ionospheric model beta parameters
          * @return true if \a a and \a b were populated successfully
          */
      bool getModel(double a[4], double b[4]) const noexcept;

         /** Dump to contents of the model in human-readable form.
          * @param[in,out] s Stream to receive the output; defaults to cout
          */
      virtual void dump(std::ostream& s=std::cout) const;

   private:

      double alpha[4];
      double beta[4];

      bool valid;
   };
      //@}

}  // namespace gnsstk

#endif  // GNSSTK_IONOMODEL_HPP
