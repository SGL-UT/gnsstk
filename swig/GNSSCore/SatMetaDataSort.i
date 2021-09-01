%extend gpstk::SatMetaDataSort
{
   bool __call__(const gpstk::SatMetaData& lhs, const gpstk::SatMetaData& rhs)
   {
      return (*($self))(lhs, rhs);
   }
}
