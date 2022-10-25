Introduction
==================

This documentation describes the architecture and design of the Python bindings
of the GPS ToolKit (GNSSTk), produced by the Space and Geophysics Laboratory (SGL)
of Applied Research Laboratories, The University of Texas at Austin (ARL:UT).


About the GNSSTk
*****************
The GNSSTK is a library of support routines designed for use by programmers
developing applications that read and write (particularly in RINEX format),
process and otherwise manipulate GPS data. It is characterized by the following:

- Modern, ANSI C++; highly object oriented
- Uses templates, the Standard Template Library (STL) and iostreams classes
- Portable and platform independent
- Includes full implementation of the RINEX (Receiver Independent Exchange Format) I/O.


The official GNSSTk homepage is at http://www.gnsstk.org/


Getting the Python GNSSTk
****************************
Building the GNSSTk from source can be a fairly involved process, see :ref:`build_label`
for the details.




About the Python Bindings
********************************************
Though effort has been taken to reduce the uniquely C++ idioms when using these Python
classes where possible and practical. The GNSSTk is primarily a C++ library and some aspects
may require unusual semantics. It is recommended that you avoid using some of these unusual
classes, such as gnsstk.vector_double (std::vector<double>) as much as a possible.
A much better replacement would be a numpy array.

Example of how C++ GNSSTk reads and prints a Rinex3Obs file:

.. parsed-literal::
    // Create the input file stream
   Rinex3ObsStream rin("rinex3obs_data.txt");

   // Read the RINEX header
   Rinex3ObsHeader head;    // RINEX header object
   rin >> head;
   std::cout << head << std::endl;

   // Loop over all data epochs
   Rinex3ObsData data;   // RINEX data object
   while (rin >> data)
   {
      std::cout << data << std::endl;
   }

Compare to the simpler Python GNSSTk read process:

.. parsed-literal::
   # read in the header and data
   header, data = readRinex3Obs('rinex3obs_data.txt')
   print header
   for dataObject in data:
       print data


Package Organization
************************
The python package is split into several subpackages that mirror the folder
organization of the source code. In addition, everything from the subpackages
is brought into the top level package scope. All classes and functions can be
accessed by  either the individual subpackages/modules or from the top level
package scope.


How to Use This Documentation
**************************************
The C++ API is the primary reference still, however much of it is embedded into
the python source in the form of docstrings. This means you can use ipython
to easily find out about classes and functions.

A good first step is to browse the things available in the :ref:`api`
section. If you are coming from writing C++ GNSSTk programs, you should be sure to read
the section on how C++ structures have been changed for python.


Scope
**********
The GNSSTk C++ library is the product of a large number of people over a
sparse time period; as a result, the focus of the library has been somewhat
lost. There are many non-GPS related utilities implemented that have been
implemented more efficiently, robustly, and accurately by others.
These bindings attempt to expose as little as needed to the programmer.

Prime examples of things **NOT wrapped** include (most of which are easily replace by scipy&numpy):

