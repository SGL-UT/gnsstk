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

/**
 * @file IonoModelStore.cpp
 * Store GPS Navigation Message based ionospheric models
 */

#include "IonoModelStore.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{

      /* Get the ionospheric correction value.
       *
       * \param time the time of the observation
       * \param rxgeo the WGS84 geodetic position of the receiver
       * \param svel the elevation angle between the rx and SV (degrees)
       * \param svaz the azimuth angle between the rx and SV (degrees)
       * \param band the GPS band the observation was made from
       * \return the ionospheric correction (meters)
       */
   double IonoModelStore::getCorrection(const CommonTime& time,
                                        const Position& rxgeo,
                                        double svel,
                                        double svaz,
                                        CarrierBand band) const
   {
      IonoModelMap::const_iterator i = ims.upper_bound(time);
      if (!ims.empty() && i != ims.begin())
      {
         i--;
         return i->second.getCorrection(time, rxgeo, svel, svaz, band);
      }
      else
      {
         NoIonoModelFound e;
         GNSSTK_THROW(e);
      }

   }  // End of method 'IonoModelStore::getCorrection()'


      /* Add an IonoModel to this collection
       *
       * \param mt the time the model is valid from
       * \param im the IonoModel to add
       * \return true if the model was added, false otherwise
       */
   bool IonoModelStore::addIonoModel(const CommonTime& mt, const IonoModel& im)
      throw()
   {
      if (!im.isValid())
         return false;

      IonoModelMap::const_iterator i = ims.upper_bound(mt);
      if (!ims.empty() && i != ims.begin())
      {
            // Compare to previous stored model and, if they have the
            // the same alpha and beta parameters, don't store it.
         i--;
         if (im == i->second)
         {
            return false;
         }
      }
      ims[mt] = im;

      return true;

   }  // End of method 'IonoModelStore::addIonoModel()'


      /** Edit the dataset, removing data outside the indicated time interval
       *
       * \param tmin defines the beginning of the time interval
       * \param tmax defines the end of the time interval
       */
   void IonoModelStore::edit(const CommonTime& tmin,
                             const CommonTime& tmax)
    {
         // Get the first element >= tmin
      IonoModelMap::iterator lower = ims.lower_bound(tmin);
      if (lower != ims.begin())
      {
         if (lower->first != tmin)
         {
               // An earlier element has not yet been superceeded at tmin, so
               // retain the earlier element and delete all previous elements.
               // This should prevent the removeal of models that are still
               // in use at tmin.
            lower--;
         }
            // Erase all old elements
         ims.erase(ims.begin(), lower);
      }
         // Get the first element > tmax
      IonoModelMap::iterator upper = ims.upper_bound(tmax);
      if (upper != ims.end())
      {
            // Erase all future elements
         ims.erase(upper, ims.end());
      }
   }


   gnsstk::CommonTime IonoModelStore::getInitialTime() const
   {
      return (ims.empty() ? CommonTime::END_OF_TIME : ims.begin()->first);
   }


   gnsstk::CommonTime IonoModelStore::getFinalTime() const
   {
      return (ims.empty() ? CommonTime::BEGINNING_OF_TIME : ims.rbegin()->first);
   }


   void IonoModelStore::dump(std::ostream& s) const
   {
      unsigned n = 1;
      IonoModelMap::const_iterator i = ims.begin();
      for ( ; i != ims.end(); ++i, ++n)
      {
         s << std::setw(3) << n << gnsstk::printTime(i->first, " : %04Y %03j %08.2s  ");
         i->second.dump(s);
      }
   }

}  // End of namespace gnsstk
