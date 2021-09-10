//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#ifndef GNSSTK_NAVDATAFACTORYWITHSTOREFILE_HPP
#define GNSSTK_NAVDATAFACTORYWITHSTOREFILE_HPP

#include "NavDataFactoryWithStore.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Define an interface for loading nav data from a file
   class NavDataFactoryWithStoreFile : public NavDataFactoryWithStore
   {
   public:
      NavDataFactoryWithStoreFile()
      {}

         /// Clean up.
      virtual ~NavDataFactoryWithStoreFile()
      {
      }
         /** Load a file into the default map, NavDataFactoryWithStore::data.
          * @param[in] source The path to the file to load.
          * @return true on success, false on failure. */
      bool addDataSource(const std::string& source) override
      { return loadIntoMap(source, data); }

         /** Abstract method that should be overridden by specific
          * file-reading factory classes in order to load the data
          * into the map.
          * @note When implementing this method, make sure to handle
          *   validity checks!
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @return true on success, false on failure. */
      virtual bool loadIntoMap(const std::string& filename,
                               NavMessageMap& navMap) = 0;
   };

      //@}

}

#endif // GNSSTK_NAVDATAFACTORYWITHSTOREFILE_HPP
