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

/**
 * @file IonoModelStore.hpp
 * Store GPS Navigation Message based ionospheric models
 */


#ifndef GPSTK_IONOMODELSTORE_HPP
#define GPSTK_IONOMODELSTORE_HPP

#include <map>
#include "CommonTime.hpp"
#include "CarrierBand.hpp"
#include "IonoModel.hpp"

namespace gpstk
{
      /// @ingroup GPSsolutions
      //@{

      /** This class defines an interface to hide how we determine
       * the ionospheric delay as determined from GPS navigation message
       * based models at some point in time
       */
   class IonoModelStore
   {
   public:

         /** Thrown when attempting to get a model that isn't stored.
          * @ingroup exceptiongroup
          */
      NEW_EXCEPTION_CLASS(NoIonoModelFound, gpstk::Exception);


         /// Constructor
      IonoModelStore() {}

         /// Destructor
      virtual ~IonoModelStore() {}

         /** Get the ionospheric correction value.
          *
          * @param[in] time Time of the observation
          * @param[in] rxgeo WGS84 geodetic position of the receiver
          * @param[in] svel Elevation angle between the rx and SV (degrees)
          * @param[in] svaz Azimuth angle between the rx and SV (degrees)
          * @param[in] freq GPS band the observation was made from
          * @return the ionospheric correction (meters)
          * @throw NoIonoModelFound
          */
      virtual double getCorrection(const CommonTime& time,
                                   const Position& rxgeo,
                                   double svel,
                                   double svaz,
                                   CarrierBand band = CarrierBand::L1) const;

         /** Add an IonoModel to this collection
          *
          * @param[in] mt Time the model is valid from
          * @param[in] im IonoModel to add
          * @return true if the model was added, false otherwise
          */
      bool addIonoModel(const CommonTime& mt,
                        const IonoModel& im) throw();

         /** Edit the dataset, removing data outside the indicated time interval
          *
          * @param[in] tmin Defines the beginning of the time interval (inclusive)
          * @param[in] tmax Defines the end of the time interval (inclusive)
          */
      void edit(const CommonTime& tmin, 
                const CommonTime& tmax = CommonTime::END_OF_TIME);

         /** Remove all data from the store
          */ 
      void clear()
      { ims.clear(); }

         /** Return the earliest time in the store, or return
          * CommonTime::END_OF_TIME if the store is empty.
          * @return The store initial time
          */
      virtual CommonTime getInitialTime() const;

         /** Return the latest time in the store, or return
          * CommonTime::BEGINNING_OF_TIME if the store is empty.
          * @return The store final time
          */
      virtual CommonTime getFinalTime() const;

         /** Return the number of models in the store.
          * @return Store size
          */
      virtual unsigned size() const
      { return ims.size(); }

         /** Return whether the store is empty.
          * @return Whether to store is empty
          */
      virtual bool empty() const
      { return ims.empty(); }

         /** Dump to contents of the store in human-readable form.
          * @param[in,out] s Stream to receive the output; defaults to cout
          */
      virtual void dump(std::ostream& s=std::cout) const;

   private:


      typedef std::map<CommonTime, IonoModel> IonoModelMap;

      IonoModelMap ims;


   }; // End of class 'IonoModelStore'
   
      //@}

}  // End of namespace gpstk

#endif  // GPSTK_IONOMODELSTORE_HPP
