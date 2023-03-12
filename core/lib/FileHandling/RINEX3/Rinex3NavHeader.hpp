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

/// @file Rinex3NavHeader.hpp
/// Encapsulate header of RINEX 3 navigation file, including RINEX 2
/// compatibility.

#ifndef GNSSTK_RINEX3NAVHEADER_HPP
#define GNSSTK_RINEX3NAVHEADER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "StringUtils.hpp"
#include "FFStream.hpp"
#include "Rinex3NavBase.hpp"
#include "RinexSatID.hpp"
#include "TimeSystemCorr.hpp"
#include "RNDouble.hpp"
#include "gnsstk_export.h"

namespace gnsstk
{

      /// @ingroup FileHandling
      //@{

      /// Ionospheric Corrections
   class IonoCorr
   {
   public:
         /// Supported ionospheric correction types
      enum CorrType
      {
         Unknown, ///< A default value
         GAL,     ///< Galileo
         GPSA,    ///< GPS alpha
         GPSB,    ///< GPS beta
         QZSA,    ///< QZSS alpha
         QZSB,    ///< QZSS beta
         BDSA,    ///< BeiDou alpha
         BDSB,    ///< BeiDou beta
         IRNA,    ///< IRNSS alpha
         IRNB     ///< IRNSS beta
      };

         /// Set data members to default values
      IonoCorr();

         /** Set type from RINEX correction type string.
          * param values are set to zero. */
      IonoCorr(std::string str);

         /// Return string version of CorrType
      std::string asString() const noexcept;

         /** Set type value from RINEX correction type string.
          * @throw Exception
          */
      void fromString(const std::string str);

         /// Equality test
      bool operator==(const IonoCorr& ic) const;

         /// Ordering operator
      bool operator<(const IonoCorr& ic) const;

         // Member data
      CorrType type;   ///< type of correction - enum CorrType
      FormattedDouble param[4]; ///< parameters ai0-ai2,0(GAL), alpha0-3 or beta0-3(GPS)
   }; // End of class 'IonoCorr'

      /// This class models the RINEX 3 Nav header for a RINEX 3 Nav file.
      /// \sa Rinex3NavData and Rinex3NavStream classes.
   class Rinex3NavHeader : public Rinex3NavBase
   {
   public:

         //// Public member functions

         /// Constructor
      Rinex3NavHeader()
         : valid(0), version(3.04),
           leapSeconds(0), leapDelta(0), leapWeek(0),leapDay(0)
      {}

         /// Destructor
      ~Rinex3NavHeader() override
      {}

         /// Rinex3NavHeader is a "header" so this function always returns true.
      bool isHeader() const override
      { return true; }

         /// This function dumps the contents of the header.
      void dump(std::ostream& s) const override;

         /** Change the file system, keeping fileType, fileSys, and fileSysSat
          * consistent.
          * @param[in] str beginning with system character or "M" for
          *   mixed
          * @throw Exception */
      void setFileSystem(const std::string& str);

         /** Compare this header with another.
          * @param[in] right the header to compare this with.
          * @param[out] diffs The header strings/identifiers that are
          *   different between this and right.
          * @param[in] inclExclList a list of header strings to be ignored
          *   or used when making the comparison (e.g. "RINEX VERSION / TYPE").
          * @param[in] incl When true, only header lines listed in
          *   inclExclList will be compared.  When false, differences in
          *   header lines in inclExclList will be ignored. */
      bool compare(const Rinex3NavHeader& right,
                   std::vector<std::string>& diffs,
                   const std::vector<std::string>& inclExclList,
                   bool incl = false);

         //// Member data
         /// All 'valid..' bits found in this header
      unsigned long valid;

