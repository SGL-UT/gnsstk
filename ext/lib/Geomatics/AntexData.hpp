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

/** @file AntexData.hpp
 * Encapsulate data from ANTEX (Antenna Exchange) format files,
 * including both receiver and satellite antennas, ANTEX file I/O,
 * discrimination between different satellite antennas based on
 * system, PRN and time, and computation of phase center offsets and
 * variations. */

#ifndef ANTEX_DATA_HPP
#define ANTEX_DATA_HPP

#include <map>
#include <string>
#include <vector>

#include "AntexBase.hpp"
#include "CivilTime.hpp"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "Triple.hpp"
#include "gnsstk_export.h"

namespace gnsstk
{
   /// @ingroup FileHandling
   //@{

   /** Antex antenna data record: PCOs and PCVs for one antenna.
    * Do not attempt to use an object that is not valid (cf. isValid()).
    *
    * @note Optional data should be accessed only if the
    * corresponding 'valid' string is true; e.g. if(valid &
    * validFromValid) then validFrom may be used.
    *
    * @note In calls to the 'get' routines,
    *        freq = string("G01");
    *        double total_PCO = getTotalPhaseCenterOffset(freq, az, el_nad);
    *        Triple PCO = getPhaseCenterOffset(freq);
    *        double PCV = getPhaseCenterVariation(freq, az, el_nad);
    * receivers and satellites (transmitters) are treated
    * differently, in that receivers call with elevation angle
    * (from North-East plane toward Up) while satellites call with
    * nadir angle (from Z axis - the bore-sight direction).
    *
    * @note The return value of getPhaseCenterOffset is a vector
    * (Triple) PCO, in the appropriate coordinate system (NEU for
    * Rx, XYZ or body for SV), that is defined as the vector from
    * the reference point (RP) to the actual phase center (PC). The
    * RP is the ARP (Antenna Reference Point) for receivers and the
    * COM (Center Of Mass) for satellites.
    *
    * @note The PCV and total_PCO value returned by the other two
    * routines has the same sense as the PCO vector; that is the
    * total offset is defined as
    *   PCO vector - PCV * LOS
    * where LOS is a unit vector along the line of sight (defined
    * by the azimuth and elevation/nadir angle passed into the
    * routines), all in the appropriate coordinate system (receiver
    * NEU or satellite body XYZ).
    *
    * @note Thus when correcting a measured range for the
    * Receiver's total phase center offset one would subtract the
    * total_PCO value (along the line of sight) from the measured
    * range, that is (scalar equation, millimeters):
    *   Range(corr) = Range(meas) + total_PCO; // OR
    *                                -->     -->
    *   Range(corr) = Range(meas) + [PCO dot LOS - PCV];
    * however when correcting the satellite (COM) position for the
    * Satellite's total phase center offset one would add the total
    * vector offset (PCO - PCV*LOS) to the satellite COM (vector)
    * position (in a consistent coordinate system, e.g. ECEF XYZ),
    * that is (vector equation):
    *   -->        -->        -->         -->
    *   SV(corr) = SV(COM) + [PCO - PCV * LOS]
    *
    * @note the PCV data is stored in a map <zenith angle, value>
    * and the getPhaseCenterVariation() routine simply interpolates
    * this map WITHOUT changing the sign of the value - it is the
    * same as that in the ANTEX file.
    *
    * @sa gnsstk::AntexStream and gnsstk::AntexHeader.
    */
   class AntexData : public AntexBase
   {
   public:
         /** @name AntexDataFormatStrings
          * ANTEX Data Formatting Strings */
         //@{
      GNSSTK_EXPORT
      static const std::string startAntennaString;    ///< "START OF ANTENNA"
      GNSSTK_EXPORT
      static const std::string typeSerNumString;      ///< "TYPE / SERIAL NO"
      GNSSTK_EXPORT
      static const std::string methodString;          ///< "METH / BY / # / DATE"
      GNSSTK_EXPORT
      static const std::string daziString;            ///< "DAZI"
      GNSSTK_EXPORT
      static const std::string zenithString;          ///< "ZEN1 / ZEN2 / DZEN"
      GNSSTK_EXPORT
      static const std::string numFreqString;         ///< "# OF FREQUENCIES"
      GNSSTK_EXPORT
      static const std::string validFromString;       ///< "VALID FROM"
      GNSSTK_EXPORT
      static const std::string validUntilString;      ///< "VALID UNTIL"
      GNSSTK_EXPORT
      static const std::string sinexCodeString;       ///< "SINEX CODE"
      GNSSTK_EXPORT
      static const std::string dataCommentString;     ///< "COMMENT"
      GNSSTK_EXPORT
      static const std::string startFreqString;       ///< "START OF FREQUENCY"
      GNSSTK_EXPORT
      static const std::string neuFreqString;         ///< "NORTH / EAST / UP"
      GNSSTK_EXPORT
      static const std::string endOfFreqString;       ///< "END OF FREQUENCY"
      GNSSTK_EXPORT
      static const std::string startFreqRMSString;    ///< "START OF FREQ RMS"
      GNSSTK_EXPORT
      static const std::string neuFreqRMSString;      ///< "NORTH / EAST / UP"
      GNSSTK_EXPORT
      static const std::string endOfFreqRMSString;    ///< "END OF FREQ RMS"
      GNSSTK_EXPORT
      static const std::string endOfAntennaString;    ///< "END OF ANTENNA"
         //@}

