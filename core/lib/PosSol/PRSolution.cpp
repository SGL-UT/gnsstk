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

/// @file PRSolution.cpp
/// Pseudorange navigation solution, either a simple solution using all the
/// given data, or a solution including editing via a RAIM algorithm.

#include <tuple>
#include <functional>

#include "Position.hpp"
#include "MathBase.hpp"
#include "Matrix.hpp"
#include "MatrixOperators.hpp"
#include "Vector.hpp"
#include "VectorBaseOperators.hpp"
#include "PRSolution.hpp"
#include "GPSEllipsoid.hpp"
#include "GlobalTropModel.hpp"
#include "Combinations.hpp"
#include "TimeString.hpp"
#include "logstream.hpp"
#include "RawRange.hpp"
#include "SVHealth.hpp"
#include "NavValidityType.hpp"
#include "NavSearchOrder.hpp"


namespace gnsstk
{
   const std::string PRSolution::calfmt = std::string("%04Y/%02m/%02d %02H:%02M:%02S %P");
   const std::string PRSolution::gpsfmt = std::string("%4F %10.3g");
   const std::string PRSolution::timfmt = gpsfmt + std::string(" ") + calfmt;

   std::ostream& operator<<(std::ostream& os, const WtdAveStats& was)
   {
      was.dump(os, was.getMessage());
      return os;
   }




   int PRSolution ::
   PreparePRSolution(const CommonTime& nominalReceive,
                     std::vector<SatID>& sats,
                     const std::vector<double>& pseudoranges,
                     NavLibrary& eph,
                     Matrix<double>& svp,
                     NavSearchOrder order) const
   {
      LOG(DEBUG) << "PreparePRSolution at time " << printTime(nominalReceive, timfmt);
      LOG(DEBUG) << "Sats.size is " << sats.size();

      if (allowedGNSS.empty())
      {
         Exception e("Must define systems vector allowedGNSS before processing");
         GNSSTK_THROW(e);
      }

      markDisallowedGNSS(sats, allowedGNSS);
      svp = createSVP(sats, nominalReceive, pseudoranges, eph, order);

         // Filter Sats to get a count of allowed sats with ephemeris
      int filteredSatCount = filterMarkedSats(sats).size();      
      return filteredSatCount == 0 ? RETURN_CODE::NO_EPHEMERIS : filteredSatCount;
   }


