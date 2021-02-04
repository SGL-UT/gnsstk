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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#ifndef GPSTK_PNBMULTIGNSSNAVDATAFACTORY_HPP
#define GPSTK_PNBMULTIGNSSNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Define a wrapper class for all known PNBNavDataFactory
       * classes, allowing a single class to be used to handle all
       * GNSSes. 
       * @warning Because the factories used by this class are
       *   maintained in a static object, the validity filters and
       *   type filters are essentially "non-volatile" across object
       *   instantiations.  That is, if you create one
       *   PNBMultiGNSSNavDataFactory, change the filter settings,
       *   destroy it and create another, the filter settings will be
       *   the same as they were in the destroyed object.  Do not
       *   expect the filter settings to be reinitialized with each
       *   constructed PNBMultiGNSSNavDataFactory.  Additionally,
       *   contained factories will retain their state unless you
       *   explicitly call resetState().  When using contiguous data,
       *   it's probably best to not use resetState(), but when using
       *   non-contiguous data, using resetState() prevents invalid
       *   assembly of data. */
   class PNBMultiGNSSNavDataFactory : public PNBNavDataFactory
   {
   public:
         /** Set the factories' handling of valid and invalid
          * navigation data.  This should be called before any addData()
          * calls.
          * @param[in] nvt The new nav data loading filter method. */
      void setValidityFilter(NavValidityType nvt) override;

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before any addData()
          * calls.
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      void setTypeFilter(const NavMessageTypeSet& nmts) override;

         /** Process a PackedNavBits object, producing NavData objects
          * as appropriate.
          * @param[in] navIn The PackedNavBits data to process.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @param[in] cadence The data rate of the navigation
          *   messages being processed.  If cadence < 0, The default
          *   values of NavData::msgLenSec will be used. Values >= 0
          *   will override the default.
          * @return false on error. */
      bool addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                   double cadence = -1)
         override;

         /** Add a new factory to the support map.  The factory must
          * be derived from PNBNavDataFactory but not an instance of
          * this class.
          * @param[in] navType The nav message that the factory processes.
          * @param[in] fact The PNBNavDataFactory object to add.
          * @return false if fact is not a valid factory class.
          */
      static bool addFactory(NavType navType, PNBNavDataFactoryPtr& fact);

         /** Reset the state of the data accumulator.  Most
          * PNBNavDataFactory child classes will maintain some state
          * to assemble data prior to processing.  This method is
          * intended to be used to clear out that intermediate data to
          * start from a fresh state, e.g. if you're loading
          * discontinuous data. */
      void resetState() override;

   protected:
         /** Known PNB -> nav data factories, organized by navigation
          * message type.  Declared static so that the user doesn't
          * have to add all the factories themselves. */
      static PNBNavDataFactoryMap& factories();
   }; // class PNBMultiGNSSNavDataFactory

      //@}

} // namespace gpstk

#endif // GPSTK_PNBMULTIGNSSNAVDATAFACTORY_HPP
