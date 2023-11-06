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
#include "SatID.hpp"

namespace gnsstk
{
   SatID ::
   SatID()
         :  id(-1), wildId(false),
            system(SatelliteSystem::GPS), wildSys(false)
   {
   }


   SatID ::
   SatID(int p, SatelliteSystem s)
         : id(p), wildId(false), system(s), wildSys(false)
   {
   }


   SatID ::
   SatID(int p)
         : id(p), wildId(false),
           system(SatelliteSystem::Unknown), wildSys(true)
   {
   }


   SatID ::
   SatID(SatelliteSystem s)
         : id(0), wildId(true), system(s), wildSys(false)
   {
   }


   void SatID ::
   makeWild()
   {
      wildId = wildSys = true;
   }


   bool SatID ::
   isWild() const
   {
      return (wildId || wildSys);
   }


   void SatID ::
   dump(std::ostream& s) const
   {
      if (wildSys)
         s << "*";
      else
         s << convertSatelliteSystemToString(system);
      s << " ";
      if (wildId)
         s << "*";
      else
         s << id;
      if (hasNorad)
      {
         s << " NORAD " << norad;
      }
   }


   bool SatID ::
   operator==(const SatID& right) const
   {
      using namespace std;
      return
         (wildId || right.wildId || (id == right.id)) &&
         (wildSys || right.wildSys || (system == right.system));
   }


   bool SatID ::
   operator<(const SatID& right) const
   {
         // if either system is a wildcard, the systems are effectively equal
      if (!wildSys && !right.wildSys)
      {
         if (system < right.system)
            return true;
         if (right.system < system)
            return false;
      }
         // if either id is a wildcard, the ids are effectively equal
      if (!wildId && !right.wildId)
      {
         if (id < right.id)
            return true;
         if (right.id < id)
            return false;
      }
      return false;
   }


   bool SatID :: isValid() const
   {
      switch(system)
      {
         case SatelliteSystem::GPS: return (id > 0 && id <= MAX_PRN);
               //case SatelliteSystem::Galileo:
               //case SatelliteSystem::Glonass:
               //case SatelliteSystem::Geosync:
               //case SatelliteSystem::LEO:
               //case SatelliteSystem::Transit:
         default: return (id > 0 && id < 100);
      }
   }

   void SatID :: setNorad(unsigned long n)
   {
      norad = n;
      hasNorad = true;
   }
}
