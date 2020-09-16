#ifndef GPSTK_ORBITDATA_HPP
#define GPSTK_ORBITDATA_HPP

#include "NavData.hpp"
#include "Xvt.hpp"

namespace gpstk
{
      /** Abstract base class for classes that compute satellite
       * positions.  Only the interface is defined as some systems use
       * Keplerian orbital elements, while others use tables. */
   class OrbitData : public NavData
   {
   public:
         /** Compute the satellites position and velocity at a time.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      virtual bool getXvt(const CommonTime& when, Xvt& xvt) = 0;
   };
}

#endif // GPSTK_ORBITDATA_HPP
