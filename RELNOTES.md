GNSSTk 11.2.2 Release Notes
========================

 * This release addresses a backport fix for bad text handling in FileSpec

Updates since v11.2.1
---------------------

**Gitlab CI**
  * Adding maintenance branch to package builds. 

**Library Changes**
  * Adding maintenance branch to package builds.

Fixes since v11.2.1
--------------------
  * Backport fix: Fixed handling of the text file spec type (%x) and added relevant unit tests as per TKS-435

Known Issues since v11.1.0
-------------------------
 * Due to code fixes for the new nav store in v11.1.0 release, downstream dependencies
 will need to be updated to latest versions to resolve errors in unittests that compare computational results. 
 These tests may fail due to a change in reference data and not a breaking API change.