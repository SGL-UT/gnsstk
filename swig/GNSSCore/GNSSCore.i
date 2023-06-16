%module GNSSCore

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
%include "std_shared_ptr.i"

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

ENUM_MAPPER(gnsstk::SatelliteSystem, SatelliteSystem, "gnsstk")
ENUM_MAPPER(gnsstk::CarrierBand, CarrierBand, "gnsstk")
ENUM_MAPPER(gnsstk::TrackingCode, TrackingCode, "gnsstk")
ENUM_MAPPER(gnsstk::ObservationType, ObservationType, "gnsstk")
ENUM_MAPPER(gnsstk::CorrectorType, CorrectorType, "gnsstk")
ENUM_MAPPER(gnsstk::CorrDupHandling, CorrDupHandling, "gnsstk")

 // needs to be before RefFrameRlz.hpp at the very least.
%import "CommonTime.hpp"

%include "SatelliteSystem.hpp"
%include "SatTimeSystem.hpp"
%include "CarrierBand.hpp"
%include "TrackingCode.hpp"
%include "ObservationType.hpp"
%include "CorrectorType.hpp"
%include "CorrDupHandling.hpp"
%include "AngleType.hpp"
%include "XmitAnt.hpp"
%include "RefFrameSys.hpp"
%include "RefFrameRlz.hpp"

%include "renameEnums.i"
%pythoncode %{
   renameEnums('SatelliteSystem')
   renameEnums('CarrierBand')
   renameEnums('TrackingCode')
   renameEnums('ObservationType')
   renameEnums('CorrectorType')
   renameEnums('CorrDupHandling')
   renameEnums('AngleType')
   renameEnums('XmitAnt')
   renameEnums('RefFrameSys')
   renameEnums('RefFrameRlz')
%}
%include "cleanup.i"

// =============================================================
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 7: C++11 shared_ptr handling
// =============================================================

%include "std_shared_ptr.i"
%shared_ptr(gnsstk::GroupPathCorrector)
%shared_ptr(gnsstk::BCIonoCorrector)
%shared_ptr(gnsstk::BCISCorrector)
%shared_ptr(gnsstk::TropCorrector<gnsstk::ZeroTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::SimpleTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::SaasTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::NBTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::GGTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::GGHeightTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::NeillTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::GlobalTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::GCATTropModel>)
%shared_ptr(gnsstk::TropCorrector<gnsstk::MOPSTropModel>)
%shared_ptr(gnsstk::Transformer)
%shared_ptr(gnsstk::HelmertTransformer)

// =============================================================
//  Section 9: typemaps that must be declared before the %includes
// =============================================================

// correction is an output
%apply double &OUTPUT { double &corrOut };
%include "gnsstk_typemaps.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

 // I don't know why some of these require the module option and others don't,
 // but without it, you get warnings saying to do it.
%import(module="gnsstk") "Exception.hpp"
%import "EngNav.hpp"
%import "EngAlmanac.hpp"
%import "NavType.hpp"
%import "ReferenceFrame.hpp"
%import(module="gnsstk.Math") "Triple.hpp"
%import "FFData.hpp"
%import "RinexObsBase.hpp"
%import "RinexObsHeader.hpp"
%import "NavLibrary.hpp"
%import "Vector.hpp"
%import "Matrix.hpp"
%import "MetReader.hpp"

