//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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

/**
 * @file Namelist.hpp
 * Include file defining class Namelist.
 * class gpstk::Namelist encapsulates a list of labels for use with classes
 * Matrix, Vector and SRI.
 * Also includes LabeledVector and LabeledMatrix, classes used to easily
 * format and output Vectors and Matrix objects.
 */

#ifndef CLASS_NAMELIST_INCLUDE
#define CLASS_NAMELIST_INCLUDE

//------------------------------------------------------------------------------------
// system includes
#include <algorithm>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
// GPSTk
#include "Matrix.hpp"

namespace gpstk
{

   //---------------------------------------------------------------------------------
   class Namelist;            ///< forward declaration

   /** class LabeledVector. Pretty print a Vector using the labels in a Namelist.
   */
   class LabeledVector
   {
   public:
      int wid;                ///< width of fields, in characters
      int prec;               ///< precision of fixed, in digits
      int form;               ///< format - 0=general, 1=fixed(default), 2=integer
      std::string msg;        ///< label (name in Namelist) for each row
      std::string tag;        ///< text at beginning of each line (row)
      const Namelist &NL;     ///< Namelist of names
      const Vector<double> &V;///< Vector to print

      /// Constructor, defaults are width 12 prec 5 format fixed
      LabeledVector(const Namelist &nl, const Vector<double> &v)
         : wid(12), prec(5), form(1), NL(nl), V(v)
      {
      }

      /// Set the width to w characters
      LabeledVector& setw(int w)
      {
         wid = w;
         return *this;
      }

      /// Set the precision to p digits
      LabeledVector& setprecision(int p)
      {
         prec = p;
         return *this;
      }

      /// Set the format to fixed
      LabeledVector& fixed()
      {
         form = 1;
         return *this;
      }

      /// Set the format to scientific
      LabeledVector& scientific()
      {
         form = 2;
         return *this;
      }

      /// Set the label or name
      LabeledVector& message(const std::string &m)
      {
         msg = m;
         return *this;
      }

      /// Set the tag
      LabeledVector& linetag(const std::string &m)
      {
         tag = m;
         return *this;
      }
   };

   /** class LabeledMatrix. Pretty print a Matrix using the labels in a Namelist.
    *  For example, the following code produces the following output:
    *  @code
    *   Matrix<double> Cneu; //....
    *   Namelist PosLabNEU("bake-N","bake-E","bake-U");
    *   LabeledMatrix LMcovNEU(PosLabNEU,Cneu);
    *   LMcovNEU.scientific().setw(15).setprecision(3).symmetric();
    *   LOG(INFO) << "\nFixed NEU covariance, " << "REF-to-REF, " << "final total";
    *   LOG(INFO) << LMcovNEU;
    *
    *      Fixed NEU covariance, REF-to-REF, final total
    *                            bake-N         bake-E         bake-U
    *             bake-N      2.824e-06
    *             bake-E     -4.396e-07      2.966e-06
    *             bake-U      1.509e-06      2.502e-07      1.266e-05
    *  @endcode
    */
   class LabeledMatrix
   {
   public:
      int wid;                ///< width of fields, in characters
      int prec;               ///< precision of fixed, in digits
      int form;               ///< format: 1=fixed, 2=scientific
      int rc;                 ///< label rows only (1) columns only (2) or both (0)
      bool sym;               ///< if true, print only lower triangle
      bool cln;               ///< if true, print 0.0 as "0"
      std::string msg;        ///< label (name in Namelist) for each row
      std::string tag;        ///< text at beginning of each line (row)
      const Namelist &NLrows; ///< Namelist of rows
      const Namelist &NLcols; ///< Namelist of columns
      const Matrix<double> &M;///< Matrix to print

      /** Constructor, defaults are width 12 prec 5 format fixed, not symmetric,
          label both rows and columns, same names on rows and columns */
      LabeledMatrix(const Namelist &nl, const Matrix<double> &m)
         : sym(false), cln(false), wid(12), prec(5), form(1), rc(0), M(m),
           NLrows(nl), NLcols(nl)
      {
      }

      /** Constructor, defaults are width 12 prec 5 format fixed, not symmetric,
          label both rows and columns, different names on rows and columns */
      LabeledMatrix(const Namelist &nr, const Namelist &nc,
                    const Matrix<double> &m)
         : sym(false), cln(false), wid(12), prec(5), form(1), rc(0), M(m),
           NLrows(nr), NLcols(nc)
      {
      }

      /// Set the width to w characters
      LabeledMatrix& setw(int w)
      {
         wid = w;
         return *this;
      }

      /// Set the precision to p digits
      LabeledMatrix& setprecision(int p)
      {
         prec = p;
         return *this;
      }

      /// Set the format to fixed
      LabeledMatrix& fixed()
      {
         form = 1;
         return *this;
      }

      /// Set the format to scientific
      LabeledMatrix& scientific()
      {
         form = 2;
         return *this;
      }

      /// If true, the matrix is symmetrical, so print only the lower triangle
      LabeledMatrix& symmetric(bool s = true)
      {
         sym = s;
         return *this;
      }

      /// If true, print 0.0 as 0
      LabeledMatrix& clean(bool s = true)
      {
         cln = s;
         return *this;
      }

      /// Print labels on both rows and columns
      LabeledMatrix& both()
      {
         rc = 0;
         return *this;
      }

