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
/** @file NavStatic.cpp contains definitions of static data that need
 * to be initialized in a particular order. */

#include "NavMessageType.hpp"
#include "NavDataFactory.hpp"
#include "MultiFormatNavDataFactory.hpp"
#include "RinexNavDataFactory.hpp"
#include "SP3NavDataFactory.hpp"
#include "PNBMultiGNSSNavDataFactory.hpp"
#include "PNBGPSLNavDataFactory.hpp"
#include "PNBGPSCNavDataFactory.hpp"
#include "PNBGPSCNav2DataFactory.hpp"
#include "PNBGalINavDataFactory.hpp"
#include "PNBGalFNavDataFactory.hpp"
#ifdef BUILD_EXT
// Support for Ext factories must be added here to enforce initialization order
#include "YumaNavDataFactory.hpp"
#include "SEMNavDataFactory.hpp"
#endif

namespace gpstk
{
      /** @note This must be initialized before instantiating
       * factories and adding them to MultiFormatNavDataFactory. */
   const NavMessageTypeSet allNavMessageTypes { NavMessageType::Almanac,
                                                NavMessageType::Ephemeris,
                                                NavMessageType::TimeOffset,
                                                NavMessageType::Health,
                                                NavMessageType::Clock,
                                                NavMessageType::Iono };

      /** This class is not for public use.  It is used to create a
       * singleton object to add supported formats to
       * MultiFormatNavDataFactory.
       * @todo Resolve the memory leak caused by the constructor here. */
   class GPSTKFormatInitializer
   {
   public:
         /// Add gpstk factories to MultiFormatNavDataFactory
      GPSTKFormatInitializer()
      {
         static bool initialized = false;
         // std::cerr << "starting GPSTKFormatInitializer() " << this << std::endl;
         if (initialized)
            return;
         // std::cerr << "continuing GPSTKFormatInitializer() " << this << std::endl;
         initialized = true;
            // create our factories
            /** @note The order that the factories are added defines
             * their precedence when doing a find in
             * MultiFormatNavDataFactory. */
         gpstk::NavDataFactoryPtr
            ndfp1(std::make_shared<gpstk::SP3NavDataFactory>());
         gpstk::NavDataFactoryPtr
            ndfp2(std::make_shared<gpstk::RinexNavDataFactory>());
            // ignore the return value
         MultiFormatNavDataFactory::addFactory(ndfp1);
         MultiFormatNavDataFactory::addFactory(ndfp2);
#ifdef BUILD_EXT
         gpstk::NavDataFactoryPtr
            ndfp3(std::make_shared<gpstk::YumaNavDataFactory>());
         gpstk::NavDataFactoryPtr
            ndfp4(std::make_shared<gpstk::SEMNavDataFactory>());
         MultiFormatNavDataFactory::addFactory(ndfp3);
         MultiFormatNavDataFactory::addFactory(ndfp4);
#endif

         gpstk::PNBNavDataFactoryPtr
            lnav(std::make_shared<gpstk::PNBGPSLNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSLNAV,lnav);

         gpstk::PNBNavDataFactoryPtr
            cnavL2(std::make_shared<gpstk::PNBGPSCNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAVL2,cnavL2);

         gpstk::PNBNavDataFactoryPtr
            cnavL5(std::make_shared<gpstk::PNBGPSCNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAVL5,cnavL5);

         gpstk::PNBNavDataFactoryPtr
            cnav2(std::make_shared<gpstk::PNBGPSCNav2DataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAV2,cnav2);

         gpstk::PNBNavDataFactoryPtr
            gali(std::make_shared<gpstk::PNBGalINavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GalINAV,gali);

         gpstk::PNBNavDataFactoryPtr
            galf(std::make_shared<gpstk::PNBGalFNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GalFNAV,galf);
      }
   };

   static GPSTKFormatInitializer gpstkMFNDFI;
}
