   // Wrap () operator of NavTimeSort
%extend gnsstk::NavTimeSort
{
   bool __call__(const gnsstk::NavFilterKey*const& l,
                 const gnsstk::NavFilterKey*const& r)
   {
      return (*($self))(l, r);
   }
}
