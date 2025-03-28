cmake_minimum_required(VERSION 2.8.12)

project("freesteam")#Version 3 by Qinfeng Xia (github.com/qingfengxia/freesteam), following John Pye
set(PROJECT_BRIEF "A free implementation of IAPWS-IF97 thermophysical properties for water and steam")

set(PACKAGE_VERSION_MAJOR "3")
set(PACKAGE_VERSION_MINOR "2")
set(PACKAGE_VERSION_PATCH "0")

option (FREESTEAM_WITH_PYTHON        "Compile Python interface of freesteam" ON)
enable_testing ()

# this version name is used by CPack
set(PACKAGE_VERSION_NAME ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR})

set(VERSION ${PACKAGE_VERSION_MAJOR})
#configure_file()
#config.h.in

# steam_uv.c seems not yet completely implemented, comented out
set(MY_SOURCES
src/b23.c
src/backwards.c
src/bounds.c
src/common.c
src/derivs.c
src/region1.c
src/region2.c
src/region3.c
src/region4.c
src/solver2.c
src/steam.c
src/steam_ph.c
src/steam_ps.c
src/steam_pT.c
src/steam_pu.c
src/steam_pv.c
src/steam_Ts.c
src/steam_Tx.c
#"src/steam_uv.c"
src/surftens.c
src/thcond.c
src/viscosity.c
src/zeroin.c
)

set(MY_TEST_SOURCES
src/test.c
)

# if installed, it should be put in subfolder of `freesteam` on system include dir
set(MY_HEADER_DIR "${PROJECT_SOURCE_DIR}/src")

set(MY_HEADERS
src/b23.h
src/backwards.h
src/backwards_impl.h
src/bounds.h
src/common.h
src/compat.h
src/config.h
src/derivs.h
src/region1.h
src/region2.h
src/region3.h
src/region4.h
src/solver2.h
src/steam.h
src/steam_ph.h
src/steam_ps.h
src/steam_pT.h
src/steam_pu.h
src/steam_pv.h
src/steam_Ts.h
src/steam_Tx.h
#"src/steam_uv.h"
src/surftens.h
src/thcond.h
src/viscosity.h
src/zeroin.h
)

install(FILES ${MY_HEADERS} DESTINATION include)

add_library(freesteamCPP SHARED ${MY_SOURCES})
target_include_directories(freesteamCPP PRIVATE ${MY_HEADER_DIR} )

#LAPACK is only mandatory depedence
find_package(LAPACK REQUIRED)
if  (LAPACK_FOUND)
    set(LAPACK_INCLUDE_DIRS "/usr/include/lapacke/ /usr/include/openblas/ /usr/include/ ${LAPACK_INCLUDE_DIRS} $ENV{LAPACK_INCLUDE_DIRS}")

    target_include_directories(freesteamCPP PRIVATE ${LAPACK_INCLUDE_DIRS} )
    target_link_libraries(freesteamCPP ${LAPACK_LIBRARIES})

    message (STATUS "LAPACK found at ${LAPACK_LIBRARIES}")
else(LAPACK_FOUND)
    message(ERROR "Linear Algebra PACKage (LAPACK) lib and headers are not found:"
                "please install lapack-devel or openblas-devel (RedHat/Fedora/CentOS) or liblapacke-dev (Debian/Ubuntu) using package manager, "
                "or export LAPACK_LIBRARIES and LAPACK_INCLUDE_DIRS variables as hint")
endif(LAPACK_FOUND)

#GSL is an optional depedence
if  (FREESTEAM_WITH_GSL)
    message(STATUS "Freesteam with gsl library. Searching gsl library.")
    find_package(GSL REQUIRED)
    message (STATUS "GSL found at ${GSL_LIBRARIES}")
    target_include_directories(freesteamCPP PRIVATE ${GSL_INCLUDE_DIRS} )
    target_link_libraries(freesteamCPP ${GSL_LIBRARIES})
else(FREESTEAM_WITH_GSL)
    message(STATUS "Freesteam with gsl-multiroots.")
    add_subdirectory(gsl)
    target_include_directories(freesteamCPP PRIVATE ./gsl/multiroots )
    target_link_libraries(freesteamCPP gsl-multiroots )
endif(FREESTEAM_WITH_GSL)

install(TARGETS freesteamCPP DESTINATION lib)

# test main exe
add_executable(freesteam_test ${MY_TEST_SOURCES})
target_link_libraries(freesteam_test freesteamCPP )
if  (FREESTEAM_WITH_GSL)
    target_link_libraries(freesteam_test ${GSL_LIBRARIES})
else(FREESTEAM_WITH_GSL)
    target_link_libraries(freesteam_test gsl-multiroots)
endif(FREESTEAM_WITH_GSL)

add_test(NAME freesteam_test  COMMAND "./freesteam_test")  
install(TARGETS freesteam_test DESTINATION test)

#swig wrapping
if   ( FREESTEAM_WITH_PYTHON )
  IF   (${CMAKE_VERSION} VERSION_GREATER "3.12.0")
    find_package(Python REQUIRED COMPONENTS Interpreter Development )
    SET(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
    set(PYTHON_LIBRARIES     ${Python_LIBRARIES})
    set(PYTHON_INCLUDE_DIRS  ${Python_INCLUDE_DIRS})
  ELSE (${CMAKE_VERSION} VERSION_GREATER "3.12.0")
    find_package(PythonInterp REQUIRED )
    find_package(PythonLibs   REQUIRED )
  ENDIF(${CMAKE_VERSION} VERSION_GREATER "3.12.0")

  find_package(SWIG REQUIRED)  # cmake 3 has official swig support

  add_subdirectory("python")
endif( FREESTEAM_WITH_PYTHON )

#RPM spec config
#configure_file()

#pkg-config
#configure_file()

# debian package could be made from this
