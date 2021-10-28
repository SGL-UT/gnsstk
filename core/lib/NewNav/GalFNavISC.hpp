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
#ifndef GNSSTK_GALFNAVISC_HPP
#define GNSSTK_GALFNAVISC_HPP

#include "InterSigCorr.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to Galileo F/NAV ISC
       * (BGD - broadcast group delay). */
   class GalFNavISC : public InterSigCorr
   {
   public:
         /// Initialize data members.
      GalFNavISC();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checks.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Get inter-signal corrections for the single-frequency user.
          * @param[in] oid The carrier band and tracking code of the
          *   signal to get the correction for.
          * @param[out] corrOut The correction in seconds for the
          *   given band/code.
          * @return true If band/code are valid for this object and
          *   corrOut was set according to available data. */
      bool getISC(const ObsID& oid, double& corrOut)
         const override;

         /** Get inter-signal corrections for the dual-frequency user.
          * @note Galileo has no ISCs for the dual frequency user (per
          *   section 5.1.5 of the SIS-ICD), so this just returns true
          *   with corrOut set to 0.
          * @param[in] oid1 The carrier band/tracking code of the
          *   primary signal that was used to create a dual-frequency,
          *   iono-free combined pseudorange.
          * @param[in] oid2 The carrier band/tracking code of the
          *   secondary signal to get the correction for.
          * @param[out] corrOut The correction in seconds for the given
          *   band/code pair.
          * @return true If bands/codes are valid for this object and
          *   corrOut was set according to available data. */
      bool getISC(const ObsID& oid1, const ObsID& oid2, double& corrOut)
         const override
      {
         corrOut = 0;
         return true;
      }
   };

      //@}

}

#endif // GNSSTK_GALFNAVISC_HPP
