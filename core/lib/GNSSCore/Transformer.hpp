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

#ifndef GNSSTK_TRANSFORMER_HPP
#define GNSSTK_TRANSFORMER_HPP

#include <memory>
#include <map>
#include "Position.hpp"
#include "Xvt.hpp"
#include "Vector.hpp"
#include "RefFrame.hpp"
#include "CommonTime.hpp"

namespace gnsstk
{
      /// @ingroup geodeticgroup
      //@{

      /** Provide an abstract base class for tools that can transform
       * between coordinate systems, e.g. HelmertTransformer. */
   class Transformer
   {
   public:
         // Initialize start and end time to min and max values.
      Transformer()
            : epoch(gnsstk::CommonTime::BEGINNING_OF_TIME)
      {}

         /** Convert a position from the reference frame in fromPos to
          * that in toPos.
          * @pre RefFrame must be set in both fromPos and toPos.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toPos reference frame realization are
          *   set in toPos.
          * @param[in] fromPos The Position to be converted.
          * @param[in,out] toPos The Position to store the converted position.
          * @return true on success, false on failure (typically if
          *   the Position RefFrame objects don't match the ones in
          *   this Transformer). */
      virtual bool transform(const Position& fromPos, Position& toPos)
         const noexcept = 0;

         /** Convert a position from the reference frame in fromPos to
          * that in toPos.
          * @pre RefFrame must be set in both fromPos and toPos.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toPos reference frame realization are
          *   set in toPos.  Velocity and clock offset are unaffected.
          * @param[in] fromPos The Xvt whose position is to be converted.
          * @param[in,out] toPos The Xvt to store the converted position.
          * @return true on success, false on failure (typically if
          *   the Xvt RefFrame objects don't match the ones in
          *   this Transformer). */
      virtual bool transform(const Xvt& fromPos, Xvt& toPos)
         const noexcept = 0;

         /** Convert a position from the reference frame in fromPos to
          * that in toPos.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toFrame reference frame realization are
          *   set in toPos.  Velocity and clock offset are unaffected.
          * @param[in] fromPos The Vector whose position is to be converted.
          * @param[in] srcFrame The RefFrame of the coordinates in fromPos.
          * @param[out] toPos The Vector to store the converted position.
          * @return true on success, false on failure (typically if
          *   srcFrame doesn't match fromFrame). */
      virtual bool transform(const Vector<double>& fromPos,
                             const RefFrame& srcFrame,
                             Vector<double>& toPos)
         const noexcept = 0;

         /** Convert a position in fromPos from the reference frame in
          * srcFrame to that in tgtFrame.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toFrame reference frame realization are
          *   set in toPos.  Velocity and clock offset are unaffected.
          * @param[in] fromPos The Vector whose position is to be converted.
          * @param[in] srcFrame The RefFrame of the coordinates in fromPos.
          * @param[out] toPos The Vector to store the converted position.
          * @return true on success, false on failure (typically if
          *   srcFrame doesn't match fromFrame). */
      virtual bool transform(const Triple& fromPos,
                             const RefFrame& srcFrame,
                             Triple& toPos)
         const noexcept = 0;

         /** Convert 3 doubles from the reference frame in srcFrame to
          * that in tgtFrame.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toFrame reference frame realization are
          *   set in toPos.  Velocity and clock offset are unaffected.
          * @param[in] fx ECEF x coordinate in meters in srcFrame.
          * @param[in] fy ECEF y coordinate in meters in srcFrame.
          * @param[in] fz ECEF z coordinate in meters in srcFrame.
          * @param[in] srcFrame The RefFrame of the coordinates in fromPos.
          * @param[out] tx ECEF x coordinate in meters in srcFrame.
          * @param[out] ty ECEF y coordinate in meters in srcFrame.
          * @param[out] tz ECEF z coordinate in meters in srcFrame.
          * @return true on success, false on failure (typically if
          *   srcFrame doesn't match fromFrame). */
      virtual bool transform(double fx, double fy, double fz,
                             const RefFrame& srcFrame,
                             double& tx, double& ty, double& tz)
         const noexcept = 0;

         /// Return the RefFrame this Transformer will convert from.
      const RefFrame& getFromFrame() const noexcept
      { return fromFrame; }
         /// Return the RefFrame this Transformer will convert to.
      const RefFrame& getToFrame() const noexcept
      { return toFrame; }
         /** Return the first time this Transformer is applicable.
          * This is not the reference frame publication date.  This is
          * to allow the use of transformation sources that use the
          * same reference frame realizations, but the transformation
          * parameters change over time. */
      const CommonTime& getEpoch() const noexcept
      { return epoch; }

   protected:
      RefFrame fromFrame;   ///< The reference frame we can transform from.
      RefFrame toFrame;     ///< The reference frame we can transform to.
      CommonTime epoch;     ///< When this Transformer was first applicable.
   }; // class Transformer

      /// Shared pointer to Transformer object.
   using TransformerPtr = std::shared_ptr<Transformer>;
      /// Transformers may change over time while maintaining the same RefFrames
   using TransformerHist = std::map<CommonTime, TransformerPtr>;
      /// Container of transformers for management.
   using TransformerMap = std::map<RefFramePair, TransformerHist>;

      //@}

} // namespace gnsstk

#endif // GNSSTK_TRANSFORMER_HPP
