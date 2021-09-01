   // Wrap () operator overloads
%extend gpstk::LNavEphMakerSort
{
   bool __call__(const gpstk::LNavFilterData*const &l,
                 const gpstk::LNavFilterData*const &r)
   {
      return (*($self))(l, r);
   }
}
