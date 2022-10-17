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
#ifndef GNSSTK_GLOCNAVLTDMP_HPP
#define GNSSTK_GLOCNAVLTDMP_HPP

#include "GLOCNavData.hpp"
#include "GLOCSatType.hpp"
#include "GLOCRegime.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GLONASS Civil C-Nav
       * long-term dynamic model parameters.
       * @see PNBGLOCNavDataFactory for additional details on the
       *   long-term dynamic model parameters. */
   class GLOCNavLTDMP
   {
   public:
         /// Set data members to NaN
      GLOCNavLTDMP();
         /// Dump (in full detail) the contents of this object.
      void dump(std::ostream& s) const;
         /// @return true if the header SVIDs match sv but not zero.
      bool isSVID(uint8_t sv) const
      { return ((sv != 0) && (header31.svid == sv) && (header32.svid == sv)); }
         /// @return true if the given t_b matches those in this object.
      bool tbMatch(uint16_t tb) const
      { return (tb == tb31) && (tb == tb32); }
         /** @return true if tb31 and tb32 match, and at least one
          * param is non-zero. */
      bool validate() const;
         /** Get the a_x, a_y and a_z values given a time offset from reference.
          * @param[in] deltat The difference in sec between time of
          *   interest and reference.
          * @return a Vector of doubles containing, in order, a_x,
          *   a_y, a_z in units of km/s**2. */
      Vector<double> geta(double deltat) const;

      GLOCNavHeader header31; ///< Header (incl xmit time) data from string 31.
      GLOCNavHeader header32; ///< Header (incl xmit time) data from string 32.
      unsigned long tb31;   ///< Reference instant in Moscow time for string 31.
      unsigned long tb32;   ///< Reference instant in Moscow time for string 32.

         // These parameters are all in km/s**(n+2), where n=[0..4]
         // e.g. dax0 is in km/s**2, etc.
      double dax0;
      double day0;
      double daz0;
      double ax1;
      double ay1;
      double az1;
      double ax2;
      double ay2;
      double az2;
      double ax3;
      double ay3;
      double az3;
      double ax4;
      double ay4;
      double az4;
   }; // class GLOCNavLTDMP
} // namespace gnsstk

#endif // GNSSTK_GLOCNAVLTDMP_HPP
