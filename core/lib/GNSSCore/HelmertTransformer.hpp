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

#ifndef GNSSTK_HELMERTTRANSFORMER_HPP
#define GNSSTK_HELMERTTRANSFORMER_HPP

#include "Transformer.hpp"
#include "Matrix.hpp"

namespace gnsstk
{
      /// @ingroup geodeticgroup
      //@{

      /** Provide a class for transforming positions between datums
       * using the Helmert 7-parameter transform. */
   class HelmertTransformer : public Transformer
   {
   public:
         /// Initialize everything to invalid values.
      HelmertTransformer();

         /** Explicit constructor, from the 7 parameters.
          * All the inputs are unchanged.
          *
          * This constructor and class are for data to be used with
          * the transformation of the either of the two following
          * forms (view via doxygen).
          *
          * @warning If the transformation parameters
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
      HelmertTransformer(const RefFrame& from, const RefFrame& to,
                         double irx, double iry, double irz,
                         double itx, double ity, double itz,
                         double sc, const std::string& desc,
                         const CommonTime& refEpoch);
         /// @copydoc Transformer::transform(const Position&,Position&)
      bool transform(const Position& fromPos, Position& toPos)
         const noexcept override;
         /// @copydoc Transformer::transform(const Xvt&,Xvt&)
      bool transform(const Xvt& fromPos, Xvt& toPos)
         const noexcept override;
         /// @copydoc Transformer::transform(const Vector<double>&,const RefFrame&,Vector<double>&)
      bool transform(const Vector<double>& fromPos,
                     const RefFrame& srcFrame,
                     Vector<double>& toPos)
         const noexcept override;
         /// @copydoc Transformer::transform(const Triple&,const RefFrame&,Triple&)
      bool transform(const Triple& fromPos,
                     const RefFrame& srcFrame,
                     Triple& toPos)
         const noexcept override;
         /// @copydoc Transformer::transform(double,double,double,const RefFrame&,double&,double&,double&)
      bool transform(double fx, double fy, double fz,
                     const RefFrame& srcFrame,
                     double& tx, double& ty, double& tz)
         const noexcept override;

         /** An arbitrary string describing the transform.  It should
          * include the source. */
      std::string description;

   protected:
         /** The matrix that applies a rotation to move from fromFrame
          * to toFrame. */
      Matrix<double> rotation;
         /** The matrix that applies a translation to move from
          * fromFrame to toFrame. */
      Vector<double> translation;
         /// Scale factor. Dimensionless.  0 = no scale.
      double scale;
   }; // class HelmertTransformer

      //@}

} // namespace gnsstk

#endif // GNSSTK_HELMERTTRANSFORMER_HPP
