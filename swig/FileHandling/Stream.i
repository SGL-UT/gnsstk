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
   void writeEpochs(std::vector<gpstk::Rinex3ObsData>& rodarr,
                    const gpstk::Rinex3ObsHeader& roh,
                    const gpstk::RinexSatID& svid,
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
            std::map<std::string , std::vector<gpstk::RinexObsID> >::const_iterator kt = roh.mapObsTypes.find(std::string(1, svid.systemChar()));
            if (kt == roh.mapObsTypes.end()) { return; }
            rodarr[*it].obs[svid] = gpstk::Rinex3ObsData::DataMap::mapped_type (kt->second.size());
            rodarr[*it].numSVs++;
         }

         rodarr[*it].obs[svid][rotidx].data = data[i];
      }
   }
%}

/* Since the ashtech stream/data is slightly different,
   it is extended separately
*/
%extend gpstk::AshtechStream
{
   // methods for the stream itself:
   static gpstk::AshtechStream* inAshtechStream(const std::string fileName)
   {
      gpstk::AshtechStream *s = new gpstk::AshtechStream (fileName.c_str());
      return s;
   }
   
   static gpstk::AshtechStream* outAshtechStream(const std::string fileName)
   {
      gpstk::AshtechStream *s = new gpstk::AshtechStream(
         fileName.c_str(), std::ios::out|std::ios::trunc);
      return s;
   }
   
   static void _remove(gpstk::AshtechStream *ptr)
   {
      delete ptr;
   }
   
   // reader functions:
   gpstk::AshtechData readHeader()
   {
      gpstk::AshtechData head;
      (*($self)) >> head;
      return head;
   }
   
   gpstk::AshtechData readData()
   {
      gpstk::AshtechData data;
      if( (*($self)) >> data )
      {
         return data;
      }
      else
      {
         gpstk::EndOfFile e("AshtechStream reached an EOF.");
         GPSTK_THROW(e);
      }
   }
}
