%module NavFilter

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
//  Section 8: function class templates
// =============================================================

namespace std
{
   template <typename ArgumentType, typename ResultType> struct unary_function {};
   template <class Arg1, class Arg2, class Result> struct binary_function {};
}

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%import "CarrierBand.hpp"
%import "PackedNavBits.hpp"

%include "NavFilterKey.hpp"
%template(std_binary_function_NavFilterKey) std::binary_function<gnsstk::NavFilterKey *, gnsstk::NavFilterKey *, bool >;
%include "NavFilter.hpp"
%include "CNav2SanityFilter.hpp"
%template(std_binary_function_CNavFilterData) std::binary_function< gnsstk::CNavFilterData,gnsstk::CNavFilterData,bool >;
%template(std_binary_function_CNavFilterDataPtr) std::binary_function< gnsstk::CNavFilterData *, gnsstk::CNavFilterData *, bool >;
%include "CNavFilterData.hpp"
%include "CNavCookFilter.hpp"
%include "NavFilterMgr.hpp"
%include "CNavCrossSourceFilter.hpp"
/* %include "CNavEmptyFilter.hpp" */
%include "CNavParityFilter.hpp"
%include "CNavTOWFilter.hpp"
%include "GenericNavFilterData.hpp"
%template(std_binary_function_LNavFilterData) std::binary_function< gnsstk::LNavFilterData *, gnsstk::LNavFilterData *, bool >;
%include "LNavFilterData.i"
%include "LNavAlmValFilter.hpp"
%include "LNavCookFilter.hpp"
%include "LNavCrossSourceFilter.hpp"
%include "LNavEmptyFilter.hpp"
%include "LNavEphMaker.i"
%include "LNavOrderFilter.i"
%include "LNavParityFilter.hpp"
%include "LNavTLMHOWFilter.hpp"
%include "NavMsgData.hpp"
%include "NavMsgDataBits.hpp"
%include "NavMsgDataPNB.hpp"
%include "NavMsgDataWords.hpp"
%template(NavMsgDataWords_30) gnsstk::NavMsgDataWords<30>;
%include "NavOrderFilter.i"

%include "CNavFilter.i"

// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================

STR_DUMP_HELPER(CNavCrossSourceFilter)
/** @todo Check why this seg faults at least on objects created with
 * default constructor */
STR_DUMP_HELPER(CNavFilterData)
STR_DUMP_HELPER(GenericNavFilterData)

STR_DUMP_DETAIL_HELPER(NavMsgDataWords)
STR_DUMP_DETAIL_HELPER(NavMsgDataBits)
STR_DUMP_DETAIL_HELPER(NavMsgDataPNB)

STR_STREAM_HELPER(NavFilterKey)
/** @todo Check why this seg faults at least on objects created with
 * default constructor */
STR_STREAM_HELPER(LNavFilterData)

STR_FILTER_HELPER(CNavParityFilter)
STR_FILTER_HELPER(CNavCookFilter)
STR_FILTER_HELPER(CNavTOWFilter)
STR_FILTER_HELPER(CNav2SanityFilter)
STR_FILTER_HELPER(LNavAlmValFilter)
STR_FILTER_HELPER(LNavCookFilter)
STR_FILTER_HELPER(LNavCrossSourceFilter)
STR_FILTER_HELPER(LNavEmptyFilter)
STR_FILTER_HELPER(LNavEphMaker)
STR_FILTER_HELPER(LNavOrderFilter)
STR_FILTER_HELPER(LNavParityFilter)
STR_FILTER_HELPER(LNavTLMHOWFilter)

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
