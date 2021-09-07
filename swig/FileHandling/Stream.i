//Create the various Streams for the data types
STREAM_HELPER(Rinex3Clock)
STREAM_HELPER(Rinex3Nav)
STREAM_HELPER(Rinex3Obs)
STREAM_HELPER(RinexMet)
STREAM_HELPER(RinexNav)
STREAM_HELPER(RinexObs)
STREAM_HELPER(SEM)
STREAM_HELPER(SP3)
STREAM_HELPER(Yuma)

%inline %{
   void writeEpochs(std::vector<gnsstk::Rinex3ObsData>& rodarr,
                    const gnsstk::Rinex3ObsHeader& roh,
                    const gnsstk::RinexSatID& svid,
                    const std::vector<double>& data,
                    const std::vector<int>& where,
                    const int rotidx)
   {
      int i = 0;
      for(std::vector<int>::const_iterator it=where.begin(); it != where.end();
          ++it, ++i)
      {
            // check if there is any sv entries at this epoch and
            // create one if necessary
         if (rodarr[*it].obs.find(svid) == rodarr[*it].obs.end())
         {
            std::map<std::string , std::vector<gnsstk::RinexObsID> >::const_iterator kt = roh.mapObsTypes.find(std::string(1, svid.systemChar()));
            if (kt == roh.mapObsTypes.end()) { return; }
            rodarr[*it].obs[svid] = gnsstk::Rinex3ObsData::DataMap::mapped_type (kt->second.size());
            rodarr[*it].numSVs++;
         }

         rodarr[*it].obs[svid][rotidx].data = data[i];
      }
   }
%}

/* Since the ashtech stream/data is slightly different,
   it is extended separately
*/
%extend gnsstk::AshtechStream
{
   // methods for the stream itself:
   static gnsstk::AshtechStream* inAshtechStream(const std::string fileName)
   {
      gnsstk::AshtechStream *s = new gnsstk::AshtechStream (fileName.c_str());
      return s;
   }
   
   static gnsstk::AshtechStream* outAshtechStream(const std::string fileName)
   {
      gnsstk::AshtechStream *s = new gnsstk::AshtechStream(
         fileName.c_str(), std::ios::out|std::ios::trunc);
      return s;
   }
   
   static void _remove(gnsstk::AshtechStream *ptr)
   {
      delete ptr;
   }
   
   // reader functions:
   gnsstk::AshtechData readHeader()
   {
      gnsstk::AshtechData head;
      (*($self)) >> head;
      return head;
   }
   
   gnsstk::AshtechData readData()
   {
      gnsstk::AshtechData data;
      if( (*($self)) >> data )
      {
         return data;
      }
      else
      {
         gnsstk::EndOfFile e("AshtechStream reached an EOF.");
         GNSSTK_THROW(e);
      }
   }
}
