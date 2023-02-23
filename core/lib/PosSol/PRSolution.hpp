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

/// @file PRSolution.hpp
/// Pseudorange navigation solution, either a simple solution using all the
/// given data, or a solution including editing via a RAIM algorithm.

#ifndef PRS_POSITION_SOLUTION_HPP
#define PRS_POSITION_SOLUTION_HPP

#include <tuple>
#include <vector>
#include <functional>

#include <ostream>
#include "gnsstk_export.h"
#include "stl_helpers.hpp"
#include "GNSSconstants.hpp"
#include "CommonTime.hpp"
#include "RinexSatID.hpp"
#include "Stats.hpp"
#include "Matrix.hpp"
#include "Namelist.hpp"
#include "NavLibrary.hpp"
#include "TropModel.hpp"

namespace gnsstk
{
   using FilteredSats = std::vector<std::pair<int, std::reference_wrapper<SatID>>>;
   using FilteredConstSats = std::vector<std::pair<int, std::reference_wrapper<const SatID>>>;

   /** @defgroup GNSSsolutions GNSS solution algorithms
    * Solution algorithms and Tropospheric models. */
   //@{

   /// Class WtdAveStats encapsulates statistics on the PR solution and residuals
   /// for a time series of data.
   class WtdAveStats {
   private:
      unsigned int N;
        //unused
      //double APV;
      std::string msg;
      std::string lab[3];
      Stats<double> S[3];
      Matrix<double> sumInfo;
      Vector<double> sumInfoState,Sbias;

   public:

      // ctor
      WtdAveStats(void)
      {
         reset();
         lab[0]="ECEF_X";  lab[1]="ECEF_Y"; lab[2]="ECEF_Z";
      }

      void setMessage(std::string m) { msg = m; }
      std::string getMessage(void) const { return msg; }

      void setLabels(std::string lab1, std::string lab2, std::string lab3)
         { lab[0]=lab1; lab[1]=lab2; lab[2]=lab3; }

      Vector<double> getSol(void) const
      {
         return (getCov()*sumInfoState + Sbias);
      }

      Matrix<double> getCov(void) const { return inverseSVD(sumInfo); }

      Matrix<double> getInfo(void) const { return sumInfo; }

      int getN(void) const { return N; }

      void reset(void)
      {
         N = 0;
         sumInfo = Matrix<double>();
         sumInfoState = Vector<double>();
         Sbias = Vector<double>(3);
         S[0].Reset();
         S[1].Reset();
         S[2].Reset();
      }

      // add to statistics, and to weighted average solution and covariance
      void add(const Vector<double>& Sol, const Matrix<double>& Cov)
      {
         try {
            // add to the statistics
            for(unsigned int i=0; i<3; i++) {
               if(N==0) Sbias(i) = Sol(i);
               S[i].Add(Sol(i)-Sbias(i));
            }

            // NB do NOT include clock(s); this can ruin the position average
            Vector<double> Sol3(Sol);
            Sol3.resize(3);               // assumes position states come first
            Sol3 = Sol3 - Sbias;
            Matrix<double> Cov3(Cov,0,0,3,3);

            // information matrix (position only)
            Matrix<double> Info(inverseSVD(Cov3));
            if(N == 0) {                  // first call: dimension and set to zero
               sumInfo = Matrix<double>(3,3,0.0);
               sumInfoState = Vector<double>(3,0.0);
            }

            // add to the total information
            sumInfo += Info;
            sumInfoState += Info * Sol3;
            ++N;
         }
         catch(Exception& e) { GNSSTK_RETHROW(e); }
      }

      // dump statistics and weighted average
      void dump(std::ostream& os, std::string msg="") const
      {
         try {
            os << "Simple statistics on " << msg << std::endl
               << std::fixed << std::setprecision(3);
            if(N > 0) {
               os << "  " << lab[0] << " N: " << S[0].N()
                  << std::fixed << std::setprecision(4)
                  << " Ave: " << S[0].Average()+Sbias[0]
                  << " Std: " << S[0].StdDev()
                  << " Min: " << S[0].Minimum()+Sbias[0]
                  << " Max: " << S[0].Maximum()+Sbias[0]
                  << std::endl;
               os << "  " << lab[1] << " N: " << S[1].N()
                  << std::fixed << std::setprecision(4)
                  << " Ave: " << S[1].Average()+Sbias[1]
                  << " Std: " << S[1].StdDev()
                  << " Min: " << S[1].Minimum()+Sbias[1]
                  << " Max: " << S[1].Maximum()+Sbias[1]
                  << std::endl;
               os << "  " << lab[2] << " N: " << S[2].N()
                  << std::fixed << std::setprecision(4)
                  << " Ave: " << S[2].Average()+Sbias[2]
                  << " Std: " << S[2].StdDev()
                  << " Min: " << S[2].Minimum()+Sbias[2]
                  << " Max: " << S[2].Maximum()+Sbias[2]
                  << std::endl;

               os << "Weighted average " << msg << std::endl;
               Matrix<double> Cov(inverseSVD(sumInfo));
               Vector<double> Sol(Cov * sumInfoState + Sbias);
               os << std::setw(14) << std::setprecision(4) << Sol << "    " << N;
            }
            else os << " No data!";
         }
         catch(Exception& e) { GNSSTK_RETHROW(e); }
      }

