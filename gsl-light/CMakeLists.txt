# nom du projet gsl-light
PROJECT(gsl-light)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(GSL_LIGHT_INCLUDE_DIRS
.
multiroots
)
FILE(GLOB_RECURSE gsl-light_CXX . *.c)

ADD_LIBRARY(gsl-light SHARED ${gsl-light_CXX})
SET_TARGET_PROPERTIES(gsl-light PROPERTIES OUTPUT_NAME "gsl-light")

TARGET_include_directories(gsl-light PRIVATE ${GSL_LIGHT_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES(gsl-light ${LAPACK_LIBRARIES} )

INSTALL(TARGETS gsl-light DESTINATION lib)



