//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#ifndef GNSSTK_GALINAVALM_HPP
#define GNSSTK_GALINAVALM_HPP

#include "OrbitDataKepler.hpp"
#include "GalHealthStatus.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to Galileo I/NAV almanacs.
   class GalINavAlm : public OrbitDataKepler
   {
   public:
         /// A ref value defined in OS-SIS-ICD Table 75 (meters).
      static const double refA;
         /// inclination offset, this + delta i = i0, defined in OS-SIS-ICD.
      static const double refioffset;

         /// Sets the nav message type.
      GalINavAlm();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time of most recent message type of the pair + 2s.
          */
      CommonTime getUserTime() const override;

         /** This is just a method for making the dump output say
          * "Almanac" vs "Ephemeris" when appropriate. */
      std::string getDataType() const override
      { return "Almanac"; }

         /** Override dumpHarmonics to hide them in output since
          * Galileo I/NAV almanacs don't contain this data. */
      void dumpHarmonics(std::ostream& s) const override
      {}

         /// Fill the beginFit and endFit values for this object.
      void fixFit();

         /** Set the SVHealth value "health" according to the health
          * status variables hsE5b and hsE1B */
      void fixHealth();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

         /** Transmit time of the second word type used in constructing
          * the almanac.  That is:
          * SVID | xmitTime word type | xmit2 word type
          * ---- | ------------------ | ---------------
          * 1    | 7                  | 8
          * 2    | 8                  | 9
          * 3    | 9                  | 10
          */
      CommonTime xmit2;
      double dAhalf;         ///< delta sqrt(A)
      double deltai;         ///< Inclination in rad relative to 0.3*pi rad.
      unsigned wna;          ///< Reference week for t0a.
      double t0a;            ///< Convenience storage of unqualified t0a.
      uint8_t ioda1;         ///< IODa for the first word type (@see xmit2).
      uint8_t ioda2;         ///< IODa for the second word type (@see xmit2).
      GalHealthStatus hsE5b; ///< Health status for E5b.
      GalHealthStatus hsE1B; ///< Health status for E1B.
   };

      //@}

}

#endif // GNSSTK_GALINAVALM_HPP
