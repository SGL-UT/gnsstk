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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#ifndef GPSTK_NAVSATELLITEID_HPP
#define GPSTK_NAVSATELLITEID_HPP

#include <iostream>
#include "NavSignalID.hpp"
#include "SatID.hpp"
#include "ObsID.hpp"
#include "NavID.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class for identifying the transmitting satellite of a
       * navigation message, as well as the "subject" satellite, in
       * the case of almanac data where the navigation data may be for
       * a different satellite than the one transmitting. */
   class NavSatelliteID : public NavSignalID
   {
   public:
         /// Sets the IDs to 0.
      NavSatelliteID();

         /** Initialize the parent class data while setting the sat
          * and xmitSat to wildcard values.
          * @param[in] right The signal to initialize from. */
      NavSatelliteID(const NavSignalID& right);

         /** Initialize all data to specific values.
          * @param[in] subj The ID of the satellite that nav data pertains to.
          * @param[in] xmit The ID of the satellite that transmitted
          *   the nav data.
          * @param[in] sys The GNSS this signal originates from.
          * @param[in] car The carrier band of this signal.
          * @param[in] track The tracking code of this signal.
          * @param[in] nmt The navigation message format of this signal. */
      NavSatelliteID(unsigned long subj, unsigned long xmit,
                     SatelliteSystem sys, CarrierBand car, TrackingCode track,
                     NavType nmt);

         /** Initialize transmit satellite to a wildcard and all other
          * data to specific values.  Intended to be used for
          * searching.
          * @param[in] subj The ID of the satellite that nav data pertains to.
          * @param[in] sys The GNSS this signal originates from.
          * @param[in] car The carrier band of this signal.
          * @param[in] track The tracking code of this signal.
          * @param[in] nmt The navigation message format of this signal. */
      NavSatelliteID(unsigned long subj,
                     SatelliteSystem sys, CarrierBand car, TrackingCode track,
                     NavType nmt);

         /** Initialize all data to specific values.  This is intended
          * to be used when decoding GPS LNAV subframe 5 page 25, but
          * it may have other uses.
          * @param[in] subj The ID of the satellite that nav data pertains to.
          * @param[in] xmit The ID of the satellite that transmitted
          *   the nav data.
          * @param[in] oid The carrier band/tracking code this signal
          *   originates from.
          * @param[in] navid The navigation message format of this signal. */
      NavSatelliteID(unsigned long subj, const SatID& xmit, const ObsID& oid,
                     const NavID& navid);

         /** Initialize all data to specific values.  This is intended
          * to be used when decoding CNAV almanacs on QZSS (which can
          * have GPS satellites as the subject ID), but it may have
          * other uses.
          * @param[in] subj The ID of the satellite that nav data pertains to.
          * @param[in] xmit The ID of the satellite that transmitted
          *   the nav data.
          * @param[in] oid The carrier band/tracking code this signal
          *   originates from.
          * @param[in] navid The navigation message format of this signal. */
      NavSatelliteID(const SatID& subj, const SatID& xmit, const ObsID& oid,
                     const NavID& navid);

         /** Initialize all data to specific values.
          * @param[in] subj The ID of the satellite that nav data pertains to.
          * @param[in] xmit The ID of the satellite that transmitted
          *   the nav data.
          * @param[in] car The carrier band of this signal.
          * @param[in] track The tracking code of this signal.
          * @param[in] nmt The navigation message format of this signal. */
      NavSatelliteID(const SatID& subj, const SatID& xmit, CarrierBand car,
                     TrackingCode track, NavType nmt);

         /** Initialize system, sat to the given value, and the
          * remaining data members to wildcards.
          * @param[in] subj The satellite that nav data pertains to. */
      NavSatelliteID(const SatID& subj);

         /// Sorting so we can use this class as a map key
      bool operator<(const NavSatelliteID& right) const;
         /// Comparison, including wildcards.
      bool operator==(const NavSatelliteID& right) const;
         /// Implicit != not available
      bool operator!=(const NavSatelliteID& right) const
      { return !(operator==(right)); }

         /// return true if any of the fields are set to match wildcards.
      bool isWild() const override;

      SatID sat;     ///< ID of satellite to which the nav data applies.
      SatID xmitSat; ///< ID of the satellite transmitting the nav data.
   };


   inline std::ostream& operator<<(std::ostream& s, const NavSatelliteID& nsid)
   {
      s << "subj:" << nsid.sat << " xmit:" << nsid.xmitSat << " "
        << static_cast<NavSignalID>(nsid);
      return s;
   }

      //@}

}

#endif // GPSTK_NAVSATELLITEID_HPP
