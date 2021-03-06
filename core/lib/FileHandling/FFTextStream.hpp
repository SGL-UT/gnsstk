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
 * @file FFTextStream.hpp
 * An FFStream for text files
 */

#ifndef GNSSTK_FFTEXTSTREAM_HPP
#define GNSSTK_FFTEXTSTREAM_HPP

#include "FFStream.hpp"

namespace gnsstk
{
      /// @ingroup FileHandling
      //@{

      /**
       * An FFStream is meant for reading text.  This also includes an
       * internal line count and a read line method. When reading and
       * using the formattedGetLine() call, the lineNumber automatically
       * increments.  However, any other read and all write calls do not
       * update the line number - the derived class or programmer
       * needs to make sure that the reader or writer increments
       * lineNumber in these cases.
       */
   class FFTextStream : public FFStream
   {
   public:
         /// Default constructor
      FFTextStream();

         /// Destructor
      virtual ~FFTextStream();

         /** Common constructor.
          *
          * @param fn file name.
          * @param mode file open mode (std::ios)
          */
      FFTextStream( const char* fn,
                    std::ios::openmode mode=std::ios::in );

         /** Common constructor.
          *
          * @param fn file name.
          * @param mode file open mode (std::ios)
          */
      FFTextStream( const std::string& fn,
                    std::ios::openmode mode=std::ios::in );

         /// Overrides open to reset the line number.
      virtual void open( const char* fn,
                         std::ios::openmode mode );

         /// Overrides open to reset the line number.
      virtual void open( const std::string& fn,
                         std::ios::openmode mode );

         /// The internal line count. When writing, make sure
         /// to increment this.
      unsigned int lineNumber;


         /**
          * Like std::istream::getline but checks for EOF and removes '/r'.
          * Also increments lineNumber.  When \a expectEOF is true and EOF
          * is found, an gnsstk::EndOfFile exception is thrown.  If
          * \a expectEOF is false and an EOF is encountered, an
          * gnsstk::FFStreamError is thrown.
          * @param[out] line is set to the value of the line read from
          *   the file.
          * @param[in] expectEOF set true if finding EOF on this read
          *   is acceptable.
          * @throw EndOfFile if \a expectEOF is true and an EOF is encountered.
          * @throw FFStreamError if EOF is found and \a expectEOF is false
          * @throw gnsstk::StringUtils::StringException when a string
          *   error occurs or if any other error happens.
          * @warning There is a maximum line length of 1500 characters when
          *   using this function.
          */
      void formattedGetLine( std::string& line,
                             const bool expectEOF = false );


   protected:

         /** calls FFStream::tryFFStreamGet and adds line number information
          * @throw FFStreamError
          * @throw StringUtils::StringException
          */
      virtual void tryFFStreamGet(FFData& rec);

         /** calls FFStream::tryFFStreamPut and adds line number information
          * @throw FFStreamError
          * @throw StringUtils::StringException
          */
      virtual void tryFFStreamPut(const FFData& rec);

   private:
         /// Initialize internal data structures
      void init();

   }; // End of class 'FFTextStream'

      //@}

}  // End of namespace gnsstk


inline std::ostream& endlpp(std::ostream& os)
{
   gnsstk::FFTextStream *tos = dynamic_cast<gnsstk::FFTextStream*>(&os);
   if (tos != nullptr)
   {
      tos->lineNumber++;
   }
   return flush(os.put(os.widen('\n')));
}

#endif   // GNSSTK_FFTEXTSTREAM_HPP
