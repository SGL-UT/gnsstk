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

#ifndef GPSTK_DEBUGTRACE_HPP
#define GPSTK_DEBUGTRACE_HPP

#include <atomic>
#include <sstream>

namespace gpstk
{
      /** Class for debugging output.
       *
       * Use DEBUGTRACE_FUNCTION at the start of a function to have
       * trace output at the start and at the return from the function
       * with the name of that function.  Indentation is increased to
       * indicate call stack.
       *
       * Use DEBUGTRACE with an argument containing stream operations to
       * output any debugging information as desired.
       *
       * Use DEBUGTRACE_ENABLE to enable the trace output at run-time.
       * By default, the trace output is disabled.
       *
       * Define the macro "DEBUG_NO_TRACE" to completely disable tracing
       * in the library and applications, which can be used to improve
       * performance if no tracing is desired.
       *
       * Generally speaking, the class should not be used directly,
       * instead use the macros described above.
       *
       * @note Although a mutex is used to serialize access to the
       * indent data member, no attempt is currently made to
       * differentiate indentation levels across multiple threads.  As
       * such, the indentation may get confusing in a multi-thread
       * application.
       */
   class DebugTrace
   {
   public:
         /** Start a function trace by printing the function name and indenting.
          * @param[in] funcName the name of the function to print when
          *   entering and leaving. */
      DebugTrace(const std::string funcName);
      ~DebugTrace();
         /// If enabled is true, print \c s to stderr.
      static void trace(const std::string& s);
         /// If true, debugging/trace output will be printed to stderr.
      static std::atomic<bool> enabled;
   private:
         /** Save the name of the function so that the destructor
          * knows what to print. */
      std::string functionName;
         /// How many spaces to indent the trace output.
      static std::atomic<unsigned> indent;
   };
}

// for windows, which doesn't define __PRETTY_FUNCTION__
#if !defined(__PRETTY_FUNCTION__)
#ifdef __FUNCSIG__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif
#endif

#ifdef DEBUG_NO_TRACE
#define DEBUGTRACE_ENABLE()
#define DEBUGTRACE_DISABLE()
#define DEBUGTRACE_FUNCTION()
#define DEBUGTRACE(EXPR)
#else
#define DEBUGTRACE_ENABLE()                                             \
   {                                                                    \
      gpstk::DebugTrace::enabled = true;                                \
      std::cerr << "WARNING: Tracing is enabled, expect slow performance" \
                << std::endl;                                           \
   }
#define DEBUGTRACE_DISABLE()                    \
   {                                            \
      gpstk::DebugTrace::enabled = false;       \
   }
#define DEBUGTRACE_FUNCTION()                                   \
   gpstk::DebugTrace gpstkTraceObject(__PRETTY_FUNCTION__)
#define DEBUGTRACE(EXPR)                        \
   {                                            \
      std::ostringstream os;                    \
      os << "* "                                \
         << EXPR << std::endl;                  \
      gpstk::DebugTrace::trace(os.str());       \
   }
#endif

#endif // GPSTK_DEBUGTRACE_HPP
