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
#include "PNBBDSD1NavDataFactory.hpp"
#include "PNBBDSD2NavDataFactory.hpp"
#include "PNBGLOFNavDataFactory.hpp"
#include "PNBGLOCNavDataFactory.hpp"
#include "YumaNavDataFactory.hpp"
#include "SEMNavDataFactory.hpp"

namespace gnsstk
{
      /** @note This must be initialized before instantiating
       * factories and adding them to MultiFormatNavDataFactory.
       * @note This deliberately omits the System type, as most users are not
       * expected to use it.*/
   const NavMessageTypeSet allNavMessageTypes { NavMessageType::Almanac,
                                                NavMessageType::Ephemeris,
                                                NavMessageType::TimeOffset,
                                                NavMessageType::Health,
                                                NavMessageType::Clock,
                                                NavMessageType::Iono,
                                                NavMessageType::ISC };

      /** This class is not for public use.  It is used to create a
       * singleton object to add supported formats to
       * MultiFormatNavDataFactory.
       * @todo Resolve the memory leak caused by the constructor here. */
   class GNSSTKFormatInitializer
   {
   public:
         /// Add gnsstk factories to MultiFormatNavDataFactory
      GNSSTKFormatInitializer()
      {
         static bool initialized = false;
         // std::cerr << "starting GNSSTKFormatInitializer() " << this << std::endl;
         if (initialized)
            return;
         // std::cerr << "continuing GNSSTKFormatInitializer() " << this << std::endl;
         initialized = true;
            // create our factories
            /** @note The order that the factories are added defines
             * their precedence when doing a find in
             * MultiFormatNavDataFactory. */
         gnsstk::NavDataFactoryPtr
            ndfp1(std::make_shared<gnsstk::SP3NavDataFactory>());
         gnsstk::NavDataFactoryPtr
            ndfp2(std::make_shared<gnsstk::RinexNavDataFactory>());
            // ignore the return value
         MultiFormatNavDataFactory::addFactory(ndfp1);
         MultiFormatNavDataFactory::addFactory(ndfp2);

         gnsstk::NavDataFactoryPtr
            ndfp3(std::make_shared<gnsstk::YumaNavDataFactory>());
         gnsstk::NavDataFactoryPtr
            ndfp4(std::make_shared<gnsstk::SEMNavDataFactory>());
         MultiFormatNavDataFactory::addFactory(ndfp3);
         MultiFormatNavDataFactory::addFactory(ndfp4);

         gnsstk::PNBNavDataFactoryPtr
            lnav(std::make_shared<gnsstk::PNBGPSLNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSLNAV,lnav);

         gnsstk::PNBNavDataFactoryPtr
            cnavL2(std::make_shared<gnsstk::PNBGPSCNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAVL2,cnavL2);

         gnsstk::PNBNavDataFactoryPtr
            cnavL5(std::make_shared<gnsstk::PNBGPSCNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAVL5,cnavL5);

         gnsstk::PNBNavDataFactoryPtr
            cnav2(std::make_shared<gnsstk::PNBGPSCNav2DataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAV2,cnav2);

         gnsstk::PNBNavDataFactoryPtr
            gali(std::make_shared<gnsstk::PNBGalINavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GalINAV,gali);

         gnsstk::PNBNavDataFactoryPtr
            galf(std::make_shared<gnsstk::PNBGalFNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GalFNAV,galf);

         gnsstk::PNBNavDataFactoryPtr
            bd1(std::make_shared<gnsstk::PNBBDSD1NavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::BeiDou_D1,bd1);

         gnsstk::PNBNavDataFactoryPtr
            bd2(std::make_shared<gnsstk::PNBBDSD2NavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::BeiDou_D2,bd2);

         gnsstk::PNBNavDataFactoryPtr
            glof(std::make_shared<gnsstk::PNBGLOFNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GloCivilF,glof);

         gnsstk::PNBNavDataFactoryPtr
            gloc(std::make_shared<gnsstk::PNBGLOCNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GloCivilC,gloc);
      }
   };

   static GNSSTKFormatInitializer gnsstkMFNDFI;
}
