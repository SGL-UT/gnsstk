// This macro was moved out of Macros.i and into Stream.i so that
// changes to it would not engender rebuilding all of the SWIG
// modules.  The macro is used here alone.

// This is a SWIG macro that creates helper methods and the read/write methods:
// It creates:
//
//  C++ methods: (all of these are hidden to the end user since the streams are hidden)
//   - static factory method for the stream to create an input stream
//   - static factory method for the stream to create an output stream
//   - readHeader method for the stream
//   - readData method for the stream
//   - writeHeader method for the stream
//   - writeData method for the stream
//   - _remove method to delete the object (called from python helpers below)
//
//  gnsstk python functions:
//   - readX, where X is the file type
//   - writeX, where X is the file type
//

%define STREAM_HELPER(FORMATNAME)
%extend gnsstk:: ## FORMATNAME ## Stream
{ 

   // methods for the stream itself:
   static gnsstk:: ## FORMATNAME ## Stream* in ## FORMATNAME ## Stream(
      const std::string fileName)
   {
      gnsstk:: ## FORMATNAME ## Stream * s = new gnsstk:: ## FORMATNAME ## Stream(
         fileName.c_str());
      return s;
   }

   static gnsstk:: ## FORMATNAME ## Stream* out ## FORMATNAME ## Stream(
      const std::string fileName)
   {
      gnsstk:: ## FORMATNAME ## Stream * s = new gnsstk:: ## FORMATNAME ## Stream(
         fileName.c_str(), std::ios::out|std::ios::trunc);
      return s;
   }

   static void _remove(gnsstk:: ## FORMATNAME ## Stream * ptr)
   {
      delete ptr;
   }

   // reader functions:
   gnsstk:: ## FORMATNAME ## Header readHeader()
   {
      gnsstk:: ##FORMATNAME ## Header head;
      (*($self)) >> head;
      return head;
   }

   gnsstk:: ## FORMATNAME ## Data readData()
   {
      gnsstk:: ## FORMATNAME ##Data data;
      *($self) >> data;

      if( *($self) )
         return data;
      
      gnsstk::EndOfFile e("## FORMATNAME ## Stream reached an EOF.");
      GNSSTK_THROW(e);
   }

   // write functions:
   void writeHeader(const gnsstk:: ## FORMATNAME ## Header & head)
   {
      (*($self)) << head;
   }

   void writeData(const gnsstk:: ## FORMATNAME ## Data & data)
   {
      (*($self)) << data;
   }

}

%pythoncode {
def read ## FORMATNAME(fileName, strict=False, filterfunction=lambda x: True):
    """
    This reads from a FORMATNAME file and returns a two-element tuple
    of the header and the sequence of data objects.

    Parameters:
    -----------

    strict:  if the data object sequence should be strictly evaluated.
           If it is, it will be a list, otherwise, it will be a generator.

    filterfunction: a function that takes a FORMATNAME Data object
                    and returns whether it should be included in the
                    data output. This is similar to using the filter()
                    function on the output list, but eliminates the extra step.
    """
    import os.path
    if not os.path.isfile(fileName):
        raise IOError(fileName + ' does not exist.')
    stream = FORMATNAME ## Stream .in ##FORMATNAME ## Stream (fileName)
    header = stream.readHeader()
    def read ## FORMATNAME ## Data (fileName):
        while True:
            try:
               x = stream.readData()
               if filterfunction(x):
                  yield x
            except gnsstk.EndOfFile:
               FORMATNAME ## Stream._remove(stream)
               break

    data = read ##FORMATNAME ## Data (fileName)
    if strict:
        return (header, list(data))
    else:
        return (header, data)


def write ## FORMATNAME(fileName, header, data):
    """
    Writes a FORMATNAME Header and sequence of FORMATNAME Data objects to a file.
    Note that this overwrites the file if it already exists.

    Parameters:
    -----------

    fileName:  the name of the file to write to.

    header:  the FORMATNAME Header object

    data:  the sequence of FORMATNAME Data objects
    """
    s = FORMATNAME ## Stream .out ##FORMATNAME ## Stream (fileName)
    s.writeHeader(header)
    for d in data:
        s.writeData(d)
    FORMATNAME ## Stream ._remove(s)

}
%enddef


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
