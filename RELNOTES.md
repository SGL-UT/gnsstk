GNSSTk 14.4.1 Release Notes
========================

* This release includes the following:
   * Updating the handling already disambiguated GPS Week in SEM Almanacs.
   * Fixing SWIG wrapping of satTimeSystem().
   * Updating navdatafactorywithstore_t find tests non breaking refactor.
 * Additionally, it contains CI updates.

Updates since v14.4.0
---------------------

**Build System and Test Suite**
  * Update navdatafactorywithstore_t find tests non breaking refactor

**Gitlab CI**
  * Update Codeowners file with additional NewNav reviewer
  * Update pipeline to use rhel 8 ubi image instead of deprecated centos 8 appstream

**Library Changes**
  * Update to handle already disambiguated GPS Week in SEM Almanacs 

Fixes since v14.4.0
--------------------
  * Fix SWIG wrapping of satTimeSystem()
