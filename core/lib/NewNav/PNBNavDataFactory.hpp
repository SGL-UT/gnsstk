#ifndef GPSTK_PNBNAVDATAFACTORY_HPP
#define GPSTK_PNBNAVDATAFACTORY_HPP

#include <memory>
#include <map>
#include "PackedNavBits.hpp"
#include "NavData.hpp"
#include "NavValidityType.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** This is the abstract base class for all PackedNavBits
       * decoders for theNavData tree.  Only one method is declared,
       * addData(), which is the intended interface to be used.  This
       * particular tree of classes implement the code necessary to
       * convert a stream of PackedNavBits objects into the
       * appropriate NavData objects.  This is not a 1:1
       * correlation. */
   class PNBNavDataFactory
   {
   public:
         /** Initialize navValidity to All and procNavTypes to all
          * known message types. */
      PNBNavDataFactory();

         /** Set the factory's handling of valid and invalid
          * navigation data.  This should be called before any addData()
          * calls.
          * @param[in] nvt The new nav data loading filter method. */
      virtual void setValidityFilter(NavValidityType nvt)
      { navValidity = nvt; }

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before any addData()
          * calls.
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      virtual void setTypeFilter(const NavMessageTypeSet& nmts);

         /** Process a PackedNavBits object, producing NavData objects
          * as appropriate.
          * @param[in] navIn The PackedNavBits data to process.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @return false on error. */
      virtual bool addData(const PackedNavBitsPtr& navIn,
                           NavDataPtrList& navOut) = 0;

   protected:
         /// Determines how the factory should filter added data.
      NavValidityType navValidity;

         /// If true, ephemeris data will be output by addData.
      bool processEph;
         /// If true, almanac data will be output by addData.
      bool processAlm;
         /// If true, health data will be output by addData.
      bool processHea;
         /// If true, time offset data will be output by addData.
      bool processTim;
   }; // class PNBNavDataFactory

      /// Managed pointer to a PNBNavDataFactory.
   using PNBNavDataFactoryPtr = std::shared_ptr<PNBNavDataFactory>;
      /// Map the navigation message type to a factory for producing that type.
   using PNBNavDataFactoryMap = std::map<NavType, PNBNavDataFactoryPtr>;

      //@}

} // namespace gpstk

#endif // GPSTK_PNBNAVDATAFACTORY_HPP
