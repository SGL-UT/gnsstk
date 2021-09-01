%include "Macros.i"

// Include STL and GPSTK Exceptions header files
%rename(__str__) gpstk::Exception::what() const;

namespace gpstk
{
   namespace StringUtils
   {
         // Create new class StringException under parent class Exception
      NEW_EXCEPTION_CLASS(StringException, Exception);
   }
}

%inline %{
   namespace gpstk
   {
         // Class for StopIterator Python exception, used by Vector.i
      class StopIterator {};
   }
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


   // Other gpstk exceptions:
   catch (const gpstk::Exception &e)
   {
      std::string s("GPSTk exception\n"), s2(e.what());
      s = s + s2;
      SWIG_exception(SWIG_RuntimeError, s.c_str());
   }

   // STL exceptions:
   catch (const std::exception &e)
   {
      std::string s("STL exception\n"), s2(e.what());
      s = s + s2;
      SWIG_exception(SWIG_RuntimeError, s.c_str());
   }

   // Exception to stop Python iterator
   catch (gpstk::StopIterator)
   {
       PyErr_SetString(PyExc_StopIteration, "Reached end of Iterator");
       return NULL;
   }

   // any other exception:
   catch (...)
   {
       SWIG_exception(SWIG_RuntimeError, "unknown exception");
   }
}
