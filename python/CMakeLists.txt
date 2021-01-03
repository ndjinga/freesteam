INCLUDE(${SWIG_USE_FILE})

######################################
# setup has been done in project level, just do it again
######################################

# cross OS setup
IF(WIN32)
   # windows
   SET(PY_EXTENSION "pyd")
ELSE(WIN32)
   SET(PY_EXTENSION "so")
ENDIF(WIN32)

# DO NOT use relative include path within *.i file, but add the include_path
include_directories(${MY_HEADER_DIR} ${PYTHON_INCLUDE_DIRS})

set(MY_MODULE "freesteam")  # it is possible to use the same name as sharedlib name
set(MY_SHARED_LIB "freesteam")
set(MY_SWIG_INTERFACE "freesteam.i")

######################################
# swig build
######################################
message( WARNING "Freesteam warning : if cmake fails, turn off SWIG option in cmake, and then build with setup.py")

file(COPY freesteam.i DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/freesteam.i PROPERTIES SWIG_DEFINITIONS "-shadow")

if(${CMAKE_VERSION} VERSION_GREATER "3.8.0") 
  SWIG_ADD_LIBRARY(freesteam TYPE SHARED LANGUAGE python SOURCES ${CMAKE_CURRENT_BINARY_DIR}/freesteam.i)
else(${CMAKE_VERSION} VERSION_GREATER "3.8.0")  # this section is not tested
  swig_add_module(${MY_MODULE}   python    ${MY_SWIG_INTERFACE}   )
endif(${CMAKE_VERSION} VERSION_GREATER "3.8.0")

SWIG_LINK_LIBRARIES(freesteam ${PYTHON_LIBRARIES}  freesteamCPP ${GSL_LIBRARIES} )

INSTALL(TARGETS _freesteam DESTINATION lib/freesteam)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/freesteam.py DESTINATION bin/freesteam)

file(COPY ${PROJECT_SOURCE_DIR}/python/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILES_MATCHING PATTERN "*.py")

add_test(NAME freesteam_pytest COMMAND ${PYTHON_EXECUTABLE} ./test.py)  
set_property(TEST freesteam_pytest PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_BINARY_DIR}:${LD_LIBRARY_PATH}")
set_property(TEST freesteam_pytest PROPERTY ENVIRONMENT      "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_BINARY_DIR}:${PYTHONPATH}")

add_test(NAME hsdiagram COMMAND ${PYTHON_EXECUTABLE} ./hsdiagram.py)  
set_property(TEST hsdiagram PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_BINARY_DIR}:${LD_LIBRARY_PATH}")
set_property(TEST hsdiagram PROPERTY ENVIRONMENT      "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_BINARY_DIR}:${PYTHONPATH}")

