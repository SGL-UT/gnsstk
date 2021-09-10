   // Wrap () operator of CNavMsgSort
%extend gnsstk::CNavMsgSort
{
   bool __call__(const gnsstk::CNavFilterData* l, const gnsstk::CNavFilterData* r)
   {
      return (*($self))(l, r);
   }
}
