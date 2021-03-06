# c-txtutil
duplicate the functionality in .../txtutil (written originally in Go,
`https://github.com/HankB/txtutil`)

## Status

* CppUTest integrated into project (`https://github.com/cpputest/cpputest`,
  `https://github.com/jwgrenning/cpputest-starter-project`)
* endLine() and formatLine() and successful tests for both coded.
* dump() implemented. 
* priority conditioned logging implemented. Could use more testing

## Dependencies

* CppUTest framework installed at the same directory level as this project.
* cpputest starter project unpacked somewhere convenient and copied to this
  project's root directory. (This is installed and copied into this project.)


## Procedure

This is entirely based on the CppUTest framework. Set up
dependencies as directed above.

``` text
cd c-txtutil/cpputest-starter-project
export CPPUTEST_HOME=../../cpputest
make
```

## TODO

* Pull to a clean directory and verify that project still builds.
* Remove superfluous files from `c-txtutil/cpputest-starter-project/`

## Errata

* cmake instructions did not work for cpputest. (Even after typing 
 `cmake CMakeLists.txt` instead of `cmake CMakeList.txt`)
* In Debian need to install `build-essential`, `libtool` and `autoconf` packages to
  build cppunit with autoconf.
* The configure command (for cppunit build) is `./configure`. 
* There are way too many extaneous files in the project due to the addition of 
  the cpputest-starter-project and eliminating them is messy. They remain.
