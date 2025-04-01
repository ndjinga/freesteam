# nom du projet gsl-multiroots
PROJECT(gsl-multiroots)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

set(MY_MULTIROOTS_SOURCES
  multiroots/enorm.c  multiroots/fdfsolver.c  multiroots/gnewton.c  multiroots/newton.c  multiroots/convergence.c  
)

set(MY_MULTIROOTS_HEADERS_DIR
  "${PROJECT_SOURCE_DIR}/gsl/multiroots"
)

set(MY_MULTIROOTS_TEST_SOURCES
  multiroots/test.c  multiroots/test_funcs.c
)

add_library(gsl-multiroots SHARED ${MY_MULTIROOTS_SOURCES})
    set(LAPACK_INCLUDE_DIRS "/usr/include/lapacke/ /usr/include/openblas/ /usr/include/ ${LAPACK_INCLUDE_DIRS} $ENV{LAPACK_INCLUDE_DIRS}")
    target_include_directories(gsl-multiroots PRIVATE ${LAPACK_INCLUDE_DIRS} )
    target_link_libraries(gsl-multiroots ${LAPACK_LIBRARIES})

#FILE(GLOB gsl-multiroots_CXX multiroots/*.c)
#ADD_LIBRARY(gsl-multiroots SHARED ${gsl-multiroots_CXX})
SET_TARGET_PROPERTIES(gsl-multiroots PROPERTIES OUTPUT_NAME "gsl-multiroots")
set_target_properties(gsl-multiroots PROPERTIES LINKER_LANGUAGE C)
TARGET_include_directories(gsl-multiroots PRIVATE multiroots )


INSTALL(TARGETS gsl-multiroots DESTINATION lib)



