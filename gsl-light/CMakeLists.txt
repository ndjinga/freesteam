# nom du projet gsl-light
PROJECT(gsl-light)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FILE(GLOB_RECURSE gsl-light_CXX . *.cxx)
FILE(GLOB_RECURSE gsl-light_HXX . *.hxx)

ADD_LIBRARY(gsl-light SHARED ${src_gsl-light_CXX})
SET_TARGET_PROPERTIES(gsl-light PROPERTIES OUTPUT_NAME "gsl-light")

TARGET_include_directories(gsl-light PRIVATE ${src_gsl-light_HXX} )
TARGET_LINK_LIBRARIES(gsl-light ${LAPACK_LIBRARIES} )

INSTALL(TARGETS gsl-light DESTINATION lib)



