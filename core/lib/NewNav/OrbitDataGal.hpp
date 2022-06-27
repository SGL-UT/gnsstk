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
#ifndef GNSSTK_ORBITDATAGAL_HPP
#define GNSSTK_ORBITDATAGAL_HPP

#include "OrbitDataKepler.hpp"
#include "GalileoEllipsoid.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Base class for all Galileo broadcast orbit data.
   class OrbitDataGal : public OrbitDataKepler
   {
   public:
      OrbitDataGal()
      {}

         /** Compute the satellite's position and velocity at a time.
          * @note Defaults to using the Galileo ellipsoid parameters.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @param[in] oid Ignored at this level, only used in derived classes.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt,
                  const ObsID& oid = ObsID()) override
      {
         GalileoEllipsoid ell;
         return OrbitDataKepler::getXvt(when, ell, xvt, oid);
      }

         /** Compute satellite relativity correction (sec) at the given time.
          * @param[in] when The time at which to get the relativity correction.
          * @return the relativity correction in seconds.
          */
      double svRelativity(const CommonTime& when) const override
      {
         GalileoEllipsoid ell;
         return OrbitDataKepler::svRelativity(when, ell);
      }
   };
} // namespace gnsstk

#endif // GNSSTK_ORBITDATAGAL_HPP
