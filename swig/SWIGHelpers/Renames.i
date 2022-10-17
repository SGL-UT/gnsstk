// Global (gnsstk-wide) %rename statements.
// Class-specific renames should go in class-specific .i files.

%rename(__str__) *::asString() const;
%rename(toCommonTime) *::convertToCommonTime() const;
 // Long doubles are used in several interfaces but
 // swig really doesn't handle them
%apply double { long double };
// avoid name conflicts with Python built-ins
%rename(lambda_) lambda;
