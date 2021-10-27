// Apply directives allow for C++ -> Python function outputs to be
// accessed, i.e any C++ double& sf as a method parameter will be
// converted to an output value in Python
%apply double& OUTPUT {double& AR};
%apply double& OUTPUT {double& lat, double& lon};

// The apply directives will be applied to all files below them.
%include "SolarPosition.hpp"

// Clear directives remove the apply directives that have been
// previously defined.
%clear double& AR;
%clear double& lat, double& lon;
