//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

#include "PositionRecord.hpp"

using namespace std;

namespace gnsstk
{
      // Output stream operator is used by dump() in TabularSatStore
   ostream& operator<<(ostream& os, const PositionRecord& rec) throw()
   {
      os << "Pos" << fixed << setprecision(6)
         << " " << setw(13) << rec.Pos[0]
         << " " << setw(13) << rec.Pos[1]
         << " " << setw(13) << rec.Pos[2]
         << " sigP" << scientific << setprecision(2)
         << " " << setw(9) << rec.sigPos[0]
         << " " << setw(9) << rec.sigPos[1]
         << " " << setw(9) << rec.sigPos[2]
         << " Vel" << fixed << setprecision(6)
         << " " << setw(13) << rec.Vel[0]
         << " " << setw(13) << rec.Vel[1]
         << " " << setw(13) << rec.Vel[2]
         << " sigV" << scientific << setprecision(2)
         << " " << setw(9) << rec.sigVel[0]
         << " " << setw(9) << rec.sigVel[1]
         << " " << setw(9) << rec.sigVel[2]
            //<< " Acc" << fixed << setprecision(6)
            //<< " " << setw(13) << rec.Acc[0]
            //<< " " << setw(13) << rec.Acc[1]
            //<< " " << setw(13) << rec.Acc[2]
            //<< " sigA" << scientific << setprecision(2)
            //<< " " << setw(9) << rec.sigAcc[0]
            //<< " " << setw(9) << rec.sigAcc[1]
            //<< " " << setw(9) << rec.sigAcc[2]
         ;
      return os;
   }

}  // namespace gnsstk
