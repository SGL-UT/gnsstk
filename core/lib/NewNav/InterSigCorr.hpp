//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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
#ifndef GPSTK_INTERSIGCORR_HPP
#define GPSTK_INTERSIGCORR_HPP

#include "NavData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class representing SV-based inter-signal bias correction information.
       * @note All ObsIDs added to refOids or validOids should change
       *   the "type" field to "Unknown", and all derived classes that
       *   override the getISC methods should also make copies of the
       *   input ObsID parameters and set the type to unknown in order
       *   to consistently match.
       * @todo Document in detail how this is meant to be used. */
   class InterSigCorr : public NavData
   {
   public:
         /// Set message type to ISC.
      InterSigCorr();

         /** Get inter-signal corrections for the single-frequency user.
          * @param[in] oid The carrier band and tracking code of the
          *   signal to get the correction for.
          * @param[out] corr The correction in seconds for the given band/code.
          * @return true If band/code are valid for this object and
          *   corr was set according to available data. */
      virtual bool getISC(const ObsID& oid, double& corr)
         const;

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
      virtual bool getISC(const ObsID& oid1, const ObsID& oid2, double& corr)
         const;

         /** The inter-signal correction value from the navigation
          * data (sometimes known as T<sub>GD</sub> or BGD). */
      double isc;

   protected:
         /** The reference signals to which this ISC applies.  These
          * must be all on the same band, though there are no explicit
          * sanity checks for this. */
      std::set<ObsID> refOids;
         /** The set of band/code combinations to which this ISC can
          * be referenced. */
      std::set<ObsID> validOids;
   }; // class InterSigCorr

      //@}

} // namespace gpstk

#endif // GPSTK_INTERSIGCORR_HPP
