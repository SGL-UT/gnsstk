%include "NavID.hpp"

%pythoncode %{
# Define a hash to allow NavID objects to be used in sets/dicts   
def __NavID_hash(self):
   return (self.navType).__hash__()

NavID.__hash__ = __NavID_hash    
%}
