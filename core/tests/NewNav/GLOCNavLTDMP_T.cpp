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
#include <math.h>
#include "TestUtil.hpp"
#include "GLOCNavLTDMP.hpp"


class GLOCNavLTDMP_T
{
public:
   unsigned constructorTest();
   unsigned isSVIDTest();
};


unsigned GLOCNavLTDMP_T ::
constructorTest()
{
   TUDEF("GLOCNavLTDMP", "GLOCNavLTDMP");
   gnsstk::GLOCNavLTDMP uut;
   TUASSERTE(int, 1, isnan(uut.dax0));
   TUASSERTE(int, 1, isnan(uut.day0));
   TUASSERTE(int, 1, isnan(uut.daz0));
   TUASSERTE(int, 1, isnan(uut.ax1));
   TUASSERTE(int, 1, isnan(uut.ay1));
   TUASSERTE(int, 1, isnan(uut.az1));
   TUASSERTE(int, 1, isnan(uut.ax2));
   TUASSERTE(int, 1, isnan(uut.ay2));
   TUASSERTE(int, 1, isnan(uut.az2));
   TUASSERTE(int, 1, isnan(uut.ax3));
   TUASSERTE(int, 1, isnan(uut.ay3));
   TUASSERTE(int, 1, isnan(uut.az3));
   TUASSERTE(int, 1, isnan(uut.ax4));
   TUASSERTE(int, 1, isnan(uut.ay4));
   TUASSERTE(int, 1, isnan(uut.az4));
   TURETURN();
}


unsigned GLOCNavLTDMP_T ::
isSVIDTest()
{
   TUDEF("GLOCNavLTDMP", "isSVID");
   gnsstk::GLOCNavLTDMP uut;
   TUASSERTE(bool, false, uut.isSVID(9));
   uut.header31.svid = 9;
   TUASSERTE(bool, false, uut.isSVID(9));
   uut.header32.svid = 9;
   TUASSERTE(bool, true, uut.isSVID(9));
   TURETURN();
}


int main()
{
   GLOCNavLTDMP_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.isSVIDTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
