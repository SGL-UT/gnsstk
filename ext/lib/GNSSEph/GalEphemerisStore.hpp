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

/// @file GalEphemerisStore.hpp
/// Class for storing and/or computing position, velocity, and clock data using
/// tables of <SatID, <time, GalEphemeris> >. Inherits OrbitEphStore, which includes
/// initial and final times and search methods. GalEphemeris inherits OrbitEph and
/// adds health and accuracy information, which this class makes use of.

#ifndef GNSSTK_GALORBITEPHSTORE_HPP
#define GNSSTK_GALORBITEPHSTORE_HPP

#include "OrbitEphStore.hpp"
#include "GalEphemeris.hpp"
#include "Exception.hpp"
#include "SatID.hpp"
#include "CommonTime.hpp"

namespace gnsstk
{
   /// @ingroup GNSSEph
   //@{

   /// Class for storing and accessing an objects position,
   /// velocity, and clock data. Also defines a simple interface to remove
   /// data that has been added.
   class GalEphemerisStore : public OrbitEphStore
   {
   public:

      /// Default empty constructor.
      GalEphemerisStore()
      {
         timeSystem = TimeSystem::GAL;
         initialTime.setTimeSystem(timeSystem);
         finalTime.setTimeSystem(timeSystem);
      }

      /// Destructor
      virtual ~GalEphemerisStore() { clear(); }

      /// Return a string that will identify the derived class
      virtual std::string getName(void) const
         { return std::string("GalEphemerisStore"); }

      /// Notes regarding the rationalize() function.
      void rationalize(void);

      /// Add a GalEphemeris object to this collection
      /// @return pointer to the new object, NULL if data could not be added.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
       virtual GalEphemeris* addEphemeris(const GalEphemeris& galeph)
      {
         try {
            GalEphemeris neweph(galeph);

            OrbitEph *oeptr = dynamic_cast<OrbitEph*>(&neweph);
            oeptr = OrbitEphStore::addEphemeris(oeptr);
            return dynamic_cast<GalEphemeris*>(oeptr);
         }
         catch(Exception& e) { GNSSTK_RETHROW(e); }
      }
#pragma clang diagnostic pop

      /// Find a GalEphemeris for the indicated satellite at time t, using the
      /// OrbitEphStore::find() routine, which considers the current search method.
      /// @param sat the satellite of interest
      /// @param t the time of interest
      /// @return a pointer to the desired OrbitEph, or NULL if no OrbitEph found.
      /// @throw InvalidRequest if the satellite system is not BeiDou, or if
      /// ephemeris is not found.
      const GalEphemeris& findEphemeris(const SatID& sat, const CommonTime& t) const
      {
         if(sat.system != SatelliteSystem::Galileo) {
            InvalidRequest e("Invalid satellite system");
            GNSSTK_THROW(e);
         }

         const OrbitEph *eph = findOrbitEph(sat,t);
         if(!eph) {
            InvalidRequest e("Ephemeris not found");
            GNSSTK_THROW(e);
         }

         // gynmastics...
         const GalEphemeris* geph = dynamic_cast<const GalEphemeris*>(eph);
         const GalEphemeris& galeph(*geph);

         return galeph;
      }

      /// Add all ephemerides to an existing list<GalEphemeris> for given satellite.
      /// If sat.id is -1 (the default), all ephemerides are added.
      /// @return the number of ephemerides added.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
       int addToList(std::list<GalEphemeris>& gallist,
                        SatID sat=SatID(-1,SatelliteSystem::Galileo)) const;

   }; // end class GalEphemerisStore
#pragma clang diagnostic pop
   //@}

} // namespace

#endif // GNSSTK_GALORBITEPHSTORE_HPP
