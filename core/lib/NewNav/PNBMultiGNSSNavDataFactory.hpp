#ifndef GPSTK_PNBMULTIGNSSNAVDATAFACTORY_HPP
#define GPSTK_PNBMULTIGNSSNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Define a wrapper class for all known PNBNavDataFactory
       * classes, allowing a single class to be used to handle all
       * GNSSes. 
       * @warning Because the factories used by this class are
       *   maintained in a static object, the validity filters and
       *   type filters are essentially "non-volatile" across object
       *   instantiations.  That is, if you create one
       *   PNBMultiGNSSNavDataFactory, change the filter settings,
       *   destroy it and create another, the filter settings will be
       *   the same as they were in the destroyed object.  Do not
       *   expect the filter settings to be reinitialized with each
       *   constructed PNBMultiGNSSNavDataFactory. */
   class PNBMultiGNSSNavDataFactory : public PNBNavDataFactory
   {
   public:
         /** Set the factories' handling of valid and invalid
          * navigation data.  This should be called before any addData()
          * calls.
          * @param[in] nvt The new nav data loading filter method. */
      void setValidityFilter(NavValidityType nvt) override;

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before any addData()
          * calls.
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      void setTypeFilter(const NavMessageTypeSet& nmts) override;

         /** Process a PackedNavBits object, producing NavData objects
          * as appropriate.
          * @param[in] navIn The PackedNavBits data to process.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @return false on error. */
      bool addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
         override;

         /** Add a new factory to the support map.  The factory must
          * be derived from PNBNavDataFactory but not an instance of
          * this class.
          * @param[in] navType The nav message that the factory processes.
          * @param[in] fact The PNBNavDataFactory object to add.
          * @return false if fact is not a valid factory class.
          */
      static bool addFactory(NavType navType, PNBNavDataFactoryPtr& fact);

   protected:
         /** Known PNB -> nav data factories, organized by navigation
          * message type.  Declared static so that the user doesn't
          * have to add all the factories themselves. */
      static PNBNavDataFactoryMap factories;
   }; // class PNBMultiGNSSNavDataFactory

      //@}

} // namespace gpstk

#endif // GPSTK_PNBMULTIGNSSNAVDATAFACTORY_HPP
