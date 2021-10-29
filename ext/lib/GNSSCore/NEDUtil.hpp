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

//
//
#ifndef GNSSTK_NEDUTIL_HPP
#define GNSSTK_NEDUTIL_HPP

// gnsstk
#include "Triple.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Xvt.hpp"

namespace gnsstk
{
    /// @ingroup geodeticgroup
    //@{

      /// A utility for converting from Cartesian in XZY to North-East-Down (NED)
   class NEDUtil
   {
      public:
            // Constructors
          /**
          * Given a location as a (geodetic) latitude and longitude
          * the constructor creates the appropriate rotation matrix
          * from XYZ to NED and retains it for later use.
          * @param refGdLatRad geodetic latitude of point of interest (radians)
          * @param refLonRad longitude of point of interest (radians).
          */
        NEDUtil( const double refGdLatRad,
                  const double refLonRad);

            // Methods
         /**
          * Convert from a vector in ECEF XYZ to ECEF NED using the
          * current rotation matrix.
          * @param[in] inV vector of interest in ECEF XYZ.
          * @return Same type as input but with the vector in ECEF NED
          */
         gnsstk::Vector<double> convertToNED( const gnsstk::Vector<double>& inV )
            const;
         /**
          * Convert from a vector in ECEF XYZ to ECEF NED using the
          * current rotation matrix.
          * @param[in] inVec vector of interest in ECEF XYZ.
          * @return Same type as input but with the vector in ECEF NED
          */
         gnsstk::Triple         convertToNED( const gnsstk::Triple& inVec ) const;
         /**
          * Convert from a vector in ECEF XYZ to ECEF NED using the
          * current rotation matrix.
          * @param[in] in vector of interest in ECEF XYZ.
          * @return Same type as input but with the vector in ECEF NED
          */
         gnsstk::Xvt            convertToNED( const gnsstk::Xvt& in ) const;

         /**
          * Update the rotation matrix to the new location without creating
          * a new object
          * @param refLatRad geodetic latitude of point of interest (radians)
          * @param refLonRad longitude of point of interest (radians).
          */
         void                  updatePosition( const double refLatRad,
                                               const double refLonRad );

            // Utilities
      protected:
         void compute( const double refLat,
                       const double refLon);

         Matrix<double> rotMat;
   };

   //@}

}
#endif
