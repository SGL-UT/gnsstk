%include "RinexObsFilterOperators.hpp"

namespace gnsstk
{
      // Wrap () operators for ObsFilterOperators
   %extend RinexObsHeaderTouchHeaderMerge
   {
      bool __call__(const RinexObsHeader& l)
      {
         return (*($self))(l);
      }
   }
}
