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

#include "BasicTimeSystemConverter.hpp"
#include "TestUtil.hpp"
#include "CivilTime.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

class BasicTimeSystemConverter_T
{
public:
   unsigned getOffsetTest()
   {
      TUDEF("TimeSystemConverter", "getOffset");
      double offs;
      gnsstk::BasicTimeSystemConverter btsc;

         //Check conversion from any given time system to UTC and back
      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::GPS,
                                           gnsstk::CivilTime(1990,11,6),
                                           offs));
      TUASSERTFE(-6, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::GPS,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(2004, 11, 16),
                                           offs));
      TUASSERTFE(13, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::GLO,
                                           gnsstk::CivilTime(1992, 10, 3),
                                           offs));
      TUASSERTFE(0, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::GLO,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(1995, 5, 10),
                                           offs));
      TUASSERTFE(0, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::GAL,
                                           gnsstk::CivilTime(1997, 7, 25),
                                           offs));
      TUASSERTFE(-12, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::GAL,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(2008, 6, 5),
                                           offs));
      TUASSERTFE(14, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::QZS,
                                           gnsstk::CivilTime(1985, 8, 10),
                                           offs));
      TUASSERTFE(-4, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::QZS,
                                           gnsstk::CivilTime(2010, 2, 14),
                                           offs));
      TUASSERTFE(-15, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::BDT,
                                           gnsstk::CivilTime(2006, 9, 21),
                                           offs));
      TUASSERTFE(0, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::BDT,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(2012, 8, 27),
                                           offs));
      TUASSERTFE(2, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::IRN,
                                           gnsstk::CivilTime(2004, 11, 16),
                                           offs));
      TUASSERTFE(-13, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::IRN,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(2004, 11, 16),
                                           offs));
      TUASSERTFE(13, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::TAI,
                                           gnsstk::CivilTime(2014, 6, 1),
                                           offs));
      TUASSERTFE(-35, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::TAI,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(2015, 1, 1),
                                           offs));
      TUASSERTFE(35, offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::TT,
                                           gnsstk::CivilTime(2005, 4, 31),
                                           offs));
      TUASSERTFE(-(13 + 51.184), offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::TT,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(1990, 7, 21),
                                           offs));
      TUASSERTFE(6 + 51.184, offs);

         //reference section B of astronomical almanac for TDB conversion
      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::UTC,
                                           gnsstk::TimeSystem::TDB,
                                           gnsstk::CivilTime(2007, 12, 25),
                                           offs));
      TUASSERTFE(-65.1840299405112091335467994213104248046875,
                 offs);

      TUASSERTE(bool, true, btsc.getOffset(gnsstk::TimeSystem::TDB,
                                           gnsstk::TimeSystem::UTC,
                                           gnsstk::CivilTime(1991, 4, 25),
                                           offs));
      TUASSERTFE(58.1838658094272460630236309953033924102783203125,
                 offs);

      TURETURN();
   }
};


int main() //Main function to initialize and run all tests above
{
   BasicTimeSystemConverter_T testClass;
   unsigned errorCounter = 0;

   errorCounter += testClass.getOffsetTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter
             << std::endl;

   return errorCounter; //Return the total number of errors
}