      friend std::ostream& operator<<(std::ostream& s, const WtdAveStats& as);

   }; // end class WtdAveStats

   /// This class defines an interface to routines which compute a position
   /// and time solution from pseudorange data, with a data editing algorithm
   /// based on Receiver Autonomous Integrity Monitoring (RAIM) concepts.
   /// RAIM ref. "A Baseline GPS RAIM Scheme and a Note on the Equivalence of
   /// Three RAIM Methods," by R. Grover Brown, Journal of the Institute of
   /// Navigation, Vol. 39, No. 3, Fall 1992, pg 301.
   ///
   /// The main point of entry is RAIMCompute(); it will compute a solution given
   /// the pseudoranges from a number of satellites, using a RAIM-based algorithm
   /// to detect and exclude 'bad' data from the solution. Alternately, the user
   /// may compute a straightforward solution using all the input data, without
   /// the RAIM algorithm; this is done by first calling PreparePRSolution()
   /// and then SimplePRSolution().
   ///
   /// The class is able to use satellite data from any GNSS (defined in SatID.hpp).
   /// The user MUST specify allowed GNSS in the vector allowedGNSS before processing.
   /// The algorithm estimates a separate clock bias for each system; the state
   /// vector (Solution) thus has components X,Y,Z,clk1,clk2,clk3... where the clocks
   /// are in the order given by the vector allowedGNSS. The time system of the clocks
   /// will be determined by the input ephemerides; usually IGS SP3 ephemerides use
   /// GPS time for all the systems (but there is still a system time offset relative
   /// to GPS for each other GNSS).
   ///
   /// Note that at any epoch it may happen that no satellites from some system are
   /// available (either in the data or after the RAIM algorithm), in this case the
   /// clock bias for that system is undefined and set to zero.

   class PRSolution
   {
   public:
         /// Constructor
       PRSolution() : RMSLimit(6.5),
                      SlopeLimit(1000.),
                      NSatsReject(-1),
                      MaxNIterations(10),
                      ConvergenceLimit(3.e-7),
                      hasMemory(true),
                      fixedAPriori(false),
                      nsol(0), ndata(0), APV(0.0),
                      Valid(false)
         {
            was.reset();
            APSolution = Vector<double>(4,0.0);
         }

      /// Return the status of solution
      bool isValid() const { return Valid; }

      // input parameters: -------------------------------------------------

      /// RMS limit (m) on residual of fit
      double RMSLimit;

      /// Slope limit (dimensionless).
      double SlopeLimit;

      /// Maximum number of satellites that may be rejected in the RAIM algorithm;
      /// if this = -1, as many as possible will be rejected (RAIM requires at least 5
      /// satellites). A (single) non-RAIM solution can be obtained by setting this
      /// to 0 before calling RAIMCompute().
      int NSatsReject;

      /// Maximum number of iterations allowed in the linearized least squares
      /// algorithm.
      int MaxNIterations;

      /// Convergence limit (m): continue iteration loop while RSS change in
      /// solution exceeds this.
      double ConvergenceLimit;

      /// vector<SatelliteSystem> containing the satellite systems allowed
      /// in the solution. **This vector MUST be defined before computing solutions.**
      /// It is used to determine which clock biases are included in the solution,
      /// as well as the apriori state vector (see hasMemory below)
      std::vector<SatelliteSystem> allowedGNSS;

      /// This determines whether this object will maintain a "memory" of all the
      /// solutions it has computed. This is used for several things, including the
      /// computation of pre-fit residuals, and thus of the aposteriori variance of
      /// unit weight (APV), the number of data, solutions and degrees of freedom
      /// and a combined weighted average solution.
      /// Most importantly, it causes the estimation algorithm at each epoch to be
      /// initialized with an apriori solution, which it 'remembers' from previous
      /// epochs. If multiple GNSS are used in the estimation, at any epoch, then
      /// then setAprioriGNSS() must be called before any processing, otherwise the
      /// system clock of any missing system will not be part of the apriori state.
      bool hasMemory;

      // input and output: -------------------------------------------------

      /// vector<SatID> containing satellite IDs for all the satellites input, with
      /// bad (excluded) ones identified by (Satellite[.] < 0). This vector is saved
      /// after each call to the computation routines (SimplePRSolution and
      /// RAIMCompute) and used for printing.
      std::vector<SatID> SatelliteIDs;

      // output: -------------------------------------------------

      /// Vector<double> containing the computed position solution (3 components,
      /// ECEF in the frame of the ephemeris, meter), the receiver clock bias (m),
      /// and the GPS-GLO time offset (m). In the case of GPS-only or GLO-only
      /// data, the last element is zero and has no meaning. This vector is valid
      /// only when isValid() is true.
      /// If this vector is defined on input, it is used as an apriori position, both
      /// to initialized the iterative algorithm, and to compute position residuals
      /// after a good solution is found.
      Vector<double> Solution;

      /// Matrix<double> containing the computed solution covariance (meter^2);
      /// see doc. for Solution for the components; valid only when isValid() is true.
      Matrix<double> Covariance;

      /// Matrix<double> containing the inverse measurement covariance matrix (m^-2)
      /// that was used in computing the final solution.
      Matrix<double> invMeasCov;

