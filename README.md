## Introduction

**CMake updates for easier installation and use**

Original repo (version 2.1) at sourceforge: <http://freesteam.sourceforge.net/>

Credit goes to the original authors: John Pye, etc

Migration from sourceforge to github and use of CMake and SWIG, to make it compatible with modern Linux system (version 3.0).

Credit goes to Qingfeng Xia <https://github.com/qingfengxia/freesteam>

Upgraded to cmake 3.14.2, swig3 and python3, use of ctest for C and Pyton tests (version 3.1)

[License](LICENSE.txt): GPL as original release

### Prerequisites

+ CMake, mandatory. Package 'cmake' on Fedora and Ubuntu
+ LAPACK (Linear Algebra PACKage), mandatory. Package 'lapack-devel' or 'openblas-devel' on Fedora, 'liblapacke-dev' on Ubuntu
+ GSL (gnu scientific library), (optional). Package 'gsl-devel' on Fedora, 'libgsl-dev' on Ubuntu
+ python (optional), required if 'FREESTEAM_WITH_PYTHON=ON'. Package 'python-devel' on Fedora, 'python-dev' on Ubuntu
+ SWIG (optional), required if 'FREESTEAM_WITH_PYTHON=ON'. Package 'swig' on Fedora and Ubuntu
+ matplotlib (optional), for curves plotting. Package 'python-matplotlib' on Fedora and Ubuntu

### Overview of the features 

On the latest github version 3.1 (released in Nov 2019)

+ New directory gsl-light 
+ Removed dependence on gsl library

[Readme of version 3.1](README-3.1.md)

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

Version definition: version jumps from 3.1 to 3.2 to indicate updates

