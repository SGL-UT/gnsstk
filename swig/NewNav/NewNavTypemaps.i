// convert output gnsstk::NavDataPtr references

%typemap(in, numinputs=0) std::shared_ptr<gnsstk::NavData> &navOut ()
{
   std::shared_ptr<gnsstk::NavData> *smartresult =
      new std::shared_ptr<gnsstk::NavData>();
   $1 = smartresult;
}

%typemap(argout) std::shared_ptr<gnsstk::NavData> &
{
      // What this does is change the python interface to the C++
      // method so that you get a list containing the original C++
      // return value (typically a bool) and the resulting NavData
      // object.
      // The shared_ptr<NavData> object needs to be converted to the
      // leaf class in order for python to be able to use it properly.
   if (!PyList_Check(resultobj))
   {
         // Turn the return value into a list and add the original
         // return value as the first member of the list.
      PyObject *temp = resultobj;
      resultobj = PyList_New(1);
      PyList_SetItem(resultobj, 0, temp);
      temp = nullptr;
         // First check to see if the NavData object has been set.
         // Usually it will be unset if, for example, find() fails,
         // and the bool return value will also be false in this case.
      if ($1->get() != nullptr)
      {
            // The NavData tree has a getClassName() method that
            // returns the qualified class name,
            // e.g. gnsstk::GPSLNavEph.  We add the shared_ptr
            // qualifiers in order for SWIG_TypeQuery to get the
            // correct python data type that we need to return.
         std::string cn = "std::shared_ptr< " + (*$1)->getClassName() + " > *";
         swig_type_info *desc = SWIG_TypeQuery(cn.c_str());
         if (desc != nullptr)
         {
               // We have a valid python type, so now we can create a
               // SWIG pointer that properly identifies the derived
               // shared_ptr<NavData> object.
            temp = SWIG_NewPointerObj(SWIG_as_voidptr($1), desc,
                                      SWIG_POINTER_OWN);
         }
      }
      if (temp == nullptr)
      {
            // We were unable to get a valid python type, probably
            // because the output shared_ptr<NavData> object was null,
            // so create a null shared_ptr<NavData> python object to
            // use as the return value.
            // This gets turned into "None" in python.
         temp = SWIG_NewPointerObj(SWIG_as_voidptr($1), $descriptor,
                                   SWIG_POINTER_OWN);
      }
         // Add the derived shared_ptr<NavData> object to the return list
      PyList_Append(resultobj, temp);
      Py_DECREF(temp);
   }
}

// convert output gnsstk::SVHealth references

%typemap(in, numinputs=0) gnsstk::SVHealth &healthOut (gnsstk::SVHealth temp)
{
      // healthOut typemap(in)
   temp = gnsstk::SVHealth::Unknown;
   $1 = &temp;
}

%typemap(argout) gnsstk::SVHealth&
{
      // healthOut typemap(argout)
      // What this does is change the python interface to the C++
      // method so that you get a list containing the original C++
      // return value (typically a bool) and the resulting NavData
      // object.
      // The shared_ptr<NavData> object needs to be converted to the
      // leaf class in order for python to be able to use it properly.
   if (!PyList_Check(resultobj))
   {
         // Turn the return value into a list and add the original
         // return value as the first member of the list.
      PyObject *temp = resultobj;
      resultobj = PyList_New(1);
      PyList_SetItem(resultobj, 0, temp);
      PyObject* sys_mod_dict = PyImport_GetModuleDict();
      PyObject* gnsstk_mod = PyMapping_GetItemString(sys_mod_dict, "gnsstk");
      PyObject* enum_instance = PyObject_CallMethod(gnsstk_mod, "SVHealth", "i", (int)*$1);
      Py_DECREF(gnsstk_mod);
      PyList_Append(resultobj, enum_instance);
   }
}

// =============================================================
// These typemaps must occur before the headers are included.
// =============================================================

// time offset is an output
%apply double &OUTPUT { double &offset };
// ionospheric correction is an output
%apply double &OUTPUT { double &corrOut };
