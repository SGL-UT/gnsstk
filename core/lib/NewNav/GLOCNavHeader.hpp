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
#ifndef GNSSTK_GLOCNAVHEADER_HPP
#define GNSSTK_GLOCNAVHEADER_HPP

#include "CommonTime.hpp"
#include "SVHealth.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements shared between all GLONASS
       * Civil C-Nav strings. */
   class GLOCNavHeader
   {
   public:
         /// Sets data members to 0.
      GLOCNavHeader();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const;

         /** Dump String header overhead information (time stamps).
          * @param[in] label The label to use for the time stamp (<=10 chars).
          * @param[in,out] s The stream to write the data to. */
      void dumpStrOverhead(const std::string& label, std::ostream& s) const;

         /** Dump String header flags.
          * @param[in,out] s The stream to write the data to. */
      void dumpFlags(std::ostream& s) const;

         /** Write a couple of lines of Detail-style dump header for
          * this type of object, corresponding to the dumpStrOverhead output.
          * @param[in,out] s The stream to write the header to. */
      static void dumpOverHeader(std::ostream& s);

      CommonTime xmit;    ///< Transmit time of the string.
      uint32_t preamble;  ///< 20 bit preamble for the message.
      uint16_t TS;        ///< Time stamp (3 second counter for each string).
         /** @note the j field is defined in ICD section 4.2.2.4 as the
          * transmit SV ID, but is then for almanac strings it's
          * denoted j<sub>A</sub> indicating the subject SV.  So in
          * the context of the gnsstk implementation, it's effectively
          * the subject SV ID. */
      uint8_t svid;       ///< Subject SV ID (j).
      bool svUnhealthy;   ///< Health flag (H<sup>j</sup>, false=healthy).
      bool dataInvalid;   ///< Data validity flag (l<sup>j</sup>, false=valid).
      SVHealth health;    ///< SV health status.
      uint8_t P1;         ///< SV call to ground control.
      bool P2;            ///< false=sun pointing (see ICD 4.2.2.8).
      unsigned KP;        ///< Leap second indicator (see ICD 4.2.2.9).
      bool A;             ///< true=time correction planned (see ICD 4.2.2.10).
   };

      //@}

}

#endif // GNSSTK_GLOCNAVHEADER_HPP
