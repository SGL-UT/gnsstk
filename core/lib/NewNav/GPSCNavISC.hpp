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
#ifndef GNSSTK_GPSCNAVISC_HPP
#define GNSSTK_GPSCNAVISC_HPP

#include "InterSigCorr.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS CNAV ISC data.
   class GPSCNavISC : public InterSigCorr
   {
   public:
         /// Sets the internal data members.
      GPSCNavISC();

         /** Print the actual corrections provided by this object in a
          * human-readable format.  Typically called by dump().
          * @param[in,out] s The stream to write the data to. */
      void dumpCorrections(std::ostream& s) const override;

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 12s or 6s, depending on whether
          *   this is CNAV on L5 or on L2.
          */
      CommonTime getUserTime() const override;

         /** Get inter-signal corrections for the single-frequency user.
          * @param[in] oid The carrier band and tracking code of the
          *   signal to get the correction for.
          * @param[out] corr The correction in seconds for the given band/code.
          * @return true If band/code are valid for this object and
          *   corr was set according to available data. */
      bool getISC(const ObsID& oid, double& corr) const override;

         /** Get inter-signal corrections for the dual-frequency user.
          * @param[in] oid1 The carrier band/tracking code of the
          *   primary signal that was used to create a dual-frequency,
          *   iono-free combined pseudorange.
          * @param[in] oid2 The carrier band/tracking code of the
          *   secondary signal to get the correction for.
          * @param[out] corr The correction in seconds for the given
          *   band/code pair.
          * @return true If bands/codes are valid for this object and
          *   corr was set according to available data. */
      bool getISC(const ObsID& oid1, const ObsID& oid2, double& corr)
         const override;

      uint32_t pre;   ///< The preamble from the start of the subframe.
      bool alert;     ///< Alert flag
      double iscL1CA; ///< Intersignal corrections for L1 C/A.
      double iscL2C;  ///< Intersignal corrections for L2C.
      double iscL5I5; ///< Intersignal corrections for L5 in-phase.
      double iscL5Q5; ///< Intersignal corrections for L5 quadrature.
   };

      //@}

}

#endif // GNSSTK_GPSCNAVISC_HPP
