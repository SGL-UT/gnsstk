   // Allows cast from Rinex3NavData to EngEphemeris
%extend gnsstk::GloEphemeris
{
   GloEphemeris(const gnsstk::Rinex3NavData& d)
   {
      return new gnsstk::GloEphemeris(d);
   }
}
   // Allows cast from Rinex3NavData to GPSEphemeris
%extend gnsstk::BDSEphemeris
{
   BDSEphemeris(const gnsstk::Rinex3NavData& d)
   {
      return new gnsstk::BDSEphemeris(d);
   }
}
   // Allows cast from Rinex3NavData to EngEphemeris
%extend gnsstk::GalEphemeris
{
   GalEphemeris(const gnsstk::Rinex3NavData& d)
   {
      return new gnsstk::GalEphemeris(d);
   }
}
   // Allows cast from Rinex3NavData to GPSEphemeris
%extend gnsstk::QZSEphemeris
{
   QZSEphemeris(const gnsstk::Rinex3NavData& d)
   {
      return new gnsstk::QZSEphemeris(d);
   }
}

%extend gnsstk::EngEphemeris
{
   EngEphemeris(const gnsstk::Rinex3NavData& d)
   {
      return new gnsstk::EngEphemeris(d);
   }
}

%extend gnsstk::GPSEphemeris
{
   GPSEphemeris(const gnsstk::Rinex3NavData& d)
   {
      return new gnsstk::GPSEphemeris(d);
   }
}
