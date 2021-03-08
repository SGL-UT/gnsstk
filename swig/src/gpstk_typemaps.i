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
   if (!PyList_Check(resultobj))
   {
      PyObject *temp = resultobj;
      resultobj = PyList_New(1);
      PyList_SetItem(resultobj, 0, temp);
         // create shadow object (do not use SWIG_POINTER_NEW)
         // stackoverflow says don't use SWIG_POINTER_NEW but since I
         // have to combine the new method for the object in order to
         // make the smart pointer, I'm not convinced that's correct.
//                                SWIGTYPE_p_std__shared_ptrT_gpstk__NavData_t,
      std::cerr << "type = " << (*$1)->getClassName() << std::endl;
      temp = SWIG_NewPointerObj(SWIG_as_voidptr($1), $descriptor,
                                SWIG_POINTER_OWN | 0);
      PyList_Append(resultobj, temp);
      Py_DECREF(temp);
   }
}
