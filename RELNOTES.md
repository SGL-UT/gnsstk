GPSTk 9.0.0 Release Notes
========================

 * This version addresses api change due to deprecation of FileHunter module.
 * Additionally, it contains a bug fix and CI related updates.

Updates since v8.0.0
---------------------
  * Update readme.
  * File_Hunter related files were deprecated and moved to tks_legacy_archive repo

**Gitlab CI**
  * Add CI stage to check files and history for sensitive words.
  * Update the CI pipeline to reflect that the new default branch name.

Fixes since v8.0.0
--------------------
  * Modify IonoModel to accommodate Klo params in different units.

Removed Code due to Deprecation
-------------------------------
    core/lib/FileDirProc/FileHunter.cpp
    core/lib/FileDirProc/FileHunter.hpp
    core/tests/FileDirProc/FileHunter_T.cpp