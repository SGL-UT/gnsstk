%extend gpstk::LNavTimeSort
{
   bool __call__(const gpstk::LNavFilterData*const &l,
                 const gpstk::LNavFilterData*const &r)
   {
      return (*($self))(l, r);
   }
}
