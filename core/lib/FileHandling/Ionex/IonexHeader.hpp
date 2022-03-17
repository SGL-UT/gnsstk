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
 * @file IonexHeader.hpp
 * This class encapsulates the header of Ionex file, including I/O
 */

#ifndef GNSSTK_IONEXHEADER_HPP
#define GNSSTK_IONEXHEADER_HPP

#include <string>
#include <vector>
#include <map>

#include "CommonTime.hpp"
#include "SatID.hpp"
#include "IonexBase.hpp"

namespace gnsstk
{


      /// @ingroup FileHandling
      //@{


      /**
       * This class models the header for a IONEX file
       *
       * @sa gnsstk::IonexData and IonexStream
       * @sa main_ionex_test.cpp for an example
       */
   class IonexHeader : public IonexBase
   {
   public:

         /// Default constructor
      IonexHeader();


         /// Clear (empty out) header
      void clear();


         /**
          * @name IonexHeaderFormatStrings
          * IONEX Header Formatting Strings
          */
         //@{
      static const std::string versionString;         ///< "IONEXVERSION / TYPE"
      static const std::string runByString;           ///< "PGM / RUN BY / DATE"
      static const std::string descriptionString;     ///< "DESCRIPTION"
      static const std::string commentString;         ///< "COMMENT"
      static const std::string firstTimeString;       ///< "EPOCH OF FIRST MAP"
      static const std::string lastTimeString;        ///< "EPOCH OF LAST MAP"
      static const std::string intervalString;        ///< "INTERVAL"
      static const std::string numMapsString;         ///< "# OF MAPS IN FILE"
      static const std::string mappingFunctionString; ///< "MAPPING FUNCTION"
      static const std::string elevationString;       ///< "ELEVATION CUTOFF"
      static const std::string observablesUsedString; ///< "OBSERVABLES USED"
      static const std::string numStationsString;     ///< "# OF STATIONS"
      static const std::string numSatsString;         ///< "# OF SATELLITES"
      static const std::string baseRadiusString;      ///< "BASE RADIUS"
      static const std::string mapDimensionString;    ///< "MAP DIMENSION"
      static const std::string hgtGridString;         ///< "HGT1 / HGT2 / DHGT"
      static const std::string latGridString;         ///< "LAT1 / LAT2 / DLAT"
      static const std::string lonGridString;         ///< "LON1 / LON2 / DLON"
      static const std::string exponentString;        ///< "EXPONENT"
      static const std::string startAuxDataString;    ///< "START OF AUX DATA"
      static const std::string endAuxDataString;      ///< "END OF AUX DATA"
      static const std::string endOfHeader;           ///< "END OF HEADER"
         //@}

         // Differential Code Bias structure
      struct DCB
      {
            // If isPRN is true, system and prn are set, otherwise
            // name and number are set.  bias and rms should always be
            // set.
         char system;   ///< one char indicating the system for this satellite
                        ///< (i.e., 'U' for unknown, 'G' or blank for GPS and
                        ///< 'R' for GLONASS)
         int prn;       ///< 2-digit satellite identifier (pseudo random number)
         double bias;   ///< differential (L1-L2) code bias in nanoseconds
         double rms;    ///< RMS error of DCB in nanoseconds
         bool isPRN;    ///< True if a PRN / BIAS / RMS block, false if STATION
         std::string name;   ///< Marker name (4 characters)
         std::string number; ///< Marker number (15 characters)

            /// Default constructor. Defines and invalid structure.
         DCB();

            /** Constructor for PRN data
             *
             * @param[in] s    System type. ('U' for unknown, 'G' or blank for
             *                 GPS, and 'R' for GLONASS).
             * @param[in] p    Satellite PRN (2-digit integer).
             * @param[in] b    Differential (L1-L2) code bias.
             * @param[in] r    RMS error of DCB, in nanoseconds.
             */
         DCB(char s, int p, double b, double r);

            /** Constructor for STATION data
             *
             * @param[in] s    4-character marker name.
             * @param[in] n    15-character (max) marker number.
             * @param[in] b    Differential (L1-L2) code bias.
             * @param[in] r    RMS error of DCB, in nanoseconds.
             */
         DCB(const std::string& s, const std::string& n, double b, double r);

            /**
             * @name DCBformatStrings
             * Differential Code Bias Formatting Strings
             */
            //@{
         static const std::string svsAuxDataString;      ///< "PRN / BIAS / RMS"
         static const std::string stationsAuxDataString; ///< "STATION/BIAS/RMS"
            //@}

            /// convert DCB structure to a string
         std::string toString() const;
      }; // End of 'DCB' data structure


