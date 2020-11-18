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
#include "MultiFormatNavDataFactory.hpp"
#include "YumaNavDataFactory.hpp"
#include "SEMNavDataFactory.hpp"

namespace gpstk
{
      /** This class is not for public use and thus does not appear in
       * any include file.  It is used to create a singleton object to
       * add supported formats to MultiFormatNavDataFactory.
       * @todo Resolve the memory leak caused by the constructor here. */
   class ExtFactoryInitializer
   {
   public:
      static bool initialized;
         /// Add gpstk ext factories to MultiFormatNavDataFactory
      ExtFactoryInitializer()
      {
         // std::cerr << "starting ExtFactoryInitializer() " << this << std::endl;
         if (initialized)
            return;
         // std::cerr << "continuing ExtFactoryInitializer() " << this << std::endl;
         initialized = true;
            // create our factories
         gpstk::NavDataFactoryPtr
            ndfp1(std::make_shared<gpstk::YumaNavDataFactory>());
         gpstk::NavDataFactoryPtr
            ndfp2(std::make_shared<gpstk::SEMNavDataFactory>());
            // ignore the return value
         MultiFormatNavDataFactory::addFactory(ndfp1);
         MultiFormatNavDataFactory::addFactory(ndfp2);
      }
   };

   bool ExtFactoryInitializer::initialized = false;
   static ExtFactoryInitializer gpstkEFI;
}
