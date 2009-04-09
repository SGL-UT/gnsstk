#pragma ident "$Id: Rinex3ObsTest.cpp 1815 2009-03-20 21:12:09Z raindave $"



/**
 * @file rinex_obs_read_write.cpp
 * tests gpslib::Rinex3ObsStream, gpslib::Rinex3NavHeader.
 */

#include "Rinex3ObsBase.hpp"
#include "Rinex3ObsStream.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"

using namespace std;

/// Returns 0 on success, and input and output files should diff without error.
main(int argc, char *argv[])
{
   if (argc<2)
   {
      cout << "rinex_obs_read_write inputfile outputfile" << endl;
      exit(-1);
   }

   try
   {
      cout << "Reading from " << argv[1] << endl;
      gpstk::Rinex3ObsStream roffs(argv[1],ios::in);
      cout << "Writing to   " << argv[2] << endl;
      gpstk::Rinex3ObsStream r3out(argv[2],ios::out);
      gpstk::Rinex3ObsHeader roh;
      gpstk::Rinex3ObsData roe;

      roffs >> roh;

      roh.dump(cout);

      r3out << roh;

      int i = 0;
      while (roffs >> roe)
      {
//         roe.dump(cout);
         r3out << roe;
         i++;
      }

      cout << "Read " << i << " records.  Done."  << endl;

      exit(0);
   }
   catch(gpstk::Exception& e)
   {
      cout << e;
      exit(1);
   }
   catch (...)
   {
      cout << "unknown error.  Done." << endl;
      exit(1);
   }

   exit(0);
} // main()
