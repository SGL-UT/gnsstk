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
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
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

/// @file ObsID.hpp
/// gnsstk::ObsID - navigation system, receiver, and file specification
/// independent representation of the types of observation data that can
/// be collected.  This class is analogous to the RinexObsType class that
/// is used to represent the observation codes in a RINEX file. It is
/// intended to support at least everything in section 5.1 of the RINEX 3
/// specifications.

#ifndef OBSID_HPP
#define OBSID_HPP

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>

#include "gnsstk_export.h"
#include "Exception.hpp"
#include "SatID.hpp"
#include "Rinex3ObsBase.hpp"
#include "ObservationType.hpp"
#include "CarrierBand.hpp"
#include "TrackingCode.hpp"
#include "XmitAnt.hpp"

// forward declaration of test class
class ObsID_T;

namespace gnsstk
{
   class ObsID;
   namespace StringUtils
   {
         /// convert this object to a string representation
      std::string asString(const ObsID& id);
   }


      /// stream output for ObsID
   std::ostream& operator<<(std::ostream& s, const ObsID& p);


   class ObsID
   {
   public:
         /// empty constructor, creates a wildcard object.
         /// mcode and freqOffs are kept wild by default intentionally.
      ObsID()
            : type(ObservationType::Unknown), band(CarrierBand::Unknown),
              code(TrackingCode::Unknown), freqOffs(0), freqOffsWild(true),
              mcode(0), mcodeMask(0), xmitAnt(XmitAnt::Any)
      {}

         /// Explicit constructor
         /// mcode and freqOffs are kept wild by default intentionally.
      ObsID(ObservationType ot, CarrierBand cb, TrackingCode tc,
            XmitAnt transmitter = XmitAnt::Any)
            : type(ot), band(cb), code(tc), freqOffs(0), freqOffsWild(true),
              mcode(0), mcodeMask(0), xmitAnt(transmitter)
      {}

         /** Explicit constructor for GLONASS.
          * @param[in] ot The observation type (range, phase,, etc.).
          * @param[in] cb The carrier band (L1, L2, etc.).
          * @param[in] tc The tracking code (CA, L2CM, etc.).
          * @param[in] fo The frequency offset of the GLONASS signal.
          * @param[in] fw If true, fo is ignored and freqOffs will
          *   match any value. */
      explicit ObsID(ObservationType ot, CarrierBand cb, TrackingCode tc,
                     int fo, XmitAnt transmitter = XmitAnt::Any,
                     bool fw = false)
            : type(ot), band(cb), code(tc), freqOffs(fo), freqOffsWild(fw),
              mcode(0), mcodeMask(0), xmitAnt(transmitter)
      {}

         /** Explicit constructor for avoiding wildcard values.
          * @param[in] ot The observation type (range, phase,, etc.).
          * @param[in] cb The carrier band (L1, L2, etc.).
          * @param[in] tc The tracking code (CA, L2CM, etc.).
          * @param[in] fo The frequency offset of the GLONASS signal.
          * @param[in] mc The mcode bits to match.
          * @param[in] fw If true, fo is ignored and freqOffs will
          *   match any value. */
      explicit ObsID(ObservationType ot, CarrierBand cb, TrackingCode tc,
                     int fo, uint32_t mc,
                     XmitAnt transmitter = XmitAnt::Standard, bool fw = false)
            : type(ot), band(cb), code(tc), freqOffs(fo), freqOffsWild(fw),
              mcode(mc), mcodeMask(-1), xmitAnt(transmitter)
      {}

         /// Equality requires all fields to be the same
      virtual bool operator==(const ObsID& right) const;

         /** This ordering is somewhat arbitrary but is required to be
          * able to use an ObsID as an index to a std::map. If an
          * application needs some other ordering, inherit and
          * override this function. One 'feature' that has been added
          * is that an Any code/carrier/type will match any other
          * code/carrier/type in the equality operator. The intent is
          * to support performing an operation like "tell me if this
          * is a pseudorange that was collected on L1 from *any*
          * code". */
      virtual bool operator<(const ObsID& right) const;

      bool operator!=(const ObsID& right) const
      { return !(operator==(right)); };

      bool operator>(const ObsID& right) const
      {  return (!operator<(right) && !operator==(right)); };

      bool operator<=(const ObsID& right) const
      { return (operator<(right) || operator==(right)); };

      bool operator>=(const ObsID& right) const
      { return !(operator<(right)); };

         /// Convenience output method
      virtual std::ostream& dump(std::ostream& s) const;

         /// Destructor
      virtual ~ObsID()
      {}

         /// Set all data to wildcard values.
      void makeWild();

         /// Return true if any of the data are wildcard values.
      bool isWild() const;

         /// Set the value of mcode while simultaneously setting the mask.
      void setMcodeBits(uint32_t newval, uint32_t newmask = -1)
      { mcode = newval; mcodeMask = newmask; }
         /** Alter the value of mcode by replacing only the masked
          * bits in newmask with the value in newval.
          * @param[in] newval The data to be inserted into the mcode bitflag.
          * @param[in] newmask Only bits set in newmask will be set in
          *   mcode, and mcodeMask will be updated to include set bits
          *   in newmask.
          * @post mcode = (mcode & ~newmask) | (newval & newmask)
          * @post mcodeMask |= newmask */
      void maskMcodeBits(uint32_t newval, uint32_t newmask)
      {
         mcode = (mcode & ~newmask) | (newval & newmask);
         mcodeMask |= newmask;
      }
         /// Return the value of mcode
      uint32_t getMcodeBits() const
      { return mcode; }
         /// Set the value of mcodeMask on its own.
      void setMcodeMask(uint32_t newmask = -1)
      { mcodeMask = newmask; }
         /// Clear bits in mcodeMask that are set in clearmask.
      void clearMcodeMask(uint32_t clearmask)
      { mcodeMask = mcodeMask & ~clearmask; }
         /// Return the value of mcodeMask
      uint32_t getMcodeMask() const
      { return mcodeMask; }

         // Note that these are the only data members of objects of this class.
      ObservationType  type;
      CarrierBand      band;
      TrackingCode     code;
      XmitAnt xmitAnt;    ///< Identify the transmitting antenna.
      int freqOffs;       ///< GLONASS frequency offset.
      bool freqOffsWild;  ///< True=Treat freqOffs as a wildcard when matching.

         /// SWIG accessor. Not overloaded, because SWIG.
      static std::string getDescTC(TrackingCode e)
      { return tcDesc[e]; }
         /// SWIG accessor. Not overloaded, because SWIG.
      static std::string getDescCB(CarrierBand e)
      { return cbDesc[e]; }
         /// SWIG accessor. Not overloaded, because SWIG.
      static std::string getDescOT(ObservationType e)
      { return otDesc[e]; }
         /// These strings are for forming a somewhat verbose description
      GNSSTK_EXPORT static std::map< TrackingCode,    std::string > tcDesc;
      GNSSTK_EXPORT static std::map< CarrierBand,     std::string > cbDesc;
      GNSSTK_EXPORT static std::map< ObservationType, std::string > otDesc;
         /** If true, dump and operator<< will include the values of
          * freqOffs, freqOffsWild, mcode, and mcodeMask.  These are
          * flags that were added more recently, so this also
          * preserves traditional output. */
      GNSSTK_EXPORT static bool verbose;

   private:
      uint32_t mcode;     ///< Data to uniquely identify M-code signal.
      uint32_t mcodeMask; ///< Bitmask for matching mcode.
      friend class ::ObsID_T;
   }; // class ObsID


} // namespace gnsstk
#endif   // OBSID_HPP
