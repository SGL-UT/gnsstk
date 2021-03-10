// These typemaps help conversions between C style arrays and Python Lists.

// Note: if any more types needed to get added, it's time to start
//       using macros to do the job!

// Python Lists -> C arrays:
//     [PyLong] -> [long]
%typemap(in) long[ANY](long temp[$1_dim0]) {
   int i;
   if (!PySequence_Check($input))
   {
      PyErr_SetString(PyExc_TypeError, "Expecting a sequence");
      return NULL;
   }
   if (PyObject_Length($input) != $1_dim0)
   {
      PyErr_SetString(PyExc_ValueError, "Expecting a sequence with $1_dim0 elements");
      return NULL;
   }
   for (i = 0; i < $1_dim0; i++)
   {
      PyObject *o = PySequence_GetItem($input,i);
      if (!PyLong_Check(o))
      {
         Py_XDECREF(o);
         PyErr_SetString(PyExc_TypeError, "Expecting a sequence of longs");
         return NULL;
      }
      temp[i] = PyLong_AsLong(o);
      Py_DECREF(o);
   }
   $1 = &temp[0];
}

// Needed for typechecking parameters of overloaded functions and methods
// See "Typemaps and Overloading" in Swig documentation
%typemap(typecheck, precedence=SWIG_TYPECHECK_INT64_ARRAY) long[ANY] {
   $1 = PySequence_Check($input) ? 1 : 0;
}

//     [PyFloat] -> [double]
%typemap(in) double[ANY](double temp[$1_dim0]) {
   int i;
   if (!PySequence_Check($input))
   {
      PyErr_SetString(PyExc_TypeError, "Expecting a sequence");
      return NULL;
   }
   if (PyObject_Length($input) != $1_dim0)
   {
      PyErr_SetString(PyExc_ValueError, "Expecting a sequence with $1_dim0 elements");
      return NULL;
   }
   for (i = 0; i < $1_dim0; i++)
   {
      PyObject *o = PySequence_GetItem($input,i);
      if (!PyFloat_Check(o))
      {
         Py_XDECREF(o);
         PyErr_SetString(PyExc_TypeError, "Expecting a sequence of doubles");
         return NULL;
      }
      temp[i] = PyFloat_AsDouble(o);
      Py_DECREF(o);
   }
   $1 = &temp[0];
}

// Needed for typechecking parameters of overloaded functions and methods
// See "Typemaps and Overloading" in Swig documentation
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY) double[ANY] {
   $1 = PySequence_Check($input) ? 1 : 0;
}

// C arrays -> Python lists:
//     [int] -> [PyInt]
%typemap(out) int [ANY] {
   int i;
   $result = PyList_New($1_dim0);
   for (i = 0; i < $1_dim0; i++) {
      PyObject *o = PyInt_FromLong(static_cast<long>($1[i]));
      PyList_SetItem($result,i,o);
   }
}

//     [unsigned int] -> [PyInt]
%typemap(out) unsigned int [ANY] {
   int i;
   $result = PyList_New($1_dim0);
   for (i = 0; i < $1_dim0; i++) {
      PyObject *o = PyInt_FromLong(static_cast<long>($1[i]));
      PyList_SetItem($result,i,o);
   }
}

//     [long] -> [PyInt]
%typemap(out) long [ANY] {
   int i;
   $result = PyList_New($1_dim0);
   for (i = 0; i < $1_dim0; i++) {
      PyObject *o = PyInt_FromLong($1[i]);
      PyList_SetItem($result,i,o);
   }
}

//     [double] -> [PyFloat]
%typemap(out) double [ANY] {
   int i;
   $result = PyList_New($1_dim0);
   for (i = 0; i < $1_dim0; i++) {
      PyObject *o = PyFloat_FromDouble($1[i]);
      PyList_SetItem($result,i,o);
   }
}

// convert output gpstk::NavDataPtr references

%typemap(in, numinputs=0) std::shared_ptr<gpstk::NavData> &navOut ()
{
   std::shared_ptr<gpstk::NavData> *smartresult = 
      new std::shared_ptr<gpstk::NavData>();
   $1 = smartresult;
}

%typemap(argout) std::shared_ptr<gpstk::NavData> &
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
            // e.g. gpstk::GPSLNavEph.  We add the shared_ptr
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
