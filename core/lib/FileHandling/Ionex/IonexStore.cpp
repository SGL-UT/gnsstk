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

/**
 * @file IonexStore.cpp
 * Read and store Ionosphere maps. It computes TEC and RMS values with respect
 * to time and receiver position. Based on extracted TEC values, it calculates
 * the ionospheric delay.
 */


#include "IonexStore.hpp"

using namespace gnsstk::StringUtils;
using namespace gnsstk;
using namespace std;

namespace gnsstk
{
      // coefficient. See more in Seeber G.(2003), Satellite Geodesy
      // 2nd edition, Walter de Gruyter, p.52-54.
   static const double C2_FACT   = 40.3e+16;

   IonexStore ::
   IonexStore()
         : initialTime(CommonTime::END_OF_TIME),
           finalTime(CommonTime::BEGINNING_OF_TIME)
   {
   }


   IonexStore ::
   ~IonexStore()
   {
   }


      // Load the given IONEX file
   void IonexStore ::
   loadFile( const std::string& filename )
   {
      try
      {
            // Stream creation
         IonexStream strm(filename.c_str(), std::ios::in);

         if (!strm)
         {
            FileMissingException e( "File " + filename +
                                    " could not be opened.");
            GNSSTK_THROW(e);
         }

            // create the header object
         IonexHeader header;
         strm >> header;

         if ( !header.valid )
         {
            FileMissingException e( "File " + filename +
                                    " could not be opened. Check again " +
                                    "the path or the name provided!");
            GNSSTK_THROW(e);
         }

            // keep an inventory of the loaded files
         addFile(filename,header);

            // this map is useful in finding DCB value
         inxDCBMap[header.firstEpoch] = header.svsmap;

            // object data. If valid, add to the map
         IonexData iod;
         while ( strm >> iod && iod.isValid() )
         {
            addMap(iod);
         }
      }
      catch (gnsstk::Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }  // End of method 'IonexStore::loadFile()'



      // Insert a new IonexData object into the store
   void IonexStore ::
   addMap(const IonexData& iod)
   {
      CommonTime t(iod.time);
      IonexData::IonexValType type(iod.type);

      if (type != IonexData::UN)
      {
         inxMaps[t][type] = iod;
      }

      if (t < initialTime)
      {
         initialTime = t;
      }
      else if (t > finalTime)
      {
         finalTime = t;
      }
   }  // End of method 'IonexStore::addMap()'


   void IonexStore ::
   dump( std::ostream& s,
         short detail ) const
   {
      s << "IonexStore dump() function" << std::endl;

      std::vector<std::string> fileNames = getFileNames();
      std::vector<std::string>::const_iterator f;

      for (f = fileNames.begin(); f != fileNames.end(); f++)
      {
         s << *f << std::endl;
      }

      s << std::endl;

      if (detail >= 0)
      {
         s << "Data stored for: " << std::endl;
         s << "  # " << fileNames.size() << " files." << std::endl;
         s << "  # " << inxMaps.size() << " epochs" << std::endl;
         s << "  # " << "over time span "<< getInitialTime()
           << " to " << getFinalTime() << "." << std::endl;

         s << std::endl;

         if (detail == 0)
         {
            return;
         }

         s << "--------------------" << std::endl;
         s << "EPOCH"
           << std::setw(21) << "TEC"
           << std::setw(5) << "RMS"<< std::endl;
         s << "--------------------" << std::endl;

         int ntec(0), nrms(0);

         IonexMap::const_iterator it;

         for (it=inxMaps.begin(); it != inxMaps.end(); it++)
         {
            s << it->first << "   ";

            if ( it->second.count(IonexData::TEC) )
            {
               ntec++;
               s << " YES ";
            }
            else
            {
               s << "   ";
            }

            if ( it->second.count(IonexData::RMS) )
            {
               nrms++;
               s << " YES ";
            }
            else
            {
               s << "     ";
            }

            s << std::endl;
         }  // End of 'for (it=inxMaps.begin(); it != inxMaps.end(); it++)...'

         s << "--------------------" << std::endl;
         s << "Total epochs:        "
           << std::setw(5) << ntec
           << std::setw(5) << nrms << std::endl;
         s << "--------------------" << std::endl;
      }  // End of 'if (detail >= 0)...'
   }  // End of method 'IonexStore::dump()'



      // Remove all data
   void IonexStore ::
   clear()
   {
      inxMaps.clear();

      initialTime = CommonTime::END_OF_TIME;
      finalTime = CommonTime::BEGINNING_OF_TIME;
   }  // End of method 'IonexStore::clear()'



   Triple IonexStore ::
   getIonexValue( const CommonTime& t,
                  const Position& RX,
                  IonexStoreStrategy strategy ) const
   {
         // Here we store the necessary IONEX-extracted values
         // (i.e, TEC, RMS, ionosphere height)
      Triple tecval(0.0,0.0,0.0);

         // current time check
      if (t < getInitialTime())
      {
         InvalidRequest e("Inadequate data before requested time");
         GNSSTK_THROW(e);
      }

      if (t > getFinalTime() )
      {
         InvalidRequest e("Inadequate data after requested time");
         GNSSTK_THROW(e);
      }

         // this never should happen but just in case
      Position pos(RX);
      if ( pos.getSystemName() != "Geocentric" )
      {
         InvalidRequest e("Position object is not in GEOCENTRIC coordinates");
         GNSSTK_THROW(e);
      }

         //let's define the number of maps to be considered
      int nmap;
      switch (strategy)
      {
         case IonexStoreStrategy::Nearest:
         case IonexStoreStrategy::Rotated:
            nmap = 1;
            break;
         case IonexStoreStrategy::Consecutive:
         case IonexStoreStrategy::ConsRot:
            nmap = 2;
            break;
         default:
         {
            InvalidRequest e("Invalid interpolation stategy");
            GNSSTK_THROW(e);
            break;
         }
      }

         // let's look for valid Ionex maps
      CommonTime T[2];
         // iterator
      IonexMap::const_iterator itm = inxMaps.find(t);
      try
      {
         if( itm != inxMaps.end() )              // exact match of t
         {
               // get the current map
            itm = inxMaps.lower_bound(t);
               // store current and next epoch
            T[0] = itm->first;
            T[1] = (++itm)->first;
         }
         else                                   // t is between two maps
         {
               // get the next valid map
            itm = inxMaps.lower_bound(t);
               // store the next and previous epoch
            T[1] = itm->first;
            T[0] = (--itm)->first;
         }  // end of 'if( itm != inxMaps.end() ) ... else ... ''
      }
      catch (...)
      {
         InvalidRequest e("IonexStore::getIonexValue() ... Invalid time!");
         GNSSTK_THROW(e);
      }

         // factors (As in Eq.(3), pag.2 of the manual)
      double f[2];
      f[0] = (T[1]-t   ) / (T[1]-T[0]);
      f[1] = (t   -T[0]) / (T[1]-T[0]);

         // if only one map, then we have to use the neareast
      if( nmap == 1 )
      {
            // closer to the next map
         if( f[1] > f[0] )
         {
            T[0] = T[1];
         }

            // than the factor is unit
         f[0] = 1.0;
      }  // if( nmap == 1 )

         // loop over the number of maps considered
      for(int imap = 0; imap < nmap; imap++)
      {
            // now let's determine if we keep fixed position or
            // take into account the rotation around the Sun
         Position pos;
         if ((strategy == IonexStoreStrategy::Nearest) ||
             (strategy == IonexStoreStrategy::Consecutive))    // fixed position
         {
            pos = RX;
         }
         else                                   // rotate the position
         {
               // seconds of time to degree (360.0 / 86400.0)
            double sec2deg( 4.16666666666667e-3 );

               // count the rotation
            pos = RX;
            pos.theArray[1] = pos.theArray[1] + ( t - T[imap] ) * sec2deg;
         }  // End of 'if (strategy == Nearest || strategy == Consecutive)...'


            // iterator to the current map
         itm = inxMaps.find(T[imap]);

            // map to hold the IONEX types for the current map
         IonexValTypeMap ivtm = (*itm).second;

            // object to hold the corresponding data to the current map
         IonexData iod;

            // Compute TEC value
         if ( ivtm.find(IonexData::TEC) != ivtm.end() )
         {
            iod = ivtm[IonexData::TEC];
            tecval[0] = tecval[0] + f[imap]*iod.getValue(pos);
         }

            // Compute RMS value
         if ( ivtm.find(IonexData::RMS) != ivtm.end() )
         {
            iod = ivtm[IonexData::RMS];
            tecval[1] = tecval[1] + f[imap]*iod.getValue(pos);
         }
      }  // End of 'for(int imap = 0; imap < nmap; imap++)...'

         // ionosphere height in meters
      tecval[2] = RX.theArray[2];

      return tecval;
   }  // End of method 'IonexStore::getIonexValue()'


   double IonexStore ::
   getSTEC( double elevation,
            double tecval,
            const std::string& ionoMapType ) const
   {
      if (tecval < 0)
      {
         InvalidParameter e("Invalid TEC parameter.");
         GNSSTK_THROW(e);
      }

      if( ionoMapType != "NONE" && ionoMapType != "SLM" &&
          ionoMapType != "MSLM" && ionoMapType != "ESM" )
      {
         InvalidParameter e("Invalid ionosphere mapping function.");
         GNSSTK_THROW(e);
      }

      if (elevation < 0.0)
      {
         return 0.0;
      }
      else
      {
         return ( tecval * ionoMappingFunction(elevation, ionoMapType) );
      }
   }  // End of method 'IonexStore::getSTEC()'


   double IonexStore ::
   getIono( double elevation,
            double tecval,
            double freq,
            const std::string& ionoMapType ) const
   {
      if (tecval < 0)
      {
         InvalidParameter e("Invalid TEC parameter.");
         GNSSTK_THROW(e);
      }

      if( ionoMapType != "NONE" && ionoMapType != "SLM" &&
          ionoMapType != "MSLM" && ionoMapType != "ESM" )
      {
         InvalidParameter e("Invalid ionosphere mapping function.");
         GNSSTK_THROW(e);
      }

      if (elevation < 0.0)
      {
         return 0.0;
      }
      else
      {
         return ( C2_FACT / (freq * freq) * tecval
                  * ionoMappingFunction(elevation, ionoMapType) );
      }
   }  // End of method 'IonexStore::getIono()'


   double IonexStore ::
   ionoMappingFunction( double elevation,
                        const std::string& ionoMapType ) const
   {
         // map
      double imap(1.0);
         // Earth's radius in KM
      double Re = 6371.0;
         // zenith angle
      double z0 = 90.0 - elevation;

      if( ionoMapType == "SLM" )
      {
            // As explained in: Hofmann-Wellenhof et al. (2004) - GPS Theory and
            // practice, 5th edition, SpringerWienNewYork, Chapter 6.3, pg. 102

            // ionosphere height in KM
         double ionoHeight = 450.0;
            // zenith angle of the ionospheric pierce point (IPP)
         double sinzipp  = Re / (Re + ionoHeight) * std::sin(z0*DEG_TO_RAD);
         double zipprad  = std::asin(sinzipp);

         imap      = 1.0/std::cos(zipprad);
      }
      else if( ionoMapType == "MSLM" )
      {
            // maximum zenith distance is 80 degrees
         if( z0 <= 80.0 )
         {
               // ionosphere height in KM
            double ionoHeight = 506.7;
            double alfa       = 0.9782;
               // zenith angle of the ionospheric pierce point (IPP)
            double sinzipp = Re / (Re + ionoHeight)
               * std::sin(alfa * z0 * DEG_TO_RAD);
            double zipprad = std::asin(sinzipp);

            imap = 1.0/std::cos(zipprad);
         }
      }
      else if( ionoMapType == "ESM" )
      {
            /// @todo Implement ESM support in IonexStore
      }
      else  // that means ionoMapType == "NONE"
      {
            /// @todo Implement something for the ionoMapType "NONE" case
      }

      return imap;
   }  // End of method 'IonexStore::ionoMappingFunction()'


   double IonexStore ::
   findDCB( const SatID& sat,
            const CommonTime& time ) const
   {
         // current time check. This is passed even if there are gaps
      if ( time < getInitialTime() )
      {
         InvalidRequest e("Inadequate data before requested time");
         GNSSTK_THROW(e);
      }

      if ( time > getFinalTime() )
      {
         InvalidRequest e("Inadequate data after requested time");
         GNSSTK_THROW(e);
      }

      double dt(0.0);
      IonexDCBMap::const_iterator itm = inxDCBMap.begin();

         // looping through the map
      while ( itm != inxDCBMap.end() )
      {
            // let's get the relative reference
         dt = time - itm->first;

            // this means we don't have maps for this day and there is a gap
         if( dt < 0.0 )
         {
            InvalidRequest e( "Inadequate data after requested time: " +
                              time.asString() );
            GNSSTK_THROW(e);
         }  // works fine for consecutive files, otherwise last epoch is thrown
         else
         {
            if( dt < 86400.0 )
            {
               IonexHeader::SatDCBMap satdcb( itm->second );
               IonexHeader::SatDCBMap::const_iterator iprn( satdcb.find(sat) );

                  // if satellite is not found, throw
               if ( iprn == satdcb.end() )
               {
                  InvalidRequest e( "There is no DCB value for satellite " +
                                    asString(sat) );
                  GNSSTK_THROW(e);
               }
               else     // ... otherwise, fetch the value
               {
                  return iprn->second.bias;
               }  // End of 'if ( iprn == satdcb.end() ) ... else ...'
            }
            else  // If everything is fine, then move forward in the map
            {
               itm++;
            }  // End of 'if( dt < 86400.0 ) ... else ...'
         }  // End of 'if( dt < 0.0 ) ... else ...'
      }  // End of 'while ( itm != inxDCBMap.end() )'

         // You should never get here, but just in case
      return 0.0;
   }  // End of method 'IonexStore::findDCB()'

}  // End of namespace gnsstk
