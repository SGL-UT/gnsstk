//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
//
//==============================================================================


//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================
#ifndef GNSSTK_NDFUNIQUEITERATOR_HPP
#define GNSSTK_NDFUNIQUEITERATOR_HPP

#include <type_traits>

namespace gnsstk
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

#endif // GNSSTK_NDFUNIQUEITERATOR_HPP
