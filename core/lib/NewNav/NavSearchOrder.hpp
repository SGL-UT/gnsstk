#ifndef GPSTK_NAVSEARCHORDER_HPP
#define GPSTK_NAVSEARCHORDER_HPP

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Specify the behavior of nav data searches in NavLibrary/NavDataFactory
   enum class NavSearchOrder
   {
      User,   ///< Return the latest message before the search time.
      Nearest ///< Return the message closest to the search time.
   };

      //@}

}

#endif // GPSTK_NAVSEARCHORDER_HPP
