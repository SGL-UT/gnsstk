%include "ObsID.hpp"

%template(std_map_SvObsEpoch) std::map< gnsstk::ObsID, double >;

%pythoncode %{
# Define a hash to allow ObsID objects to be used in sets/dicts   
def __ObsID_hash(self):
   return (self.type, self.code, self.band).__hash__()

ObsID.__hash__ = __ObsID_hash    
%}
