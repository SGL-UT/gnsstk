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
#ifndef GNSSTK_GPSCNAVREDALM_HPP
#define GNSSTK_GPSCNAVREDALM_HPP

#include "GPSCNavAlm.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS CNav reduced almanac.
   class GPSCNavRedAlm : public GPSCNavAlm
   {
   public:
         /// Set data to default values.
      GPSCNavRedAlm();
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<GPSCNavRedAlm>(*this); }

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Sets the fixed values (e.g. delta i) according to the
          * satellite system.  GPS has different reference values for
          * the reduced almanac from QZSS.  This method should be
          * called to fill out the remaining values in the reduced
          * almanac after loading the bits from the packet and setting
          * the signal. */
      void fixValues();

      double deltaA;      ///< Semi-major axis relative to reference value.
      double phi0;        ///< Argument of latitude at reference time (M0+w).
   };

      //@}

}

#endif // GNSSTK_GPSCNAVREDALM_HPP
