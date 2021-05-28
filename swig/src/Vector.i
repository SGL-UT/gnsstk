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

%extend gpstk::Vector
{
   gpstk::Vector<double> gpstk::Vector(std::vector<double> input)
   {
      const int n = input.size();
      std::valarray<double> tmp(n);
      for (int i = 0; i < n; i++)
      {
         tmp[i] = input[i];
      }
      return new gpstk::Vector<double>(tmp);
   }

   double __getitem__(unsigned int i)
   {
      return (*($self))[i];
   }

      // alias for operator<<
   gpstk::Vector& concatenate(const gpstk::Vector& b)
   {
      (*$self) << b;
      return (*$self);
   }

   std::string __str__()
   {
      std::ostringstream ss;
      size_t i;
      for(i = 0; i < $self->size() - 1; i++)
         ss << (*($self))[i] << ", ";
      ss << (*($self))[i];
      return ss.str();
   }
}


// be careful with whitespace inside templates, SWIG needs the extra spacing
%template (ConstVectorBase_double) gpstk::ConstVectorBase< double,gpstk::Vector< double > >;
%template (RefVectorBase_double) gpstk::RefVectorBase< double,gpstk::Vector< double > >;
%template (vector) gpstk::Vector<double>;


%pythoncode %{
    vector.__len__ = lambda self: self.size()

    def __iter__(self):
        self.index = 0
        return self
    vector.__iter__ = __iter__

    def next(self):
          if self.index >= len(self):
            raise StopIteration
          else:
            self.index += 1
            return self[self.index - 1]
    vector.next = next
%}
