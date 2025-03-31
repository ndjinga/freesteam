# nom du projet gsl-multiroots
PROJECT(gsl-multiroots)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

set(MY_MULTIROOTS_SOURCES
  multiroots/enorm.c  multiroots/fdfsolver.c  multiroots/gnewton.c  multiroots/newton.c 
)

set(MY_MULTIROOTS_HEADERS_DIR
  "${PROJECT_SOURCE_DIR}/gsl/multiroots"
)

set(MY_MULTIROOTS_TEST_SOURCES
  multiroots/test.c  multiroots/test_funcs.c multiroots/convergence.c  
)

add_library(gsl-multiroots SHARED ${MY_MULTIROOTS_SOURCES})
target_include_directories(freesteamCPP PRIVATE ${MY_MULTIROOTS_HEADERS_DIR} )
#FILE(GLOB gsl-multiroots_CXX multiroots/*.c)
#ADD_LIBRARY(gsl-multiroots SHARED ${gsl-multiroots_CXX})
SET_TARGET_PROPERTIES(gsl-multiroots PROPERTIES OUTPUT_NAME "gsl-multiroots")
set_target_properties(gsl-multiroots PROPERTIES LINKER_LANGUAGE C)
TARGET_include_directories(gsl-multiroots PRIVATE multiroots )


INSTALL(TARGETS gsl-multiroots DESTINATION lib)



