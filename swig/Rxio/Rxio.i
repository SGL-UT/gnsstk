%module Rxio

%pythonbegin %{
from __future__ import absolute_import
%}

// =============================================================
//  Section 0: C++ template containers & typedefs
// =============================================================

%{
// The header includes are kept in a separate file so they can be used
// to build other swig modules
#include "gnsstk_swig.hpp"
%}


// =============================================================
//  Section 1: C++ template containers & typedefs
// =============================================================
%feature("autodoc", "1");
%include "doc/doc.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_map.i"
%include "std_list.i"
%include "std_set.i"
%include "std_multimap.i"
%include "stdint.i"
%include "typemaps.i"
%include "exception.i"
%include "carrays.i"

// =============================================================
//  Section 2: Macros
// =============================================================
// Must be included here so that everything else has access to them.
%include "Macros.i"

// =============================================================
//  Section 3: Renaming C++ methods and types to Python
// =============================================================

%include "Renames.i"

// =============================================================
//  Section 4: C++ things to ignore when generating code
// =============================================================

%include "Ignore.i"

// =============================================================
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 8: C++ container template instances using only atomic types
// =============================================================

%import "STLTemplates.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%import(module="gnsstk.FileHandling") "FFData.hpp"
%import "FFStream.hpp"
%import(module="gnsstk.FileHandling") "FFBinaryStream.hpp"

%include "DataStatus.hpp"
%include "AshtechData.hpp"
%include "AshtechALB.hpp"
%include "AshtechEPB.hpp"
%feature("flatnested");
%include "AshtechMBEN.hpp"
%feature("flatnested", "");
%include "AshtechPBEN.hpp"
%include "AshtechStream.hpp"

// =============================================================
//  Section 11: Explicit Python wrappers
// =============================================================

%extend gnsstk::AshtechStream
{
   // methods for the stream itself:
   static gnsstk::AshtechStream* inAshtechStream(const std::string fileName)
   {
      gnsstk::AshtechStream *s = new gnsstk::AshtechStream (fileName.c_str());
      return s;
   }

   static gnsstk::AshtechStream* outAshtechStream(const std::string fileName)
   {
      gnsstk::AshtechStream *s = new gnsstk::AshtechStream(
         fileName.c_str(), std::ios::out|std::ios::trunc);
      return s;
   }

   static void _remove(gnsstk::AshtechStream *ptr)
   {
      delete ptr;
   }

   // reader functions:
   gnsstk::AshtechData readHeader()
   {
      gnsstk::AshtechData head;
      (*($self)) >> head;
      return head;
   }

   gnsstk::AshtechData readData()
   {
      gnsstk::AshtechData data;
      if( (*($self)) >> data )
      {
         return data;
      }
      else
      {
         gnsstk::EndOfFile e("AshtechStream reached an EOF.");
         GNSSTK_THROW(e);
      }
   }
}

// =============================================================
//  Section 14: Final clean-up
// =============================================================
%pythoncode %{
        # clean-up the dir listing by removing *_swigregister.
        import gnsstk
        stuff=None
        to_remove=[]
        for stuff in locals():
            if stuff.endswith('_swigregister'):
                to_remove.append(stuff)
        for stuff in to_remove:
            del locals()[stuff]
        del locals()['stuff']
        del locals()['to_remove']
    %}
