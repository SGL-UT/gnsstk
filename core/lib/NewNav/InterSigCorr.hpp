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
#ifndef GNSSTK_INTERSIGCORR_HPP
#define GNSSTK_INTERSIGCORR_HPP

#include "NavData.hpp"
#include "PackedNavBits.hpp"

namespace gnsstk
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

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Print the actual corrections provided by this object in a
          * human-readable format.  Typically called by dump().
          * @param[in,out] s The stream to write the data to. */
      virtual void dumpCorrections(std::ostream& s) const;

         /** Get inter-signal corrections for the single-frequency user.
          * @param[in] oid The carrier band and tracking code of the
          *   signal to get the correction for.
          * @param[out] corrOut The correction in seconds for the
          *   given band/code.
          * @return true If band/code are valid for this object and
          *   corrOut was set according to available data. */
      virtual bool getISC(const ObsID& oid, double& corrOut)
         const;

         /** Get inter-signal corrections for the dual-frequency user.
          * @param[in] oid1 The carrier band/tracking code of the
          *   primary signal that was used to create a dual-frequency,
          *   iono-free combined pseudorange.
          * @param[in] oid2 The carrier band/tracking code of the
          *   secondary signal to get the correction for.
          * @param[out] corrOut The correction in seconds for the given
          *   band/code pair.
          * @return true If bands/codes are valid for this object and
          *   corrOut was set according to available data. */
      virtual bool getISC(const ObsID& oid1, const ObsID& oid2, double& corrOut)
         const;

         /** Decode a GPS ISC/Tgd value, setting the result to NaN if
          * the bits match the pattern b1_0000_0000_0000, which indicates
          * not available.
          * @note Value is expected to be 13 bits with a scale factor of 2^-35.
          * @param[in] navIn The PackedNavBits data to process.
          * @param[in] startBit The first bit in navIn containing the Tgd/ISC.
          * @return The ISC value, which may be NaN. */
      static double getGPSISC(const PackedNavBitsPtr& navIn, unsigned startBit);

         /// @copydoc NavData::isSameData
      bool isSameData(const NavDataPtr& right) const override
      {
         Exception exc("Unimplemented function");
         GNSSTK_THROW(exc);
      }
         /// @copydoc NavData::compare
      std::list<std::string> compare(const NavDataPtr& right)
         const override
      {
         Exception exc("Unimplemented function");
         GNSSTK_THROW(exc);
      }

         /** The inter-signal correction value from the navigation
          * data (sometimes known as T<sub>GD</sub> or BGD). */
      double isc;

         /** Label for the ISC value when dumping.  May be changed by
          * derived classes.  Length should be less than 20
          * characters. */
      std::string iscLabel;

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

} // namespace gnsstk

#endif // GNSSTK_INTERSIGCORR_HPP
