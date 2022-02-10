namespace gnsstk
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
      static gnsstk::Triple convertSphericalToCartesian(const gnsstk::Triple& tpr)
      {
         gnsstk::Triple xyz;
         gnsstk::Position::convertSphericalToCartesian(tpr, xyz);
         return xyz;
      }
         // Conversion from Cartesian to Spherical
      static gnsstk::Triple convertCartesianToSpherical(const gnsstk::Triple& xyz)
      {
         gnsstk::Triple tpr;
         gnsstk::Position::convertCartesianToSpherical(xyz, tpr);
         return tpr;
      }
         // Conversion from Cartesian to Geodetic
      static gnsstk::Triple convertCartesianToGeodetic( const Triple& xyz,
                                                       const double A,
                                                       const double eccSq)
      {
         gnsstk::Triple llh;
         gnsstk::Position::convertCartesianToGeodetic(xyz, llh, A, eccSq);
         return llh;
      }
         // Conversion from Geodetic to Cartesian
      static gnsstk::Triple convertGeodeticToCartesian( const Triple& llh,
                                                       const double A,
                                                       const double eccSq)
      {
         gnsstk::Triple xyz;
         gnsstk::Position::convertGeodeticToCartesian(llh, xyz, A, eccSq);
         return xyz;
      }
         // Conversion from Cartesian to Geocentric
      static gnsstk::Triple convertCartesianToGeocentric(const Triple& xyz)
      {
         gnsstk::Triple llr;
         gnsstk::Position::convertCartesianToGeocentric(xyz, llr);
         return llr;
      }
         // Conversion from Geocentric to Cartesian
      static gnsstk::Triple convertGeocentricToCartesian(const gnsstk::Triple& llr)
      {
         gnsstk::Triple xyz;
         gnsstk::Position::convertGeocentricToCartesian(llr, xyz);
         return xyz;
      }
         // Conversion from Geocentric to Geodetic
      static gnsstk::Triple convertGeocentricToGeodetic( const Triple& llr,
                                                        const double A,
                                                        const double eccSq)
      {
         gnsstk::Triple geodeticllh;
         gnsstk::Position::convertGeocentricToGeodetic(llr, geodeticllh, A, eccSq);
         return geodeticllh;
      }
         // Conversion from Geodetic to Geocentric
      static gnsstk::Triple convertGeodeticToGeocentric( const Triple& geodeticllh,
                                                        const double A,
                                                        const double eccSq)
      {
         gnsstk::Triple llr;
         gnsstk::Position::convertGeodeticToGeocentric(geodeticllh, llr, A, eccSq);
         return llr;
      }
   }
};
