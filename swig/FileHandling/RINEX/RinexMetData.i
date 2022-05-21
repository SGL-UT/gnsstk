%include "RinexMetData.hpp"

    // Add lookup method to look up data given an integer form of an enum
%extend gnsstk::RinexMetData {
   double _lookup_data(int x) {
      return $self->data[static_cast<gnsstk::RinexMetHeader::RinexMetType>(x)];
   }
}

%pythoncode %{
def _RinexMetData_getData(self):
    """
    Returns a dict of the (int->double) data stored.
    The ints are drawn from an enum defined in RinexMetHeader.
    For example, to get the pressure you would use:
        d = myMetData.getData()
        print d[gnsstk.RinexMetHeader.PR]
    """
    data = {}
    for i in xrange(RinexMetHeader.HI + 1):
        data[i] = self._lookup_data(i)
    return data

RinexMetData.getData = _RinexMetData_getData
%}