         /** Validity bits for the ANTEX Data
          * @note if version is updated, add allValid<ver> and update isValid()
          */
      enum validBits
      {
         startAntennaValid = 0x00001, ///< "START OF ANTENNA"       Required
         typeSerNumValid   = 0x00002, ///< "TYPE / SERIAL NO"       Required
         methodValid       = 0x00004, ///< "METH / BY / # / DATE"   Required
         daziValid         = 0x00008, ///< "DAZI"                   Required
         zenithValid       = 0x00010, ///< "ZEN1 / ZEN2 / DZEN"     Required
         numFreqValid      = 0x00020, ///< "# OF FREQUENCIES"       Required
         validFromValid    = 0x00040, ///< "VALID FROM"
         validUntilValid   = 0x00080, ///< "VALID UNTIL"
         sinexCodeValid    = 0x00100, ///< "SINEX CODE"
         dataCommentValid  = 0x00200, ///< "COMMENT"
         startFreqValid    = 0x00400, ///< "START OF FREQUENCY"     Required
         neuFreqValid      = 0x00800, ///< "NORTH / EAST / UP"      Required
         endOfFreqValid    = 0x01000, ///< "END OF FREQUENCY"       Required
         startFreqRMSValid = 0x02000, ///< "START OF FREQ RMS"
         neuFreqRMSValid   = 0x04000, ///< "NORTH / EAST / UP"
         endOfFreqRMSValid = 0x08000, ///< "END OF FREQ RMS"
         endOfAntennaValid = 0x10000, ///< "END OF ANTENNA"         Required
         allValid13        = 0x11C3F  ///< mask for all required valid fields
      };

         /** Values of 'type' that are satellites
          * @note keep this updated from the IGS file 'rcvr_ant.tab' */
      GNSSTK_EXPORT static const std::vector<std::string> SatelliteTypes;

      /// map from zenith angle (degrees) to PC offset (millimeters)
      typedef std::map<double, double> zenOffsetMap;

      /** map from azimuth angle (deg) to zenOffsetMap
       * the zenOffsetMap WITHOUT azimuth dependence (NOAZI) will be
       * azimZenMap[-1.0] (this may be the only entry) */
      typedef std::map<double, zenOffsetMap> azimZenMap;

      /** class encapsulating the PCOs and PCVs of the antenna. See
       * the ANTEX documentation for discussion of how the PCO/Vs
       * are defined, sign conventions and how to apply the
       * PCOs. */
      class antennaPCOandPCVData
      {
       public:
         /** nominal phase center offsets in mm, and RMS values,
          * in NEU coordinates (for Receiver antennas)
          * or XYZ (for Satellite antennas); from "NORTH / EAST / UP" record
          * RMS values are OPTIONAL */
         double PCOvalue[3], PCOrms[3];

         /** if false, there is no azimuth dependence in the PCVs
          * and only PCV[0.0] is defined. */
         bool hasAzimuth;

