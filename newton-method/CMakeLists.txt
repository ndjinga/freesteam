# nom du projet newton-method
PROJECT(newton-method)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

set(MY_MULTIROOTS_SOURCES
  multiroots/enorm.c  multiroots/fdfsolver.c  multiroots/gnewton.c  multiroots/newton.c  multiroots/convergence.c  
)

set(MY_MULTIROOTS_HEADERS_DIR
  "${PROJECT_SOURCE_DIR}/newton-method/multiroots"
)

set(MY_MULTIROOTS_TEST_SOURCES
  multiroots/test.c  multiroots/test_funcs.c
)

add_library(newton-method SHARED ${MY_MULTIROOTS_SOURCES})
    set(LAPACK_INCLUDE_DIRS "/usr/include/lapacke/ /usr/include/openblas/ /usr/include/ ${LAPACK_INCLUDE_DIRS} $ENV{LAPACK_INCLUDE_DIRS}")
    target_include_directories(newton-method PRIVATE ${LAPACK_INCLUDE_DIRS} )
    target_link_libraries(newton-method ${LAPACK_LIBRARIES})

SET_TARGET_PROPERTIES(newton-method PROPERTIES OUTPUT_NAME "newton-method")
set_target_properties(newton-method PROPERTIES LINKER_LANGUAGE C)
TARGET_include_directories(newton-method PRIVATE multiroots )


INSTALL(TARGETS newton-method DESTINATION lib)



