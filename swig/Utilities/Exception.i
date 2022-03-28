%include "Macros.i"

// Include STL and GNSSTK Exceptions header files
%rename(__str__) gnsstk::Exception::what() const;

namespace gnsstk
{
   namespace StringUtils
   {
         // Create new class StringException under parent class Exception
      NEW_EXCEPTION_CLASS(StringException, Exception);
   }
}

%inline %{
#ifndef EXCEPTION_I_INLINE
#define EXCEPTION_I_INLINE
   namespace gnsstk
   {
         // Class for StopIterator Python exception, used by Vector.i
      class StopIterator {};
   }
#endif
%}

%exception
{
   try
   {
      $action //Exception special variable 
   }
   // Explicitly handled exceptions:
   CATCHER(InvalidParameter) 
   CATCHER(InvalidRequest)
   CATCHER(AssertionFailure)
   CATCHER(ObjectNotFound)
   CATCHER(AccessError)
   CATCHER(IndexOutOfBoundsException)
   CATCHER(InvalidArgumentException)
   CATCHER(ConfigurationException)
   CATCHER(FileMissingException)
   CATCHER(SystemSemaphoreException)
   CATCHER(SystemPipeException)
   CATCHER(SystemQueueException)
   CATCHER(OutOfMemory)
   CATCHER(NullPointerException)
   CATCHER(UnimplementedException)
   CATCHER(EndOfFile)
   CATCHER(FFStreamError)


   // Other gnsstk exceptions:
   catch (const gnsstk::Exception &e)
   {
      std::string s("GNSSTk exception\n"), s2(e.what());
      s = s + s2;
      SWIG_exception_fail(SWIG_RuntimeError, s.c_str());
   }

   // STL exceptions:
   catch (const std::exception &e)
   {
      std::string s("STL exception\n"), s2(e.what());
      s = s + s2;
      SWIG_exception_fail(SWIG_RuntimeError, s.c_str());
   }

   // Exception to stop Python iterator
   catch (gnsstk::StopIterator)
   {
      PyErr_SetString(PyExc_StopIteration, "Reached end of Iterator");
      return NULL;
   }

   // any other exception:
   catch (...)
   {
      SWIG_exception_fail(SWIG_RuntimeError, "unknown exception");
   }
}
