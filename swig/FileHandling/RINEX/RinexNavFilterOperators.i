%include "RinexNavFilterOperators.hpp"

   // Wrap () operators for NavDataOperators
%extend gnsstk::RinexNavDataOperatorLessThanFull
{
   bool __call__(const gnsstk::RinexNavData& l, const gnsstk::RinexNavData& r)
   {
      return (*($self))(l, r);
   }
}
%extend gnsstk::RinexNavDataOperatorEqualsFull
{
   bool __call__(const gnsstk::RinexNavData& l, const gnsstk::RinexNavData& r)
   {
      return (*($self))(l, r);
   }
}
%extend gnsstk::RinexNavDataOperatorLessThanSimple
{
   bool __call__(const gnsstk::RinexNavData& l, const gnsstk::RinexNavData& r)
   {
      return (*($self))(l, r);
   }
}
%extend gnsstk::RinexNavHeaderTouchHeaderMerge
{
   bool __call__(const gnsstk::RinexNavHeader& l)
   {
      return (*($self))(l);
   }
}
%extend gnsstk::RinexNavDataFilterPRN
{
   bool __call__(const gnsstk::RinexNavData& l)
   {
      return (*($self))(l);
   }
}
