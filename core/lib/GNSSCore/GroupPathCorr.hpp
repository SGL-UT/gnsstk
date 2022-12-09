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


#ifndef GNSSTK_GROUPPATHCORR_HPP
#define GNSSTK_GROUPPATHCORR_HPP

#include <list>
#include "Position.hpp"
#include "Xvt.hpp"
#include "SatID.hpp"
#include "ObsID.hpp"
#include "CommonTime.hpp"
#include "NavID.hpp"
#include "GroupPathCorrector.hpp"
#include "CorrectionResults.hpp"
#include "NavLibrary.hpp"

namespace gnsstk
{
      /// @ingroup GNSSsolutions
      //@{

      /** Provide a class for summing a set of group path corrections.
       * Use it by instantiating the GroupPathCorrector objects that
       * are desired and adding them to #calcs.  Then, call getCorr()
       * for each observation to be corrected.
       * @code
       * GroupPathCorr pec;
       * GroupPathCorrectorPtr iono = std::make_shared<BCIonoCorrector>();
       * GroupPathCorrectorPtr trop = std::make_shared<NBTropCorrector>();
       * GroupPathCorrectorPtr isc = std::make_shared<BCISCorrector>();
       * pec.calcs.push_back(iono);
       * pec.calcs.push_back(trop);
       * pec.calcs.push_back(isc);
       * ...
       * if (!pec.getCorr(rxPos, svPos, sat, obsID, when, nav, corrOut))
       *    return false;
       * cout << "Correction sum: " << corrOut << endl;
       * @endcode
       *
       * You may wish to use the init(), initGlobal(), initNB() method
       * or manually add GroupPathCorrector objects as noted above, or
       * really you can use both.  The ordering of the correctors is
       * key to how the results work, but is only relevant when e.g. an
       * ISC from broadcast nav data is allowed and ISC data from a
       * simple text file (not currently supported, example only) is
       * added.
       *
       * @note The Global and New Brunwswick Trop models are the most
       * accurate models and are the only ones supported in this
       * fashion.  This keeps us from implementing a method for each
       * trop model, and avoids using template dynamic unsupported by
       * Python.
       */
   class GroupPathCorr
   {
   public:
         /// Declaring it explicitly so people don't complain.
      GroupPathCorr() = default;

         /** Fill calcs with the default set of BCISCorrector and
          * BCIonoCorrector.
          * @param[in] navLib The NavLibrary for the correctors to use.
          * @return true on success, false on error. */
      bool init(std::shared_ptr<NavLibrary>& navLib);

         /** Fill calcs with the default set of BCISCorrector,
          * BCIonoCorrector and GlobalTropCorrector.
          * @param[in] navLib The NavLibrary for the correctors to use.
          * @param[in] rinMetFile A path to a RINEX MET file to read
          *   for trop corrections.  If no path is specified (the
          *   variable is left blank), no data will be read and it
          *   will be up to the caller to do any loading of data
          *   later.
          * @return true on success, false on error. */
      bool initGlobal(std::shared_ptr<NavLibrary>& navLib,
                      const std::string& rinMetFile = "");

         /** Fill calcs with the default set of BCISCorrector,
          * BCIonoCorrector and NBTropCorrector.
          * @param[in] navLib The NavLibrary for the correctors to use.
          * @param[in] rinMetFile A path to a RINEX MET file to read
          *   for trop corrections.  If no path is specified (the
          *   variable is left blank), no data will be read.  The
          *   NBTropModel in this case will use its own internal data
          *   model.
          * @return true on success, false on error. */
      bool initNB(std::shared_ptr<NavLibrary>& navLib,
                  const std::string& rinMetFile = "");

         /** @param[in] dups Indicate how duplicate CorrectorType
          *    objects will be used. 
          *      * ComputeFirst indicates the sum should include the
          *        first valid correction of a given type, and no
          *        further computation of that type will be done after
          *        the first valid correction is computed.
          *      * ComputeLast indicates the sum should include the
          *        last valid correction of a given type (all
          *        corrections will be necessarily computed).
          *      * UseFirst indicates the sum should include the first
          *        valid correction of a given type, but the
          *        CorrectionResults object will include biases for
          *        all GroupPathCorrector objects in #calcs.
          * @note A return value of false only indicates that one or
          *   more of the GroupPathCorrector objects in #calcs
          *   returned a false.  There may still be usable corrections
          *   returned.  It is up to the caller to decide whether or
          *   not to use a partial success.  Example: Corrector 1
          *   spans time t1-t2, Corrector 2 spans time t2-t3.  The
          *   getCorr() method for Corrector 1 might return true for a
          *   time within the span of t1-t2 while Corrector 2 might
          *   not.
          * @copydoc GroupPathCorrector::getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, double&)
          */
      bool getCorr(const Position& rxPos, const Position& svPos,
                   const SatID& sat, const ObsID& obs, const CommonTime& when,
                   NavType nav, CorrectionResults& corrOut,
                   CorrDupHandling dups = CorrDupHandling::ComputeFirst);
         /// @copydoc getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, CorrectionResults&, CorrDupHandling)
      bool getCorr(const Position& rxPos, const Xvt& svPos,
                   const SatID& sat, const ObsID& obs, const CommonTime& when,
                   NavType nav, CorrectionResults& corrOut,
                   CorrDupHandling dups = CorrDupHandling::ComputeFirst);
         /// @copydoc getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, CorrectionResults&, CorrDupHandling)
      bool getCorr(const Position& rxPos, const Position& svPos,
                   const SatID& sat, const ObsID& obs, const CommonTime& when,
                   NavType nav, double& corrOut,
                   CorrDupHandling dups = CorrDupHandling::ComputeFirst);
         /// @copydoc getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, CorrectionResults&, CorrDupHandling)
      bool getCorr(const Position& rxPos, const Xvt& svPos, const SatID& sat,
                   const ObsID& obs, const CommonTime& when, NavType nav,
                   double& corrOut,
                   CorrDupHandling dups = CorrDupHandling::ComputeFirst);

         /// The list of GroupPathCorrector objects to use in the calculation.
      GroupPathCorrectorList calcs;
   }; // class GroupPathCorr

      //@}

} // namespace gnsstk

#endif // GNSSTK_GROUPPATHCORR_HPP
