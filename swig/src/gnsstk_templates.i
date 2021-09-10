%{
#include "ValidType.hpp"
#include "Matrix.hpp"
%}

%include "ValidType.hpp"
%include "Matrix.hpp"

// ValidType wraps
%template(validtype_float) gnsstk::ValidType<float>;
%template(validtype_double) gnsstk::ValidType<double>;
%template(validtype_char) gnsstk::ValidType<char>;
%template(validtype_short) gnsstk::ValidType<short>;
%template(validtype_int) gnsstk::ValidType<int>;
%template(validtype_long) gnsstk::ValidType<long>;
%template(validtype_uchar) gnsstk::ValidType<unsigned char>;
%template(validtype_ushort) gnsstk::ValidType<unsigned short>;
%template(validtype_uint) gnsstk::ValidType<unsigned int>;
%template(validtype_ulong) gnsstk::ValidType<unsigned long>;

// Matrix wraps
