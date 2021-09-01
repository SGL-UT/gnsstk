// Rename a method so Python programmers don't have to type so much.
%rename(toCommonTime) *::convertToCommonTime() const;

// Include interface files needed to build TimeHandling
%include "TimeString.i"
