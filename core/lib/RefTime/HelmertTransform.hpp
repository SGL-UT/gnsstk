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

#ifndef GNSSTK_HELMERTTRANSFORM_HPP
#define GNSSTK_HELMERTTRANSFORM_HPP

#include <map>
#include <string>

#include "gnsstk_export.h"
#include "Exception.hpp"
#include "ReferenceFrame.hpp"
#include "CommonTime.hpp"
#include "Position.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Xvt.hpp"

namespace gnsstk
{
      /** Helmert transformations, which are 7-parameter
       * transformations between reference frames (i.e. ECEF position
       * coordinates). A Helmert tranformation is defined by 7
       * parameters: a rotation(3), a translation(3) and scale
       * factor(1).
       * @deprecated This enum is deprecated as of Dec 2022 and should
       *   not be used.  Use TransformLibrary or HelmertTransformer instead. */
   class HelmertTransform
   {
   public:
         /// Default constructor
      HelmertTransform() noexcept;

         /** Explicit constructor, from the 7 parameters.
          * All the inputs are unchanged.
          *
          * This constructor and class are for data to be used with
          * the transformation of the either of the two following
          * forms (view via doxygen).
          *
          * @warning If the transformation parameters you wish to use
          * use the transposed version of the rotation matrix
          * (i.e. the signs on the rotation angles are reversed from
          * what is shown below), make sure to flip the sign on the
          * irx,iry,irz parameters to the constructor.
          *
          * \f[
          * \left[ {\begin{array}{c} X_{tgt} \\
          *                          Y_{tgt} \\
          *                          Z_{tgt} \\
          *         \end{array} }
          * \right] = \left[ {\begin{array}{c} T_x \\
          *                                    T_y \\
          *                                    T_z \\
          *           \end{array} }
          *  \right] + (1 + S_c) \left[ {\begin{array}{ccc} 1 & R_z & -R_y \\
          *                                                 -R_z & 1 & R_x \\
          *                                                 R_y & -R_x & 1 \\
          *                              \end{array} }
          *                      \right]
          * \left[ {\begin{array}{c} X_{src} \\ 
          *                          Y_{src} \\ 
          *                          Z_{src} \\
          *         \end{array} }
          * \right]
          * \f]
          *
          * \f[
          * \left[ {\begin{array}{c} X_{tgt} \\ 
          *                          Y_{tgt} \\ 
          *                          Z_{tgt} \\
          *         \end{array} }
          * \right] = \left[ {\begin{array}{c} X_{src} \\ 
          *                                    Y_{src} \\
          *                                    Z_{src} \\ 
          *                   \end{array} } 
          * \right] + \left[ {\begin{array}{c} T_x \\ 
          *                                    T_y \\ 
          *                                    T_z \\
          *                   \end{array} }
          * \right] + \left[ {\begin{array}{ccc} S_c & R_z & -R_y \\
          *                                      -R_z & S_c & R_x \\ 
          *                                      R_y & -R_x & S_c \\
          *                   \end{array} }
          * \right] \left[ {\begin{array}{c} X_{src} \\
          *                                  Y_{src} \\
          *                                  Z_{src} \\ 
          *                 \end{array} }
          * \right]
          * \f]
          *
          * @param[in] from Transform takes "from" -> "to"
          * @param[in] to Transform takes "from" -> "to"
          * @param[in] irx X axis rotation angle in degrees
          * @param[in] iry Y axis rotation angle in degrees
          * @param[in] irz Z axis rotation angle in degrees
          * @param[in] itx X axis translation in meters
          * @param[in] ity Y axis translation in meters
          * @param[in] itz Z axis translation in meters
          * @param[in] sc scale factor (dimensionless)
          * @param[in] desc description of the transform, should include
          * @param[in] refEpoch time when transform became applicable
          *   (default=BOT) reference frames and an indication of the
          *   source (e.g. literature citation).
          * @throw InvalidRequest if the transform is invalid. */
      HelmertTransform(ReferenceFrame from, ReferenceFrame to,
                       double irx, double iry, double irz,
                       double itx, double ity, double itz,
                       double sc, const std::string& desc,
                       const CommonTime& refEpoch);

