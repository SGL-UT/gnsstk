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


#ifndef GNSSTK_GROUPPATHCORRECTOR_HPP
#define GNSSTK_GROUPPATHCORRECTOR_HPP

#include <list>
#include <memory>
#include "Position.hpp"
#include "Xvt.hpp"
#include "SatID.hpp"
#include "ObsID.hpp"
#include "CommonTime.hpp"
#include "NavID.hpp"
#include "CorrectorType.hpp"

namespace gnsstk
{
      /// @ingroup GNSSsolutions
      //@{

      /** Provide an abstract base class that defines the interface
       * for a series of classes that are used to compute pseudorange
       * bias.  Used by GroupPathCorr. */
   class GroupPathCorrector
   {
   public:
         /// Set the #corrType to Unknown by default.
      GroupPathCorrector()
            : corrType(CorrectorType::Unknown)
      {}
         /** Get the bias in meters given the supplied state arguments.
          * @param[in] rxPos The position of the GNSS receiver antenna.
          * @param[in] svPos The position of the satellite with delayed signal.
          * @param[in] sat The identity of the satellite with delayed signal.
          * @param[in] obs The ID of the signal being un-delayed.
          * @param[in] when The time of measurement.
          * @param[in] nav The navigation message type of the signal.
          * @param[out] corrOut The computed bias in meters.
          * @return true if successful, false on error. */
      virtual bool getCorr(const Position& rxPos, const Position& svPos,
                           const SatID& sat, const ObsID& obs,
                           const CommonTime& when, NavType nav,
                           double& corrOut) = 0;
         /// @copydoc getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, double&)
      virtual bool getCorr(const Position& rxPos, const Xvt& svPos,
                           const SatID& sat, const ObsID& obs,
                           const CommonTime& when, NavType nav,
                           double& corrOut) = 0;
         /// Set by child classes, indicates what type of bias is computed.
      CorrectorType corrType;
   }; // class GroupPathCorrector

      /// Short-hand for shared_ptr.
   typedef std::shared_ptr<GroupPathCorrector> GroupPathCorrectorPtr;
      /// Short-hand for container.
   typedef std::list<GroupPathCorrectorPtr> GroupPathCorrectorList;

      //@}

} // namespace gnsstk

#endif // GNSSTK_GROUPPATHCORRECTOR_HPP
