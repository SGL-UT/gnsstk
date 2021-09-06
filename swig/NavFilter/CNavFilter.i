   // Wrap () operator of CNavMsgSort
%extend gpstk::CNavMsgSort
{
   bool __call__(const gpstk::CNavFilterData* l, const gpstk::CNavFilterData* r)
   {
      return (*($self))(l, r);
   }
}
