# nom du projet gsl-light
PROJECT(gsl-light)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

FILE(GLOB gsl-vector_CXX vector/*.c)
FILE(GLOB gsl-vector_HXX vector/*.h)

ADD_LIBRARY(gsl-vector SHARED ${gsl-vector_CXX})
SET_TARGET_PROPERTIES(gsl-vector PROPERTIES OUTPUT_NAME "gsl-vector")
set_target_properties(gsl-vector PROPERTIES LINKER_LANGUAGE CXX)
TARGET_include_directories(gsl-vector PRIVATE . )

set(GSL_MULTIROOTS_INCLUDE_DIRS
.
vector
multiroots
)
FILE(GLOB gsl-multiroots_CXX multiroots *.*)

ADD_LIBRARY(gsl-multiroots SHARED ${gsl-multiroots_CXX})
SET_TARGET_PROPERTIES(gsl-multiroots PROPERTIES OUTPUT_NAME "gsl-multiroots")
set_target_properties(gsl-multiroots PROPERTIES LINKER_LANGUAGE CXX)
TARGET_include_directories(gsl-multiroots PRIVATE ${GSL_MULTIROOTS_INCLUDE_DIRS} )
target_link_libraries(gsl-multiroots gsl-vector)


set(GSL_LIGHT_INCLUDE_DIRS
.
vector
multiroots
)
FILE(GLOB_RECURSE gsl-light_CXX . *.c)

#ADD_LIBRARY(gsl-light )
#SET_TARGET_PROPERTIES(gsl-light PROPERTIES OUTPUT_NAME "gsl-light")

#TARGET_include_directories(gsl-light PRIVATE ${GSL_LIGHT_INCLUDE_DIRS} )
#TARGET_LINK_LIBRARIES(gsl-light gsl-vector gsl-multiroots ${LAPACK_LIBRARIES} )

#INSTALL(TARGETS gsl-light DESTINATION lib)



