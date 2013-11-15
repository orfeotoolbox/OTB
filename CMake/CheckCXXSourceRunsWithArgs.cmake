# This macro is stolen from the CMake official module CheckCXXSourceRuns.cmake
# and slightly adapted :
# - SOURCE_FILE parameter specifies a file name
# - ARGUMENT parameter specifies a runtime argument

macro(CHECK_CXX_SOURCE_RUNS_ARGS SOURCE_FILE ARGUMENT VAR)
  if("${VAR}" MATCHES "^${VAR}$")
    set(MACRO_CHECK_FUNCTION_DEFINITIONS
      "-D${VAR} ${CMAKE_REQUIRED_FLAGS}")
    if(CMAKE_REQUIRED_LIBRARIES)
      set(CHECK_CXX_SOURCE_COMPILES_ADD_LIBRARIES
        "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    else(CMAKE_REQUIRED_LIBRARIES)
      set(CHECK_CXX_SOURCE_COMPILES_ADD_LIBRARIES)
    endif(CMAKE_REQUIRED_LIBRARIES)
    if(CMAKE_REQUIRED_INCLUDES)
      set(CHECK_CXX_SOURCE_COMPILES_ADD_INCLUDES
        "-DINCLUDE_DIRECTORIES:STRING=${CMAKE_REQUIRED_INCLUDES}")
    else(CMAKE_REQUIRED_INCLUDES)
      set(CHECK_CXX_SOURCE_COMPILES_ADD_INCLUDES)
    endif(CMAKE_REQUIRED_INCLUDES)

    message(STATUS "Performing Test ${VAR}")
    try_run(${VAR}_EXITCODE ${VAR}_COMPILED
      ${CMAKE_BINARY_DIR}
      ${SOURCE_FILE}
      COMPILE_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
      -DCMAKE_SKIP_RPATH:BOOL=${CMAKE_SKIP_RPATH}
      "${CHECK_CXX_SOURCE_COMPILES_ADD_LIBRARIES}"
      "${CHECK_CXX_SOURCE_COMPILES_ADD_INCLUDES}"
      COMPILE_OUTPUT_VARIABLE OUTPUT
      ARGS "\"${ARGUMENT}\"")

    # if it did not compile make the return value fail code of 1
    if(NOT ${VAR}_COMPILED)
      set(${VAR}_EXITCODE 1)
    endif(NOT ${VAR}_COMPILED)
    # if the return value was 0 then it worked
    if("${${VAR}_EXITCODE}" EQUAL 0)
      set(${VAR} 1 CACHE INTERNAL "Test ${VAR}")
      message(STATUS "Performing Test ${VAR} - Success")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Performing C++ SOURCE FILE Test ${VAR} succeded with the following output:\n"
        "${OUTPUT}\n"
        "Return value: ${${VAR}}\n"
        "Argument: ${ARGUMENT}\n"
        "Source file was: ${SOURCE_FILE}\n\n")
    else("${${VAR}_EXITCODE}" EQUAL 0)
      if(CMAKE_CROSSCOMPILING AND "${${VAR}_EXITCODE}" MATCHES  "FAILED_TO_RUN")
        set(${VAR} "${${VAR}_EXITCODE}")
      else(CMAKE_CROSSCOMPILING AND "${${VAR}_EXITCODE}" MATCHES  "FAILED_TO_RUN")
        set(${VAR} "" CACHE INTERNAL "Test ${VAR}")
      endif(CMAKE_CROSSCOMPILING AND "${${VAR}_EXITCODE}" MATCHES  "FAILED_TO_RUN")

      message(STATUS "Performing Test ${VAR} - Failed")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
        "Performing C++ SOURCE FILE Test ${VAR} failed with the following output:\n"
        "${OUTPUT}\n"
        "Return value: ${${VAR}_EXITCODE}\n"
        "Argument: ${ARGUMENT}\n"
        "Source file was: ${SOURCE_FILE}\n\n")
    endif("${${VAR}_EXITCODE}" EQUAL 0)
  endif("${VAR}" MATCHES "^${VAR}$")
endmacro(CHECK_CXX_SOURCE_RUNS_ARGS)