      /// Print labels on rows only
      LabeledMatrix& rows()
      {
         rc = 1;
         return *this;
      }

      /// Print labels on columns only
      LabeledMatrix& cols()
      {
         rc = 2;
         return *this;
      }

      /// Print the given message
      LabeledMatrix& message(const std::string &m)
      {
         msg = m;
         return *this;
      }

      /// Print the given tag at the beginning of each line
      LabeledMatrix& linetag(const std::string &m)
      {
         tag = m;
         return *this;
      }
   };

      /// output ostream operator for LabeledMatrix
   std::ostream& operator<<(std::ostream &os, const LabeledMatrix &lm);

      /// output ostream operator for LabeledVector
   std::ostream& operator<<(std::ostream &os, const LabeledVector &lv);

   //---------------------------------------------------------------------------------
   /** class Namelist. A Namelist is simply an ordered set of unique strings
    * ('names' or 'labels') of any length. Namelists are used to label or
    * identify elements of Vectors or Matrix rows and columns. Namelist is
    * particularly useful in class SRI, which includes a Matrix and Vector that
    * hold state and covariance information in an estimation problem; SRI
    * include a Namelist which associates readable labels with the elements of
    * the state and covariance.
    */
   class Namelist
   {
      friend class SRI;
      friend std::ostream& operator<<(std::ostream &, const LabeledMatrix &);
      friend std::ostream& operator<<(std::ostream &, const LabeledVector &);

   public:
      /// empty constructor
      Namelist() {}

      /// constructor given dimension - creates default labels
      Namelist(const unsigned int &);

      /** explicit constructor - only a unique subset of the input will be
          included. */
      Namelist(const std::vector<std::string> &);

      /// copy constructor
      Namelist(const Namelist &names) { labels = names.labels; }

      /// destructor
      ~Namelist() { labels.clear(); }

      /// operator=
      Namelist& operator=(const Namelist &right)
      {
         labels = right.labels;
         return *this;
      }

      /** add a single name to the Namelist
          @throw if the name is not unique */
      Namelist& operator+=(const std::string &);

      /// add entire Namelist to this using operator+=(string)
      Namelist& operator+=(const Namelist &right)
      {
         try
         {
            for (unsigned int i = 0; i < right.size(); i++)
               this->operator+=(right.getName(i));
         }
         catch (gpstk::Exception &e)
         {
            GPSTK_RETHROW(e);
         }
         return *this;
      }

      /** remove a name from the Namelist; does nothing if the name is not
          found. */
      Namelist& operator-=(const std::string &);

      /** swap two elements, as given by their indexes; no effect if either
          index is out of range. */
      void swap(const unsigned int &, const unsigned int &);

      /// reorder the list by sorting
      void sort();

      /// resize the list by either truncation or adding default names.
      void resize(unsigned int);

      /// randomize the list
      void randomize(long seed = 0);

      /// empty the list
      void clear() { labels.clear(); }

      /// is the Namelist valid? checks for repeated names
      bool valid() const;

      /// does the Namelist contain the input name?
      bool contains(const std::string &) const;

      /// are two Namelists identical, ignoring permutations?
      friend bool operator==(const Namelist &, const Namelist &);

      /// are two Namelists different, ignoring permutations?
      friend bool operator!=(const Namelist &, const Namelist &);

      /// are two Namelists exactly identical, even considering permutations?
      friend bool identical(const Namelist &, const Namelist &);

      /// construct the subset Namelist which is common to the two inputs (AND)
      friend Namelist operator&(const Namelist &, const Namelist &);

      /// merge two Namelists, i.e. construct a non-redundant combination (OR)
      friend Namelist operator|(const Namelist &, const Namelist &);

      /// construct the subset Namelist which is NOT common to two others (XOR)
      friend Namelist operator^(const Namelist &, const Namelist &);

      /// replace this with (this & input)
      Namelist& operator&=(const Namelist &N);

      /// replace this with (this | input)
      Namelist& operator|=(const Namelist &N);

      /// replace this with (this ^ input)
      Namelist& operator^=(const Namelist &N);

      /** bind a Namelist to a Matrix<double> before sending it to an output
          stream, to get a 'labelled display' of the matrix.
       */
      LabeledMatrix operator()(Matrix<double> &m)
      {
         return LabeledMatrix(*this, m);
      }

      /** bind a Namelist to a Vector<double> before sending it to an output
          stream, to get a 'labelled display' of the vector.
       */
      LabeledVector operator()(Vector<double> &v)
      {
         return LabeledVector(*this, v);
      }

      // member access

      /// return the size of the list.
      inline unsigned int size() const { return labels.size(); }

      /* std::string& operator[](const unsigned int);
         Don't do this, b/c it means user could create invalid Namelists,
         and b/c passing an invalid int would require a throw
      */

      /** access to a specific name, given its index.
          returns 'out-of-range' if the index is out of range.
       */
      std::string getName(const unsigned int) const;

      /** assign a specific name, given its index;
          no effect if the name is not unique;
          return true if successful.
       */
      bool setName(const unsigned int, const std::string &);

      /** return the index of the name in the list that matches the input,
          -1 if not found.
      */
      int index(const std::string &) const;

      /// output ostream operator
      friend std::ostream& operator<<(std::ostream &s, const Namelist &);

      // member data

      /// vector of names (strings)
      std::vector<std::string> labels;

   }; // end class Namelist

} // end of namespace gpstk

#endif
