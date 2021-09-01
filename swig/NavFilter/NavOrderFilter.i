   // Wrap () operator of NavTimeSort
%extend gpstk::NavTimeSort
{
   bool __call__(const gpstk::NavFilterKey*const& l,
                 const gpstk::NavFilterKey*const& r)
   {
      return (*($self))(l, r);
   }
}
