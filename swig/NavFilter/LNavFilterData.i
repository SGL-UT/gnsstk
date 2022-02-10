%extend gnsstk::LNavMsgSort
{
   bool __call__(const gnsstk::LNavFilterData*const &l,
                 const gnsstk::LNavFilterData*const &r)
   {
      return (*($self))(l, r);
   }
}
