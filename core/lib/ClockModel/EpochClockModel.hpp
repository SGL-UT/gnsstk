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

/**
 * @file EpochClockModel.hpp
 * Finally a concrete class. This model just uses an epoch of ORDs to determine
 * the clock offset at that point in time.
 */

#ifndef EPOCHCLOCKMODEL_HPP
#define EPOCHCLOCKMODEL_HPP

#include <map>
#include "Exception.hpp"
#include "gps_constants.hpp"

#include "ObsClockModel.hpp"
#include "ORDEpoch.hpp"

namespace gnsstk
{
      /// @ingroup ClockModel
      //@{

   class EpochClockModel : public ObsClockModel
   {
   public:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"
      EpochClockModel(double sigma = 2,
                      double elmask = 0,
                      SvMode mode = ALWAYS)
            : ObsClockModel(sigma, elmask, mode), valid(false), clkc(0){}
#pragma clang diagnostic pop
         /**
          * @throw InvalidArgumentException
          */
      virtual double getOffset(const gnsstk::CommonTime& t) const
      {
         if (t!=time)
         {
            gnsstk::InvalidArgumentException e;
            GNSSTK_THROW(e);
         }
         return clkc;
      };

         /**
          * @throw InvalidArgumentException
          */
      virtual bool isOffsetValid(const gnsstk::CommonTime& t) const
      {
         if (t!=time)
         {
            gnsstk::InvalidArgumentException e;
            GNSSTK_THROW(e);
         }
         return valid;
      };


         // An unchecked accessor for programs that don't need the generic
         // interface
      double getOffset() const
         noexcept {return clkc;};

      bool isOffsetValid() const
         noexcept {return valid;};

         /**
          * @throw InvalidValue
          */
      virtual void addEpoch(const ORDEpoch& oe)
      {
         gnsstk::Stats<double> stat = simpleOrdClock(oe);
         clkc = stat.Average();
         valid = stat.N() >=  3; /// we need at least three to have a real avg
         time = oe.time;
      }

   private:
      gnsstk::CommonTime time;   ///< The time of this offset
      double clkc;           ///< clock bias value (same units as residuals)
      bool valid;            ///< flag indicating clock bias statistical validity
   };

      //@}

}
#endif