      /// Matrix<double> containing the partials matrix used in the final solution.
      Matrix<double> Partials;

      /// vector<SatelliteSystem> containing the satellite systems found in
      /// the data at each epoch, after calls to SimplePRSolution and RAIMCompute.
      /// See also allowedGNSS.
      std::vector<SatelliteSystem> dataGNSS;

      /// The "memory" of this object, used only when hasMemory is true.
      WtdAveStats was;
      double APV;
      int ndata,nsol,ndof;
      /// if true, use the given APriori position instead of the current solution
      /// define by calling void fixAPSolution(X,Y,Z)
      bool fixedAPriori;
      Triple fixedAPrioriPos;
      /// Caller is responsible for setting APSolution before first call, if desired;
      /// after that SimplePRSolution() and RAIMCompute() will update it.
      Vector<double> APSolution;


      /// Prefit residuals; only valid if memory exists b/c it needs apriori solution.
      /// Vector<double> of 'pre-fit' residuals, computed by the solution routines,
      /// but only if APrioriSol is defined; equal to Partials*(Sol-APrioriSol)-Resid
      /// where Resid is the data residual vector on the first iteration.
      Vector<double> PreFitResidual;

      /// Root mean square residual of fit (except when RMSDistanceFlag is set,
      /// then RMS distance from apriori position); in meters.
      double RMSResidual;

      /// Slope computed in the RAIM algorithm (largest of all satellite values)
      /// for the returned solution, dimensionless.
      double MaxSlope;

      /// DOPs computed in a call to DOPCompute() or outputString()
      double TDOP,PDOP,GDOP;

      /// the actual number of iterations used
      int NIterations;

      /// the RSS change in solution at the end of iterations.
      double Convergence;

      /// the number of good satellites used in the final computation
      int Nsvs;

      /// if true, the returned solution may be degraded because the tropospheric
      /// correction was not applied to one or more satellites; applies after calls to
      /// both SimplePRSolution() and RAIMCompute().
      bool TropFlag;

      /// if true, the returned solution may be degraded because the RMS residual or
      /// the slope is large; applies only after calls to RAIMCompute().
      bool RMSFlag, SlopeFlag;

      // member functions -------------------------------------------

      /// Compute the satellite position / corrected range matrix (SVP) which is used
      /// by SimplePRSolution(). SVP is output, dimensioned (N,4) where N is the
      /// number of satellites and the length of both Satellite and Pseudorange.
      /// Data is ignored whenever Sats[i].id is < 0 and when system is not in
      /// allowedGNSS. NB caller should verify that the number of good entries
      /// (Satellite[.] > 0) is > 4 before proceeding.
      /// Even though this is a member function, it changes none of the member data.
      /// @param Tr          input Measured time of reception of the data.
      /// @param Sats        input std::vector<SatID> of satellites; satellites that
      ///                     are to be excluded by the algorithm are marked by a
      ///                     negative 'id' member; this call will mark satellites for
      ///                     which there is no ephemeris.
      /// @param Pseudorange input std::vector<double> of raw pseudoranges (parallel
      ///                     to Sats), in meters
      /// @param eph[in]     NavLibrary to be used
      /// @param SVP         output gnsstk::Matrix<double> of dimension (N,4), N is
      ///                     the number of satellites in Sats[] (marked or not),
      ///                     on output this contains the satellite positions at
      ///                     transmit time (cols 0-2), the corrected pseudorange (1).
      /// @param[in] order   How NavLibrary searches are performed.
      /// @return Return values:
      ///  >= 0 number of good satellites found
      /// -4    ephemeris not found for all the satellites
      int PreparePRSolution(const CommonTime& Tr,
                            std::vector<SatID>& Sats,
                            const std::vector<double>& Pseudorange,
                            NavLibrary& eph,
                            Matrix<double>& SVP,
                            NavSearchOrder order = NavSearchOrder::User) const;

      /// Compute a single autonomous pseudorange solution, after calling
      /// PreparePRSolution(). On output, all the member data is filled with results.
      ///
      /// Input only (first 3 should be just as returned from PreparePRSolution()):
      /// @param Tr          const. Measured time of reception of the data.
      ///                     On output member currTime set to this.
      /// @param Sats        const std::vector<SatID> of satellites. Satellites
      ///                     that are to be excluded by the algorithm are marked by a
      ///                     negative 'id' member. Length N.
      ///                     Also systems not in allowedGNSS are ignored.
      ///                     On output member SatelliteIDs set to this.
      /// @param SVP         const Matrix<double> of dimension (N,5) contains sat.
      ///                     direction cosines and corrected pseudorange data.
      /// @param invMC       const gnsstk::Matrix<double> NXN measurement covariance
      ///                     matrix inverse (meter^-2) of the pseudorange data (for N
      ///                     see Sats). If this matrix has dimension 0, no weighting
      ///                     of the data is done.
      /// @param pTropModel  pointer to a gnsstk::TropModel for trop correction.
      ///
      /// Input and output:
      /// @param niterLimit  integer limit on the number of iterations. On output,
      ///                     member NIterations = number of iterations actually used.
      /// @param convLimit   double convergence criterion, = RSS change in solution,
      ///                     in meters. On output, member Convergence = final value.
      ///
      /// Output:  (these will be resized within the function)
      /// @param Resids      gnsstk::Vector<double> post-fit range residuals for each
      ///                     satellite (m), the length of this Vector is the number
      ///                     of satellites actually used (see Sats).
      /// @param Slopes      gnsstk::Vector<double> slope value used in RAIM for each
      ///                     good satellite, length m.
      ///
      /// @return Return values:
      ///  0  ok      (but check TropFlag to see if trop. correction was not applied)
      /// -1  failed to converge
      /// -2  singular problem
      /// -3  not enough good data to form a solution (at least 4 satellites required)
      int SimplePRSolution(const CommonTime& Tr,
                           const std::vector<SatID>& Sats,
                           const Matrix<double>& SVP,
                           const Matrix<double>& invMC,
                           TropModel *pTropModel,
                           const int& niterLimit,
                           const double& convLimit,
                           Vector<double>& Resids,
                           Vector<double>& Slopes);

