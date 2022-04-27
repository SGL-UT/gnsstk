%include "Rinex3NavFilterOperators.hpp"

namespace gnsstk
{
      // Wrap () operators for NavDataOperators
   %extend Rinex3NavDataOperatorLessThanFull
   {
      bool __call__(const Rinex3NavData &l, const Rinex3NavData &r)
      {
         return (*($self))(l, r);
      }
   }
   %extend Rinex3NavDataOperatorEqualsFull
   {
      bool __call__(const Rinex3NavData &l, const Rinex3NavData &r)
      {
         return (*($self))(l, r);
      }
   }
   %extend Rinex3NavDataOperatorLessThanSimple
   {
      bool __call__(const Rinex3NavData &l, const Rinex3NavData &r)
      {
         return (*($self))(l, r);
      }
   }
   %extend Rinex3NavHeaderTouchHeaderMerge
   {
      bool __call__(const Rinex3NavHeader &l)
      {
         return (*($self))(l);
      }
   }
   %extend Rinex3NavDataFilterPRN
   {
      bool __call__(const Rinex3NavData &l)
      {
         return (*($self))(l);
      }
   }
}
