namespace gnsstk
{
      // Wrap () operators for MetData operators
   %extend RinexMetDataFilterTime
   {
      bool __call__(const RinexMetData& l)
      {
         return (*($self))(l);
      }
   }
   %extend RinexMetDataOperatorLessThanFull
   {
      bool __call__(const RinexMetData& l, const RinexMetData& r)
      {
         return (*($self))(l, r);
      }
   }
   %extend RinexMetDataOperatorLessThanSimple
   {
      bool __call__(const RinexMetData& l, const RinexMetData& r)
      {
         return (*($self))(l, r);
      }
   }
   %extend RinexMetDataOperatorEqualsSimple
   {
      bool __call__(const RinexMetData& l, const RinexMetData& r)
      {
         return (*($self))(l, r);
      }
   }
   %extend RinexMetHeaderTouchHeaderMerge
   {
      bool __call__(const RinexMetHeader& l)
      {
         return (*($self))(l);
      }
   }
}
