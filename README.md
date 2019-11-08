## Introduction

**migration of `freesteam`  a free implementation of IAPWS-IF97 thermalphysical propeties for water and steam from sourceforge to github, to make it compatible with modern Linux system.**

Original repo at sourceforge: <http://freesteam.sourceforge.net/>

Credit goes to the original authors: John Pye, etc

[License](LICENSE.txt): GPL as original release

### Overview of the features 

on the latest sourceforge version 2.1 (released in 2013)

[Readme of sourceforge](README.txt)

+ qt4 and gtk2.0 are general not available on modern system 
+  `scons` build system provided by the authors
+ python wrapping using `swig` >=2.0
+ cross-platform building and packaging: macosx PackageMaker and windows msvc and NSIS, linux RPM 


### used in OpenFOAM

water steam properties used by OpenFOAm  wiki:
<https://github.com/romansCode/IAPWS-IF97-OF>

This repository is also outdated, will not compiled on modern C++11 compiler and latest OpenFOAM versions.

Code_Saturne using another open source software. EOS ??



## Migration work

### Top up

+ move all source into `src` folder, this will break `scons` build system
+ `cmake` building system setup
+ deb package generation by cmake

Tested on Ubuntu 18.04 64bit

### Todo

+ gtk2.0 will not upgrade to gtk4.0, no such expertise
+ qt4 to qt5 or qt6, wait until there is such a requirement
+ RPM spec
+ cross-platform packaging, invite contribution, describing how GSL installation

### CMake support

cmake is now supported to compile the C shared library and build swig python module.

```bash
cd freesteam
mkdir build
cd build
cmake ..
#install 
```

version definition: version jumps from 2.1 to 3.0 to indicating building system change

```cpp
#define FREESTEAM_VERSION "@VERSION@"
```

[config.h.in](config.h.in)  it is compatible with cmake `configure_file` command 

`steam_uv.c` seems has not yet completed, is excluded from compiling

`ascend` is not yet compiled, what is that?

### RPM packaging spec file

`freestream.spec.in`

cmake should be enable to use this template, todo!

### package-config file to assist compiling in other platforms

`freestream-config.in`

this configuration is generated by python script



### swig to generate python wrapping

cmake setup added