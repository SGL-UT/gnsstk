#ifndef GPSTK_NDFUNIQUEITERATOR_HPP
#define GPSTK_NDFUNIQUEITERATOR_HPP

#include <type_traits>

namespace gpstk
{
      /** This class provides a non-const iterator for iterating over
       * *unique* elements of a NavDataFactoryMap.  This is done by
       * wrapping a standard iterator, keeping track of the pointer
       * values when this iterator is dereferenced, and skipping over
       * iterators that have been dereferenced already.  It can be
       * used via:
       * \code
       * for (auto& i : NDFUniqIterator<NavDataFactoryMap>(factories))
       * {
       *    std::cerr << "  expecting to be unique: " << i.second << std::endl;
       * }
       * \endcode
       */
   template <typename T>
   class NDFUniqIterator
   {
   public:
      NDFUniqIterator()
            : map(nullptr)
      {}
      NDFUniqIterator(T& omap)
            : nuit(omap.begin()), map(&omap)
      {}
      NDFUniqIterator(T& omap, typename T::iterator i)
            : nuit(i), map(&omap)
      {}
      NDFUniqIterator& operator++()
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
      typename T::value_type& operator*()
      {
         if (nuit != map->end())
         {
            NavDataFactory *ndfp = dynamic_cast<NavDataFactory*>(
               nuit->second.get());
            uniques.insert(ndfp);
         }
         return *nuit;
      }
      NDFUniqIterator begin()
      { return NDFUniqIterator(*map); }
      NDFUniqIterator end()
      { return NDFUniqIterator(*map, map->end()); }
      bool operator!=(const NDFUniqIterator& i)
      { return ((map != i.map) || (nuit != i.nuit)); }
   private:
         /// non-unique iterator.
      typename T::iterator nuit;
         /// Pointers already encountered.  This assumes a map to a shared_ptr.
      std::set<typename T::mapped_type::element_type*> uniques;
         /// Original map
      T* map;
   };
}

#endif // GPSTK_NDFUNIQUEITERATOR_HPP
