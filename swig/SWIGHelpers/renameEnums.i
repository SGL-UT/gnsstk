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
%}
