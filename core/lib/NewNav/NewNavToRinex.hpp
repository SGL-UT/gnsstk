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

#ifndef GNSSTK_NEWNAVTORINEX_HPP
#define GNSSTK_NEWNAVTORINEX_HPP

#include "NavLibrary.hpp"
#include "NavData.hpp"
#include "OrbitDataKepler.hpp"
#include "StdNavTimeOffset.hpp"
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** HealthGetter is a callback class used by NewNavToRinex for
       * obtaining health information when converting to RINEX that is
       * otherwise unavailable in the ephemeris. */
   class HealthGetter
   {
   public:
         /** Get the health status of a satellite at a specific time.
          * @param[in] nsid Satellite to get the health status for.
          * @param[in] when The time that the health should be retrieved.
          * @param[out] healthOut The health status at when.
          * @return true if successful, false if no nav data was found
          *   containing health status. */
      virtual bool getHealth(const NavSatelliteID& nsid, const CommonTime& when,
                             NavDataPtr& healthOut);
      NavSatMap healthMap; ///< Contains the health data to look up.
   };

      /** HealthGetter callback for using NavLibrary as the source for health.
       * @note The extra options to NavLibrary::getHealth() are data
       *   fields in this class.  These can be set by the user, but
       *   are intended to default to the same defaults that
       *   NavLibrary::getHealth() uses.
       * @note the healthMap object in the parent class is unused in
       *   this context. */
   class NavLibraryHealthGetter : public HealthGetter
   {
   public:
         /** Set NavLibrary::getHealth() parameters to defaults, and
         * initialize navLib reference. 
         * @param[in] nl The NavLibrary object to use to look up health. */
      NavLibraryHealthGetter(NavLibrary& nl);
         /// @copydoc HealthGetter::getHealth
      bool getHealth(const NavSatelliteID& nsid, const CommonTime& when,
                     NavDataPtr& healthOut) override;
      NavLibrary& navLib;    ///< NavLibrary to use for health look-up.
      SVHealth xmitHealth;   ///< Parameter to NavLibrary::getHealth()
      NavValidityType valid; ///< Parameter to NavLibrary::getHealth()
      NavSearchOrder order;  ///< Parameter to NavLibrary::getHealth()
   };

      /** Implements the capability of translating NewNav data into
       * RINEX NAV data.
       * The numerous static methods may be used as needed, but the
       * typical use case will likely be:
       * \li Construct a NewNavToRinex object
       * \li Fill the header object contents (version, fileProgram, etc.)
       * \li call NewNavToRinex.translate()
       * \li call NewNavToRinex.write()
       */
   class NewNavToRinex
   {
   public:
         /** Given an ordered list of NavData, generate RINEX
          * equivalents and store them internally.
          * @param[in] data The NavData to process and turn into RINEX.
          * @param[in] healthGet Callback for obtaining health
          *   information, used to fill in the blanks in RINEX.
          *   Currently this is only used for Galileo ephemerides
          *   where F/NAV doesn't contain I/NAV health and vice versa.
          * @return true if any nav data was converted successfully. */
      bool translate(const NavDataPtrList& data, HealthGetter& healthGet);

         /** Write the internally stored RINEX header and data to
          * the specified output file name. 
          * @param[in] filename The path to the RINEX NAV file to write.
          * @return true on success, false on failure. */
      bool write(const std::string& filename);

         /** Fill the contents of the given RINEX nav header with data
          * from ndp.  This method may (should) be called multiple
          * times for a given header object in order to get all of the
          * relevant time offset and ionospheric model data (no other
          * header data is affected).
          * @param[in] ndp The NavDataPtr to get data from.
          * @param[out] h The header object to add time offset or iono data to.
          * @return true if successful, false if ndp contained no relevant data.
          */
      static bool fillHeader(const NavDataPtr& ndp, Rinex3NavHeader& h);

         /** Fill the contents of the given RINEX nav data object with
          * data from ndp.  This method need only be called once for a
          * given NavDataPtr object. 
          * @param[in] ndp A NavDataPtr containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @param[in] healthGet Callback for obtaining health
          *   information, used to fill in the blanks in RINEX.
          * @return true on success, false on failure. */
      static bool fillData(const NavDataPtr& ndp, Rinex3NavData& rnd,
                           HealthGetter& healthGet);

         /** Fill the contents of the given RINEX nav data object with
          * data from ndp, which is expected to contain a GPS LNAV
          * ephemeris.
          * @param[in] ndp A NavDataPtr containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @return true on success, false on failure (i.e. ndp is not
          *   a GPS LNAV ephemeris. */
      static bool fillDataGPSLNav(const NavDataPtr& ndp, Rinex3NavData& rnd);

         /** Fill the contents of the given RINEX nav data object with
          * data from ndp, which is expected to contain a Galileo F/Nav
          * ephemeris.
          * @param[in] ndp A NavDataPtr containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @param[in] healthGet Callback for obtaining health
          *   information, used to fill in the blanks in RINEX.
          * @return true on success, false on failure (i.e. ndp is not
          *   a Galileo F/Nav ephemeris. */
      static bool fillDataGalFNav(const NavDataPtr& ndp, Rinex3NavData& rnd,
                                  HealthGetter& healthGet);

         /** Fill the contents of the given RINEX nav data object with
          * data from ndp, which is expected to contain a Galileo I/Nav
          * ephemeris.
          * @param[in] ndp A NavDataPtr containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @param[in] healthGet Callback for obtaining health
          *   information, used to fill in the blanks in RINEX.
          * @return true on success, false on failure (i.e. ndp is not
          *   a Galileo I/Nav ephemeris. */
      static bool fillDataGalINav(const NavDataPtr& ndp, Rinex3NavData& rnd,
                                  HealthGetter& healthGet);

         /** Fill the contents of the given RINEX nav data object with
          * data from ndp, which is expected to contain a BeiDou D1
          * ephemeris.
          * @param[in] ndp A NavDataPtr containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @return true on success, false on failure (i.e. ndp is not
          *   a BeiDou D1 ephemeris. */
      static bool fillDataBDSD1Nav(const NavDataPtr& ndp, Rinex3NavData& rnd);

         /** Fill the contents of the given RINEX nav data object with
          * data from ndp, which is expected to contain a BeiDou D2
          * ephemeris.
          * @param[in] ndp A NavDataPtr containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @return true on success, false on failure (i.e. ndp is not
          *   a BeiDou D2 ephemeris. */
      static bool fillDataBDSD2Nav(const NavDataPtr& ndp, Rinex3NavData& rnd);

         /** Fill the contents of the given RINEX nav data object with
          * data from ndp, which is expected to contain a GLONASS FDMA
          * ephemeris.
          * @param[in] ndp A NavDataPtr containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @return true on success, false on failure (i.e. ndp is not
          *   a GLONASS FDMA ephemeris. */
      static bool fillDataGLOFNav(const NavDataPtr& ndp, Rinex3NavData& rnd);

         /** Fill the contents of the given RINEX nav data object with
          * data from odk.  This handles all the common Keplerian
          * orbit parameters across those systems that use them.
          * @param[in] odk An OrbitDataKepler object containing an ephemeris.
          * @param[out] rnd The resulting converted Rinex3NavData object.
          * @return true on success, false on failure (i.e. ndp is not
          *   a Keplerian ephemeris. */
      static bool fillDataKepler(const std::shared_ptr<OrbitDataKepler>& odk,
                                 Rinex3NavData& rnd);

         /** Fill the contents of the given TimeSystemCorrection from
          * a StdNavTimeOffset object. 
          * @param[in] toffs The time offset data to convert.
          * @param[out] tsc The converted TimeSystemCorrection.
          * @return true if successful, false on failure (e.g. the
          *   time system from/to pair can't be represented in
          *   RINEX). */
      static bool fillTimeSystemCorrection(
         const std::shared_ptr<StdNavTimeOffset>& toffs,
         TimeSystemCorrection& tsc);

         /// The header that will be used by translate() and write().
      Rinex3NavHeader header;
         /// The data created by translate() to be written.
      std::list<Rinex3NavData> rndList;
   };

      //@}

}

#endif // GNSSTK_NEWNAVTORINEX_HPP
