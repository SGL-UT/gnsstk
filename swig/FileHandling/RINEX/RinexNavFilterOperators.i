   // Wrap () operators for NavDataOperators
%extend gpstk::RinexNavDataOperatorLessThanFull
{
   bool __call__(const gpstk::RinexNavData& l, const gpstk::RinexNavData& r)
   {
      return (*($self))(l, r);
   }
}
%extend gpstk::RinexNavDataOperatorEqualsFull
{
   bool __call__(const gpstk::RinexNavData& l, const gpstk::RinexNavData& r)
   {
      return (*($self))(l, r);
   }
}
%extend gpstk::RinexNavDataOperatorLessThanSimple
{
   bool __call__(const gpstk::RinexNavData& l, const gpstk::RinexNavData& r)
   {
      return (*($self))(l, r);
   }
}
%extend gpstk::RinexNavHeaderTouchHeaderMerge
{
   bool __call__(const gpstk::RinexNavHeader& l)
   {
      return (*($self))(l);
   }
}
%extend gpstk::RinexNavDataFilterPRN
{
   bool __call__(const gpstk::RinexNavData& l)
   {
      return (*($self))(l);
   }
}
