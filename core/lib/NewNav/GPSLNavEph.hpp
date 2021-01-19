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
#ifndef GPSTK_GPSLNAVEPH_HPP
#define GPSTK_GPSLNAVEPH_HPP

#include "GPSLNavData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS LNav ephemerides.
   class GPSLNavEph : public GPSLNavData
   {
   public:
         /// Codes on L2 channel, per IS-GPS-200 20.3.3.3.1.2
      enum class L2Codes
      {
         Invalid1 = 0,
         Pcode = 1,
         CAcode = 2,
         Invalid2 = 3
      };

         /// Sets the nav message type and all other data members to 0.
      GPSLNavEph();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Fill the beginFit and endFit values for this object.
          * @pre Toe, iodc, fitIntFlag and xmitTime must all be set. */
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

      uint32_t pre2;      ///< The TLM preamble from word 1 of subframe 2.
      uint32_t pre3;      ///< The TLM preamble from word 1 of subframe 3.
      uint32_t tlm2;      ///< The TLM message from word 1 of subframe 2.
      uint32_t tlm3;      ///< The TLM message from word 1 of subframe 3.
      uint16_t iodc;      ///< Issue Of Data-Clock for the ephemeris.
      uint16_t iode;      ///< Issue Of Data-Ephemeris.
      uint8_t fitIntFlag; ///< Fit interval flag from subframe 2.
      uint8_t healthBits; ///< 6 SV health bits from subframe 1, word 3.
      uint8_t uraIndex;   ///< 4-bit URA index from subframe 1, word 3.
      double tgd;         ///< Ionospheric group delay in seconds.
      bool alert2;        ///< Alert flag from SF2 HOW
      bool alert3;        ///< Alert flag from SF3 HOW
      bool asFlag2;       ///< Anti-spoof flag from SF2 HOW.
      bool asFlag3;       ///< Anti-spoof flag from SF3 HOW.
      L2Codes codesL2;    ///< Code on L2 in-phase component.
         /** L2 P data flag from subframe 1, word 3.
          * @note This retains the behavior as described in
          *   IS-GPS-200K 20.3.3.3.1.6, in that true (1) indicates the
          *   LNAV data stream was commanded OFF on the P-code of the
          *   in-phase component of the L2 channel */
      bool L2Pdata;
   };


   namespace StringUtils
   {
         /// Convert L2Codes to a printable string for dump().
      std::string asString(GPSLNavEph::L2Codes e);
   }

      //@}

}

#endif // GPSTK_GPSLNAVEPH_HPP
