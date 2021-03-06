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

/// @file SatPassUtilities.hpp Various utilities using class SatPass.

#ifndef GNSSTK_SATELLITE_PASS_UTILS_INCLUDE
#define GNSSTK_SATELLITE_PASS_UTILS_INCLUDE
#include "SatPassIterator.hpp"
#include "Rinex3ObsHeader.hpp"
#include "RinexObsHeader.hpp"
#include "RinexSatID.hpp"
#include "RinexUtilities.hpp"
#include "msecHandler.hpp"

namespace gnsstk
{
   // -------------------------------------------------------------------------------
   // -------------------------------------------------------------------------------
      /**
       Read a set of RINEX observation files, filling a vector of SatPass
       objects. Create the SatPass objects using the input list of observation
       types and timestep. If there are no obs types given (vector obstypes has
       size 0), then use the obs types in the header of the first file read. The
       files are sorted on their begin time before processing, and the resulting
       list of SatPass objects is in time order. Data before the begin time, and
       after the end time, is ignored. The list of SatPass objects need not be
       empty on input; however if not empty, then the list must be consistent
       with the input timestep and obs type list, and it will be sorted to be in
       time order. If a file cannot be opened, or it is not RINEX observation,
       simply skip it. NB. ngood for this SPL reflects # epochs with ALL
       obstypes != 0
       @param filenames vector of input RINEX observation file names
       @param obstypes  vector of observation types to include in SatPass (may
                         be empty: include all)
       @param dt        data interval of existing SPList and input files
       @param SPList    vector of SatPass objects to fill, need not be empty, but
                         if not empty, must have consistent obs types and dt.
       @param exSats    vector of satellites to exclude
       @param lenient   if true (default), be lenient in reading the RINEX format
       @param beginTime reject data before this time (BEGINNING_OF_TIME)
       @param endTime   reject data after this time (END_OF TIME)
       @return -1 if the filenames list is empty, otherwise return the number of
                      files successfully read (may be less than the number input).
       @throw gnsstk::Exception if there are exceptions while reading, if the
       data
                    in the file is out of time order.
      */
   int SatPassFromRinexFiles(
      std::vector<std::string>& filenames, std::vector<std::string>& obstypes,
      double dt, std::vector<SatPass>& SPList,
      std::vector<RinexSatID> exSats = std::vector<RinexSatID>(),
      bool lenient                   = true,
      gnsstk::Epoch beginTime         = gnsstk::CommonTime::BEGINNING_OF_TIME,
      gnsstk::Epoch endTime           = gnsstk::CommonTime::END_OF_TIME);

   // -------------------------------------------------------------------------------
      /**
       deprecated - use SatPassToRinex3File for both 3 and 2.
       Iterate over the input vector of SatPass objects (sorted to be in time
       order) and write them, with the given header, to a RINEX v.2 observation
       file of the given filename.
       @return -1 if the file could not be opened, otherwise return 0.
       @throw Exception
      */
   int SatPassToRinex2File(const std::string& filename, RinexObsHeader& header,
                           std::vector<SatPass>& SPList);

   // -------------------------------------------------------------------------------
      /**
       Iterate over the input vector of SatPass objects (sorted to be in time
       order) and write them, with the given header, to a RINEX VER 3
       observation file of the given filename.
       @param filename  RINEX ver 3 file to be written
       @param header RINEX header to write
       @param sysobs map of system-char and vector of obs types (strings)
       @param SPList  vector of SatPass over which to iterate
       @return -1 if the file could not be opened, otherwise return 0.
       @throw Exception
      */
   int
   SatPassToRinex3File(const std::string& filename, const Rinex3ObsHeader& header,
                       const std::map<char, std::vector<std::string>>& sysobs,
                       std::vector<SatPass>& SPList);

   // -------------------------------------------------------------------------------
      /**
       Find millisecond adjusts of the time tag, pseudoranges C1 C2 P1 P2,
       phases L1 L2. User the handler to print messages, etc.
       @param SPList input SatPass list for analysis
       @param msh millisecond handler
       @return number of ms adjusts found
       @throw Exception
      */
   int findMilliseconds(std::vector<SatPass>& SPList, msecHandler& msh);

   // -------------------------------------------------------------------------------
      /**
       remove millisecond adjusts of the time tags and pseudoranges (C1 C2 P1
       P2) and phases (L1 L2), given handler passed to earlier call to
       FindMilliseconds(). User the handler to print messages, etc.
       @param SPList input SatPass list for modification
       @param msh message returned from FindMilliseconds()
       @throw Exception
      */
   void removeMilliseconds(std::vector<SatPass>& SPList, msecHandler& msh);

   // -------------------------------------------------------------------------------
      /**
       Dump an entire list of SatPass, in time order
       @param SatPassList input SatPass list for dump
       @param os ostream to write to
       @param bool rev reverse order
       @param bool dbug turn on extra output
       @throw Exception
      */
   void dump(std::vector<SatPass>& SatPassList, std::ostream& os,
             bool rev = false, bool dbug = false);

} // namespace gnsstk

#endif // define GNSSTK_SATELLITE_PASS_UTILS_INCLUDE
// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
