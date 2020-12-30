## Introduction

**CMake updates for easier installation and use**

Original repo (version 2.1) at sourceforge: <http://freesteam.sourceforge.net/>

Credit goes to the original authors: John Pye, etc

migration from sourceforge to github <https://github.com/qingfengxia/freesteam>, to make it compatible with modern Linux system (version 3.0).

Credit goes to Qingfeng Xia

[License](LICENSE.txt): GPL as original release

### Overview of the features 

on the latest github version 3.0 (released in 2019)

[Readme of version 3.0](README-3.0.md)

+ cmake updates (cmake version 3.14.2 used) for python detection and use 
+ Added a cmake option for the generation of the python interface (FREESTEAM_WITH_PYTHON) 
+ added targets 'make install' and 'make test' 
+ tested and used on Fedora 30 with gcc 9.0.1, swig3 andpython3

### Installation with CMake

```bash
mkdir build
cd build
cmake ../freesteam_src -DCMAKE_INSTALL_PREFIX=../freesteam_install -DFREESTEAM_WITH_PYTHON=ON
make
make test 
make install 
```

version definition: version jumps from 3.0 to 3.1 to indicate updates

