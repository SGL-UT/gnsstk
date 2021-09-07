namespace gnsstk
{
      // Wraps overloaded () operator
   %extend BinexDataOperatorEquals
   {
      bool __call__(const BinexData& l, const BinexData& r)
      {
         return (*($self))(l, r);
      }
   }
}