      /// Compute a RAIM solution without the measurement covariance matrix,
      /// i.e. without measurement weighting.
      int RAIMComputeUnweighted(const CommonTime& Tr,
                                std::vector<SatID>& Satellites,
                                const std::vector<double>& Pseudorange,
                                NavLibrary& eph,
                                TropModel *pTropModel,
                                NavSearchOrder order = NavSearchOrder::User);

      /// Compute a position/time solution, given satellite PRNs and pseudoranges
      /// using a RAIM algorithm. This is the main computation done by this class.
      /// Before this call, allowedGNSS must be defined.
      /// @param Tr          Measured time of reception of the data.
      /// @param Satellites  std::vector<SatID> of satellites; on successful
      ///                    return, satellites that were excluded by the algorithm
      ///                    are marked by a negative 'id' member.
      ///                    Also systems not in allowedGNSS are ignored.
      /// @param Pseudorange std::vector<double> of raw pseudoranges (parallel to
      ///                    Satellite), in meters.
      /// @param invMC       gnsstk::Matrix<double> NXN measurement covariance matrix
      ///                    inverse (meter^-2) of the pseudorange data (for N
      ///                    see Sats). If this matrix has dimension 0, no weighting
      ///                    of the data is done.
      /// @param eph         NavLibrary to be used in the algorithm.
      /// @param pTropModel  pointer to gnsstk::TropModel for trop correction.
      ///
      /// @return Return values:
      ///  1  solution is ok, but may be degraded; check TropFlag, RMSFlag, SlopeFlag
      ///  0  ok
      /// -1  algorithm failed to converge
      /// -2  singular problem, no solution is possible
      /// -3  not enough good data (> 4) to form a (RAIM) solution
      ///     (the 4 satellite solution might be ok)
      /// -4  ephemeris not found for all the satellites
      int RAIMCompute(const CommonTime& Tr,
                      std::vector<SatID>& Satellites,
                      const std::vector<double>& Pseudorange,
                      const Matrix<double>& invMC,
                      NavLibrary& eph,
                      TropModel *pTropModel,
                      NavSearchOrder order = NavSearchOrder::User);

      /// Compute DOPs using the partials matrix from the last successful solution.
      /// RAIMCompute(), if successful, calls this before returning.
      /// Results stored in PRSolution::TDOP,PDOP,GDOP.
      int DOPCompute(void);

      // output -----------------------------------------------------
      /// conveniences for printing the results of the pseudorange solution algorithm
      /// return string of position, error code and V/NV
      std::string outputPOSString(const std::string& tag, int iret=-99,
                                    const Vector<double>& Vec=PRSNullVector);

      /// return string of {SYS clock} for all systems, error code and V/NV
      std::string outputCLKString(std::string tag, int iret=-99);

      /// return string of info in POS and CLK
      std::string outputNAVString(std::string tag, int iret=-99,
                                    const Vector<double>& Vec=PRSNullVector);

      /// return string of Nsvs, RMS residual, TDOP, PDOP, GDOP, Slope, niter, conv,
      /// satellites, error code and V/NV
      std::string outputRMSString(const std::string& tag, int iret=-99);
      std::string outputValidString(int iret=-99);

      /// return string of NAV and RMS strings
      std::string outputString(const std::string& tag, int iret=-99,
                               const Vector<double>& Vec=PRSNullVector);
      /// return string of the form "#tag label etc" which is header for data strings
      std::string outputStringHeader(std::string tag)
         { return outputString(tag,-999); }

      /// A convenience for printing the error code (return value)
      std::string errorCodeString(int iret);

      /// A convenience for printing the current configuarion
      std::string configString(const std::string& tag);

      // ------------------------------------------------------------
      /// Fix the apriori solution to the given constant value (XYZ,m)
      /// and initialize the
      void fixAPSolution(const double& X, const double& Y, const double& Z)
      {
         //fixedAPriori = true;   //TD user input

         fixedAPrioriPos[0] = X;
         fixedAPrioriPos[1] = Y;
         fixedAPrioriPos[2] = Z;

         if(hasMemory) {
            APSolution = Vector<double>(3+allowedGNSS.size(),0.0);
            for(unsigned int i=0; i<3; i++)
               APSolution(i) = fixedAPrioriPos[i];
         }
      }