         /**
          * @name IonexHeaderValues
          */
         //@{
      double version;            ///< IONEX version

      std::string fileType;      ///< IONEX filetype ('I' for Ionoshere Maps)
      std::string system;        ///< Satellite system or theoretical model
      std::string fileProgram;   ///< Name of program creating this file
      std::string fileAgency;    ///< Name of agency creating this file
      std::string date;          ///< Date and time of file creation

      std::vector<std::string> descriptionList;///< Descriptions in header (opt)
      std::vector<std::string> commentList;    ///< Comments in header(optional)

      CommonTime firstEpoch;           ///< Epoch of first map
      CommonTime lastEpoch;            ///< Epoch of last map

      int interval;                 ///< Time interval between maps (seconds)
      size_t numMaps;               ///< Total number of TEC/RMS/HGT maps
      std::string mappingFunction;  ///< Mapping function adopted
      double elevation;             ///< Minimum elevation angle, in degrees
      std::string observablesUsed;  ///< One-line specification of used obs.

      size_t numStations;       ///< Number of contributing stations (optional)
      size_t numSVs;            ///< Number of contributing satellites (opt)

      double baseRadius;  ///< Mean earth radius, or bottom of height grid (km)
      size_t mapDims;     ///< Dimension of maps (2 or 3)

      double hgt[3];    ///< Definition of an equidistant grid in height.
                        /// 'hgt[0]' to 'hgt[1]' with increment 'hgt[2]' in km.
                        /// For 2-dimensional maps hgt[0]=hgt[1] and hgt[2]=0.
      double lat[3];    ///< Definition of the grid in latitude.
                        /// 'lat[0]' to 'lat[1]' with increment 'hgt[2]' in deg.
      double lon[3];    ///< Definition of the grid in longitude.
                        /// 'lon[0]' to 'lon[1]' with increment 'hon[2]' in deg.

      int exponent;    ///< Exponent defining the unit of the values (optional)
      std::string auxData;          ///< Type of auxiliar data (optional)

         /// The key to this map is the svid of the satellite (usually the prn)
      typedef std::map<SatID,DCB> SatDCBMap;
         /// The key to this map is the marker name of the station
      typedef std::map<std::string,DCB> StaDCBMap;

      SatDCBMap svsmap;    ///< Map of satellites' DCBs (in nanoseconds)
      StaDCBMap stamap;    ///< Map of stations' DCBs (in nanoseconds)
      bool auxDataFlag;    ///< Flag to monitor the sequence of auxiliar data

         /// return code, Am I valid?
      bool valid;
         //@}

         /// Destructor
      virtual ~IonexHeader();


         // IonexHeader is a "header" so this function always returns true.
      virtual bool isHeader() const
      { return true; }


         /** Simple debug output function.
          *
          * It simply outputs the version, name and number of maps contained
          * in this Ionex header.
          */
      virtual void dump(std::ostream& s = std::cout) const;


         /**
          * Parse a single auxiliary header record that contains "Differential
          * code biases".
          * @throw FFStreamError
          */
      void parseDcbRecord(const std::string& line);


         /** Parse a single header record, and modify 'valid' accordingly.
          *
          * Used by reallyGetRecord for both IonexHeader and IonexData.
          * @throw FFStreamError
          */
      void parseHeaderRecord(const std::string& line);


         /**
          * Write all valid header records to the given stream.
          * Used by reallyPutRecord for both IonexHeader and IonexData.
          * @throw FFStreamError
          * @throw StringUtils::StringException
          */
      void writeHeaderRecords(FFStream& s) const;



   protected:


         /** Writes the record formatted to the FFStream \a s.
          *
          * @throw std::exception
          * @throw FFStreamError
          * @throw StringException when a StringUtils function fails
          */
      void reallyPutRecord(FFStream& s) const override;


         /** This function retrieves the IONEX header from the given FFStream.
          *
          * If an stream error is encountered, the stream is reset to its
          * original position and its fail-bit is set.
          *
          * @throw std::exception
          * @throw StringException when a StringUtilis function fails
          * @throw FFStreamError when exceptions (failbit) is set and
          *  a read or formatting error occurs. This also resets the stream
          *  to its pre-read position.
          */
      void reallyGetRecord(FFStream& s) override;

   private:
         /** Converts the CommonTime \a dt into a Ionex Obs time
          * string for the header
          */
      std::string writeTime(const CommonTime& dt) const;


         /** This function sets the time for this header.
          *
          * It looks at \a line to obtain the needed information.
          */
      CommonTime parseTime(const std::string& line) const;
   }; // End of class 'IonexHeader'

      //@}

}  // End of namespace gnsstk
#endif   // GNSSTK_IONEXHEADER_HPP
