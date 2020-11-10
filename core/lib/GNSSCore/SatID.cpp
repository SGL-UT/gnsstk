#include "SatID.hpp"

namespace gpstk
{
      /** @note Originally, extraMask was being set to 0, making it a
       * wildcard by default, but doing so means that nav store
       * searches will by default require linear searches through all
       * signal identifiers, which is not optimal.  The choice was
       * between making small changes to regression tests (that expect
       * the string representation of SatID to not include the "extra"
       * data) and having sub-optimal searches when looking for nav
       * data.  I opted for the former. */
   SatID ::
   SatID()
         :  id(-1), wildId(false), extra(0), extraMask(-1),
            system(SatelliteSystem::GPS), wildSys(false)
   {
   }


   SatID ::
   SatID(int p, SatelliteSystem s)
         : id(p), wildId(false), extra(0), extraMask(-1), system(s),
           wildSys(false)
   {
   }


   SatID ::
   SatID(int p, SatelliteSystem s, int64_t x, int64_t xm)
         : id(p), wildId(false), extra(x), extraMask(xm), system(s),
           wildSys(false)
   {
   }


   SatID ::
   SatID(int p)
         : id(p), wildId(false), extra(0), extraMask(-1),
           system(SatelliteSystem::Unknown), wildSys(true)
   {
   }


   SatID ::
   SatID(SatelliteSystem s)
         : id(0), wildId(true), extra(0), extraMask(-1),
           system(s), wildSys(false)
   {
   }


   void SatID ::
   makeWild()
   {
      wildId = wildSys = true;
      extraMask = 0;
   }


   bool SatID ::
   isWild() const
   {
      return (wildId || wildSys || (extraMask != -1));
   }


   void SatID ::
   dump(std::ostream& s) const
   {
      if (wildSys)
         s << "*";
      else
         s << convertSatelliteSystemToString(system);
      s << " ";
      if (wildId)
         s << "*";
      else
         s << id;
         // don't print extra if the mask is zero, to keep consistent
         // with earlier behavior.
      if (extraMask)
      {
         s << " ";
         s << (extra & extraMask);
      }
   }


   bool SatID ::
   operator==(const SatID& right) const
   {
      using namespace std;
         // combined mask, which basically means that a 0 in either
         // mask is a wildcard.
      int64_t cmask = extraMask & right.extraMask;
      return
         (wildId || right.wildId || (id == right.id)) &&
         (wildSys || right.wildSys || (system == right.system)) &&
         ((extra & cmask) == (right.extra & cmask));
   }


   bool SatID ::
   operator<(const SatID& right) const
   {
         // if either system is a wildcard, the systems are effectively equal
      if (!wildSys && !right.wildSys)
      {
         if (system < right.system)
            return true;
         if (right.system < system)
            return false;
      }
         // if either id is a wildcard, the ids are effectively equal
      if (!wildId && !right.wildId)
      {
         if (id < right.id)
            return true;
         if (right.id < id)
            return false;
      }
         // combined mask, which basically means that a 0 in either
         // mask is a wildcard.
      int64_t cmask = extraMask & right.extraMask;
      return ((extra & cmask) < (right.extra & cmask));
   }


   bool SatID :: isValid() const
   {
      switch(system)
      {
         case SatelliteSystem::GPS: return (id > 0 && id <= MAX_PRN);
               //case SatelliteSystem::Galileo:
               //case SatelliteSystem::Glonass:
               //case SatelliteSystem::Geosync:
               //case SatelliteSystem::LEO:
               //case SatelliteSystem::Transit:
         default: return (id > 0 && id < 100);
      }
   }
}