      /// get the aposteriori variance of unit weight; return zero if not enough
      /// data has been collected.
      inline double getAPV(void) {
         if(ndof > 0) return APV/ndof;
         return 0.0;
      }

      // dump solution, statistics and weighted average
      void dumpSolution(std::ostream& os, std::string msg="PRS")
      {
         try {
            was.setMessage(msg);
            os << was << std::endl;

            Matrix<double> Cov(was.getCov());
            double sig(ndof > 0 ? ::sqrt(APV/ndof) : 0.0);
            if(ndof > 0)               // scale covariance
            {
               for(size_t i=0; i<Cov.rows(); i++)
               {
                  for(size_t j=i; j<Cov.cols(); j++)
                     Cov(i,j) = Cov(j,i) = Cov(i,j)*sig;
               }
            }
            // print cov as labeled matrix
            Namelist NL;
            NL += "ECEF_X"; NL += "ECEF_Y"; NL += "ECEF_Z";
            LabeledMatrix LM(NL,Cov);
            LM.scientific().setprecision(3).setw(14).symmetric(true);

            os << "Covariance: " << msg << std::endl << LM << std::endl;
            os << "APV: " << msg << std::fixed << std::setprecision(3)
               << " sigma = " << sig << " meters with "
               << ndof << " degrees of freedom.\n";
         }
         catch(Exception& e) { GNSSTK_RETHROW(e); }
      }

      /// update apriori solution with a known solution; this is done at the end of
      /// both SimplePRSolution() and RAIMCompute()
      void updateAPSolution(const Vector<double>& Sol)
      {
         int k;
         unsigned int i;

         // first call
         if(APSolution.size() == 0) {
            APSolution = Vector<double>(3+allowedGNSS.size(),0.0);
            for(i=0; i<3; i++)
               APSolution(i) = fixedAPrioriPos[i];
         }

         // must expand Sol to have all allowed clocks
         Vector<double> S(3+allowedGNSS.size(),0.0);
         for(i=0; i<3; i++)
            S(i) = (fixedAPriori ? fixedAPrioriPos[i] : Sol(i));
         for(i=0; i<allowedGNSS.size(); i++) {
            k = vectorindex(dataGNSS,allowedGNSS[i]);
            S(3+i) = (k == -1 ? APSolution[3+i] : Sol[3+k]);
         }

         APSolution = S;

         //APSolution = Sol;
         //if(fixedAPriori)
         //   for(int i=0; i<3; i++) APSolution(i) = fixedAPrioriPos[i];
      }

      /// add newly computed solution (must be valid); update counts, APV and apriori.
      /// input parameters are from PRSolution after computing a solution.
      void addToMemory(const Vector<double>& Sol, const Matrix<double>& Cov,
               const Vector<double>& PreFitResid, const Matrix<double>& Partials,
               const Matrix<double>& invMeasCov)
      {
         was.add(Sol, Cov);

         // first solution: apriori solution has no clock, so PFR bad
         if(was.getN() == 1) return;

         try {
            // consider only the XYZ states, ignore clocks
            Matrix<double> Part(Partials,0,0,Partials.rows(),3);
            Matrix<double> invMC(invMeasCov);
            if(invMC.rows() == 0) {
               invMC=Matrix<double>(Part.rows(),Part.rows());
               ident(invMC);
            }
            Matrix<double> sumInfo(was.getInfo());
            Matrix<double> Ginv(Part*sumInfo*transpose(Part) + invMC);
            Matrix<double> G(inverseSVD(Ginv));
            Vector<double> Gpfr(G*PreFitResid);
            APV += dot(PreFitResid,Gpfr);
            ndata += PreFitResid.size();
            ndof = ndata-sumInfo.rows();
         }
         catch(Exception& e) {
            e.addText("APV failed.");
            GNSSTK_RETHROW(e);
         }
      }

   private:

         /// flag: output content is valid.
      bool Valid;

         /// time tag of the current solution
      CommonTime currTime;

         /// time formats used in prints
      static const std::string calfmt,gpsfmt,timfmt;

         /// empty vector used to detect default
      GNSSTK_EXPORT static const Vector<double> PRSNullVector;


         /// Mark SVs that are disallowed GNSS
      void markDisallowedGNSS(
            std::vector<SatID>& sats, 
            const std::vector<SatelliteSystem>& allowed) const;


         /** Compute the time-of-flight corrected range between SV and RX.
          *
          * @param[in,out] svPos the SV's position at transmit time. This will be
          *    updated to account for the rotation of the ECEF frame during the
          *    transmission.
          * @param[in] rxPos the receiver's ECEF position.
          * @param[in] firstIteration specify if this is the first iteration
          *    in an iterative weighted least squares solution.
          *    If True, the SV to RX range uses an intial estimate.
          * @returns the distance between the SV at transmit time and the
          *    receiver at receive time.
          */
      double computeRange(
            Position& svPos, 
            const Position& rxPos, 
            bool firstIteration) const;