         /// These are validity bits used in checking the RINEX NAV header.
      enum validBits
      {
         validVersion     = 0x01,   ///< Set if RINEX version is valid.
         validRunBy       = 0x02,   ///< Set if Run-by value is valid.
         validComment     = 0x04,   ///< Set if Comments are valid
         validIonoCorr    = 0x08,   ///< Set if Iono Correction data is valid.
         validTimeSysCorr = 0x020,  ///< Set if Time System Correction is valid.
         validLeapSeconds = 0x040,  ///< Set if the Leap Seconds value is valid.
         validEoH         = 0x080000000,  ///< Set if the End of Header is valid.

            /// This bitset checks that all required header items are available
            /// for a Rinex (2 or 3) version file - only Version, RunBy, EOH
            /// are required.
         allValid3 = 0x080000003,
            // the only changes 3->3.01 in optional rec. (Leap) allValid301 =
            // 0x080000003,
         allValid2 = 0x080000003
      };



      double version;                ///< RINEX Version
      std::string fileType;          ///< File type "N...."
      std::string fileSys;           ///< File system string
      SatID fileSysSat;              ///< File system as a SatID
      std::string fileProgram;       ///< Program string
      std::string fileAgency;        ///< Agency string
      std::string date;              ///< Date string; includes "UTC" at the end
      std::vector<std::string> commentList;  ///< Comment list
         /// map of label: GAUT, GPUT, etc, and TimeCorr
      std::map<std::string,TimeSystemCorrection> mapTimeCorr;
         /// map of label : GAL, GPSA or GPSB, and IONO CORRs
      std::map<IonoCorr::CorrType, IonoCorr> mapIonoCorr;
      long leapSeconds;              ///< Leap seconds
      long leapDelta;                ///< Change in Leap seconds at ref time
      long leapWeek;                 ///< Week number of ref time
      long leapDay;                  ///< Day of week of ref time

      GNSSTK_EXPORT
      static const std::string stringVersion;      // "RINEX VERSION / TYPE"
      GNSSTK_EXPORT
      static const std::string stringRunBy;        // "PGM / RUN BY / DATE"
      GNSSTK_EXPORT
      static const std::string stringComment;      // "COMMENT"
         // R3.x
      GNSSTK_EXPORT
      static const std::string stringIonoCorr;     // "IONOSPHERIC CORR"
      GNSSTK_EXPORT
      static const std::string stringTimeSysCorr;  // "TIME SYSTEM CORR"
      GNSSTK_EXPORT
      static const std::string stringLeapSeconds;  // "LEAP SECONDS"
      GNSSTK_EXPORT
      static const std::string stringDeltaUTC;     // "DELTA-UTC: A0,A1,T,W" // R2.11 GPS
      GNSSTK_EXPORT
      static const std::string stringCorrSysTime;  // "CORR TO SYSTEM TIME"  // R2.10 GLO
      GNSSTK_EXPORT
      static const std::string stringDUTC;         // "D-UTC A0,A1,T,W,S,U"  // R2.11 GEO
      GNSSTK_EXPORT
      static const std::string stringIonAlpha;     // "ION ALPHA"            // R2.11
      GNSSTK_EXPORT
      static const std::string stringIonBeta;      // "ION BETA"             // R2.11
      GNSSTK_EXPORT
      static const std::string stringEoH;          // "END OF HEADER"


   protected:


         /** Protected member functions
          * Write this header to stream \a s.
          * @throw std::exception
          * @throw FFStreamError
          * @throw StringUtils::StringException
          */
      virtual void reallyPutRecord(FFStream& s) const;


         /** This function reads the RINEX Nav header from the given FFStream.
          * If an error is encountered in reading from the stream, the stream
          * is reset to its original position and its fail-bit is set.
          * @throw std::exception
          * @throw StringException when a StringUtils function fails
          * @throw FFStreamError when exceptions(failbit) is set and a read
          *         or formatting error occurs.  This also resets the stream
          *         to its pre-read position.
          */
      virtual void reallyGetRecord(FFStream& s);

   }; // End of class 'Rinex3NavHeader'

      //@}

}  // End of namespace gnsstk

#endif   // GNSSTK_RINEX3NAVHEADER_HPP
