// GPS_URA.i contained this check bounds macro method
// Requires a variable named index to be in the scope
// of this macro call in order for this macro to be correctly executed

%define CHECK_BOUNDS(MAX)
   if (index > MAX)
   {
      std::ostringstream ss;
      ss << "Index " << index << " is greater than the max allowed index of " << MAX << ".";
      std::string message = ss.str();
      gnsstk::IndexOutOfBoundsException e(message);
      GNSSTK_THROW(e);
   }
%enddef

// Used to check if an index is in the interval [MIN, MAX)
// Throws an exception if index is not in the interval
%define CHECK_INDEX(index, MIN, MAX)
    if (index >= MAX || index < MIN) {
        std::ostringstream ss;
        ss << "Index " << index << " is not within the allowed bounds [" << MIN << ", " << MAX << ")";
        std::string message = ss.str();
        gnsstk::IndexOutOfBoundsException e(message);
        GNSSTK_THROW(e);
    }
%enddef

// This macro is used to help the C++ exceptions be translated to the python
// wraps of the exception class - this is not automatically done by SWIG!
%define CATCHER(NAME)
    catch(const gnsstk:: NAME &e)
    {
        SWIG_Python_Raise(
            SWIG_NewPointerObj(new gnsstk:: NAME (static_cast<const gnsstk:: NAME &>(e)),
                                SWIGTYPE_p_gnsstk__##NAME,
                                SWIG_POINTER_OWN),
            "NAME",
            SWIGTYPE_p_gnsstk__##NAME);
        SWIG_fail;
    }
    %enddef

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
            except EndOfFile:
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


///////////////////////////////////////////////
//      Macros for string (__str__) stuff
///////////////////////////////////////////////
// Uses the dump method in the class to get string output
%define STR_DUMP_HELPER(name)
%extend gnsstk:: ## name
{
   std::string __str__()
   {
      std::ostringstream stream;
      $self->dump(stream);
      return stream.str();
   }
}
%enddef


// Uses the print method in the class to get string output
%define STR_PRINT_HELPER(name)
%extend gnsstk:: ## name
{
   std::string __str__()
   {
      std::ostringstream stream;
      $self->print(stream);
      return stream.str();
   }

    std::string __repr__()
    {
        std::ostringstream stream;
        $self->print(stream);
        return stream.str();
    }
}   
%enddef



// Uses the dump method in the class to get string output
// for dump methods that have a detail parameter
%define STR_DUMP_DETAIL_HELPER(name)
%extend gnsstk:: ## name {
   std::string __str__() {
      std::ostringstream stream;
      $self->dump(stream, 1);
      return stream.str();
   }
}
%enddef

// Uses the dump method in the class to get string output
// for dump methods that have a detail parameter
%define STR_NNDUMP_DETAIL_HELPER(name)
%extend gnsstk:: ## name {
   std::string __str__() {
      std::ostringstream stream;
      $self->dump(stream, gnsstk::DumpDetail::Full);
      return stream.str();
   }
}
%enddef


// Uses the operator<< in the class to get string output
%define STR_STREAM_HELPER(name)
%extend gnsstk:: ##name {
   std::string __str__() {
      std::ostringstream stream;
      stream << *($self);
      return stream.str();
   }
   std::string __repr__() {
      std::ostringstream stream;
      stream << *($self);
      return stream.str();
   }
}
%enddef

%define STR_STREAM_VEC_HELPER(name)
%extend name
{
   std::string __str__()
   {
      std::ostringstream ss;
      size_t i;
      for(i = 0; i < $self->size() - 1; i++)
         ss << (*($self))[i] << ", ";
      ss << (*($self))[i];
      return ss.str();
   }
}
%enddef

// Uses gnsstk::StringUtils::asString(x) to get string output
%define AS_STRING_HELPER(name)
%extend gnsstk:: ##name {
   std::string __str__() {
      return gnsstk::StringUtils::asString(*($self));
   }

    std::string __repr__() {
        return gnsstk::StringUtils::asString(*($self));
    } 
}
%enddef

// Uses self->asString() to get string output
%define AS_STRING_HELPER2(name)
%extend gnsstk:: ##name
{
    std::string __str__()
    {
        return $self->asString();
    }

    std::string __repr__()
    {
        return $self->asString();
    }
}
%enddef

// Uses self->asString() to get string output
%define STR_FILTER_HELPER(name)
%extend gnsstk:: ##name
{
   std::string __str__()
   {
      return $self->filterName();
   }

   std::string __repr__()
   {
      return $self->filterName();
   }
}
%enddef


// Create Mappings from C Enum to Python Enum
// Without this they will be transformed to integers.
%define ENUM_MAPPER(C_NAME, P_NAME)
%typemap(out) C_NAME {
    // C_NAME typemap for $symname out
    // printf("Received an out enumeration: %d $symname\n", $1);

    // Note: We need to DECREF the gnsstk_mod only.
    // The sys_mod_dict is borrowed, and enum_instance is returned.
    PyObject* sys_mod_dict = PyImport_GetModuleDict();
    PyObject* gnsstk_mod = PyMapping_GetItemString(sys_mod_dict, "gnsstk");
    PyObject* enum_instance = PyObject_CallMethod(gnsstk_mod, "P_NAME", "i", (int)$1);
    Py_DECREF(gnsstk_mod);
    $result = enum_instance;
    return $result;
}
%enddef
