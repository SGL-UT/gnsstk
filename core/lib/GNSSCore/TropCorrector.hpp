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


#ifndef GNSSTK_TROPCORRECTOR_HPP
#define GNSSTK_TROPCORRECTOR_HPP

#include "GroupPathCorrector.hpp"
#include "SimpleTropModel.hpp"
#include "SaasTropModel.hpp"
#include "NBTropModel.hpp"
#include "GGTropModel.hpp"
#include "GGHeightTropModel.hpp"
#include "NeillTropModel.hpp"
#include "GlobalTropModel.hpp"
#include "MetReader.hpp"
#include "YDSTime.hpp"

// forward declaration of test class
class GroupPathCorr_T;

namespace gnsstk
{
      /// @ingroup GNSSsolutions
      //@{

      /** Provide a means for estimating the pseudorange delay due to
       * tropospheric conditions.  To use it, instantiate one of the
       * typedefs, then call one of either setDefaultWx() or
       * loadFile() (the most recently called will be used to set the
       * model's weather data).  At that point, the getCorr() methods
       * may be used.
       * @code
       * gnsstk::GlobalTropCorrector trop;
       * double corr;
       * if (!trop.loadFile("arlm2000.15m"))
       *    return false;
       * if (!trop.getCorr(stnPos, svPos, sat, obsID, when, nav, corr))
       *    return false;
       * cout << "trop delay: " << corr << " m" << endl;
       * @endcode
       *
       * @note The NBTropModel::setWeather() approach (calling with no
       * arguments so that the data may be modeled using latitude and
       * time) can be used by simply not calling setDefaultWx() or
       * loadFile() either one.  This approach is unique to
       * NBTropModel.  All other models will indicate an error
       * condition if no weather data is loaded.
       *
       * @param Model The tropospheric model class, which is expected
       *   to be derived from the TropModel class.  Since there is a
       *   relatively limited number of these, there are typedefs for
       *   each of the existing TropModel classes. */
   template <class Model>
   class TropCorrector : public GroupPathCorrector
   {
   public:
         /// Set the #corrType to Trop for GroupPathCorr.
      TropCorrector();
         /// @copydoc GroupPathCorrector::getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, double&)
      bool getCorr(const Position& rxPos, const Position& svPos,
                   const SatID& sat, const ObsID& obs,
                   const CommonTime& when, NavType nav,
                   double& corrOut) override;
         /// @copydoc GroupPathCorrector::getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, double&)
      bool getCorr(const Position& rxPos, const Xvt& svPos,
                   const SatID& sat, const ObsID& obs,
                   const CommonTime& when, NavType nav,
                   double& corrOut) override;
         /** Set default weather data if no time series is available
          * @param[in] temp The new default temperature (degrees C).
          * @param[in] pres The new default pressure (millibars).
          * @param[in] hum The new default humidity (%). */
      virtual void setDefaultWx(double temp=20, double pres=1013,
                                double hum=50);
         /// Load RINEX MET data into wxData, uses Model
      virtual bool loadFile(const std::string& fn);
         /// Read and store weather data for look-up (single site)
      MetReader wxData;
   protected:
         /// Set to true if setDefaultWx was called more recently than loadFile
      bool useDefault;
      double defTemp; ///< Default temperature value (degrees C).
      double defPres; ///< Default pressure value (millibars).
      double defHum;  ///< Default humidity (%).

         /** Tell the model what weather to use.
          * @param[in,out] model The model whose weather data is to be set.
          * @param[in] when The time of the GNSS observation.
          * @post The weather data for model is set. */
      virtual void setWeather(Model& model, const CommonTime& when);

         // For testing.  Tried adding a const ref accessor, but SWIG
         // complained about it.  This is the easier solution.
      friend class ::GroupPathCorr_T;
   }; // class TropCorrector


