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

#ifndef GNSSTK_TRANSFORM_LIBRARY
#define GNSSTK_TRANSFORM_LIBRARY

#include "Transformer.hpp"

namespace gnsstk
{
      /// @ingroup geodeticgroup
      //@{

      /** This class provides storage and access to datum
       * transformations.  Recommended use is to create one instance
       * of this class and either use the transformation data that is
       * loaded by default and/or add additional transformation data.
       * Then call the appropriate transform method for each
       * coordinate transformation.
       *
       * Call the clear() method on \a transformers to empty the
       * default transformation data if desired.
       *
       * Typical usage would be something like:
       * @code
       * TransformLibrary xflib;
       * ...
       * CommonTime ct = ...;
       * ObsID oid = ...;
       * Xvt xvt, xvtITRF;
       * eph.getXvt(ct, xvt, oid);
       * xvtITRF.frame = RefFrame(RefFrameRlz::ITRF2020);
       * if (xflib.transform(xvt, xvtITRF, ct))
       * {
       *    cout << "success!" << endl;
       * }
       * @endcode
       */
   class TransformLibrary
   {
   public:
         /// Fill the library with a default set of transformation parameters.
      TransformLibrary();

         /** Add a new Transformer to the library.
          * @param[in] ptr The Transformer to be added.
          * @post transformers contains ptr with keys in forward and reverse. */
      void addTransform(const TransformerPtr& ptr);

         /** Get the Transformer in \a transformers that matches the requested
          * parameters.
          * @param[in] fromFrame The RefFrame being converted from.
          * @param[in] toFrame The RefFrame being converted to.
          * @param[out] ptr The Transformer that matches, if true is returned.
          * @param[in] when The time of interest for the
          *   transformation.  This does not affect the RefFrame, it
          *   is only used when multiple transformation parameter sets
          *   are available over a span of time. Default = most recent.
          * @return true if a match was found. */
      bool getTransform(const RefFrame& fromFrame, const RefFrame& toFrame,
                        TransformerPtr& ptr,
                        const CommonTime& when = gnsstk::CommonTime::END_OF_TIME)
         const;

         /** Convert a position from the reference frame in fromPos to
          * that in toPos.
          * @pre RefFrame must be set in both fromPos and toPos.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toPos reference frame realization are
          *   set in toPos.
          * @param[in] fromPos The Position to be converted.
          * @param[in,out] toPos The Position to store the converted position.
          * @param[in] when The time of interest for the
          *   transformation.  This does not affect the RefFrame, it
          *   is only used when multiple transformation parameter sets
          *   are available over a span of time. Default = most recent.
          * @return true on success, false on failure (typically if
          *   the Position RefFrame objects don't match the ones in
          *   this Transformer). */
      bool transform(const Position& fromPos, Position& toPos,
                     const CommonTime& when = gnsstk::CommonTime::END_OF_TIME)
         const noexcept;

         /** Convert a position from the reference frame in fromPos to
          * that in toPos.
          * @pre RefFrame must be set in both fromPos and toPos.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toPos reference frame realization are
          *   set in toPos.  Velocity and clock offset are unaffected.
          * @param[in] fromPos The Xvt whose position is to be converted.
          * @param[in,out] toPos The Xvt to store the converted position.
          * @param[in] when The time of interest for the
          *   transformation.  This does not affect the RefFrame, it
          *   is only used when multiple transformation parameter sets
          *   are available over a span of time. Default = most recent.
          * @return true on success, false on failure (typically if
          *   the Xvt RefFrame objects don't match the ones in
          *   this Transformer). */
      bool transform(const Xvt& fromPos, Xvt& toPos,
                     const CommonTime& when = gnsstk::CommonTime::END_OF_TIME)
         const noexcept;

         /** Convert a position in fromPos from the reference frame in
          * srcFrame to that in tgtFrame.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toFrame reference frame realization are
          *   set in toPos.  Velocity and clock offset are unaffected.
          * @param[in] fromPos The Vector whose position is to be converted.
          * @param[in] srcFrame The RefFrame of the coordinates in fromPos.
          * @param[out] toPos The Vector to store the converted position.
          * @param[in] tgtFrame The RefFrame to convert to.
          * @param[in] when The time of interest for the
          *   transformation.  This does not affect the RefFrame, it
          *   is only used when multiple transformation parameter sets
          *   are available over a span of time. Default = most recent.
          * @return true on success, false on failure (typically if
          *   srcFrame doesn't match fromFrame). */
      bool transform(const Vector<double>& fromPos,
                     const RefFrame& srcFrame,
                     Vector<double>& toPos,
                     const RefFrame& tgtFrame,
                     const CommonTime& when = gnsstk::CommonTime::END_OF_TIME)
         const noexcept;

         /** Convert a position in fromPos from the reference frame in
          * srcFrame to that in tgtFrame.
          * @pre Coordinates must be set in fromPos.
          * @post Coordinates in toFrame reference frame realization are
          *   set in toPos.  Velocity and clock offset are unaffected.
          * @param[in] fromPos The Vector whose position is to be converted.
          * @param[in] srcFrame The RefFrame of the coordinates in fromPos.
          * @param[out] toPos The Vector to store the converted position.
          * @param[in] tgtFrame The RefFrame to convert to.
          * @param[in] when The time of interest for the
          *   transformation.  This does not affect the RefFrame, it
          *   is only used when multiple transformation parameter sets
          *   are available over a span of time. Default = most recent.
          * @return true on success, false on failure (typically if
          *   srcFrame doesn't match fromFrame). */
      bool transform(const Triple& fromPos,
                     const RefFrame& srcFrame,
                     Triple& toPos,
                     const RefFrame& tgtFrame,
                     const CommonTime& when = gnsstk::CommonTime::END_OF_TIME)
         const noexcept;

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
          * @param[in] tgtFrame The RefFrame to convert to.
          * @param[in] when The time of interest for the
          *   transformation.  This does not affect the RefFrame, it
          *   is only used when multiple transformation parameter sets
          *   are available over a span of time. Default = most recent.
          * @return true on success, false on failure (typically if
          *   srcFrame doesn't match fromFrame). */
      bool transform(double fx, double fy, double fz,
                     const RefFrame& srcFrame,
                     double& tx, double& ty, double& tz,
                     const RefFrame& tgtFrame,
                     const CommonTime& when = gnsstk::CommonTime::END_OF_TIME)
         const noexcept;

      TransformerMap transformers; ///< More than meets the eye
   }; // class TransformLibrary

      //@}

} // namespace gnsstk

#endif // GNSSTK_TRANSFORM_LIBRARY
