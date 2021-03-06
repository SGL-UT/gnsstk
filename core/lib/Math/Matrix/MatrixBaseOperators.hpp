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
/**
 * @file MatrixBaseOperators.hpp
 * Matrix operators for the base class
 */

#ifndef GNSSTK_MATRIX_BASE_OPERATORS_HPP
#define GNSSTK_MATRIX_BASE_OPERATORS_HPP

#include <fstream> // for copyfmt
#include <iomanip>
#include "MatrixBase.hpp"

namespace gnsstk
{

      /// @ingroup MathGroup
      //@{

      /// Output operator for ConstMatrixBase classes
   template <class T, class E>
   std::ostream& operator<<(std::ostream& s, const ConstMatrixBase<T, E>& a)
   {
      size_t i, j;
      std::ofstream savefmt;
      savefmt.copyfmt(s);
      for (i=0; i<a.rows(); i++)
      {
         for (j=0; j< a.cols(); j++) {
            s << std::setw(1) << ' ';
            s.copyfmt(savefmt);
            s << a(i,j);
         }
         if(i < a.rows()-1) s << std::endl;
      }
      return s;
   }

      /**
       * Turns the square RefMatrixBase matrix into an identity matrix
       * @throw MatrixException
       */
   template <class T, class BaseClass>
   BaseClass& ident(RefMatrixBase<T, BaseClass>& m)
   {
      BaseClass& me = static_cast<BaseClass&>(m);
      if ( (me.rows() != me.cols()) || (me.cols() < 1) )
      {
         MatrixException e("invalid matrix dimensions for ident()");
         GNSSTK_THROW(e);
      }
      m.assignFrom(T(0));
      size_t i;
      for (i = 0; i < me.rows(); i++)
         me(i,i) = T(1);
      return me;
   }

      /**
       * Returns the trace of the matrix
       * @throw MatrixException
       */
   template <class T, class BaseClass>
   inline T trace(const ConstMatrixBase<T, BaseClass>& m)
   {
      if ((!m.isSquare()) || (m.rows() == 0))
      {
         MatrixException e("Invalid matrix for trace()");
         GNSSTK_THROW(e);
      }
      size_t index = 0;
      T answer = m(index,index);
      for (index = 1; index < m.rows(); index++)
         answer += m(index,index);
      return answer;
   }

      /**
       * returns the frobenius norm or RSS of the matrix
       */
   template <class T, class BaseClass>
   inline T normF(const ConstMatrixBase<T, BaseClass>& m)
   {
      T sum(0);
      size_t i,j;
      for (i = 0; i < m.rows(); i++)
         for (j = 0; j < m.cols(); j++)
            sum += m(i,j) * m(i,j);
      return SQRT(sum);
   }

      /**
       * returns the column sum norm of the matrix
       */
   template <class T, class BaseClass>
   inline T normCol(const ConstMatrixBase<T, BaseClass>& m)
   {
      T sum(0), tempSum;
      size_t i,j;
      for (i = 0; i < m.rows(); i++)
      {
         tempSum = T(0);
         for (j = 0; j < m.cols(); j++)
            tempSum += ABS(m(i,j));
         if (tempSum > sum)
            sum = tempSum;
      }
      return sum;
   }

      /**
       * Uses the sum of minor determinates to calculate the whole det.
       * Slow for large matricies, but it works.
       * @throw MatrixException
       */
   template <class T, class BaseClass>
   inline T slowDet(const ConstMatrixBase<T, BaseClass>& l)
   {
      if (!l.isSquare() || (l.rows() <= 1))
      {
         MatrixException e("Invalid matrix for det()");
         GNSSTK_THROW(e);
      }
         // go recursion!
      if (l.rows() == 2)
         return l(0,0)*l(1,1) - l(0,1)*l(1,0);
      else
      {
            // use v[0,0] * det(minor matrix(0,0)) +
            //     v[0,1] * det(minor matrix(0,1)) + ...
         size_t i;
         int sign;
         T det = 0;
         for (i = 0; i < l.rows(); i++)
         {
            sign = (i % 2) ? -1 : 1;
            if (l(0,i) != 0)
               det += sign * l(0,i) * slowDet(minorMatrix(l,0,i));
         }
         return det;
      }
   }

      //@}

}  // namespace

#endif
