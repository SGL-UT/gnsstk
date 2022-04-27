%include "SatID.hpp"

%pythoncode %{
# Define a hash to allow SatID objects to be used in sets/dicts   
def __SatID_hash(self):
   return (self.id, self.system).__hash__()

SatID.__hash__ = __SatID_hash
%}
