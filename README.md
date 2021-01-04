## Introduction

**CMake updates for easier installation and use**

Original repo (version 2.1) at sourceforge: <http://freesteam.sourceforge.net/>

Credit goes to the original authors: John Pye, etc

Migration from sourceforge to github and use of CMake and SWIG, to make it compatible with modern Linux system (version 3.0).

Credit goes to Qingfeng Xia <https://github.com/qingfengxia/freesteam>

[License](LICENSE.txt): GPL as original release

### Prerequisites

+ GSL (gnu scientific library), mandatory. Package gsl-devel on Fedora, libgsl-dev on Ubuntu
+ python (optional), required if FREESTEAM_WITH_PYTHON=ON. Package python-devel on Fedora, python-dev on Ubuntu
+ SWIG (optional), required if FREESTEAM_WITH_PYTHON=ON. Package swig on Fedora, swig on Ubuntu
### Overview of the features 

On the latest github version 3.0 (released in Nov 2019)

+ cmake updates (cmake version 3.14.2 used) for python detection and use 
+ Added a cmake option for the generation of the python interface (FREESTEAM_WITH_PYTHON) 
+ added targets 'make install' and 'make test' 
+ tested and used on Ubuntu 20.04 and Fedora 30 with gcc 9.0.1, swig3 and python3

[Readme of version 3.0](README-3.0.md)

[Readme of version 2.1](README-2.1.txt)

### Installation with CMake

```bash
mkdir build
cd build
cmake ../freesteam_src -DCMAKE_INSTALL_PREFIX=../freesteam_install -DFREESTEAM_WITH_PYTHON=ON
make
make test 
make install 
```

Version definition: version jumps from 3.0 to 3.1 to indicate updates

