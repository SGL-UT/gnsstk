%module TimeHandling

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
//  Section 5: Enumerations
// =============================================================

ENUM_MAPPER(gnsstk::TimeSystem, TimeSystem, "gnsstk")

// =============================================================
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

 // I don't know why some of these require the module option and others don't,
 // but without it, you get warnings saying to do it.
%import(module="gnsstk.gnsstk") "Exception.hpp"
%import "TimeSystemConverter.hpp"
%import "TimeSystem.hpp"

%include "TimeConstants.hpp"
%include "CommonTime.hpp"
%include "TimeTag.hpp"
%include "ANSITime.hpp"
%include "TimeConverters.hpp"
%include "Week.hpp"
%include "WeekSecond.hpp"
%include "BDSWeekSecond.hpp"
%include "CivilTime.hpp"
%include "GPSZcount.hpp"
%include "UnixTime.hpp"
%include "SystemTime.hpp"
%include "JulianDate.hpp"
%include "MJD.hpp"
%include "YDSTime.hpp"
%include "GPSWeek.hpp"
%include "GPSWeekZcount.hpp"
%include "GPSWeekSecond.hpp"
%include "GALWeekSecond.hpp"
%include "QZSWeekSecond.hpp"
/* %include "Epoch.hpp" */
%include "IRNWeekSecond.hpp"
%include "PosixTime.hpp"
%include "TimeCorrection.hpp"
%feature("flatnested");
%include "TimeRange.hpp"
%feature("flatnested", "");
%include "TimeString.i"

// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================
STR_STREAM_HELPER(GPSZcount)

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
