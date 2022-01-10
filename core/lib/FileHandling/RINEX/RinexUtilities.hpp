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

/// @file RinexUtilities.hpp
/// Miscellaneous RINEX-related utilities.

#ifndef GNSSTK_RINEX_UTILITIES_INCLUDE
#define GNSSTK_RINEX_UTILITIES_INCLUDE

// system includes
#include <string>
#include <vector>

namespace gnsstk
{
      /// @ingroup FileHandling
      //@{

      /// Register commonly used extended Rinex observation types.
      /// @return 0 ok, or the return value of RegisterExtendedRinexObsType().
   int RegisterARLUTExtendedTypes(void);

      /// Determine if the given file is an SP3 format file.
      /// @param file the filename
      /// @return true if the file is SP3 format.
   bool isSP3File(const std::string& file);

      /// Determine if the given file is a RINEX clock format file.
      /// @param file the filename
      /// @return true if the file is RINEX clock format.
      //NB This was removed since it is never defined.
      // bool isRinexClockFile(const std::string& file);

      /// Determine if the given file is a RINEX navigation file.
      /// Open the file, read the header, and test its validity.
      /// @param file the filename
      /// @return true if the file is a valid Rinex navigation file.
   bool isRinexNavFile(const std::string& file);

      /// Determine if the given file is a RINEX 3 navigation file.
      /// Open the file, read the header, and test its validity.
      /// @param file the filename
      /// @return true if the file is a valid Rinex navigation file.
   bool isRinex3NavFile(const std::string& file);

      /// Determine if the given file is a RINEX observation file.
      /// Open the file, read the header, and test its validity.
      /// @param file the filename
      /// @return true if the file is a valid Rinex observation file.
   bool isRinexObsFile(const std::string& file);

      /// Determine if the given file is a RINEX 3 observation file.
      /// Open the file, read the header, and test its validity.
      /// @param file the filename
      /// @return true if the file is a valid Rinex observation file.
   bool isRinex3ObsFile(const std::string& file);

      /** Sort a vector of RINEX obs file names on the time of the
       * first observation as found in the header. Return the sorted
       * list in the calling argument.  Do not include files that are
       * found not to be RINEX obs files, or that have invalid
       * headers.
       * @param files  vector<string> containing filenames, with path
       * @return string containing error messages, if any
       * @throw Exception
       */
   std::string sortRinexObsFiles(std::vector<std::string>& files);

      /// Sort a vector of RINEX 3 obs file names on the time of the
      /// first observation as found in the header. Return the sorted
      /// list in the calling argument.  Do not include files that are
      /// found not to be RINEX 3 obs files, or that have invalid
      /// headers.
      /// @param files  vector<string> containing filenames, with path
      /// @return string containing error messages, if any
   std::string sortRinex3ObsFiles(std::vector<std::string>& files);

      //@}

} // end namespace gnsstk

#endif

