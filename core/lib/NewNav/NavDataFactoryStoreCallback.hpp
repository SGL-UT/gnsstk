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
#ifndef GNSSTK_NAVDATAFACTORYSTORECALLBACK_HPP
#define GNSSTK_NAVDATAFACTORYSTORECALLBACK_HPP

#include "NavDataFactoryCallback.hpp"
#include "NavDataFactoryWithStore.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Standard version of Callback used in conjunction with
       * NavDataFactoryWithStoreFile::loadIntoMap(). */
   class NavDataFactoryStoreCallback : public NavDataFactoryCallback
   {
   public:
         /** Initialize the factory pointer.  Maps default to internals.
          * @warning Because this is a "classic" pointer and not a
          *   shared_ptr, care must be taken to ensure that the object
          *   isn't deleted before the callback is done with it. */
      NavDataFactoryStoreCallback(NavDataFactoryWithStore* ndf);
         /** Initialize the factory pointer with separate data maps.
          * @warning Because this is a "classic" pointer and not a
          *   shared_ptr, care must be taken to ensure that the object
          *   isn't deleted before the callback is done with it. */
      NavDataFactoryStoreCallback(
         NavDataFactoryWithStore* ndf,
         NavMessageMap& othNavMap,
         NavNearMessageMap& othNavNearMap,
         NavDataFactoryWithStore::OffsetCvtMap& othOfsMap);

         /// Factory to use to call addNavData.
      NavDataFactoryWithStore *fact;
         /// Map in which to store user-order data.
      NavMessageMap& navMap;
         /// Map in which to store nearest-order data.
      NavNearMessageMap& navNearMap;
         /// Map in which to store time offset data.
      NavDataFactoryWithStore::OffsetCvtMap& ofsMap;
         /** Store processed data internally.
          * @param[in] navOut The data to process in the callback.
          * @return true if successful. */
      bool process(const NavDataPtr& navOut) override;
   };

      //@}
} // namespace gnsstk

#endif // GNSSTK_NAVDATAFACTORYSTORECALLBACK_HPP
