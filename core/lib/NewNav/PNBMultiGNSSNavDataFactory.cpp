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
#include "PNBMultiGNSSNavDataFactory.hpp"

namespace gpstk
{
   void PNBMultiGNSSNavDataFactory ::
   setValidityFilter(NavValidityType nvt)
   {
      for (auto& fi : factories())
      {
         fi.second->setValidityFilter(nvt);
      }
   }


   void PNBMultiGNSSNavDataFactory ::
   setTypeFilter(const NavMessageTypeSet& nmts)
   {
      for (auto& fi : factories())
      {
         fi.second->setTypeFilter(nmts);
      }
   }


   bool PNBMultiGNSSNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      NavType navType = navIn->getNavID().navType;
      auto fi = factories().find(navType);
      if (fi == factories().end())
      {
            // We don't have a factory for this navigation message type
         return false;
      }
      return fi->second->addData(navIn, navOut, cadence);
   }


   bool PNBMultiGNSSNavDataFactory ::
   addFactory(NavType navType, PNBNavDataFactoryPtr& fact)
   {
         // make sure it's a valid class first
      PNBNavDataFactory *ndfp = fact.get();
      if (ndfp == nullptr)
      {
         return false;
      }
      if (dynamic_cast<PNBMultiGNSSNavDataFactory*>(ndfp) != nullptr)
      {
         return false;
      }
      factories()[navType] = fact;
      return true;
   }


   void PNBMultiGNSSNavDataFactory ::
   resetState()
   {
      for (auto& fi : factories())
      {
         fi.second->resetState();
      }
   }


   PNBNavDataFactoryMap& PNBMultiGNSSNavDataFactory ::
   factories()
   {
      static PNBNavDataFactoryMap rv;
      return rv;
   }
}
