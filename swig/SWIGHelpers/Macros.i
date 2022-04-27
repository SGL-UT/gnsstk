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
      // This name is used because it's the only consistent naming
      // convention that yields a look-up across the Utility submodule
      // that define the Exception class mapping, and the other
      // submodules that simply use them.
      // Refer to definitions in the generated xxxPYTHON_wrap.cxx
      // files such as _swigt__p_gnsstk__AssertionFailure.
      // Also, mangled name lookup is done first, so this reduces overhead.
   swig_type_info *desc = SWIG_TypeQuery("_p_gnsstk__" #NAME);
      //swig_type_info *desc = SWIG_TypeQuery("gnsstk::" #NAME " *");
   if (desc != nullptr)
   {
      SWIG_Python_Raise(
         SWIG_NewPointerObj(new gnsstk:: NAME (e),
                            desc,
                            SWIG_POINTER_OWN),
         "NAME",
         desc);
   }
   SWIG_fail;
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
 // C_NAME: enum name in C++
 // P_NAME: enum name in Python
 // M_NAME: module name containing enum
%define ENUM_MAPPER(C_NAME, P_NAME, M_NAME)
%typemap(out) C_NAME {
    // C_NAME typemap for $symname out
    // printf("Received an out enumeration: %d $symname\n", $1);

    // Note: We need to DECREF the tk_mod only.
    // The sys_mod_dict is borrowed, and enum_instance is returned.
    PyObject* sys_mod_dict = PyImport_GetModuleDict();
    PyObject* tk_mod = PyMapping_GetItemString(sys_mod_dict, M_NAME);
    PyObject* enum_instance = PyObject_CallMethod(tk_mod, "P_NAME", "i", (int)$1);
    Py_DECREF(tk_mod);
    $result = enum_instance;
    return $result;
}
%enddef
