# nom du projet gsl-light
PROJECT(gsl-multiroots)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

FILE(GLOB gsl-multiroots_CXX multiroots *.*)

ADD_LIBRARY(gsl-multiroots SHARED ${gsl-multiroots_CXX})
SET_TARGET_PROPERTIES(gsl-multiroots PROPERTIES OUTPUT_NAME "gsl-multiroots")
set_target_properties(gsl-multiroots PROPERTIES LINKER_LANGUAGE CXX)
TARGET_include_directories(gsl-multiroots PRIVATE ${GSL_MULTIROOTS_INCLUDE_DIRS} )


set(GSL_MULTIROOTS_INCLUDE_DIRS
.
multiroots
)
FILE(GLOB_RECURSE gsl-multiroots_CXX . *.c)

#INSTALL(TARGETS gsl-multiroots DESTINATION lib)



