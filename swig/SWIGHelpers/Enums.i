ENUM_MAPPER(gnsstk::SatelliteSystem, SatelliteSystem)
ENUM_MAPPER(gnsstk::CarrierBand, CarrierBand)
ENUM_MAPPER(gnsstk::TrackingCode, TrackingCode)
ENUM_MAPPER(gnsstk::ObservationType, ObservationType)
ENUM_MAPPER(gnsstk::NavType, NavType)
ENUM_MAPPER(gnsstk::TimeSystem, TimeSystem)
ENUM_MAPPER(gnsstk::ReferenceFrame, ReferenceFrame)
ENUM_MAPPER(gnsstk::NavMessageType, NavMessageType)

%include "SatelliteSystem.hpp"
%include "CarrierBand.hpp"
%include "TrackingCode.hpp"
%include "ObservationType.hpp"
%include "NavType.hpp"
%include "TimeSystem.hpp"
%include "ReferenceFrame.hpp"
%include "NavMessageType.hpp"
%include "SVHealth.hpp"
%include "NavSearchOrder.hpp"
%include "NavValidityType.hpp"
%include "DumpDetail.hpp"
%include "GalHealthStatus.hpp"
%include "GalDataValid.hpp"
%include "XmitAnt.hpp"
%include "AngleType.hpp"

// This code allows us to turn C++ enums into Python enums while
// maintaining compatibility in SWIG bindings.  We specifically use an
// IntEnum class in Python to allow the implicit conversion to int
// when calling C++ code from Python, as SWIG treats the enums as ints
// when generating code.
/** @note During the implementation of this code, Python would
 * commonly issue an exception with the text "Wrong number or type of
 * arguments for overloaded function".  This turned out to be caused
 * by SWIG not having knowledge of the C++ enumeration's definition
 * before generating code to use it, thus resulting in the enumeration
 * being handled like an object.  To resolve this issue, we have
 * inline forward declarations for the enums at the top of this
 * file.
 * However, this must occur _after_ the std*.i files are included, or it won't
 * be able to handle things like python string conversions. */
%pythoncode %{
from enum import IntEnum
def renameEnums(prefix):
    tmpD = {k:v for k,v in globals().items() if k.startswith(prefix+'_')}
    for k,v in tmpD.items():
        del globals()[k]
    tmpD = {k[len(prefix)+1:]:v for k,v in tmpD.items()}
    globals()[prefix] = IntEnum(prefix,tmpD)
    globals()[prefix].__str__ = lambda x: str(x.name)
# Turn the gnsstk.SatelliteSystem_* constants into a Python enum
renameEnums('SatelliteSystem')
renameEnums('CarrierBand')
renameEnums('TrackingCode')
renameEnums('ObservationType')
renameEnums('NavType')
renameEnums('TimeSystem')
renameEnums('ReferenceFrame')
renameEnums('NavMessageType')
renameEnums('SVHealth')
renameEnums('NavSearchOrder')
renameEnums('NavValidityType')
renameEnums('DumpDetail')
renameEnums('GalHealthStatus')
renameEnums('GalDataValid')
renameEnums('XmitAnt')
renameEnums('AngleType')
del renameEnums
del IntEnum
%}