      /// Somewhat pointless wrapper for zero trop correction model.
   typedef TropCorrector<ZeroTropModel> ZeroTropCorrector;
      /// Wrapper for the "simple" trop model.
   typedef TropCorrector<SimpleTropModel> SimpleTropCorrector;
      /// Wrapper for the Saastamoinen trop model.
   typedef TropCorrector<SaasTropModel> SaasTropCorrector;
      /// Wrapper for the Univeristy of New Brunswick trop model.
   typedef TropCorrector<NBTropModel> NBTropCorrector;
      /// Wrapper for the Goad and Goodman trop model.
   typedef TropCorrector<GGTropModel> GGTropCorrector;
      /// Wrapper for the Goad and Goodman trop model with heights.
   typedef TropCorrector<GGHeightTropModel> GGHeightTropCorrector;
      /// Wrapper for the A.E. Neill trop model.
   typedef TropCorrector<NeillTropModel> NeillTropCorrector;
      /// Wrapper for the global trop model (Boehm et al).
   typedef TropCorrector<GlobalTropModel> GlobalTropCorrector;

      //@}

   template <class Model>
   TropCorrector<Model> ::
   TropCorrector()
         : useDefault(false),
           defTemp(std::numeric_limits<double>::quiet_NaN()),
           defPres(std::numeric_limits<double>::quiet_NaN()),
           defHum(std::numeric_limits<double>::quiet_NaN())
   {
      corrType = CorrectorType::Trop;
   }


   template <class Model>
   bool TropCorrector<Model> ::
   getCorr(const Position& rxPos, const Position& svPos,
           const SatID& sat, const ObsID& obs,
           const CommonTime& when, NavType nav,
           double& corrOut)
   {
      Model model;
      model.setReceiverHeight(rxPos.height());
      model.setReceiverLatitude(rxPos.getGeodeticLatitude());
      model.setReceiverLongitude(rxPos.getLongitude());
      model.setDayOfYear(YDSTime(when).doy);
      try
      {
         setWeather(model, when);
         corrOut = model.correction(rxPos, svPos, when);
         return true;
      }
      catch (...)
      {
         corrOut = std::numeric_limits<double>::quiet_NaN();
         return false;
      }
   }


   template <class Model>
   bool TropCorrector<Model> ::
   getCorr(const Position& rxPos, const Xvt& svPos,
           const SatID& sat, const ObsID& obs,
           const CommonTime& when, NavType nav,
           double& corrOut)
   {
      Position svp(svPos.x);
      return getCorr(rxPos, svp, sat, obs, when, nav, corrOut);
   }


   template <class Model>
   void TropCorrector<Model> ::
   setDefaultWx(double temp, double pres, double hum)
   {
      useDefault = true;
      defTemp = temp;
      defPres = pres;
      defHum = hum;
   }


   template <class Model>
   bool TropCorrector<Model> ::
   loadFile(const std::string& fn)
   {
      useDefault = false;
      wxData.read(fn);
      return true;
   }


   template <class Model>
   void TropCorrector<Model> ::
   setWeather(Model& model, const CommonTime& when)
   {
      if (!useDefault)
      {
         model.setWeather(wxData.wx.getWxObservation(when));
      }
      else
      {
         model.setWeather(defTemp, defPres, defHum);
      }
   }


      /// Template specialization for NBTropModel.
   template <>
   inline void TropCorrector<NBTropModel> ::
   setWeather(NBTropModel& model, const CommonTime& when)
   {
      if (!useDefault)
      {
         if (wxData.wx.obs.empty())
         {
               // Haven't used the default weather via
               // setDefaultWx() and if loadFile was called, it was
               // unsuccessful.  Maybe using the New Brunswick
               // special case, so try that.
            model.setWeather();
         }
         else
         {
            model.setWeather(wxData.wx.getWxObservation(when));
         }
      }
      else
      {
         model.setWeather(defTemp, defPres, defHum);
      }
   }

} // namespace gnsstk

#endif // GNSSTK_TROPCORRECTOR_HPP
