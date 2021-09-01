namespace gpstk
{
      // Wrap () operators for ObsFilterOperators
   %extend Rinex3ObsDataOperatorLessThanFull
   {
      bool __call__(const Rinex3ObsData& l, const Rinex3ObsHeader& lheader,
                    const Rinex3ObsData& r, const Rinex3ObsHeader& rheader,
                    double epsilon)
      {
         return (*($self))(l, lheader, r, rheader, epsilon);
      }
   }
   %extend Rinex3ObsDataOperatorLessThanSimple
   {
      bool __call__(const Rinex3ObsData& l, const Rinex3ObsData& r)
      {
         return (*($self))(l, r);
      }
   }
   %extend Rinex3ObsDataOperatorEqualsSimple
   {
      bool __call__(const Rinex3ObsData& l, const Rinex3ObsData& r)
      {
         return (*($self))(l, r);
      }
   }
   %extend Rinex3ObsHeaderTouchHeaderMerge
   {
      bool __call__(const Rinex3ObsHeader &l)
      {
         return (*($self))(l);
      }
   }
}
