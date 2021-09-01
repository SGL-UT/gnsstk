namespace gpstk
{
   %extend FormattedDouble
   {
      double toDouble()
      {
         return double(*($self));
      }
   }
}