         /** Compute tropospheric delay.
          *
          * @param[in] svPos the ECEF position of the SV.
          * @param[in] rxPos the ECEF position of the receiver.
          * @param[in] pTropModel the tropospheric model to compute the delay.
          * @param[in] time the receiver epoch time.
          * @return a tuple of tropSuccess (bool) and delay correction (double).
          *    The tropSuccess is true if the tropospheric delay was
          *    computed without issue. A trop correction will not be computed
          *    for certain scenarios -- negative SV elevation,
          *    or unreasonable rx height -- which may come from a poorly known
          *    receiver position.
          */
      std::tuple<bool, double> computeTropDelay(
            const Position& svPos, 
            const Position& rxPos, 
            TropModel *pTropModel, 
            const CommonTime& time) const;
         

         /** Mark the satellite to remove from processing.
          *
          * Marking is currently performed by setting the Satellite ID as
          * negative. 
          *
          * @param[in, out] sat Satellite to mark. 
          *    The sat's id will be modified.
          */
      void markSatellite(SatID& sat) const;


         /** Checks if a satellite is marked.
          *
          * See markSatellite() for marking a satellite.
          *
          * @param[in] sat the SatID to check.
          * @return true if the satellite is marked.
          */
      bool isMarked(const SatID& sat) const;
   

         /** Create a filtered view, that cannot be modified, of unmarked sats.
          *
          * @param[in] sats vector of SatIDs that may be marked.
          * @returns a vector of pairs where each pair consists
          * of the index into the original sats vector and a reference to the
          * sat at that index.
          */
      FilteredConstSats filterMarkedSats(const std::vector<SatID>& sats) const;
      

         /** Create a filtered view, that can be modified, of unmarked sats.
          *
          * @param[in] sats vector of SatIDs that may be marked.
          * @returns a vector of pairs where each pair consists
          * of the index into the original sats vector and a reference to the
          * sat at that index.
          */
      FilteredSats filterMarkedSats(std::vector<SatID>& sats) const;
      
         
         /** Search for satellite ephemeris at the SV transmit time.
          *
          * Uses pseudorange obs to estimate the SV's transmit time.
          *
          * @param[out] satXvt the resulting SV ephemeris, if found.
          * @param[in] nominalReceive the receiver's nominal receive time
          *    of the provided pseudorange.
          * @param[in] pseudorange the observed pseudorange of the SV at the
          *    nominalReceive time.
          * @param[in] eph the ephemeris to search through.
          * @param[in] sat the SatID to find ephemeris for.
          * @param[in] order Specify whether to search by receiver behavior
          *    or by nearest in time.
          * @return true if ephemeris was found for the SV.
          */
      bool getSatPVT(
            Xvt& satXvt, 
            const CommonTime& nominalReceive, 
            double pseudorange, 
            NavLibrary& eph, 
            SatID& sat, 
            NavSearchOrder order) const;


         /* Compute the single point weighted least squares solution.
          *
          * At each receiver epoch, the set of pseudorange residual equations
          * can be linearized as such:
          *
          * @f[
          *    \delta p = P * \delta x + \delta\epsilon
          * @f]
          *
          * where @f$\delta p@f$ is the pseudorange residual (i.e.
          * measured minus predicted), @f$P@f$ is the partial derivatives
          * matrix (a.k.a. the geometry matrix), @f$\delta x@f$ is the correction
          * to the initial state estimate, and @f$\delta\epsilon@f$ is the
          * measurement error.
          *
          * In this equation, the @f$\delta x@f$ correction is unknown but
          * the residuals vector and partials matrix can be computed with 
          * known information. The state estimate correction must be solved
          * by using a weighted least squares regression.
          *
          * @f[
          * \delta x \approx (P^{T} * W * P)^{-1} * P^{T} * W * \delta p
          * @f]
          *
          * The W matrix is a weight matrix that assigns a weight to each 
          * observation. If all observations are equally weighted then an
          * identity matrix can be used. The most common weight matrix
          * is to use the inverse of the measurement covariance matrix.
          * Another possible weighting scheme is to set less weight to lower
          * elevation observations.
          *
          * @param[out] dX the correction to the a priori state estimate
          *    as solved by the weighted least squares regression.
          * @param[out] partials the partials matrix (a.k.a. geometry matrix)
          *    of the linearized pseudorange resdiual equation. This matrix
          *    contains the line-of-sight unit vectors and clock state index
          *    for each sat. The size of this matrix will be NxM where
          *    N is the number of unmarked sats and M is size of the state
          *    estimate (i.e. position (3) + rx clock biases in a given time
          *    system (1 per system).
          * @param[out] G the weighted least squares solution matrix.
          *    i.e. G = (P^T * W * P)^-1 * P^T * W , where P is the partials matrix
          *    and W is the weight matrix.
          * @param[out] covariance the computed covariance matrix from the
          *    weighted least squares solution.
          *    i.e. covariance = (P^T * W * P)^-1
          * @param[out] residuals the computed pseudorange residuals from
          *    the last iteration.
          * @param[in] aPriorSolution the initial estimate of the receiver
          *    position and receiver clock biases in each satellite system's
          *    time.
          * @param[in] filteredSats a view of the unmarked sats for the
          *    current solution.
          * @param[in] svp the matrix of SV positions and a portion of the
          *    pseudorange residuals. This matrix is sized by the original
          *    full vector of sats and must be indexed by the original indices
          *    of the sat as stored in filteredSats.
          * @param[in] pTropModel the tropospheric model to generate the 
          *    tropospheric delay at the given time for each SV.
          * @param[in] nominalReceive the receiver epoch time.
          * @param[in] firstIteration specify if this is the first iteration
          *    in an iterative weighted least squares solution.
          *    If True, the tropospheric delay is not applied and the SV to RX
          *    range uses an intial estimate. If the aPrioriSolution is 
          *    extremely off then the tropospheric delay can potentially
          *    cause the solution to never converge. Therefore, on the first
          *    iteration, the tropospheric delay is skipped in order to compute
          *    a good first-order solution.
          * @param[in] currGNSS the set of systems for the current solution.
          *    Used to determine the size and ordering of the state estimate
          *    vector.
          * @param[in] weights a weight matrix for the unmarked sats in the
          *    current weighted least squares solution.
          * @return true if the tropospheric delay was evaluated and applied
          *    without issues.
          */
      bool singlePointWLSSolution(
            Vector<double>& dX,
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
            const Matrix<double>& weights) const;
   

