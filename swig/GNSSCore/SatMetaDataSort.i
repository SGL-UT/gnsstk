%extend gnsstk::SatMetaDataSort
{
   bool __call__(const gnsstk::SatMetaData& lhs, const gnsstk::SatMetaData& rhs)
   {
      return (*($self))(lhs, rhs);
   }
}
