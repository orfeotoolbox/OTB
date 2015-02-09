include(CMakeParseArguments)

# Macro SETUP_SYSTEM_LIBRARY : defines USE_SYSTEM_XXX option, initialize
# internal variables
macro(SETUP_SYSTEM_LIBRARY)
  cmake_parse_arguments(NEW_SYSLIB  "" "PROJECT;DOC" "" ${ARGN})
  option(USE_SYSTEM_${NEW_SYSLIB_PROJECT}
    ${NEW_SYSLIB_DOC}
    ${DEFAULT_USE_SYSTEM_${NEW_SYSLIB_PROJECT}}
    )
  mark_as_advanced(USE_SYSTEM_${NEW_SYSLIB_PROJECT})
  set(SYSTEM_${NEW_SYSLIB_PROJECT}_CMAKE_CACHE)
endmacro(SETUP_SYSTEM_LIBRARY)

# Macro ADD_SYSTEM_LOCATION : define variables to specific system locations
macro(ADD_SYSTEM_LOCATION)
  cmake_parse_arguments(NEW_SYSLOC  "" "PROJECT" "VARIABLES" ${ARGN})
  foreach(var ${NEW_SYSLOC_VARIABLES})
    set(${var} "" CACHE FILEPATH "Set a custom system location")
    mark_as_advanced(${var})
    if(USE_SYSTEM_${NEW_SYSLOC_PROJECT})
      set_property(CACHE ${var} PROPERTY TYPE FILEPATH)
    else()
      set_property(CACHE ${var} PROPERTY TYPE INTERNAL)
    endif()
    # add variable to cache if not empty
    if(NOT ${var} STREQUAL "")
      # additional argument for CMake cache
      set(ITEM_FOR_CMAKE_CACHE "-D${var}:STRING=${${var}}")
      list(APPEND SYSTEM_${NEW_SYSLOC_PROJECT}_CMAKE_CACHE ${ITEM_FOR_CMAKE_CACHE})
    endif()
  endforeach(var)
endmacro(ADD_SYSTEM_LOCATION)
