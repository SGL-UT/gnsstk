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
#ifndef GNSSTK_GPSLNAVISC_HPP
#define GNSSTK_GPSLNAVISC_HPP

#include "InterSigCorr.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GPS LNAV ISC
       * (T<sub>GD</sub> - group delay differential).
       * @note The getISC() methods are inherited unchanged from
       *   InterSigCorr. */
   class GPSLNavISC : public InterSigCorr
   {
   public:
         /// Initialize data members.
      GPSLNavISC();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

      uint32_t pre;    ///< The TLM preamble from word 1 of the subframe.
      uint32_t tlm;    ///< The TLM message from word 1 of the subframe.
      bool isf;        ///< Integrity status flag.
      bool alert;      ///< Alert flag from HOW.
      bool asFlag;     ///< Anti-spoof flag from HOW.
   };

      //@}

}

#endif // GNSSTK_GPSLNAVISC_HPP
