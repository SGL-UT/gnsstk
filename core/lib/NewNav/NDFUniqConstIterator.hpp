#ifndef GPSTK_NDFUNIQUECONSTITERATOR_HPP
#define GPSTK_NDFUNIQUECONSTITERATOR_HPP

#include <type_traits>

namespace gpstk
{
      /** This class provides a const iterator for iterating over
       * *unique* elements of a NavDataFactoryMap.  This is done by
       * wrapping a standard iterator, keeping track of the pointer
       * values when this iterator is dereferenced, and skipping over
       * iterators that have been dereferenced already.  It can be
       * used via:
       * \code
       * for (auto& i : NDFUniqConstIterator<NavDataFactoryMap>(factories))
       * {
       *    std::cerr << "  expecting to be unique: " << i.second << std::endl;
       * }
       * \endcode
       */
   template <typename T>
   class NDFUniqConstIterator
   {
   public:
      NDFUniqConstIterator()
            : map(nullptr)
      {}
      NDFUniqConstIterator(const T& omap)
            : nuit(omap.begin()), map(&omap)
      {}
      NDFUniqConstIterator(const T& omap, typename T::const_iterator i)
            : nuit(i), map(&omap)
      {}
      NDFUniqConstIterator& operator++()
      {
         nuit++;
         NavDataFactory *ndfp = dynamic_cast<NavDataFactory*>(
            nuit->second.get());
         while ((nuit != map->end()) && (uniques.count(ndfp) > 0))
         {
            nuit++;
            if (nuit != map->end())
            {
               ndfp = dynamic_cast<NavDataFactory*>(nuit->second.get());
            }
         }
         return *this;
      }
      const typename T::value_type& operator*()
      {         
         if (nuit != map->end())
         {
            NavDataFactory *ndfp = dynamic_cast<NavDataFactory*>(
               nuit->second.get());
            uniques.insert(ndfp);
         }
         return *nuit;
      }
      NDFUniqConstIterator begin()
      { return NDFUniqConstIterator(*map); }
      NDFUniqConstIterator end()
      { return NDFUniqConstIterator(*map, map->end()); }
      bool operator!=(const NDFUniqConstIterator& i)
      { return ((map != i.map) || (nuit != i.nuit)); }
   private:
         /// non-unique iterator.
      typename T::const_iterator nuit;
         /// Pointers already encountered.  This assumes a map to a shared_ptr.
      std::set<typename T::mapped_type::element_type*> uniques;
         /// Original map
      const T* map;
   };
}

#endif // GPSTK_NDFUNIQUECONSTITERATOR_HPP
