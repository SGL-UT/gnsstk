   // Allows cast from RinexNavData and Rinex3NavData to EngEphemeris
%extend gpstk::EngEphemeris
{
   EngEphemeris(const gpstk::RinexNavData& d)
   {
      return new gpstk::EngEphemeris(d);
   }
}
   // Allows cast from RinexNavData and Rinex3NavData to GPSEphemeris
%extend gpstk::GPSEphemeris
{
   GPSEphemeris(const gpstk::RinexNavData& d)
   {
      return new gpstk::GPSEphemeris(d);
   }
}