%include "AngleType.hpp"
%include "AngleReduced.hpp"
%include "Angle.hpp"
%include "EllipsoidModel.hpp"
%include "RefFrame.hpp"
%include "CGCS2000Ellipsoid.hpp"
%include "CarrierBand.hpp"
%template(std_map_CarrierBand_string) std::map<gnsstk::CarrierBand, std::string>;
%template(std_map_char_CarrierBand) std::map<char, gnsstk::CarrierBand>;
%template(std_map_CarrierBand_char) std::map<gnsstk::CarrierBand,char>;
%include "DeprecatedConsts.hpp"
%include "gps_constants.hpp"
%include "SatelliteSystem.hpp"
%template(std_vector_GNSS)       std::vector<gnsstk::SatelliteSystem>;
%include "SatID.i"
%template(std_vector_SatID)      std::vector<gnsstk::SatID>;
%include "FreqConsts.hpp"
%include "GNSSconstants.hpp"
%include "Xvt.hpp"
%include "ENUUtil.hpp"
%include "FreqConv.hpp"
%include "WxObsMap.hpp"
%include "Position.hpp"
%include "Position.i"
%include "TropModel.hpp"
%include "GCATTropModel.hpp"
%include "GGHeightTropModel.hpp"
%include "GGTropModel.hpp"
%include "WGS84Ellipsoid.hpp"
%include "GPSEllipsoid.hpp"
%include "GalileoEllipsoid.hpp"
%include "GalileoIonoEllipsoid.hpp"
%include "GlobalTropModel.hpp"
%include "Transformer.hpp"
%include "HelmertTransformer.hpp"
%feature("flatnested");
%include "IonoModel.hpp"
%feature("flatnested", "");
%feature("flatnested");
%include "IonoModelStore.hpp"
%feature("flatnested", "");
%include "MOPSTropModel.hpp"
%include "NBTropModel.hpp"
%include "NEDUtil.hpp"
%include "NeillTropModel.hpp"
%include "ObservationType.hpp"
%template(std_map_ObservationType_string) std::map<gnsstk::ObservationType, std::string>;
%template(std_map_char_ObservationType) std::map<char, gnsstk::ObservationType>;
%template(std_map_ObservationType_char) std::map<gnsstk::ObservationType,char>;
%include "TrackingCode.hpp"
%template(std_map_TrackingCode_string) std::map<gnsstk::TrackingCode, std::string>;
%template(std_map_char_TrackingCode) std::map<char, gnsstk::TrackingCode>;
%template(std_map_TrackingCode_char) std::map<gnsstk::TrackingCode,char>;
%include "XmitAnt.hpp"
%include "ObsID.i"
%template(std_vector_ObsID)      std::vector<gnsstk::ObsID>;
%include "OceanLoading.hpp"
%include "PZ90Ellipsoid.hpp"
%include "PoleTides.hpp"
%include "RinexObsID.i"
%template(std_vector_RinexObsID) std::vector<gnsstk::RinexObsID>;
%template(std_map_string_vector_RinexObsID) std::map<std::string, std::vector<gnsstk::RinexObsID> >;
%include "RinexSatID.i"
%template(std_vector_RinexSatID) std::vector<gnsstk::RinexSatID>;
%include "SaasTropModel.hpp"
%include "SatMetaData.hpp"
%include "SatMetaDataSort.i"
%feature("flatnested");
%include "SatMetaDataStore.hpp"
%feature("flatnested", "");
%include "SimpleTropModel.hpp"
%include "TransformLibrary.hpp"
%include "convhelp.hpp"
%include "GroupPathCorrector.hpp"
%template(GroupPathCorrectorList) std::list<std::shared_ptr<gnsstk::GroupPathCorrector> >;
%include "BCIonoCorrector.hpp"
%include "BCISCorrector.hpp"
%include "TropCorrector.hpp"
%template(ZeroTropCorrector) gnsstk::TropCorrector<gnsstk::ZeroTropModel>;
%template(SimpleTropCorrector) gnsstk::TropCorrector<gnsstk::SimpleTropModel>;
%template(SaasTropCorrector) gnsstk::TropCorrector<gnsstk::SaasTropModel>;
%template(NBTropCorrector) gnsstk::TropCorrector<gnsstk::NBTropModel>;
%template(GGTropCorrector) gnsstk::TropCorrector<gnsstk::GGTropModel>;
%template(GGHeightTropCorrector) gnsstk::TropCorrector<gnsstk::GGHeightTropModel>;
%template(NeillTropCorrector) gnsstk::TropCorrector<gnsstk::NeillTropModel>;
%template(GlobalTropCorrector) gnsstk::TropCorrector<gnsstk::GlobalTropModel>;
%template(GCATTropCorrector) gnsstk::TropCorrector<gnsstk::GCATTropModel>;
%template(MOPSTropCorrector) gnsstk::TropCorrector<gnsstk::MOPSTropModel>;
%include "CorrectionResult.hpp"
%template(CorrectionResultList) std::list<gnsstk::CorrectionResult>;
%include "CorrectionResults.hpp"
%include "GroupPathCorr.hpp"

// =============================================================
//  Section 11: Explicit Python wrappers
// =============================================================

%pythoncode %{

from gnsstk.RefTime import ReferenceFrame
import sys
if sys.version_info[0] < 3:
    from collections import Iterable
else:
    from collections.abc import Iterable
enum_vec_classes = [std_vector_GNSS]
for cls in enum_vec_classes:
    orig_constructor = cls.__init__
    def new_constructor(self, *args):
        # We assume that the argument is not exhaustible
        if len(args) == 1 and isinstance(args[0], Iterable) and all(isinstance(x, int) for x in args[0]):
            orig_constructor(self)
            for x in args[0]:
                self.append(x)
        else:
            orig_constructor(self, *args)
    cls.__init__ = new_constructor


def cartesian(x=0.0, y=0.0, z=0.0,
              model=None, frame=RefFrame()):
    "Returns a Position in the Cartesian coordinate system."
    return Position(x, y, z, Position.Cartesian, model, frame)


def geodetic(latitude=0.0, longitude=0.0, height=0.0,
             model=None, frame=RefFrame()):
    "Returns a Position in the Geodetic coordinate system."
    return Position(latitude, longitude, height, Position.Geodetic, model, frame)


def spherical(theta=0.0, phi=0.0, radius=0.0,
              model=None, frame=RefFrame()):
    "Returns a Position in the Spherical coordinate system."
    return Position(theta, phi, radius, Position.Spherical, model, frame)


def geocentric(latitude=0.0, longitude=0.0, radius=0.0,
               model=None, frame=RefFrame()):
    "Returns a Position in the Geocentric coordinate system."
    return Position(latitude, longitude, radius, Position.Geocentric, model, frame)


def poleTides(time, position, x, y):
    """
    Returns the effect (a gnsstk.Triple) of pole tides (meters)
    on the given position, in the Up-East-North (UEN) reference frame.
    This is a functional wrapper on the (hidden) PoleTides class.
    """
    return PoleTides().getPoleTide(time, position, x, y)

%}

// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================

STR_STREAM_HELPER(Position)
STR_STREAM_HELPER(SatMetaData)

AS_STRING_HELPER(ObsID)
AS_STRING_HELPER(SatID)
AS_STRING_HELPER(RinexSatID)
AS_STRING_HELPER2(RinexObsID)


// STR_STREAM_HELPER(Xvt)
// Q: Why is this (below) here instead of the macro for Xvt?
// A: There is an ambiguity issue for the operator<< for Xvt,
// see the end of TabularSatStore.hpp for a conflicting defintion
// of the Xvt out stream operator and Xvt.hpp+Xvt.cpp.
%extend gnsstk::Xvt {
   std::string __str__() {
      std::ostringstream os;
         os << "x:" << $self->x
            << ", v:" << $self->v
            << ", clk bias:" << $self->clkbias
            << ", clk drift:" << $self->clkdrift
            << ", relcorr:" << $self->relcorr;
      return os.str();
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
