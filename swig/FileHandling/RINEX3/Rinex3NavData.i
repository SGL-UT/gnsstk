   // Allows cast from Rinex3NavData to EngEphemeris
%extend gpstk::GloEphemeris
{
   GloEphemeris(const gpstk::Rinex3NavData& d)
   {
      return new gpstk::GloEphemeris(d);
   }
}
   // Allows cast from Rinex3NavData to GPSEphemeris
%extend gpstk::BDSEphemeris
{
   BDSEphemeris(const gpstk::Rinex3NavData& d)
   {
      return new gpstk::BDSEphemeris(d);
   }
}
   // Allows cast from Rinex3NavData to EngEphemeris
%extend gpstk::GalEphemeris
{
   GalEphemeris(const gpstk::Rinex3NavData& d)
   {
      return new gpstk::GalEphemeris(d);
   }
}
   // Allows cast from Rinex3NavData to GPSEphemeris
%extend gpstk::QZSEphemeris
{
   QZSEphemeris(const gpstk::Rinex3NavData& d)
   {
      return new gpstk::QZSEphemeris(d);
   }
}

%extend gpstk::EngEphemeris
{
   EngEphemeris(const gpstk::Rinex3NavData& d)
   {
      return new gpstk::EngEphemeris(d);
   }
}

%extend gpstk::GPSEphemeris
{
   GPSEphemeris(const gpstk::Rinex3NavData& d)
   {
      return new gpstk::GPSEphemeris(d);
   }
}
