%module gpstk

// =============================================================
//  Section 0: C++ template containers & typedefs
// =============================================================

%{
// The header includes are kept in a separate file so they can be used
// to build other swig modules
#include "gpstk_swig.hpp"
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

%include "Enums.i"

// =============================================================
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 7: C++11 shared_ptr handling
// =============================================================

%include "NewNavSharedPtr.i"

// =============================================================
//  Section 8: C++ container template instances using only atomic types
// =============================================================

%include "STLTemplates.i"
 // This causes build errors downstream.
 // %include "Arrays.i"

// =============================================================
//  Section 9: typemaps that must be declared before the %includes
// =============================================================

 /** @todo Resolve the SWIG warnings caused by this include:
gpstk/core/lib/NewNav/NavLibrary.hpp:984: Warning 509: Overloaded method gpstk::NavLibrary::getOffset(gpstk::TimeSystem,gpstk::TimeSystem,gpstk::CommonTime const &,double &) effectively ignored,
gpstk/core/lib/NewNav/NavLibrary.hpp:964: Warning 509: as it is shadowed by gpstk::NavLibrary::getOffset(gpstk::TimeSystem,gpstk::TimeSystem,gpstk::CommonTime const &,gpstk::NavDataPtr &).
gpstk/core/lib/NewNav/NavLibrary.hpp:964: Warning 509: Overloaded method gpstk::NavLibrary::getOffset(gpstk::TimeSystem,gpstk::TimeSystem,gpstk::CommonTime const &,gpstk::NavDataPtr &,gpstk::SVHealth) effectively ignored,
gpstk/core/lib/NewNav/NavLibrary.hpp:984: Warning 509: as it is shadowed by gpstk::NavLibrary::getOffset(gpstk::TimeSystem,gpstk::TimeSystem,gpstk::CommonTime const &,double &,gpstk::SVHealth).
gpstk/core/lib/NewNav/NavLibrary.hpp:984: Warning 509: Overloaded method gpstk::NavLibrary::getOffset(gpstk::TimeSystem,gpstk::TimeSystem,gpstk::CommonTime const &,double &,gpstk::SVHealth,gpstk::NavValidityType) effectively ignored,
gpstk/core/lib/NewNav/NavLibrary.hpp:964: Warning 509: as it is shadowed by gpstk::NavLibrary::getOffset(gpstk::TimeSystem,gpstk::TimeSystem,gpstk::CommonTime const &,gpstk::NavDataPtr &,gpstk::SVHealth,gpstk::NavValidityType).
 */
%include "NewNavTypemaps.i"
%include "gpstk_typemaps.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%include "gpstk_swig.i"

// =============================================================
//  Section 11: Explicit Python wrappers
// =============================================================

// Encapsulation of many the __str__, __getitem__, etc. functions to
// avoid clutter.  When the only change to a class is adding a simple
// wrapper, add to pythonfunctions instead of creating another small
// file.
%include "pythonfunctions.i"
%include "Stream.i"

%include "FileHandling.i"
%include "Geomatics.i"
%include "GNSSCore.i"
%include "GNSSEph.i"
%include "Math.i"
%include "NavFilter.i"
%include "TimeHandling.i"
%include "Utilities.i"

// =============================================================
//  Section 12: Template declarations
// =============================================================

%include "STLHelpers.i"


// v 2.0.12 required for KLOBUCHAR IONO MODEL
// We need to provide methods that allow casting objects to their instance types.
#define KLOBUCHAR_SUPPORT (SWIG_VERSION >= 0x020012 ? True : False)
#if SWIG_VERSION >= 0x020012
%inline %{
  gpstk::OrbSysGpsC_30* cast_to_OrbSysGpsC_30(gpstk::OrbDataSys* ods) {
    return static_cast<gpstk::OrbSysGpsC_30*>(ods);
  }
  gpstk::OrbSysGpsL_56* cast_to_OrbSysGpsL_56(gpstk::OrbDataSys* ods) {
    return static_cast<gpstk::OrbSysGpsL_56*>(ods);
  }
%}
// Include to generate swig::trait::typename()
%{
  namespace swig {
    template <> struct traits<gpstk::OrbDataSys> {
        typedef pointer_category category;
        static const char* type_name() {return "gpstk::OrbDataSys";}
    };
  }
%}

// SWIG out the msgMap in gpstk::OrbSysStore
%template (TimeMsgMap) std::map<gpstk::CommonTime, gpstk::OrbDataSys* >;
%template (UIDMsgMap) std::map<uint16_t, std::map<gpstk::CommonTime, gpstk::OrbDataSys* > >;
%template (NavIDMsgMap) std::map<gpstk::NavID, std::map<uint16_t, std::map<gpstk::CommonTime, gpstk::OrbDataSys* > > >;
%template (SatIDMsgMap) std::map<gpstk::SatID, std::map<gpstk::NavID, std::map<uint16_t, std::map<gpstk::CommonTime, gpstk::OrbDataSys* > > > >;
#endif



// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================
%include "StringPrint.i"

// =============================================================
//  Section 14: Final clean-up
// =============================================================
%pythoncode %{
        # clean-up the dir listing by removing *_swigregister.
        import gpstk
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