         /** Dump the object to a multi-line string including
          * reference frames, the 7 parameters and description. */
      std::string asString() const noexcept;

         /** Transform Position to another frame using this transform
          * or its inverse.
          * @param[in] pos position to be transformed; unchanged on output.
          * @param[out] result position after transformation.
          * @throw InvalidRequest if transformation, or inverse,
          *   cannot act on ReferenceFrame of input. */
      void transform(const Position& pos, Position& result);

         /** Transform a 3-vector, in the given frame, using this
          * Helmert transformation.
          * @param[in] vec 3-vector of position coordinates in "from" frame.
          * @param[in] frame Transform takes "frame" -> "new-frame"
          * @param[out] result 3-vector in "new-frame" frame.
          * @throw InvalidRequest if transformation, or inverse,
          *   cannot act on ReferenceFrame of input. */
      void transform(const Vector<double>& vec, ReferenceFrame frame,
                     Vector<double>& result);

         /** Transform a Triple using this Helmert transformation or
          * its inverse.
          * @param[in] vec containing position and frame to be transformed.
          * @param[in] frame Transform takes "frame" -> "new-frame"
          * @param[out] result with position in new frame
          * @throw InvalidRequest if transformation, or inverse,
          *   cannot act on ReferenceFrame of input. */
      void transform(const Triple& vec, ReferenceFrame frame, Triple& result);

         /** Transform an Xvt using this Helmert transformation or its inverse.
          * @param[in] xvt containing position and frame to be transformed.
          * @param[out] result with position in new frame
          * @throw InvalidRequest if transformation, or inverse,
          *   cannot act on ReferenceFrame of input. */
      void transform(const Xvt& xvt, Xvt& result);

         /** Transform 3 doubles, in the given frame, using this
          * Helmert transformation.
          * @param[in] x,y,z 3-vector of position coordinates in "from" frame.
          * @param[in] frame Transform takes "frame" -> "new-frame"
          * @param[out] rx,ry,rz result 3-vector in "new-frame" frame.
          * @throw InvalidRequest if transformation, or inverse,
          *   cannot act on ReferenceFrame of input. */
      void transform(double x, double y, double z,
                     ReferenceFrame frame,
                     double& rx, double& ry, double& rz);

         /// Get the reference frame this transform can convert from (or to).
      ReferenceFrame getFromFrame() const noexcept
      { return fromFrame; }

         /// Get the reference frame this transform can convert to (or from).
      ReferenceFrame getToFrame() const noexcept
      { return toFrame; }

         /// Get the time at which this transform becomes valid.
      CommonTime getEpoch() const noexcept
      { return epoch; }

         /// Epoch at which GLONASS transitions from PZ90 to PZ90.02
      GNSSTK_EXPORT static const CommonTime PZ90Epoch;

         /// length of array of pre-defined HelmertTransforms
      GNSSTK_EXPORT static const int stdCount = 5; // equal to number listed in HelmertTransform.cpp

         /// array of all pre-defined HelmertTransforms
      GNSSTK_EXPORT static const HelmertTransform stdTransforms[stdCount];

   protected:
         // member data

         // this transformation takes fromFrame -> toFrame
      ReferenceFrame fromFrame;  ///< Reference frame to which *this is applied.
      ReferenceFrame toFrame;    ///< Reference frame resulting from *this transform.

         // the 7 parameters that define the tranformation
      double rx;                 ///< X axis rotation in radians
      double ry;                 ///< Y axis rotation in radians
      double rz;                 ///< Z axis rotation in radians
      double tx;                 ///< X axis translation in meters
      double ty;                 ///< Y axis translation in meters
      double tz;                 ///< Z axis translation in meters
      double scale;              ///< scale factor, dimensionless, 0=no scale

         // transform quantities derived from the 7 parameters
      Matrix<double> rotation;   ///< the transform 3x3 rotation matrix (w/o scale)
      Vector<double> translation;///< the transform 3-vector in meters

         /// epoch at which transform is first applicable
      CommonTime epoch;

         /** an arbitrary string describing the transform; it should
          * include the source. */
      std::string description;

   }; // end class HelmertTransform

} // end namespace gnsstk

#endif