* SpecialFunctions (e.g. the gamma function)
* Stats, GaussianDistribution, etc.
* Command Line utilities (python's built-in argparse does the job better)
* String utilities
* Vector graphics utilities


How some particular C++ structures are translated to Python
***************************************************************

To wrap the library we used the Standard Wrapper Interface Generator (SWIG).
Without SWIG, the amount of work to wrap the library would have made it completely infeasible.
However, some elements of C++ do not have clear analogues in Python; as such, some
compromises had to be made to allow for clean python programming without a complete
rework of the well-established C++ API.

These are important to recognize because they present technical reasons why some
things seem odd; the bindings were written in a way that had to be maintainable above all else.
Any changes for the sake of the Python API could not duplicate code.


**Namespaces:**

The GNSSTk largely only uses the gnsstk namespace, but the python bindings are
split into several different subpackages. All contents of the subpackages are imported
into the top level gnsstk package to rougly imitate the C++ namespace.

**Enumerations:**

Many classes use enums extensively (see: ObsID), especially in their constructors
to specify options. These enums are simply wrapped to integer values. A enum
value can be referred to by ClassName.ValueName. Note that the name given to the
whole enum is no longer used.

**Templates:**

Templates in the GNSSTk were largely for 3 purposes:

* standard library containers (std::vector<T>, std::map<K,V>)
* accepting any numeric type (gnsstk::Vector<T>)
* creating specialized class for use in inheritance
    (such as how YumaAlmanacStore is a subclass of FileStore<YumaHeader>)

Using the standard library containers of C++ is discouraged,
so only a few were created for use.

The only class that uses the numeric template type that is wrapped
is gnsstk::Vector, which only accepts floating point numbers now.

The templates in inheritance are not needed as they are not visible to the end-user.


**Streams:**

Streams were used all over the GNSSTk for input and output.
Many classes had a dump or operator<< function that could be used for
string output, these have been replaced with the __str__ method.

Streams were also used heavily for reading in Almanac and Ephemeris data from files.
A more pythonic interface was provided over these streams (the streams are now hidden).
To replace the stream-oriented paradigm for file i/o used in the C++
implementation, some thin wrapping was placed over the Stream objects.

Because the i/o strategy was uniform across all file types in C++, the
strategy is similarly uniform for the python bindings.

As an example, to read a SP3 file: ::

    >>> header, data = gnsstk.readSP3('sp3_data.txt')


And to write an SP3 file... ::

    >>> gnsstk.writeSP3('sp3_data.txt.new', header, data)



**Inner Classes:**

SWIG cannot wrap nested classes/structs (as of v2.0.7), so many of the
inner classes and exceptions were simply pulled into the gnsstk namespace.


**Standard Library Classes:**

Occasionally, you may need a std::vector or std::map
for some purpose (i.e. a function takes them as a parameter). They are included
in a few common templated forms (string->char, etc.), but you should avoid
the use of these whenever possible.

For the most part, a vector object has the same semantics as a list and a
map object has the same semantics as a dict.


When C++ functions deal with arrays, they are automatically converted
(since the Python C API already uses arrays),
but when other containers are used, they must be explicitly converted.
Conversion functions (such as seqToVector for list->std::vector conversion)
are defined in the cpp submodule.


.. warning::
    Reading documentation can get difficult in dealing with heavily-templated
    wrapped C++ code. Use these structures as little as possible.
    If containers are small it might be wise to use the provided functions in
    gnsstk.cpp to convert the containers to native python lists/dicts.




**Exceptions:**

Exceptions were tricky to get right. In general, most exceptions thrown by
calling GNSSTk routines should be caught in the standard way in Python. ::

    try:
        a = gnsstk.someFunction()
    except gnsstk.exceptions.InvalidRequest:  # the most used exception in the GNSSTk
        print 'Could not process data.'

When an exception is thrown by the C++ code, it is propagated to the SWIG
system which has code (written in Exception.i).


**Arrays:**

SWIG typemaps have been written to automatically convert between C style arrays
and Python lists. Any function that takes a array parameter will accept a list
and any array return will actually return a list.

This conversion can be done efficiently since Python are already backed by C arrays.


**Functions that modify a parameter that is passed by reference:**

Some C++ functions didn't return a value, but just modify a non-const parameter.
For example, from Position.hpp:

.. parsed-literal::
    static void convertCartesianToGeocentric(const Triple& xyz, Triple& llr)

Simple wrappers are added to the library that have a signature of:

.. parsed-literal::
    static Triple convertCartesianToGeocentric(const Triple& xyz)

Which behaves as expected when used in Python: ::

    >>> import gnsstk
    >>> u = gnsstk.Position(100, 200, 300)
    >>> v = gnsstk.Position.convertCartesianToGeocentric(u)
    >>> print v
    (53.30077479951032, 63.434948822921655, 374.16573867739413)

