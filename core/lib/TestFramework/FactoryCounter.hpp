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

#ifndef GNSSTK_TEST_FACTORYCOUNTER_HPP
#define GNSSTK_TEST_FACTORYCOUNTER_HPP

#include "NavData.hpp"
#include "NavMessageType.hpp"
#include "TestUtil.hpp"

// similar to TUASSERTE except requires a source line and counted data
// type name to be specified
#define FCASSERTE(TYPE,COUNTER,EXP,GOT,LINE)                            \
   try                                                                  \
   {                                                                    \
      std::ostringstream ostr;                                          \
      ostr << std::boolalpha << "Expected " << COUNTER << "=" << EXP    \
           << ", but got " << COUNTER << "=" << GOT;                    \
      testFramework.assert_equals<TYPE>(EXP, GOT, LINE, ostr.str());    \
   }                                                                    \
   catch (gnsstk::Exception &exc)                                        \
   {                                                                    \
      std::cerr << exc << std::endl;                                    \
      testFramework.assert(false,                                       \
                           "Exception evaluating " #EXP " or " #GOT,    \
                           LINE);                                       \
   }                                                                    \
   catch (...)                                                          \
   {                                                                    \
      testFramework.assert(false,                                       \
                           "Exception evaluating " #EXP " or " #GOT,    \
                           LINE);                                       \
   }

/** Class for counting instances of various types of nav messages
 * produced by PNBNavDataFactory objects and the like. System messages are
 * deliberately excluded due to a lack of system message types in some GNSS
 * systems. */
template <class Alm, class Eph, class TimeOffs, class Health, class Iono,
          class GrpDelay>
class FactoryCounter
{
public:
   FactoryCounter(gnsstk::TestUtil& tf)
         : testFramework(tf), almInc(true), ephInc(true), toInc(true),
           heaInc(true), ionoInc(true), iscInc(true)
   { resetCount(); }


   void resetCount()
   {
      almCount = 0;
      ephCount = 0;
      toCount = 0;
      heaCount = 0;
      ionoCount = 0;
      iscCount = 0;
      otherCount = 0;
   }


   void setInc(const gnsstk::NavMessageTypeSet& nmts)
   {
      almInc = (nmts.count(gnsstk::NavMessageType::Almanac) > 0);
      ephInc = (nmts.count(gnsstk::NavMessageType::Ephemeris) > 0);
      toInc = (nmts.count(gnsstk::NavMessageType::TimeOffset) > 0);
      heaInc = (nmts.count(gnsstk::NavMessageType::Health) > 0);
      ionoInc = (nmts.count(gnsstk::NavMessageType::Iono) > 0);
      iscInc = (nmts.count(gnsstk::NavMessageType::ISC) > 0);
   }


   void countResults(const gnsstk::NavDataPtrList& navOut)
   {
      resetCount();
      for (const auto& i : navOut)
      {
         if (dynamic_cast<Alm*>(i.get()) != nullptr)
         {
            almCount++;
         }
         else if (dynamic_cast<Eph*>(i.get()) != nullptr)
         {
            ephCount++;
         }
         else if (dynamic_cast<TimeOffs*>(i.get()) != nullptr)
         {
            toCount++;
         }
         else if (dynamic_cast<Health*>(i.get()) != nullptr)
         {
            heaCount++;
         }
         else if (dynamic_cast<Iono*>(i.get()) != nullptr)
         {
            ionoCount++;
         }
         else if (dynamic_cast<GrpDelay*>(i.get()) != nullptr)
         {
            iscCount++;
         }
         else
         {
            otherCount++;
         }
      }
   }


   void validateResults(gnsstk::NavDataPtrList& navOut,
                        unsigned lineNo,
                        size_t totalExp = 0,
                        unsigned almExp = 0,
                        unsigned ephExp = 0,
                        unsigned toExp = 0,
                        unsigned heaExp = 0,
                        unsigned ionoExp = 0,
                        unsigned iscExp = 0,
                        unsigned otherExp = 0)
   {
      countResults(navOut);
         // sanity check.
      FCASSERTE(unsigned, "summed total", totalExp,
                almExp+ephExp+toExp+heaExp+ionoExp+iscExp+otherExp, lineNo);
         // Get an expected total based on what's included and what isn't.
      totalExp = ((almInc ? almExp : 0) + (ephInc ? ephExp : 0) +
                  (toInc ? toExp : 0) + (heaInc ? heaExp : 0) +
                  (ionoInc ? ionoExp : 0) + (iscInc ? iscExp : 0) + otherExp);
      FCASSERTE(size_t, "total", totalExp, navOut.size(), lineNo);
      FCASSERTE(unsigned, "almCount", (almInc ? almExp : 0), almCount, lineNo);
      FCASSERTE(unsigned, "ephCount", (ephInc ? ephExp : 0), ephCount, lineNo);
      FCASSERTE(unsigned, "toCount", (toInc ? toExp : 0), toCount, lineNo);
      FCASSERTE(unsigned, "heaCount", (heaInc ? heaExp : 0), heaCount, lineNo);
      FCASSERTE(unsigned, "ionoCount", (ionoInc ? ionoExp : 0), ionoCount,
                lineNo);
      FCASSERTE(unsigned, "iscCount", (iscInc ? iscExp : 0), iscCount, lineNo);
      FCASSERTE(unsigned, "otherCount", otherExp, otherCount, lineNo);
      navOut.clear();
   }

      /// TestUtil object to use when assertions are integrated.
   gnsstk::TestUtil& testFramework;
      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, ionoCount, iscCount,
      otherCount;
      /** Include (or ignore) specific counts passed to
       * validateResults (used so the same test code can be used with
       * different type filters).  When false, the given count will be
       * expected to be zero. */
   bool almInc, ephInc, toInc, heaInc, ionoInc, iscInc;
};

#endif // GNSSTK_TEST_FACTORYCOUNTER_HPP
