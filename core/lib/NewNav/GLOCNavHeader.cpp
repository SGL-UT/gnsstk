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
#include "GLOCNavHeader.hpp"
#include "GLOCBits.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GLOCNavHeader ::
   GLOCNavHeader()
         : preamble(0),
           TS(0),
           svid(0),
           svUnhealthy(true),
           dataInvalid(true),
           health(SVHealth::Unknown),
           P1(0),
           P2(false),
           KP(0),
           A(false)
   {
   }


   bool GLOCNavHeader ::
   validate() const
   {
      return (!dataInvalid &&
              (preamble == gloc::valPreamble));
   }


   void GLOCNavHeader ::
   dumpOverHeader(std::ostream& s)
   {
      s << "           STRING OVERHEAD" << endl << endl
        << "               SOD    DOW:HH:MM:SS          TS  P1  P2  KP  A  H  l"
        << endl;
   }


   void GLOCNavHeader ::
   dumpStrOverhead(const std::string& label, std::ostream& s) const
   {
      s << left << setw(11) << label << right
        << gnsstk::printTime(xmit, "%7.0s  %3a-%1w:%02H:%02M:%02S %3P")
        << "   " << setw(5) << TS << "  " << setw(2) << (unsigned)P1 << "  "
        << setw(2) << P2 << "  " << setw(2) << KP << "  " << setw(1) << A
        << "  " << svUnhealthy << "  " << dataInvalid << endl;
   }


   void GLOCNavHeader ::
   dumpFlags(std::ostream& s) const
   {
      s << "P1          " << setw(16) << (unsigned)P1
        << endl
        << "P2          " << setw(16) << P2 << " encoded:";
      if (P2)
      {
         s << " turn maneuver";
      }
      else
      {
         s << " Sun-pointing";
      }
      s << endl
        << "KP          " << setw(16) << KP << " encoded:";
      switch (KP)
      {
         case 0:
            s << " no correction planned";
            break;
         case 1:
            s << " day length increased by 1s";
            break;
         case 2:
            s << " correction decision pending";
            break;
         case 3:
            s << " day length decreased by 1s";
            break;
         default:
            s << " ????";
            break;
      }
      s << endl
        << "A:          " << setw(16) << A << " encoded:"
        << (A ? " correction planned" : " no correction planned")
        << endl
        << "H^j         " << setw(16) << (unsigned)svUnhealthy << " encoded:";
      if (svUnhealthy)
      {
         s << " non-healthy";
      }
      else
      {
         s << " healthy";
      }
      s << endl
        << "l^j         " << setw(16) << (unsigned)dataInvalid << " encoded:"
        << setw(0);
      if (dataInvalid)
      {
         s << " data not valid";
      }
      else
      {
         s << " data valid";
      }
      s << endl;
   }
}