         /** Iteratively compute a single point weighted least squares solution.
          * 
          * If the a priori estimate is off by a lot, the single point
          * least squares solution can be solved iteratively until the 
          * change in the state estimate is small. This function returns
          * the total correction to the original a priori solution.
          *
          * The iterative single point least square solution can be used as a 
          * generalized version of the least square solution even if the
          * a priori solution is good. A better a priori estimate will
          * result in fewer iterations.
          *
          * See singlePointWLSSolution() for more info.
          *
          * @param[out] dX the total correction to the a priori state estimate.
          * @param[out] partials the partials matrix (a.k.a. geometry matrix)
          *    of the linearized pseudorange resdiual equation from the
          *    final iteration.
          * @param[out] G the weighted least squares solution matrix from the
          *    final iteration.
          *    i.e. G = (P^T * W * P)^-1 * P^T * W , where P is the partials matrix
          *    and W is the weight matrix.
          * @param[out] covariance the computed covariance matrix from the
          *    weighted least squares solution from the last iteration.
          *    i.e. covariance = (P^T * W * P)^-1
          * @param[out] residuals the computed pseudorange residuals from
          *    the last iteration.
          * @param[in] aPriorSolution the initial estimate of the receiver
          *    position and receiver clock biases to each satellite system's
          *    time.
          * @param[in] filteredSats a view of the unmarked sats for the
          *    current solution.
          * @param[in] svp the matrix of SV positions and a portion of the
          *    pseudorange residuals. This matrix is sized by the original
          *    full vector of sats and must be indexed by the original indices
          *    of the sat as stored in filteredSats.
          * @param[in] pTropModel the tropospheric model to generate the 
          *    tropospheric delay at the given time for each SV.
          * @param[in] nominalReceive the receiver epoch time.
          * @param[in] currGNSS the set of systems for the current solution.
          *    Used to determine the size and ordering of the state estimate
          *    vector.
          * @param[in] weights a weight matrix for the unmarked sats in the
          *    current weighted least squares solution.
          * @param[in] convLimit the convergence threshold for the norm of the
          *    state estimate correction.
          * @param[in] niterLimit a max number of iterations.
          * @return a tuple of tropSuccess (bool), the last computed convergence
          *    test (double), and the number actual number of iterations
          *    used in the solution (int). The tropSuccess returns true if
          *    the tropospheric delay was correctly applied on the final 
          *    iteration. The returned convergence and iterations can be used
          *    to determine if convergence failed.
          */
      std::tuple<bool, double, int> iterativeSinglePointWLSSolution(
            Vector<double>& dX,
            Matrix<double>& partials,
            Matrix<double>& G,
            Matrix<double>& covariance,
            Vector<double>& residuals,
            const Vector<double>& aPrioriSolution, 
            const FilteredConstSats& filteredSats, 
            const Matrix<double>& svp, 
            TropModel *pTropModel, 
            const CommonTime& nominalRecieve, 
            const std::vector<SatelliteSystem>& currGNSS,
            const Matrix<double>& weights,
            double convLimit,
            int niterLimit) const;

