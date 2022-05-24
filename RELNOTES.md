GNSSTk 11.2.3 Release Notes
========================

 * This patch version was released to backport a fix that allows CMake version compatibility between major versions only.

Updates since v11.2.2
---------------------
  * Update to allow co-installation of python3 and python2 packages.
  * Update 11.x CMake package for major version compatibility.
  * Update add maintenance branches to pipeline.

Fixes since v11.2.1
--------------------
  * Backport fix: Fixed handling of the text file spec type (%x) and added relevant unit tests as per TKS-435

Known Issues since v11.1.0
-------------------------
 * Due to code fixes for the new nav store in v11.1.0 release, downstream dependencies
   will need to be updated to latest versions to resolve errors in unittests that compare computational results. 

