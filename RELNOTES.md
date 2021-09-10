GNSSTk 11.2.1 Release Notes
========================

 * This release introduces Gitlab CI updates to use new sgl git location.

Updates since v11.2.0
---------------------

**Gitlab CI**
  * Updated git url ref to new sgl git location

Known Issues since v11.1.0
-------------------------
 * Due to code fixes for the new nav store in v11.1.0 release, downstream dependencies
 will need to be updated to latest versions to resolve errors in unittests that compare computational results. 
 These tests may fail due to a change in reference data and not a breaking API change.