   int PRSolution ::
   SimplePRSolution(const CommonTime& T,
                    const std::vector<SatID>& Sats,
                    const Matrix<double>& SVP,
                    const Matrix<double>& invMC,
                    TropModel *pTropModel,
                    const int& niterLimit,
                    const double& convLimit,
                    Vector<double>& Resids,
                    Vector<double>& Slopes)
   {
      if (!pTropModel)
      {
         Exception e("Undefined tropospheric model");
         GNSSTK_THROW(e);
      }

      Matrix<double> weights(invMC);
      if (weights.rows() == 0)
      {
         weights = ident<double>(Sats.size());
      }

      if (Sats.size() != SVP.rows() || (weights.rows() != Sats.size()))
      {
         LOG(ERROR) << "Sats has length " << Sats.size();
         LOG(ERROR) << "SVP has dimension " << SVP.rows() << "x" << SVP.cols();
         LOG(ERROR) << "invMC has dimension " << invMC.rows() << "x" << invMC.cols();
         Exception e("Invalid dimensions");
         GNSSTK_THROW(e);
      }

      if (allowedGNSS.empty())
      {
         Exception e("Must define systems vector allowedGNSS before processing");
         GNSSTK_THROW(e);
      }

      int iret = RETURN_CODE::OK;

      Valid = false;

      try
      {
         const FilteredConstSats filteredSats = filterMarkedSats(Sats);
         
         Nsvs = filteredSats.size();
         std::vector<SatelliteSystem> currGNSS = filterGNSS(allowedGNSS, filteredSats);

            // dimension of the solution vector (3 pos + 1 clk/sys)
         const size_t dim(3 + currGNSS.size());

            // require number of good satellites to be >= number unknowns (no RAIM here)
         if (Nsvs < dim)
         {
            return RETURN_CODE::NOT_ENOUGH_SVS;
         }

         LOG(DEBUG) << "Build inverse MCov";

         Matrix<double> filteredWeights = filterWeights(weights, filteredSats);
         
         LOG(DEBUG) << "inv MCov matrix is\n" << std::fixed << std::setprecision(4) << filteredWeights;
         LOG(DEBUG) << " Solution dimension is " << dim << " and Nsvs is " << Nsvs;

            // start with solution = apriori, cut down to match current dimension
         Vector<double> localAPSol(dim, 0.0);
         if(hasMemory)
         {
            localAPSol[X] = APSolution[X];
            localAPSol[Y] = APSolution[Y];
            localAPSol[Z] = APSolution[Z];

            for (size_t i = 0; i < currGNSS.size(); ++i) 
            {
               int k = vectorindex(allowedGNSS,currGNSS[i]);
               localAPSol[3 + i] = (k == -1 ? 0.0 : APSolution[3 + k]);
            }
         }
         else
         {
            LOG(DEBUG) << " no memory - no apriori solution";
         }
         Solution = localAPSol;

         double converge;
         Matrix<double> partials;
         Matrix<double> G;
         try
         {
            Vector<double> dX(dim, 0.0);
            bool tropSuccess;
            std::tie(tropSuccess, converge, NIterations) =
               iterativeSinglePointWLSSolution(dX, partials, G, Covariance, Resids, 
                                               Solution, filteredSats, SVP, 
                                               pTropModel, T, currGNSS, 
                                               filteredWeights, convLimit, 
                                               niterLimit); 
            TropFlag = !tropSuccess;
            Solution += dX;
         }
         catch(SingularMatrixException& sme)
         {
            return RETURN_CODE::SINGULAR_SOLUTION;
         }
        
         if (converge >= convLimit)
         {
            iret = RETURN_CODE::FAILED_CONVERGENCE; 
         }

         if (TropFlag)
         {
            LOG(DEBUG) << "Trop correction not applied at time "
                           << printTime(T,timfmt);
         }

         double maxSlope = 0.0;
         Vector<double> slopes;
         if (iret == RETURN_CODE::OK)
         {
            computeSlopes(slopes, partials, G, filteredSats);
            maxSlope = max<double>(slopes);
            LOG(DEBUG) << "Computed slopes, found max member";
         }

            // compute pre-fit residuals
         if (hasMemory)
         {
            PreFitResidual = partials * (Solution - localAPSol) - Resids;
            LOG(DEBUG) << "Computed pre-fit residuals";
         }

            // save to member data
         RMSResidual = RMS(Resids);
         LOG(DEBUG) << "Computed RMS residual";
         currTime = T;
         SatelliteIDs = Sats;
         dataGNSS = currGNSS;
         invMeasCov = filteredWeights;
         Partials = partials;
         Convergence = converge;
         MaxSlope = maxSlope;
         Slopes = slopes;
         Valid = true;

         return iret;

      } 
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }

   } // end PRSolution::SimplePRSolution


   int PRSolution ::
   RAIMComputeUnweighted(const CommonTime& Tr,
                         std::vector<SatID>& Sats,
                         const std::vector<double>& Pseudorange,
                         NavLibrary& eph,
                         TropModel *pTropModel,
                         NavSearchOrder order)
   {
      try
      {
            // measurement covariance is empty
            // TODO: would an identity matrix make things easier?
         Matrix<double> weights = ident<double>(Sats.size());
         return (RAIMCompute(Tr, Sats, Pseudorange, weights, eph, pTropModel,
                             order));
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }


   int PRSolution ::
   RAIMCompute(const CommonTime& Tr,
               std::vector<SatID>& Sats,
               const std::vector<double>& Pseudorange,
               const Matrix<double>& invMC,
               NavLibrary& eph,
               TropModel *pTropModel,
               NavSearchOrder order)
   {
      try
      {
            // uncomment to turn on DEBUG output to stdout
         //LOGlevel = ConfigureLOG::Level("DEBUG");

         LOG(DEBUG) << "RAIMCompute at time " << printTime(Tr,gpsfmt);

         int iret,N;
         size_t j;
         std::vector<int> GoodIndexes;
            // use these to save the 'best' solution within the loop.
            // BestRMS marks the 'Best' set as unused.
         bool BestTropFlag(false);
         int BestNIter(0), BestIret(-5);
         double BestRMS(-1.0), BestSL(0.0), BestConv(0.0);
         Vector<double> BestSol(3,0.0), BestPFR;
         std::vector<SatID> BestSats, SaveSats;
         Matrix<double> SVP, BestCov, BestInvMCov, BestPartials;
         std::vector<SatelliteSystem> BestGNSS;

            // initialize
         Valid = false;
         currTime = Tr;
         TropFlag = SlopeFlag = RMSFlag = false;

            // ----------------------------------------------------------------
            // fill the SVP matrix, and use it for every solution
            // NB this routine will reject sat systems not found in allowedGNSS, and
            //    sats without ephemeris.
         N = PreparePRSolution(Tr, Sats, Pseudorange, eph, SVP, order);

         if (LOGlevel >= ConfigureLOG::Level("DEBUG"))
         {
            LOG(DEBUG) << "Prepare returns " << N;
            std::ostringstream oss;
            oss << "RAIMCompute: after PrepareAS(): Satellites:";
            for (SatID& sat : Sats)
            {
               RinexSatID rs(::abs(sat.id), sat.system);
               oss << " " << (isMarked(sat) ? "-" : "") << rs;
            }
            oss << std::endl;

            oss << " SVP matrix("
               << SVP.rows() << "," << SVP.cols() << ")" << std::endl;
            oss << std::fixed << std::setw(16) << std::setprecision(5) << SVP;

            LOG(DEBUG) << oss.str();
         }

            // return is >=0(number of good sats) or -4(no ephemeris)
         if (N <= 0)
         {
            return RETURN_CODE::NO_EPHEMERIS;
         }

            // ----------------------------------------------------------------
            // Build GoodIndexes based on Sats; save Sats as SaveSats.
            // Sats is used to mark good sats (id > 0) and those to ignore (id <= 0).
            // SaveSats saves the original so it can be reused for each RAIM solution.
            // Let GoodIndexes be all the indexes of Sats that are good:
            // Sats[GoodIndexes[.]].id > 0 by definition.
         SaveSats = Sats;
         for (auto &kv : filterMarkedSats(Sats))
         {
            int satIndex = kv.first;
            GoodIndexes.push_back(satIndex);
         }

            // dump good satellites for debug
         if (LOGlevel >= ConfigureLOG::Level("DEBUG"))
         {
            std::ostringstream oss;
            oss << " Good satellites (" << N << ") are:";
            for (int index : GoodIndexes)
            {
               oss << " " << RinexSatID(Sats[index]);
            }
            LOG(DEBUG) << oss.str();
         }

            // ----------------------------------------------------------------
            // now compute the solution, first with all the data. If this fails,
            // RAIM: reject 1 satellite at a time and try again, then 2, etc.

            // Slopes for each satellite are computed (cf. the RAIM algorithm)
         Vector<double> Slopes;
            // Resids stores the post-fit data residuals.
         Vector<double> Resids;

         for (int stage = 0;; ++stage)
         {
               // compute all the combinations of N satellites taken stage at a time
            Combinations Combo(N,stage);

               // compute a solution for each combination of marked satellites
            do {
                  // Mark the satellites for this combination
               Sats = SaveSats;
               for (size_t i = 0; i < GoodIndexes.size(); ++i)
               {
                  if (Combo.isSelected(i))
                  {
                     Sats[GoodIndexes[i]].id = -::abs(Sats[GoodIndexes[i]].id);
                  }
               }

               if (LOGlevel >= ConfigureLOG::Level("DEBUG"))
               {
                  std::ostringstream oss;
                  oss << " RAIM: Try the combo ";
                  for (SatID& sat : Sats)
                  {
                     RinexSatID rs(::abs(sat.id), sat.system);
                     oss << " " << (isMarked(sat) ? "-" : " ") << rs;
                  }
                  LOG(DEBUG) << oss.str();
               }

                  // ----------------------------------------------------------------
                  // Compute a solution given the data; ignore ranges for marked
                  // satellites. Fill Vector 'Slopes' with slopes for each unmarked
                  // satellite.
                  // Return 0  ok
                  //       -1  failed to converge
                  //       -2  singular problem
                  //       -3  not enough good data
                  //       -4  no ephemeris
               iret = SimplePRSolution(Tr, Sats, SVP, invMC, pTropModel,
                       MaxNIterations, ConvergenceLimit, Resids, Slopes);

               LOG(DEBUG) << " RAIM: SimplePRS returns " << iret;
               if (iret <= RETURN_CODE::OK && iret > BestIret)
               {
                  BestIret = iret;
               }

                  // ----------------------------------------------------------------
                  // if error, either quit or continue with next combo (SPS sets Valid F)
               if (iret < RETURN_CODE::OK)
               {
                  if (iret == RETURN_CODE::FAILED_CONVERGENCE)
                  {
                     LOG(DEBUG) << " SPS: Failed to converge - go on";
                     continue;
                  }
                  else if (iret == RETURN_CODE::SINGULAR_SOLUTION)
                  {
                     LOG(DEBUG) << " SPS: singular - go on";
                     continue;
                  }
                  else if (iret == RETURN_CODE::NOT_ENOUGH_SVS)
                  {
                     LOG(DEBUG) <<" SPS: not enough satellites: quit";
                     break;
                  }
                  else if (iret == RETURN_CODE::NO_EPHEMERIS)
                  {
                     LOG(DEBUG) <<" SPS: no ephemeris: quit";
                     break;
                  }
               }

                  // ----------------------------------------------------------------
                  // print solution with diagnostic information
               LOG(DEBUG) << outputString(std::string("RPS"),iret);

                  // do again for residuals
                  // if memory exists, output residuals
                  //if(hasMemory) LOG(DEBUG) << outputString(string("RAP"), -99,
                     //(Solution-memory.APSolution));

                  // deal with the results of SimplePRSolution()
                  // save 'best' solution for later
               if (BestRMS < 0.0 || RMSResidual < BestRMS)
               {
                  BestRMS = RMSResidual;
                  BestSol = Solution;
                  BestSats = SatelliteIDs;
                  BestGNSS = dataGNSS;
                  BestSL = MaxSlope;
                  BestConv = Convergence;
                  BestNIter = NIterations;
                  BestCov = Covariance;
                  BestInvMCov = invMeasCov;
                  BestPartials = Partials;
                  BestPFR = PreFitResidual;
                  BestTropFlag = TropFlag;
                  BestIret = iret;
               }

               if (stage == 0 && RMSResidual < RMSLimit)
               {
                  break;
               }

            } while (Combo.Next() != -1);  // get the next combinations and repeat

               // end of the stage
               // success
            if (BestRMS > 0.0 && BestRMS < RMSLimit)
            {
               LOG(DEBUG) << " RAIM: Success in the RAIM loop";
               iret = RETURN_CODE::OK;
               break;
            }

               // but not if too many are being rejected
            if (NSatsReject > -1 && stage > NSatsReject)
            {
               LOG(DEBUG) << " RAIM: break before stage " << stage
                  << " due to NSatsReject " << NSatsReject;
               break;
            }

               // already broke out of the combo loop...
            if (iret == RETURN_CODE::NOT_ENOUGH_SVS || iret == RETURN_CODE::NO_EPHEMERIS)
            {
               LOG(DEBUG) << " RAIM: break before stage " << stage
                  << "; " << (iret == RETURN_CODE::NOT_ENOUGH_SVS ? "too few sats" : "no ephemeris");
               break;
            }

            LOG(DEBUG) << " RAIM: go to stage " << stage;
         }

            // ----------------------------------------------------------------
            // copy out the best solution
         if (iret >= RETURN_CODE::OK)
         {
            Sats = SatelliteIDs = BestSats;
            dataGNSS = BestGNSS;
            Solution = BestSol;
            Covariance = BestCov;
            invMeasCov = BestInvMCov;
            Partials = BestPartials;
            PreFitResidual = BestPFR;
            Convergence = BestConv;
            NIterations = BestNIter;
            RMSResidual = BestRMS;
            MaxSlope = BestSL;
            TropFlag = BestTropFlag;
            iret = BestIret;

               // must add zeros to state, covariance and partials if these don't match
            if (dataGNSS.size() != BestGNSS.size())
            {
               N = 3 + dataGNSS.size();
               Solution = Vector<double>(N, 0.0);
               Covariance = Matrix<double>(N, N, 0.0);
               Partials = Matrix<double>(BestPartials.rows(), N, 0.0);

                  // build a little map of indexes; note systems are sorted alike
               std::vector<int> indexes;
               for (size_t j = 0, i = 0; i < dataGNSS.size(); ++i)
               {
                  indexes.push_back(dataGNSS[i] == BestGNSS[j] ? j++ : -1);
               }

                  // copy over position and position,clk parts
               for (size_t i = 0; i < 3; ++i)
               {
                  Solution(i) = BestSol(i);
                  for (size_t j = 0; j < Partials.rows(); ++j)
                  {
                     Partials(j,i) = BestPartials(j,i);
                  }

                  for (size_t j = 0; j < 3; ++j)
                  {
                     Covariance(i, j) = BestCov(i, j);
                  }

                  for (size_t j = 0; j < indexes.size(); ++j)
                  {
                     Covariance(i, 3 + j) = (indexes[j] == -1 ? 0. : BestCov(i, 3 + indexes[j]));
                     Covariance(3 + j, i) = (indexes[j] == -1 ? 0. : BestCov(3 + indexes[j], i));
                  }
               }

                  // copy the clock part, inserting zeros where needed
               for (size_t j = 0; j < indexes.size(); ++j)
               {
                  int n(indexes[j]);
                  bool ok(n != -1);
                  Solution(3 + j) = (ok ? BestSol(3 + n) : 0.0);
                  for (size_t i = 0; i < Partials.rows(); ++i)
                  {
                     Partials(i, 3 + j) = (ok ? BestPartials(i, 3 + n) : 0.0);
                  }

                  for (size_t i = 0; i < indexes.size(); ++i)
                  {
                     Covariance(3 + i, 3 + j) = (ok ? BestCov(3 + i, 3 + n) : 0.0);
                     Covariance(3 + j, 3 + i) = (ok ? BestCov(3 + n, 3 + i) : 0.0);
                  }
               }
            }

               // compute number of satellites actually used
            Nsvs = filterMarkedSats(SatelliteIDs).size();

            if (iret == RETURN_CODE::OK)
            {
               DOPCompute();
            }

            if (hasMemory && iret == RETURN_CODE::OK)
            {
                  // update memory solution
               addToMemory(Solution, Covariance, PreFitResidual, Partials, invMeasCov);
                  // update apriori solution
               updateAPSolution(Solution);
            }

         }

            // ----------------------------------------------------------------
         if (iret == RETURN_CODE::OK)
         {
            if (BestSL > SlopeLimit)
            { 
               iret = RETURN_CODE::DEGRADED;
               SlopeFlag = true;
            }

            if (BestSL > SlopeLimit / 2.0 && Nsvs == 5)
            {
               iret = RETURN_CODE::DEGRADED;
               SlopeFlag = true;
            }

            if (BestRMS >= RMSLimit)
            { 
               iret = RETURN_CODE::DEGRADED;
               RMSFlag = true;
            }

            if (TropFlag)
            {
               iret = RETURN_CODE::DEGRADED;
            }

            Valid = true;
         }
         else if (iret == RETURN_CODE::FAILED_CONVERGENCE)
         {
            Valid = false;
         }

         LOG(DEBUG) << " RAIM exit with ret value " << iret
                     << " and Valid " << (Valid ? "T":"F");

         return iret;
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }  // end PRSolution::RAIMCompute()


   int PRSolution ::
   DOPCompute()
   {
      try
      {
         Matrix<double> PTP(transpose(Partials) * Partials);
         Matrix<double> Cov(inverseLUD(PTP));
         PDOP = SQRT(Cov(0, 0) + Cov(1, 1) + Cov(2, 2));
         TDOP = 0.0;
         for (size_t i = 3; i < Cov.rows(); ++i)
         {
            TDOP += Cov(i, i);
         }
         TDOP = SQRT(TDOP);
         GDOP = RSS(PDOP,TDOP);
         return 0;
      }
      catch (Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }

   
   std::string PRSolution ::
   outputValidString(int iret)
   {
      std::ostringstream oss;
      if (iret != -99)
      {
         oss << " (" << iret << " " << errorCodeString(iret);
         if (iret == RETURN_CODE::DEGRADED)
         {
            oss << " due to";
            if (RMSFlag)
            {
               oss << " large RMS residual";
            }
            
            if (SlopeFlag)
            {
               oss << " large slope";
            }

            if (TropFlag)
            {
               oss << " missed trop. corr.";
            }
         }
         oss << ") " << (Valid ? "" : "N") << "V";
      }
      return oss.str();
   }

   
   std::string PRSolution ::
   outputNAVString(std::string tag, int iret, const Vector<double>& Vec)
   {
      std::ostringstream oss;

         // output header describing regular output
      if (iret == -999)
      {
         oss << printTime(currTime,gpsfmt);
         int len = oss.str().size();
         oss.str("");
         oss << "#" << tag << " NAV " << std::setw(len) << "time"
            << " " << std::setw(18) << "Sol/Resid:X(m)"
            << " " << std::setw(18) << "Sol/Resid:Y(m)"
            << " " << std::setw(18) << "Sol/Resid:Z(m)"
            << " " << std::setw(18) << "sys clock" << " [sys clock ...]   Valid/Not";
         return oss.str();
      }

         // tag NAV timetag X Y Z clks endtag
      oss << tag << " NAV " << printTime(currTime,gpsfmt)
         << std::fixed << std::setprecision(6)
         << " " << std::setw(16) << (&Vec==&PRSNullVector ? Solution(0) : Vec(0))
         << " " << std::setw(16) << (&Vec==&PRSNullVector ? Solution(1) : Vec(1))
         << " " << std::setw(16) << (&Vec==&PRSNullVector ? Solution(2) : Vec(2))
         << std::fixed << std::setprecision(3);
      for (size_t i = 0; i < dataGNSS.size(); ++i)
      {
         RinexSatID sat(1, dataGNSS[i]);
         oss << " " << sat.systemString3() << " " << std::setw(11) << Solution(3 + i);
      }
      oss << outputValidString(iret);

      return oss.str();
   }


   std::string PRSolution ::
   outputPOSString(const std::string& tag, int iret, const Vector<double>& Vec)
   {
      std::ostringstream oss;

         // output header describing regular output
      if (iret == -999)
      {
         oss << printTime(currTime,gpsfmt);
         int len = oss.str().size();
         if (len > 3)
         {
            len -= 3;
         }
         oss.str("");
         oss << "#" << tag << " POS " << std::setw(len) << "time"
            << " " << std::setw(16) << "Sol-X(m)"
            << " " << std::setw(16) << "Sol-Y(m)"
            << " " << std::setw(16) << "Sol-Z(m)"
            << " (ret code) Valid/Not";
         return oss.str();
      }

         // tag POS timetag X Y Z endtag
      oss << tag << " POS " << printTime(currTime, gpsfmt)
         << std::fixed << std::setprecision(6)
         << " " << std::setw(16) << (&Vec==&PRSNullVector ? Solution(0) : Vec(0))
         << " " << std::setw(16) << (&Vec==&PRSNullVector ? Solution(1) : Vec(1))
         << " " << std::setw(16) << (&Vec==&PRSNullVector ? Solution(2) : Vec(2))
         << outputValidString(iret);

      return oss.str();
   }


   std::string PRSolution ::
   outputCLKString(std::string tag, int iret)
   {
      std::ostringstream oss;

         // output header describing regular output
      if (iret == -999)
      {
         oss << printTime(currTime,gpsfmt);
         int len = oss.str().size();
         if (len > 3)
         {
            len -= 3;
         }
         oss.str("");
         oss << "#" << tag << " CLK " << std::setw(len) << "time"
            << " sys " << std::setw(11) << "clock" << " ...";
         return oss.str();
      }

         // tag CLK timetag SYS clk [SYS clk SYS clk ...] endtag
      oss << tag << " CLK " << printTime(currTime,gpsfmt)
         << std::fixed << std::setprecision(3);
      //for(size_t i = 0; i < SystemIDs.size(); ++i) {
      //   RinexSatID sat(1,SystemIDs[i]);
      for (size_t i = 0; i < dataGNSS.size(); ++i) 
      {
         RinexSatID sat(1,dataGNSS[i]);
         oss << " " << sat.systemString3() << " " << std::setw(11) << Solution(3+i);
      }
      oss << outputValidString(iret);

      return oss.str();
   }


      // NB must call DOPCompute() if SimplePRSol() only was called.
   std::string PRSolution ::
   outputRMSString(const std::string& tag, int iret)
   {
      std::ostringstream oss;

         // output header describing regular output
         // TODO: why would iret ever be -999?
      if (iret == -999)
      {
         oss << printTime(currTime,gpsfmt);
         int len = oss.str().size();
         if (len > 3)
         {
            len -= 3;
         }
         oss.str("");
         oss << "#" << tag << " RMS " << std::setw(len) << "time"
            << " " << std::setw(2) << "Ngood"
            << " " << std::setw(8) << "resid"
            << " " << std::setw(7) << "TDOP"
            << " " << std::setw(7) << "PDOP"
            << " " << std::setw(7) << "GDOP"
            << " " << std::setw(5) << "Slope"
            << " " << std::setw(2) << "nit"
            << " " << std::setw(8) << "converge"
            << " sats(-rej)... (ret code) Valid/Not";
         return oss.str();
      }

         // remove duplicates from satellite list, and find "any good data" ones
         // this gets tricky since there may be >1 datum from one satellite
         // in the following, good means 1+ good data exist; bad means all data bad
      size_t i;
      std::vector<RinexSatID> sats, goodsats;
      for (SatID& sat : SatelliteIDs)
      {
         RinexSatID rs(::abs(sat.id), sat.system);
         if (find(sats.begin(), sats.end(), rs) == sats.end())
         {
               // all satellites
            sats.push_back(rs);
         }

         if (!isMarked(sat)) 
         {
            if (find(goodsats.begin(), goodsats.end(), rs) == goodsats.end())
            {
                  // good satellites
               goodsats.push_back(rs);
            }
         }
      }

         //# tag RMS week  sec.of.wk nsat rmsres    tdop    pdop    gdop slope it
         //                converg   sats... (return) [N]V"
      oss << tag << " RMS " << printTime(currTime,gpsfmt)
         << " " << std::setw(2) << goodsats.size()
         << std::fixed << std::setprecision(3)
         << " " << std::setw(8) << RMSResidual
         << std::setprecision(2)
         << " " << std::setw(7) << TDOP
         << " " << std::setw(7) << PDOP
         << " " << std::setw(7) << GDOP
         << std::setprecision(1)
         << " " << std::setw(5) << MaxSlope
         << " " << std::setw(2) << NIterations
         << std::scientific << std::setprecision(2)
         << " " << std::setw(8) << Convergence;
      for (RinexSatID& sat : sats)
      {
         if (find(goodsats.begin(), goodsats.end(), sat) != goodsats.end())
         {
            oss << " " << sat;
         }
         else
         {
            oss << " -" << sat;
         }
      }
      oss << outputValidString(iret);

      return oss.str();
   }  // end PRSolution::outputRMSString


   std::string PRSolution ::
   outputString(const std::string& tag, int iret, const Vector<double>& Vec)
   {
      std::ostringstream oss;
      //oss << outputPOSString(tag,iret) << std::endl;  // repeated in NAV
      oss << outputNAVString(tag, iret, Vec) << std::endl;
      oss << outputRMSString(tag, iret);

      return oss.str();
   }


   std::string PRSolution ::
   errorCodeString(int iret)
   {
      std::string str("unknown");
      if (iret == RETURN_CODE::DEGRADED)
      {
         str = std::string("ok but perhaps degraded");
      }
      else if (iret == RETURN_CODE::OK)
      {
         str = std::string("ok");
      }
      else if (iret == RETURN_CODE::FAILED_CONVERGENCE)
      {
         str = std::string("failed to converge");
      }
      else if (iret == RETURN_CODE::SINGULAR_SOLUTION)
      {
         str = std::string("singular solution");
      }
      else if (iret == RETURN_CODE::NOT_ENOUGH_SVS)
      {
         str = std::string("not enough satellites");
      }
      else if (iret == RETURN_CODE::NO_EPHEMERIS)
      {
         str = std::string("not any ephemeris");
      }
      return str;
   }


   std::string PRSolution ::
   configString(const std::string& tag)
   {
      std::ostringstream oss;
      oss << tag
         << "\n   iterations " << MaxNIterations
         << "\n   convergence " << std::scientific << std::setprecision(2) << ConvergenceLimit
         << "\n   RMS residual limit " << std::fixed << RMSLimit
         << "\n   RAIM slope limit " << std::fixed << SlopeLimit << " meters"
         << "\n   Maximum number of satellites to reject is " << NSatsReject
         << "\n   Memory information IS " << (hasMemory ? "":"NOT ") << "stored"
         ;

         // output memory information
      //if(APrioriSol.size() >= 4) oss
      //   << "\n   APriori is " << std::fixed << std::setprecision(3) << APrioriSol(0)
      //      << " " << APrioriSol(1) << " " << APrioriSol(2) << " " << APrioriSol(3);
      //else oss << "\n   APriori is undefined";

      return oss.str();
   }
   
   
   void PRSolution :: 
   markSatellite(SatID& sat) const
   {
      sat.id = -std::abs(sat.id);
   }


   bool PRSolution :: 
   isMarked(const SatID& sat) const
   {
      return sat.id <= 0;
   }


   FilteredSats PRSolution ::
   filterMarkedSats(std::vector<SatID>& sats) const
   {
      FilteredSats filteredSats;

      for (size_t i = 0; i < sats.size(); ++i)
      {
         if (!isMarked(sats[i]))
         {
            filteredSats.push_back(std::make_pair(i, std::ref(sats[i])));
         }
      }

      return filteredSats;
   }


   FilteredConstSats PRSolution :: 
   filterMarkedSats(const std::vector<SatID>& sats) const
   {
      FilteredConstSats filteredSats;

      for (size_t i = 0; i < sats.size(); ++i)
      {
         if (!isMarked(sats[i]))
         {
            filteredSats.push_back(std::make_pair(i, std::cref(sats[i])));
         }
      }

      return filteredSats;
   }

   
   void PRSolution :: 
   markDisallowedGNSS(std::vector<SatID>& sats, 
                      const std::vector<SatelliteSystem>& allowed) const
   {
      for (SatID &sat : sats)
      {
         if (vectorindex(allowed, sat.system) == -1)
         {
            markSatellite(sat);
         }
      }
   }


   double PRSolution ::
   computeRange(Position& svPos, 
                const Position& rxPos, 
                bool firstIteration) const
   {
         // TODO: choice of ellipsoid needs to be configurable
      GPSEllipsoid ellip;
      
      double tof = 0.0;
      if (firstIteration)
      {
         tof = 0.070;
      }
      else
      {
         tof = range(rxPos, svPos) / ellip.c();
      }

      double rho;
      std::tie(rho, svPos) = RawRange::computeRange(rxPos, svPos, tof, ellip);

      return rho;
   }


   std::tuple<bool, double> PRSolution :: 
   computeTropDelay(const Position& svPos, 
                    const Position& rxPos, 
                    TropModel *pTropModel, 
                    const CommonTime& time) const
   {
         // must test R for reasonableness to avoid corrupting TropModel
         // Global model sets the upper limit - first test it
      GlobalTropModel* p = dynamic_cast<GlobalTropModel*>(pTropModel);
      bool bad(p && rxPos.getHeight() > p->getHeightLimit());
      
      if (bad || rxPos.elevation(svPos) < 0.0 || rxPos.getHeight() < -1000.0)
      {
         return std::make_tuple(false, 0.0);
      }
      
      double tropDelay = pTropModel->correction(rxPos, svPos, time);
      return std::make_tuple(true, tropDelay);
   }


   bool PRSolution ::
   getSatPVT(Xvt& svXvt, 
             const CommonTime& nominalReceive, 
             double pseudorange, 
             NavLibrary& eph, 
             SatID& sat, 
             NavSearchOrder order) const
   {
      NavSatelliteID navSat(sat);

      bool success;
      CommonTime transmitTime;
      std::tie(success, transmitTime) = 
            RawRange::estTransmitFromObs(nominalReceive, pseudorange, eph, 
                                         navSat, SVHealth::Healthy, 
                                         NavValidityType::ValidOnly, 
                                         order);
            
      if (!success)
      {
         return false;
      }

      success = eph.getXvt(navSat, transmitTime, svXvt, 
                           false,  // useAlm 
                           SVHealth::Healthy, NavValidityType::ValidOnly, 
                           order);
      return success;
   }


   bool PRSolution ::
   computePartialsAndResiduals(
         Matrix<double>& partials,
         Vector<double>& residuals,
         const FilteredConstSats& filteredSats, 
         const Matrix<double>& svp, 
         const Vector<double>& aPrioriSolution,
         bool firstIteration, 
         TropModel *pTropModel, 
         const CommonTime& nominalReceive, 
         const std::vector<SatelliteSystem>& currGNSS) const
   {
      bool tropSuccess = true;
      double dim = 3 + currGNSS.size();
      partials.resize(filteredSats.size(), dim, 0.0);
      Position rxPos(aPrioriSolution(X), aPrioriSolution(Y), aPrioriSolution(Z));

      for (size_t filteredSatIndex = 0; filteredSatIndex < filteredSats.size(); ++filteredSatIndex)
      {
         int satIndex = filteredSats[filteredSatIndex].first;
         const SatID &sat = filteredSats[filteredSatIndex].second;
           
         Position svPos(svp(satIndex, X), svp(satIndex, Y), svp(satIndex, Z));
         
            // svPos is modified by this function
            // to account for Earth's rotations
         double rho  = computeRange(svPos, rxPos, firstIteration);

         Triple dirCos((rxPos[X] - svPos[X]) / rho,
                       (rxPos[Y] - svPos[Y]) / rho,
                       (rxPos[Z] - svPos[Z]) / rho);

         double residual = svp(satIndex, P_HAT) - rho;

            // correct for troposphere (but not on the first iteration)
         if (!firstIteration)
         {
            double tropDelay;
            bool success;
            std::tie(success, tropDelay) = 
                  computeTropDelay(svPos, rxPos, pTropModel, nominalReceive);
            tropSuccess = tropSuccess && success;
            residual -= tropDelay;
            LOG(DEBUG) << "Trop " << satIndex << " " << sat << " "
               << std::fixed << std::setprecision(3) << tropDelay;
         }

            // find the clock for the sat's system
            // Solution ~ X,Y,Z,clks
         size_t clockIndex = 3 + vectorindex(currGNSS, sat.system);

         const double clk(aPrioriSolution(clockIndex));
         LOG(DEBUG) << "Clock is (" << clockIndex << ") " << clk;

            // data vector: corrected range residual
         residuals(filteredSatIndex) = residual - clk;

            // partials matrix
         partials(filteredSatIndex, X) = dirCos[X];
         partials(filteredSatIndex, Y) = dirCos[Y];
         partials(filteredSatIndex, Z) = dirCos[Z];
         partials(filteredSatIndex, clockIndex) = 1.0;
      }

      return tropSuccess;
   }

   
   bool PRSolution ::
   singlePointWLSSolution(Vector<double>& dX,
                          Matrix<double>& partials,
                          Matrix<double>& G,
                          Matrix<double>& covariance,
                          Vector<double>& residuals,
                          const Vector<double>& aPrioriSolution, 
                          const FilteredConstSats filteredSats, 
                          const Matrix<double>& svp, 
                          TropModel *pTropModel, 
                          const CommonTime& nominalReceive, 
                          bool firstIteration, 
                          const std::vector<SatelliteSystem>& currGNSS, 
                          const Matrix<double>& weights) const
   {
      bool tropSuccess = computePartialsAndResiduals(
            partials, residuals, filteredSats, svp, aPrioriSolution, 
            firstIteration, pTropModel, nominalReceive, currGNSS);


      Matrix<double> partialsT = transpose(partials);
      covariance = partialsT * weights * partials;

      try
      {
         covariance = inverseSVD(covariance);
      }
      catch (MatrixException& sme)
      {
            // Using the more specific SingluarMatrixException so that the caller 
            // can recover for this particular case.
         GNSSTK_THROW(SingularMatrixException());
      }

      G = covariance * partialsT * weights;
 
      dX = G * residuals;
     
      LOG(DEBUG) << "Partials (" << partials.rows() << "x" << partials.cols() << ")\n"
         << std::fixed << std::setprecision(4) << partials;
      LOG(DEBUG) << "Resids (" << residuals.size() << ") "
         << std::fixed << std::setprecision(3) << residuals;
      LOG(DEBUG) << "InvCov (" << covariance.rows() << "x" << covariance.cols()
         << ")\n" << std::fixed << std::setprecision(4) << covariance;
      LOG(DEBUG) << "Computed dX(" << dX.size() << ")";
            
      return tropSuccess;
   }

   
   std::tuple<bool, double, int> PRSolution ::
   iterativeSinglePointWLSSolution(Vector<double>& totaldX,
                                Matrix<double>& partials,
                                Matrix<double>& G,
                                Matrix<double>& covariance,
                                Vector<double>& residuals,
                                const Vector<double>& aPrioriSolution, 
                                const FilteredConstSats& filteredSats, 
                                const Matrix<double>& svp, 
                                TropModel *pTropModel, 
                                const CommonTime& nominalReceive, 
                                const std::vector<SatelliteSystem>& currGNSS,
                                const Matrix<double>& weights,
                                double convLimit,
                                int niterLimit) const
   {
      bool tropSuccess;
      double dim = 3 + currGNSS.size();
      partials.resize(filteredSats.size(), dim, 0.0);
      Vector<double> dX(dim, 0.0);
      totaldX.resize(dim, 0.0);
      residuals.resize(filteredSats.size());
         
         // Perform two iterations regardless of the iteration limit.
         // Don't apply trop delays on the first iteration.
      tropSuccess = singlePointWLSSolution(dX, partials, G, covariance, 
                                           residuals, aPrioriSolution, 
                                           filteredSats, svp, 
                                           pTropModel, nominalReceive, true, 
                                           currGNSS, weights);
      totaldX += dX;
      
      tropSuccess = singlePointWLSSolution(dX, partials, G, covariance, 
                                           residuals, aPrioriSolution + totaldX, 
                                           filteredSats, svp, pTropModel,
                                           nominalReceive, false, currGNSS, 
                                           weights);
      totaldX += dX;
      
         // defined outside of the loop so they can be returned
      double converge = norm(dX);
      size_t iter = 2;

      for (; iter < niterLimit && converge >= convLimit && converge <= 1.e10; ++iter)
      {
         tropSuccess = singlePointWLSSolution(dX, partials, G, covariance, 
                                              residuals,
                                              aPrioriSolution + totaldX, 
                                              filteredSats, svp, pTropModel,
                                              nominalReceive, false, currGNSS, 
                                              weights);
         totaldX += dX;
         converge = norm(dX);
      }

      return std::make_tuple(tropSuccess, converge, iter);
   }


   Matrix<double> PRSolution ::
   createSVP(
         std::vector<SatID>& sats, 
         const CommonTime& time, 
         const std::vector<double>& pseudoranges, 
         NavLibrary& eph, 
         NavSearchOrder order) const
   {
         // SVP matrix contains a row of SV position (X, Y, Z) 
         // and an adjusted pseudorange for each SV (i.e. 4 elements per row).
      Matrix<double> svp(sats.size(), 4, 0.0);
     
         // For each sat, get ephemeris or mark the sat if no ephemeris
      for (const auto &kv : filterMarkedSats(sats))
      {
         int satIndex = kv.first;
         SatID &sat = kv.second;
         
         LOG(DEBUG) << " Process sat " << RinexSatID(sat);

         Xvt pvt;
         bool success = getSatPVT(pvt, time, pseudoranges[satIndex], 
                                  eph, sat, order);
         
         if (!success)
         {
            LOG(DEBUG) << "Warning - PRSolution ignores satellite (no ephemeris) "
               << RinexSatID(sat) << " at time " << printTime(time, timfmt);
            markSatellite(sat);
            continue;
         }
         
         svp(satIndex, X) = pvt.x[X];
         svp(satIndex, Y) = pvt.x[Y];
         svp(satIndex, Z) = pvt.x[Z];
            // TODO: why is this a `+`?
         svp(satIndex, P_HAT) = pseudoranges[satIndex] + C_MPS * (pvt.clkbias + pvt.relcorr);

         LOG(DEBUG) << "SVP: Sat " << RinexSatID(sat)
            << " PR " << std::fixed << std::setprecision(3) << pseudoranges[satIndex]
            << " clkbias " << C_MPS * pvt.clkbias
            << " relcorr " << C_MPS * pvt.relcorr;
      }

      return svp;
   }


   void PRSolution ::
   computeSlopes(Vector<double>& slopes, 
                const Matrix<double>& partials, 
                const Matrix<double>& G, 
                const FilteredConstSats& filteredSats) const
   { 
      slopes.resize(filteredSats.size(), 0.0);
       
      Matrix<double> PG(partials * G);

      for (size_t i = 0; i < filteredSats.size(); ++i)
      {
         const SatID &sat = filteredSats[i].second;

            // When one (few) sats have their own clock, PG(j,j) = 1 (nearly 1)
            // and slope is inf (large)
         if (std::fabs(1.0 - PG(i, i)) < 1.e-8)
         {
            continue;
         }

         for (int k = 0; k < G.rows(); ++k)
         {
            slopes(i) += G(k, i) * G(k, i);
         }

         int n = filteredSats.size();
         slopes(i) = SQRT(slopes(i) * double(n - G.rows()) / (1.0 - PG(i, i)));
      }
   }


   Matrix<double> PRSolution ::
   filterWeights(const Matrix<double>& weights, 
                 const FilteredConstSats& filteredSats) const
   {
      Matrix<double> filteredWeights(filteredSats.size(), filteredSats.size(), 0.0);
      
      int n = 0;
      for (const auto &kvI : filteredSats)
      {
         int satI = kvI.first;
         int k = 0;
         for (const auto &kvJ : filteredSats)
         {
            int satJ = kvJ.first;
            filteredWeights(n, k) = weights(satI, satJ);
            ++k;
         }
         ++n;
      }
      
      return filteredWeights;
   }


   std::vector<SatelliteSystem> PRSolution ::
   filterGNSS(const std::vector<SatelliteSystem>& allowedGNSS, 
              const FilteredConstSats& filteredSats) const
   {
      std::vector<SatelliteSystem> tempGNSS;
      for (const auto &kv : filteredSats)
      {
         const SatID &sat = kv.second;
      
         if (vectorindex(tempGNSS, sat.system) == -1)
         {
            tempGNSS.push_back(sat.system);
         }
      }

         // must sort as in allowedGNSS
      std::vector<SatelliteSystem> currGNSS;
      for (SatelliteSystem system : allowedGNSS)
      {
         if (vectorindex(tempGNSS, system) != -1)
         {
            currGNSS.push_back(system);
         }
      }

      return currGNSS;
   }



   const Vector<double> PRSolution::PRSNullVector;

} // namespace gnsstk
