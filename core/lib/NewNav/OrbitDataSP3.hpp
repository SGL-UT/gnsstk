#ifndef GPSTK_ORBITDATASP3_HPP
#define GPSTK_ORBITDATASP3_HPP

#include <vector>
#include "OrbitData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class for orbit information using SP3 data tables.
   class OrbitDataSP3 : public OrbitData
   {
   public:
         /// Set the vector sizes and initialize everything to 0.
      OrbitDataSP3();

         /** Copy only the position, velocity and acceleration data.
          * @param[in] right The OrbitDataSP3 to copy XV data from.
          */
      void copyXV(const OrbitDataSP3& right);
         /** Copy only the clock correction data.
          * @param[in] right The OrbitDataSP3 to copy clock data from.
          */
      void copyT(const OrbitDataSP3& right);
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
         /** Returns the time for the data to be used when searching
          * in "Nearest" mode. */
      CommonTime getNearTime() const override
      { return timeStamp; }
         /** Print the contents of this NavData object in a
          * human-readable format.  Brief detail includes P/V.  Full
          * detail includes P/V/A and sigma.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, Detail dl) const override;
         /** Compute the satellites position and velocity at a time.
          * @note Defaults to using the GPS ellipsoid parameters.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt) override;

      Triple pos;      ///< ECEF position (km) of satellite at time.
      Triple posSig;   ///< Standard deviation of position.
      Triple vel;      ///< ECEF velocity (dm/s) of satellite at time.
      Triple velSig;   ///< Standard deviation of velocity.
      Triple acc;      ///< Acceleration (m/s/s) of satellite at time.
      Triple accSig;   ///< Standard deviation of acceleration.
      double clkBias;  ///< SV clock bias in microseconds.
      double biasSig;  ///< SV clock bias std deviation in microseconds.
      double clkDrift; ///< SV clock drift in s/s.
      double driftSig; ///< SV clock drift std deviation in microseconds/sec.
      double clkDrRate;///< SV clock drift rate in s/s**2.
      double drRateSig;///< SV clock drift rate std deviation.
   };

      //@}

}

#endif // GPSTK_ORBITDATASP3_HPP
