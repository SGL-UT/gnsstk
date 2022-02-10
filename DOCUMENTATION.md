DOCUMENTATION
-------------

Contents:
---------

* Doxygen API Documentation - Automated Build
* Doxygen API Documentation - Manual Build


Doxygen API Documentation - Automated Build:
------------------------------------------------------------------------

   1. Navigate to the root directory of the extract GNSSTk file tree, herein called $gnsstk_root:

        $ cd $gnsstk_root
         
   2a. Execute the build script to build the Core Doxygen API Documentation
        
        $ ./build.sh -d 
   
   2b. Execute the build script to build the Complete (Core/Ext) Doxygen API Documentation & 
       Improved Python Docstrings
        
        $ ./build.sh -de


Doxygen API Documentation - Manual Build:
----------------------------------------------------------------------

   1. Navigate to the root directory of the extract GNSSTk file tree:

         $ cd $gnsstk_root

         $ doxygen 