         /** Compute the partial derivates matrix and pseudorange residuals.
          *
          * The partials matrix, also known as the geometry matrix in GNSS
          * literature, is the matrix consisting of line-of-sight unit vectors
          * and clock status for each satellite in the solution. Here, the
          * matrix is denoted as `partials` or more simply @f$P@f$.
          *
          * The pseudorange measurement residuals are the differences between
          * predicted and observed pseudorange measurements. The residuals
          * vector is usually denoted as @f$\delta p@f$.
          *
          * Together, these quantities make up the linearized measurement
          * residuals equation that needs to be solved.
          *
          * @f[
          * \delta p = P\delta x + \delta\epsilon
          * @f]
          *
          * where @f$\delta x@f$ is a correction to the a priori state estimate
          * and @f$\delta\epsilon@f$ is measurement error.
          *
          * @param[out] partials the partial derivatives matrix for the 
          *    linearized measurement residuals of the given sats. This matrix 
          *    will be sized NxM where N is the number of sats and M is 
          *    dimension of the state estimate
          *    (i.e. position (3) + number of system clocks).
          * @param[out] residuals the computed pseudorange residuals.
          *    This vector will be size N where N is the nubmer of sats.
          * @param[in] filteredSats a view of the unmarked sats for the
          *    current solution.
          * @param[in] svp the matrix of SV positions and a portion of the
          *    pseudorange residuals. This matrix is sized by the original
          *    full vector of sats and must be indexed by the original indices
          *    of the sat as stored in filteredSats.
          * @param[in] aPrioriSolution the inital conditions of the state
          *    estimate. Might be a previously surveyed position or a
          *    previous solution.
          * @param[in] firstIteration specify if this is the first iteration
          *    in an iterative weighted least squares solution.
          *    If True, the tropospheric delay is not applied and the SV to RX
          *    range uses an intial estimate. If the aPrioriSolution is 
          *    extremely off then the tropospheric delay can potentially
          *    cause the solution to never converge. Therefore, on the first
          *    iteration, the tropospheric delay is skipped in order to compute
          *    a good first-order solution.
          * @param[in] pTropModel the tropospheric model to generate the 
          *    tropospheric delay at the given time for each SV.
          * @param[in] nominalReceive the receiver epoch time.
          * @param[in] currGNSS the set of systems for the current solution.
          *    Used to determine the size and ordering of the state estimate
          *    vector.
          * @return true if the tropospheric delay was evaluated and applied
          *    without issues.
          */
      bool computePartialsAndResiduals(
            Matrix<double>& partials,
            Vector<double>& residuals,
            const FilteredConstSats& filteredSats, 
            const Matrix<double>& svp, 
            const Vector<double>& aPrioriSolution,
            bool firstIteration, 
            TropModel *pTropModel, 
            const CommonTime& nominalReceive, 
            const std::vector<SatelliteSystem>& currGNSS) const;

         /** Reduce the weight matrix to the matching filtered sats.
          *
          * @param[in] weights matrix sized by the original sats vector.
          *    For each solution computed, a set of sats are marked to be
          *    removed from the solution. Thus, the weight matrix must
          *    also be reconstructed by only keeping the weights related
          *    to the filtered sats.
          * @param[in] filteredSats the view of unmarked sats to be used
          *    in the weighted least squares solution. This view must
          *    contain the indices into the original larger sat vector
          *    to properly select the elements in the weight matrix
          */
      Matrix<double> filterWeights(
            const Matrix<double>& weights, 
            const FilteredConstSats& filteredSats) const;

         /** Compute the slopes of the linear regression solution.
          *
          * The slopes vector matches the size and ordering of the
          * *filtered* sats.
          *
          * @param[out] slopes the computed slope for each unmarked SV.
          * @param[in] partials the partial derivatives matrix from the 
          *    weighted least squares solution.
          * @param[in] G the weighted least squares solution matrix
          *    i.e. (P^T * W * P)^-1 * P^T * W , where P is the partials matrix
          *    and W is the weight matrix.
          * @params[in] filteredSats the view of unmarked sats used in the
          *    weighted least squares solution.
          */
      void computeSlopes(
            Vector<double>& slopes,
            const Matrix<double>& partials, 
            const Matrix<double>& G, 
            const FilteredConstSats& filteredSats) const;

         /** Create matrix to contain SV position and portion of pseudorange residual.
          * 
          * The order of the returned SVP matrix matches the order of the `sats`
          * vector. Use the index of the `sats` vector to index into the SVP
          * matrix. If ephemeris could not be found for a sat, the sat is marked
          * and the row in the SVP matrix is left empty.
          *
          * @param[in,out] sats SatIDs to look up ephemeris for. If the 
          *    ephemeris lookup fails for a sat it is marked.
          * @param[in] time a receiver epoch time to determine SV ephemeris.
          * @param[in] pseudorange a vector of pseudorange observations to match
          *    the vector of sats.
          * @param[in] NavLibrary to search ephemeris with
          * @param[in] order Specify whether to search by receiver behavior
          *    or by nearest in time.
          * @return a matrix where each row is an SV position (SVP) and
          *    a portion of a pseudorange residual.
          */
      Matrix<double> createSVP(
            std::vector<SatID>& sats, 
            const CommonTime& time, 
            const std::vector<double>& pseudorange, 
            NavLibrary& eph, 
            NavSearchOrder order) const;
   
         /** Filter systems down to those represented in the filtered satellites.
          * 
          * The return vector maintains the same ordering of SatelliteSystems
          * as allowedGNSS.
          *
          * @param[in] allowedGNSS containing the satellite systems allowed
          *    in the solution.
          * @param[in] filteredSats a filtered view of unmarked satellites
          *    for the current solution.
          * @return a new vector of SatelliteSystems represented in the filtered
          *    sats.
          */
      std::vector<SatelliteSystem> filterGNSS(
            const std::vector<SatelliteSystem>& allowedGNSS, 
            const FilteredConstSats& filteredSats) const;

         /// Provides human readable names to the exit codes of PRSolution
      enum RETURN_CODE {
         DEGRADED = 1,
         OK = 0,
         FAILED_CONVERGENCE = -1,
         SINGULAR_SOLUTION = -2,
         NOT_ENOUGH_SVS = -3,
         NO_EPHEMERIS = -4,
      };

      enum INDEX {
         X = 0,
         Y = 1,
         Z = 2,
         P_HAT = 3
      };

   }; // end class PRSolution

   //@}

} // namespace gnsstk

#endif