         /** map from azimuth to <zenith,offset> map:
          * PCVvalues[azim][zen] = offset in mm from the nominal
          * PCVrms[azim][zen] = RMS of these values, also in mm.
          * if there is no azimuth dependence, there will be
          * only one entry in this map, with azimuth = -1.0
          * RMS values are OPTIONAL */
         azimZenMap PCVvalue, PCVrms;

      }; // end of class antennaPCOandPCVData

      // member data

      /** Bits of valid are set when corresponding labels are found
       * and data defined */
      unsigned long valid;

      /** if true, PCOs are absolute, else they are relative to
       * another antenna */
      bool absolute;

      /** if true, this is a receiver antenna, otherwise its a
       * satellite; this flag is set based on the IGS codes kept in
       * array SatelliteTypes.
       * @note this flag need not be used, if you know which
       *     antenna you have; however if used, the array
       *     SatelliteTypes must be kept updated. */
      bool isRxAntenna;

      /** PRN and SVN numbers; used only in the case of satellite
       * antennas, and may not be present, in which case these are
       * both -1.
       * @note PRNs apply to GLONASS as well as GPS */
      int PRN, SVN;

      /** system character: G or blank GPS, R GLONASS, E GALILEO, etc
       * used only in the case of satellite antennas */
      char systemChar;

      /** number of frequencies stored, equal to number of keys in map
       * from "# OF FREQUENCIES" record */
      unsigned int nFreq;

      /** delta azimuth (degrees) stored in azimZenMap
       * equal to 0 if there is no azimuth dependence
       * from "DAZI" record */
      double azimDelta;

      /** minimum, maximum and delta zenith (degrees) stored in zenOffsetMap
       * from "ZEN1 / ZEN2 / DZEN" record */
      double zenRange[3];

      /** Time limits of validity (OPTIONAL); otherwise set to
       * BEGINNING and END.
       * From "VALID FROM" and "VALID UNTIL" records.
       * Keep the string version for file I/O b/c sometimes the
       * time is of the form:
       * 1994     4    17    23    59   59.9999999                 VALID UNTIL
       * and converting this to CommonTime replaces it with ... 24 0 0.000 */
      CommonTime validFrom, validUntil;
      std::string stringValidFrom, stringValidUntil;

      /// map from frequency to antennaPCOandPCVData
      std::map<std::string, antennaPCOandPCVData> freqPCVmap;

      std::string type;     ///< antenna type from "TYPE / SERIAL NO"
      std::string serialNo; ///< antenna serial number from "TYPE / SERIAL NO"
      std::string satCode;  ///< satellite code from "TYPE / SERIAL NO"
      std::string cospar;   ///< satellite COSPAR ID from "TYPE / SERIAL NO"
      std::string method;   ///< calibration method from "METH / BY / # / DATE"
      std::string agency;   ///< agency from "METH / BY / # / DATE"
      int noAntCalibrated;  ///< num. of ant. calibrated from "METH / BY / # /
                            ///< DATE"
      std::string date;     ///< date from "METH / BY / # / DATE"
      std::string
         sinexCode;         ///< name of ant. cal. model from "SINEX CODE" OPTIONAL

      /// comments found in the data portion of the file
      std::vector<std::string> commentList;

      // member functions

      /// Constructor.
      AntexData()
         : valid(0), absolute(true), PRN(0), SVN(0), nFreq(0),
           validFrom(CommonTime::BEGINNING_OF_TIME),
           validUntil(CommonTime::END_OF_TIME)
      {
      }
      /// Destructor
      virtual ~AntexData() {}

      /// AntexData is a "data", so this function always returns true.
      virtual bool isData() const { return true; }

      /// Convenience function returns true only if a valid object
      bool isValid() const { return ((valid & allValid13) == allValid13); }

      /** @return true if the antenna object is valid at the given time.
       * Base on the 'validFrom' and 'validUntil' fields.
       * @return true if the input time is either BEGINNING_ or END_OF_TIME
       * @return true if the 'valid' time limits are not given.
       * @note useful when adding satellite antennas for processing
       *   with a dataset; pass any time tag from the dataset. */
      bool isValid(CommonTime& time) const;

      /// Generate a name from type and serial number
      std::string name() const;

