#ifndef GPSTK_GPSCNAVREDALM_HPP
#define GPSTK_GPSCNAVREDALM_HPP

#include "GPSCNavAlm.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS CNav reduced almanac.
   class GPSCNavRedAlm : public GPSCNavAlm
   {
   public:
         /// Set data to default values.
      GPSCNavRedAlm();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Sets the fixed values (e.g. delta i) according to the
          * satellite system.  GPS has different reference values for
          * the reduced almanac from QZSS.  This method should be
          * called to fill out the remaining values in the reduced
          * almanac after loading the bits from the packet and setting
          * the signal. */
      void fixValues();

      double deltaA;      ///< Semi-major axis relative to reference value.
      double phi0;        ///< Argument of latitude at reference time (M0+w).
   };

      //@}

}

#endif // GPSTK_GPSCNAVREDALM_HPP
