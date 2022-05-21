%include "FormattedDouble.hpp"

namespace gnsstk
{
   %extend FormattedDouble
   {
      double toDouble()
      {
         return double(*($self));
      }
   }
}
