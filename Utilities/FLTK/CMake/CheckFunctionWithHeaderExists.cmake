#
# Check if the symbol exists in include files
#
# CHECK_FUNCTIONWITHHEADER_EXISTS - macro which checks the symbol exists in include files.
# SYMBOL - symbol
# FILES  - include files to check
# VARIABLE - variable to return result
#

macro(CHECK_FUNCTIONWITHHEADER_EXISTS SYMBOL FILES VARIABLE)
  if("${VARIABLE}" MATCHES "^${VARIABLE}$")
    set(CHECK_SYMBOL_EXISTS_CONTENT "/* */\n")
    set(MACRO_CHECK_SYMBOL_EXISTS_FLAGS ${CMAKE_REQUIRED_FLAGS})
    if(CMAKE_REQUIRED_LIBRARIES)
      set(CHECK_SYMBOL_EXISTS_LIBS
        "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    endif()
    foreach(FILE ${FILES})
      set(CHECK_SYMBOL_EXISTS_CONTENT
        "${CHECK_SYMBOL_EXISTS_CONTENT}#include <${FILE}>\n")
    endforeach()
    set(CHECK_SYMBOL_EXISTS_CONTENT
      "${CHECK_SYMBOL_EXISTS_CONTENT}\nint main()\n{\n${SYMBOL};return 0;\n}\n")

    file(WRITE ${CMAKE_BINARY_DIR}/CMakeTmp/CheckSymbolExists.c
      "${CHECK_SYMBOL_EXISTS_CONTENT}")

    message(STATUS "Looking for ${SYMBOL}")
    try_compile(${VARIABLE}
      ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/CMakeTmp/CheckSymbolExists.c
      CMAKE_FLAGS
      -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_SYMBOL_EXISTS_FLAGS}
      "${CHECK_SYMBOL_EXISTS_LIBS}"
      OUTPUT_VARIABLE OUTPUT)
    if(${VARIABLE})
      message(STATUS "Looking for ${SYMBOL} - found")
      set(${VARIABLE} 1 CACHE INTERNAL "Have symbol ${SYMBOL}")
      file(APPEND ${CMAKE_BINARY_DIR}/CMakeOutput.log
        "Determining if the ${SYMBOL} "
        "exist passed with the following output:\n"
        "${OUTPUT}\nFile ${CMAKE_BINARY_DIR}/CMakeTmp/CheckSymbolExists.c:\n"
        "${CHECK_SYMBOL_EXISTS_CONTENT}\n")
    else()
      message(STATUS "Looking for ${SYMBOL} - not found.")
      set(${VARIABLE} "" CACHE INTERNAL "Have symbol ${SYMBOL}")
      file(APPEND ${CMAKE_BINARY_DIR}/CMakeError.log
        "Determining if the ${SYMBOL} "
        "exist failed with the following output:\n"
        "${OUTPUT}\nFile ${CMAKE_BINARY_DIR}/CMakeTmp/CheckSymbolExists.c:\n"
        "${CHECK_SYMBOL_EXISTS_CONTENT}\n")
    endif()
  endif()
endmacro()
