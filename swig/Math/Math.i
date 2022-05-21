%module Math

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
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%import "FFData.hpp"
%import "RinexObsBase.hpp"
%import "RinexObsData.hpp"
%import(module="gnsstk.GNSSCore") "ObsID.i"
%import "SvObsEpoch.hpp"

%include "AllanDeviation.hpp"
%include "MathBase.hpp"
%include "MiscMath.hpp"
%template(SimpleLagrangeInterpolation_double) gnsstk::SimpleLagrangeInterpolation<double>;
%template(LagrangeInterpolation_double) gnsstk::LagrangeInterpolation<double>;
%template(LagrangeInterpolating2ndDerivative_double) gnsstk::LagrangeInterpolating2ndDerivative<double>;
%template(RSS_double) gnsstk::RSS<double>;
%include "VectorBase.hpp"
%include "VectorBaseOperators.hpp"
%include "Vector.hpp"
%include "Stats.hpp"
   // Template Instantiations for non-member Stats Functions
%template(median_double) gnsstk::median<double>;
%template(median_int) gnsstk::median<int>;
%template(mad_double) gnsstk::mad<double>;
   // Stats Class
%template(Stats_double) gnsstk::Stats<double>;
%template(SeqStats_double) gnsstk::SeqStats<double>;
%template(WtdStats_double) gnsstk::WtdStats<double>;
%template(TwoSampleStats_double) gnsstk::TwoSampleStats<double>;
%include "BivarStats.hpp"
%template(BivarStats_double) gnsstk::BivarStats<double>;
%include "Expression.hpp"
%include "MatrixBase.hpp"
%include "MatrixBaseOperators.hpp"
%include "Matrix.hpp"
%include "Matrix.i"
%include "MatrixImplementation.hpp"
%include "MatrixFunctors.hpp"
%include "MatrixOperators.hpp"
%include "PolyFit.hpp"
%template(PolyFit_double) gnsstk::PolyFit<double>;
 //%include "PowerSum.hpp"
%include "Triple.i"
%include "RACRotation.hpp"
%include "VectorOperators.hpp"
// Include all the appropriate swig files for this subdirectory
 //%include "Triple.i"
%include "Vector.i"
 // This is included by gnsstk_swig.i to keep SWIG directives in proper order
/* %include "Matrix.i" */

// =============================================================
//  Section 11: Explicit Python wrappers
// =============================================================

%pythoncode %{


def eval(expr, **kwargs):
    e = Expression(expr)
    e.setGPSConstants()
    for key in kwargs:
        e.set(key, kwargs[key])
    return e.evaluate()

%}

// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================

STR_PRINT_HELPER(Expression)

STR_STREAM_HELPER(Stats)
STR_STREAM_HELPER(SeqStats)
STR_STREAM_HELPER(WtdStats)
STR_STREAM_HELPER(TwoSampleStats)
STR_STREAM_HELPER(BivarStats)
STR_STREAM_HELPER(Vector)
STR_STREAM_HELPER(Matrix)

STR_STREAM_VEC_HELPER(gnsstk::Triple)

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
