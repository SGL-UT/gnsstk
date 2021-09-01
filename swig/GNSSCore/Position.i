namespace gpstk
{
   %extend Position
   {
         // Multiplies a position with a scalar
      Position __mul__(const double& d)
      {
         return (*($self)) * d;
      }

         // Adds two positions together
      Position __add__(const Position& p)
      {
         return (*($self)) + p;
      }

         // Subtracts two positions
      Position __sub__(const Position& p)
      {
         return (*($self)) - p;
      }
         // Exposes the range method as a method for Position
      double range(const Position& A, const Position& B)
      {
         return range(A,B);
      }
         // Conversion from Spherical to Cartesian
      static gpstk::Triple convertSphericalToCartesian(const gpstk::Triple& tpr)
      {
         gpstk::Triple xyz;
         gpstk::Position::convertSphericalToCartesian(tpr, xyz);
         return xyz;
      }
         // Conversion from Cartesian to Spherical
      static gpstk::Triple convertCartesianToSpherical(const gpstk::Triple& xyz)
      {
         gpstk::Triple tpr;
         gpstk::Position::convertCartesianToSpherical(xyz, tpr);
         return tpr;
      }
         // Conversion from Cartesian to Geodetic
      static gpstk::Triple convertCartesianToGeodetic( const Triple& xyz,
                                                       const double A,
                                                       const double eccSq)
      {
         gpstk::Triple llh;
         gpstk::Position::convertCartesianToGeodetic(xyz, llh, A, eccSq);
         return llh;
      }
         // Conversion from Geodetic to Cartesian
      static gpstk::Triple convertGeodeticToCartesian( const Triple& llh,
                                                       const double A,
                                                       const double eccSq)
      {
         gpstk::Triple xyz;
         gpstk::Position::convertGeodeticToCartesian(llh, xyz, A, eccSq);
         return xyz;
      }
         // Conversion from Cartesian to Geocentric
      static gpstk::Triple convertCartesianToGeocentric(const Triple& xyz)
      {
         gpstk::Triple llr;
         gpstk::Position::convertCartesianToGeocentric(xyz, llr);
         return llr;
      }
         // Conversion from Geocentric to Cartesian
      static gpstk::Triple convertGeocentricToCartesian(const gpstk::Triple& llr)
      {
         gpstk::Triple xyz;
         gpstk::Position::convertGeocentricToCartesian(llr, xyz);
         return xyz;
      }
         // Conversion from Geocentric to Geodetic
      static gpstk::Triple convertGeocentricToGeodetic( const Triple& llr,
                                                        const double A,
                                                        const double eccSq)
      {
         gpstk::Triple geodeticllh;
         gpstk::Position::convertGeocentricToGeodetic(llr, geodeticllh, A, eccSq);
         return geodeticllh;
      }
         // Conversion from Geodetic to Geocentric
      static gpstk::Triple convertGeodeticToGeocentric( const Triple& geodeticllh,
                                                        const double A,
                                                        const double eccSq)
      {
         gpstk::Triple llr;
         gpstk::Position::convertGeodeticToGeocentric(geodeticllh, llr, A, eccSq);
         return llr;
      }
   }
};
