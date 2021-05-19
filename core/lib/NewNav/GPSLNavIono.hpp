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
#ifndef GPSTK_GPSLNAVIONO_HPP
#define GPSTK_GPSLNAVIONO_HPP

#include "IonoData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS LNav ionospheric data.
   class GPSLNavIono : public IonoData
   {
   public:
         /// Sets the nav message type.
      GPSLNavIono();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override
      { return true; }

         /** Get the ionospheric correction in meters.
          * @param[in] when The time of the observation to correct.
          * @param[in] rxgeo The receiver's geodetic position.
          * @param[in] svgeo The observed satellite's geodetic position.
          * @param[in] band The carrier band of the signal being corrected.
          * @return The ionospheric delay, in meters, on band. */
      double getCorrection(const CommonTime& when,
                           const Position& rxgeo,
                           const Position& svgeo,
                           CarrierBand band) const override;

      uint32_t pre;    ///< The TLM preamble from word 1 of the subframe.
      uint32_t tlm;    ///< The TLM message from word 1 of the subframe.
      bool alert;      ///< Alert flag from HOW.
      bool asFlag;     ///< Anti-spoof flag from HOW.
         // alpha/Beta terms are seconds, seconds/semi-circle,
         // seconds/semi-circle**2, seconds/semi-circle**3.  Refer to
         // IS-GPS-200 Table 20-X.
      double alpha[4]; ///< Alpha terms of Klobuchar model, as-broadcast units.
      double beta[4];  ///< Beta terms of Klobuchar model, as-broadcast units.
   };

      //@}

}

#endif // GPSTK_GPSLNAVIONO_HPP