      /** Compute the total phase center offset at the given
       * azimuth and elev_nadir, including both nominal offset
       * (PCO) and variation (PCV).
       * @note see documentation of the class for coordinates,
       *   signs and application.
       * @param freq frequency e.g. G01
       * @param azimuth the azimuth angle in degrees, from N going
       *        toward E for receivers, or from X going toward Y
       *        for satellites
       * @param elevation elevation in deg from horizontal
       *         (North-East) plane for receivers, or nadir angle
       *         in degrees from Z axis for satellites
       * @return total phase center offset in millimeters
       * @throw Exception if this object is invalid
       *         if frequency does not exist for this data
       *         if azimuth is out of range; azimuth is replaced
       *         with azim mod 360 */
      double getTotalPhaseCenterOffset(const std::string& freq,
                                       double azimuth,
                                       double elevation) const;

      /** Get the PC offset values in mm (only, NOT the phase
       * center variations, which should be computed using
       * getPhaseCenterVariations() and added to the PCOs to get
       * the total phase center offset).
       * @note see documentation of the class for coordinates,
       *   signs and application.
       * @param freq frequency (usually G01 or G02)
       * @return Triple containing offsets in millimeters, in
       *                appropriate coordinate system
       *                (satellite-based XYZ or receiver-based
       *                NEU).
       * @throw Exception if this object is invalid
       *         if frequency does not exist for this data */
      Triple getPhaseCenterOffset(const std::string& freq) const;

      /** Compute the phase center variation at the given azimuth
       * and elev_nadir
       * @note see documentation of the class for coordinates,
       *   signs and application.
       * @param freq frequency (usually G01 or G02)
       * @param azimuth the azimuth angle in degrees, from N going
       *        toward E for receivers, or from X going toward Y
       *        for satellites
       * @param elev_nadir elevation in deg from horizontal
       *         (North-East) plane for receivers, or nadir angle
       *         in degrees from Z axis for satellites
       * @return phase center offset in millimeters
       * @throw Exception if this object is invalid
       *         if frequency does not exist for this data
       *         if azimuth is out of range, azimuth is replaced
       *         with azim % 360 */
      double getPhaseCenterVariation(const std::string& freq,
                                     double azimuth,
                                     double elev_nadir) const;

      /** Dump AntexData. Set detail = 0 for type, serial no., sat
       * codes only.
       * @param[in] detail 1 for all information except phase
       *   center offsets, 2 for all data. */
      virtual void dump(std::ostream& s, int detail = 0) const;

   protected:
      /** Find zenith angles bracketing the input zenith angle
       * within the given map, and the corresponding PCOs. */
      void evaluateZenithMap(double zen, const zenOffsetMap& eomap,
                             double& zen_lo, double& zen_hi, double& pco_lo,
                             double& pco_hi) const;

      /** Writes a correctly formatted record from this data to stream \a s.
       * @throw std::exception
       * @throw FFStreamError
       * @throw StringUtils::StringException
       */
      virtual void reallyPutRecord(FFStream& s) const;

      /** This functions obtains Antex antenna record from the
       * given FFStream.  If there is an error in reading from the
       * stream, it is reset to its original position and its
       * fail-bit is set.
       * @throw std::exception
       * @throw StringException when a StringUtils function fails
       * @throw FFStreamError when exceptions(failbit) is set and
       *  a read or formatting error occurs.  This also resets the
       *  stream to its pre-read position. */
      virtual void reallyGetRecord(FFStream& s);

   private:
      /** helper routine to throw when records are out of order
       * throws if valid contains test (test & valid), otherwise
       * does nothing */
      void throwRecordOutOfOrder(unsigned long test, std::string& label);

      /** parse a line from the Antex file, filling the data object
       * @throw FFStreamError
       */
      void ParseDataRecord(std::string& line);

      /** Writes the CommonTime object into Antex ('VALID FROM') format.
       * If it's a bad time, it will return blanks.
       * @throw StringUtils::StringException */
      std::string writeTime(const CommonTime& dt) const;

      /** This function constructs a CommonTime object from the
       * line for VALID FROM and VALID UNTIL records; default is to
       * return BEGINNING_OF_TIME.
       * @param line the encoded time string found in the Antex record.
       * @throw FFStreamError */
      CommonTime parseTime(const std::string& line) const;

   }; // class AntexData

   //@}

} // namespace gnsstk

#endif
