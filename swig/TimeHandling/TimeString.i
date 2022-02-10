%inline %{ //Insert into header portion of TimeString.hpp
   gnsstk::CommonTime scanTime(const std::string& str, const std::string& fmt)
   {
      gnsstk::CommonTime m;
      gnsstk::scanTime(m, str, fmt);
      return m;
   }
   gnsstk::CommonTime mixedScanTime(const std::string& str, const std::string& fmt)
   {
      gnsstk::CommonTime m;
      gnsstk::mixedScanTime(m, str, fmt);
      return m;
   }
%}
