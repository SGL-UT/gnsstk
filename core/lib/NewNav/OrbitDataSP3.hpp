#ifndef GPSTK_ORBITDATASP3_HPP
#define GPSTK_ORBITDATASP3_HPP

#include <vector>
#include "OrbitData.hpp"

namespace gpstk
{
      /// Class for orbit information using SP3 data tables.
   class OrbitDataSP3 : public OrbitData
   {
   public:
         /// Set the vector sizes and initialize everything to 0.
      OrbitDataSP3();

         /** Checks the contents of this message.
          * @todo Determine and implement validity criteria.
          * @return true if this data is valid according to some criteria.
          */
      bool validate() const override
      { return true; }
         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.  This being
          * precise orbit data, the data is valid as of the
          * timestamp. */
      CommonTime getUserTime() const override
      { return timeStamp; }
         /** Print the contents of this NavData object in a
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, Detail dl) override;
         /** Compute the satellites position and velocity at a time.
          * @note Defaults to using the GPS ellipsoid parameters.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt) override;

      std::vector<double> pos;  ///< ECEF position (m) of satellite at time.
      std::vector<double> vel;  ///< ECEF velocity (m/s) of satellite at time.
      double clkBias;           ///< SV clock bias in microseconds.
      double clkDrift;          ///< SV clock drift in s/s.
   };
}

#endif // GPSTK_ORBITDATASP3_HPP
