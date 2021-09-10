%module gnsstk

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
gnsstk/core/lib/NewNav/NavLibrary.hpp:984: Warning 509: Overloaded method gnsstk::NavLibrary::getOffset(gnsstk::TimeSystem,gnsstk::TimeSystem,gnsstk::CommonTime const &,double &) effectively ignored,
gnsstk/core/lib/NewNav/NavLibrary.hpp:964: Warning 509: as it is shadowed by gnsstk::NavLibrary::getOffset(gnsstk::TimeSystem,gnsstk::TimeSystem,gnsstk::CommonTime const &,gnsstk::NavDataPtr &).
gnsstk/core/lib/NewNav/NavLibrary.hpp:964: Warning 509: Overloaded method gnsstk::NavLibrary::getOffset(gnsstk::TimeSystem,gnsstk::TimeSystem,gnsstk::CommonTime const &,gnsstk::NavDataPtr &,gnsstk::SVHealth) effectively ignored,
gnsstk/core/lib/NewNav/NavLibrary.hpp:984: Warning 509: as it is shadowed by gnsstk::NavLibrary::getOffset(gnsstk::TimeSystem,gnsstk::TimeSystem,gnsstk::CommonTime const &,double &,gnsstk::SVHealth).
gnsstk/core/lib/NewNav/NavLibrary.hpp:984: Warning 509: Overloaded method gnsstk::NavLibrary::getOffset(gnsstk::TimeSystem,gnsstk::TimeSystem,gnsstk::CommonTime const &,double &,gnsstk::SVHealth,gnsstk::NavValidityType) effectively ignored,
gnsstk/core/lib/NewNav/NavLibrary.hpp:964: Warning 509: as it is shadowed by gnsstk::NavLibrary::getOffset(gnsstk::TimeSystem,gnsstk::TimeSystem,gnsstk::CommonTime const &,gnsstk::NavDataPtr &,gnsstk::SVHealth,gnsstk::NavValidityType).
 */
%include "NewNavTypemaps.i"
%include "gnsstk_typemaps.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%include "gnsstk_swig.i"

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
  gnsstk::OrbSysGpsC_30* cast_to_OrbSysGpsC_30(gnsstk::OrbDataSys* ods) {
    return static_cast<gnsstk::OrbSysGpsC_30*>(ods);
  }
  gnsstk::OrbSysGpsL_56* cast_to_OrbSysGpsL_56(gnsstk::OrbDataSys* ods) {
    return static_cast<gnsstk::OrbSysGpsL_56*>(ods);
  }
%}
// Include to generate swig::trait::typename()
%{
  namespace swig {
    template <> struct traits<gnsstk::OrbDataSys> {
        typedef pointer_category category;
        static const char* type_name() {return "gnsstk::OrbDataSys";}
    };
  }
%}

// SWIG out the msgMap in gnsstk::OrbSysStore
%template (TimeMsgMap) std::map<gnsstk::CommonTime, gnsstk::OrbDataSys* >;
%template (UIDMsgMap) std::map<uint16_t, std::map<gnsstk::CommonTime, gnsstk::OrbDataSys* > >;
%template (NavIDMsgMap) std::map<gnsstk::NavID, std::map<uint16_t, std::map<gnsstk::CommonTime, gnsstk::OrbDataSys* > > >;
%template (SatIDMsgMap) std::map<gnsstk::SatID, std::map<gnsstk::NavID, std::map<uint16_t, std::map<gnsstk::CommonTime, gnsstk::OrbDataSys* > > > >;
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
