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

#ifndef GNSSTK_REFFRAME_HPP
#define GNSSTK_REFFRAME_HPP

#include "RefFrameSys.hpp"
#include "RefFrameRlz.hpp"
#include "ReferenceFrame.hpp" // deprecated

namespace gnsstk
{
      /// @ingroup geodeticgroup
      //@{

      /** Class for keeping track of specific reference frame system
       * and realization pairs. */
   class RefFrame
   {
   public:
         /// Set both system and realization to Unknown.
      RefFrame();
         /** Construct a RefFrame from a RefFrameRlz.  The system is
          * set from an assumption based on the specified realization.
          * @see getRefFrameSys().
          * @post system and realization are set. 
          * @param[in] rlz The reference frame realization being used. */
      RefFrame(RefFrameRlz rlz);
         /** Construct a RefFrame from a RefFrameSys and a time stamp.
          * The realization is set based on the time of publication.
          * @see getRefFrameRlz().
          * @post system and realization are set. 
          * @param[in] sys The reference frame system being used.
          * @param[in] when The timestamp when the reference frame
          *   system was being used, maps to a realization. */
      RefFrame(RefFrameSys sys, const gnsstk::CommonTime& when);
         /** Construct from a string representation of a realization
          * (e.g. from an SP3 file header).
          * @post system and realization are set. 
          * @param[in] str The string representation of the ref frame
          *   realization
          * @param[in] when The time the reference frame is being
          *   used.  If \a str is insufficient on its own to translate
          *   to a realization, str will be treated as a system and an
          *   attempt will be made to convert the system + when to a
          *   realization. */
      RefFrame(const std::string& str, const gnsstk::CommonTime& when);
         /// Compare this with right.
      bool operator==(const RefFrame& right) const noexcept
      { return (system == right.system) && (realization == right.realization); }
         /// Compare this with right.
      bool operator!=(const RefFrame& right) const noexcept
      { return (system != right.system) || (realization != right.realization); }
         /// Ordering for maps etc.
      bool operator<(const RefFrame& right) const noexcept;
         /// Return the reference frame system represented by this object.
      RefFrameSys getSystem() const noexcept
      { return system; }
         /// Return the reference frame realization represented by this object.
      RefFrameRlz getRealization() const noexcept
      { return realization; }

         /** @deprecated This is a temporary implementation to maintain
          * some usability of the original HelmertTransform class and
          * will be removed in the future. */
      bool operator==(ReferenceFrame orf) const noexcept;
         /** @deprecated This is a temporary implementation to maintain
          * some usability of the original HelmertTransform class and
          * will be removed in the future. */
      RefFrame(ReferenceFrame orf, const gnsstk::CommonTime& when);
   private:
         /// The reference frame system this object represents.
      RefFrameSys system;
         /// The reference frame realization this object represents.
      RefFrameRlz realization;

      friend std::ostream& operator<<(std::ostream& s, const RefFrame& rf);
   };


      /// A little something to use for TransformerMap.
   using RefFramePair = std::pair<RefFrame, RefFrame>;


      /** Stream output operator for RefFrame, obviously.
       * @param[in,out] s The stream to write to.
       * @param[in] rf The RefFrame object to write.
       * @return the reference s, after writing. */
   inline std::ostream& operator<<(std::ostream& s, const RefFrame& rf)
   {
         // just print the realization, it should be enough.
      s << gnsstk::StringUtils::asString(rf.realization);
      return s;
   }

      //@}

}

#endif // GNSSTK_REFFRAME_HPP
