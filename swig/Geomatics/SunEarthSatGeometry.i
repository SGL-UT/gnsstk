// Apply directives allow for C++ -> Python function outputs to be
// accessed, i.e any C++ double& sf as a method parameter will be
// converted to an output value in Python
%apply double& OUTPUT {double& sf};
%apply double& OUTPUT {double& nadir, double& azimuth};
%apply double& OUTPUT {double& beta, double& phi};
%apply double& OUTPUT {double& yawrate};

// The apply directives will be applied to all files below them.
%include "SunEarthSatGeometry.hpp"

// Clear directives remove the apply directives that have been
// previously defined.
%clear double& sf;
%clear double& nadir, double& azimuth;
%clear double& beta, double& phi;
%clear double& yawrate;
