%pythoncode %{
# Define a hash to allow RinexObsID objects to be used in sets/dicts
def __RinexObsID_hash(self):
   return (self.type, self.code, self.band).__hash__()
   
RinexObsID.__hash__ = __RinexObsID_hash
%}
