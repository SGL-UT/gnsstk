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
 * @file FFTextStream.cpp
 * An FFStream for text files
 */

#include "FFTextStream.hpp"

namespace gnsstk
{
   FFTextStream ::
   FFTextStream()
   {
      init();
   }


   FFTextStream ::
   ~FFTextStream()
   {
   }


   FFTextStream ::
   FFTextStream( const char* fn,
                 std::ios::openmode mode )
         : FFStream(fn, mode)
   {
      init();
   }


   FFTextStream ::
   FFTextStream( const std::string& fn,
                 std::ios::openmode mode )
         : FFStream( fn.c_str(), mode )
   {
      init();
   }


   void FFTextStream ::
   open( const char* fn,
         std::ios::openmode mode )
   {
      FFStream::open(fn, mode);
      init();
   }


   void FFTextStream ::
   open( const std::string& fn,
         std::ios::openmode mode )
   {
      open(fn.c_str(), mode);
   }


   void FFTextStream ::
   init()
   {
      lineNumber = 0;
   }


   void FFTextStream ::
   tryFFStreamGet(FFData& rec)
   {
      unsigned int initialLineNumber = lineNumber;

      try
      {
         FFStream::tryFFStreamGet(rec);
      }
      catch(gnsstk::Exception& e)
      {
         e.addText( std::string("Near file line ") +
                    gnsstk::StringUtils::asString(lineNumber) );
         lineNumber = initialLineNumber;
         mostRecentException = e;
         conditionalThrow();
      }
   }


   void FFTextStream ::
   tryFFStreamPut(const FFData& rec)
   {
      unsigned int initialLineNumber = lineNumber;

      try
      {
         FFStream::tryFFStreamPut(rec);
      }
      catch(gnsstk::Exception& e)
      {
         e.addText( std::string("Near file line ") +
                    gnsstk::StringUtils::asString(lineNumber) );
         lineNumber = initialLineNumber;
         mostRecentException = e;
         conditionalThrow();
      }
   }


      // the reason for checking ffs.eof() in the try AND catch block is
      // because if the user enabled exceptions on the stream with exceptions()
      // then eof could throw an exception, in which case we need to catch it
      // and rethrow an EOF or FFStream exception.  In any event, EndOfFile
      // gets thrown whenever there's an EOF and expectEOF is true
   void FFTextStream ::
   formattedGetLine( std::string& line,
                     const bool expectEOF )
   {
      try
      {
         std::getline(*this, line);
            // Remove CR characters left over in the buffer from windows files
         size_t crpos = line.find_last_not_of('\r');
         if ((crpos+1) < line.length())
            line.erase(crpos+1);
         for (int i=0; i<line.length(); i++)
         {
            if (!isprint(line[i]))
            {
               FFStreamError err("Non-text data in file.");
               GNSSTK_THROW(err);
            }
         }

         lineNumber++;
         if(fail() && !eof())
         {
            FFStreamError err("Line too long");
            GNSSTK_THROW(err);
         }
            // catch EOF when stream exceptions are disabled
         if ((line.size() == 0) && eof())
         {
            if (expectEOF)
            {
               EndOfFile err("EOF encountered");
               GNSSTK_THROW(err);
            }
            else
            {
               FFStreamError err("Unexpected EOF encountered");
               GNSSTK_THROW(err);
            }
         }
      }
      catch(std::exception &e)
      {
            // catch EOF when exceptions are enabled
         if ( (line.size() == 0) && eof())
         {
            if (expectEOF)
            {
               EndOfFile err("EOF encountered");
               GNSSTK_THROW(err);
            }
            else
            {
               FFStreamError err("Unexpected EOF");
               GNSSTK_THROW(err);
            }
         }
         else
         {
            FFStreamError err("Critical file error: " +
                              std::string(e.what()));
            GNSSTK_THROW(err);
         }
      }
   }  // End of method 'FFTextStream::formattedGetLine()'

}  // End of namespace gnsstk
