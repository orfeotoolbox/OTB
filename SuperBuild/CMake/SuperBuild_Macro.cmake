include(CMakeParseArguments)

# Macro SETUP_SYSTEM_LIBRARY : defines USE_SYSTEM_XXX option, initialize
# internal variables
macro(SETUP_SYSTEM_LIBRARY)
  cmake_parse_arguments(NEW_SYSLIB  "" "PROJECT;DEFAULT" "" ${ARGN})
  option(USE_SYSTEM_${NEW_SYSLIB_PROJECT}
    "  Use a system build of ${NEW_SYSLIB_PROJECT}."
    ${NEW_SYSLIB_DEFAULT}
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

# Macro ADD_SYSTEM_PREFIX : defines a system prefix for the given project
#   - creates a cache variable SYSTEM_${PROJECT}_PREFIX
#   - if CMAKE_ALIAS is defined, it is used to add an entry in cache
#       -D${CMAKE_ALIAS}:PATH=${SYSTEM_${PROJECT}_PREFIX}
macro(ADD_SYSTEM_PREFIX)
  cmake_parse_arguments(NEW_SYSPREFIX  "" "PROJECT;CMAKE_ALIAS" "" ${ARGN})
  set(SYSTEM_${NEW_SYSPREFIX_PROJECT}_PREFIX "" CACHE PATH "Set a custom system prefix")
  mark_as_advanced(SYSTEM_${NEW_SYSPREFIX_PROJECT}_PREFIX)
  if(USE_SYSTEM_${NEW_SYSPREFIX_PROJECT})
    set_property(CACHE SYSTEM_${NEW_SYSPREFIX_PROJECT}_PREFIX PROPERTY TYPE PATH)
  else()
    set_property(CACHE SYSTEM_${NEW_SYSPREFIX_PROJECT}_PREFIX PROPERTY TYPE INTERNAL)
  endif()
  if(NOT ${NEW_SYSPREFIX_CMAKE_ALIAS} STREQUAL "")
    if(NOT SYSTEM_${NEW_SYSPREFIX_PROJECT}_PREFIX STREQUAL "")
      set(ITEM_FOR_CMAKE_CACHE
        "-D${NEW_SYSPREFIX_CMAKE_ALIAS}:PATH=${SYSTEM_${NEW_SYSPREFIX_PROJECT}_PREFIX}")
      list(APPEND SYSTEM_${NEW_SYSLOC_PROJECT}_CMAKE_CACHE ${ITEM_FOR_CMAKE_CACHE})
    endif()
  endif()
endmacro(ADD_SYSTEM_PREFIX)

# Macro SETUP_SUPERBUILD
# Initialize usefull variables to build a superbuild project
macro(SETUP_SUPERBUILD)
  cmake_parse_arguments(NEW_SB "" "PROJECT" "" ${ARGN})
  set(${NEW_SB_PROJECT}_DEPENDENCIES)
  set(${NEW_SB_PROJECT}_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${NEW_SB_PROJECT}/build)
  set(${NEW_SB_PROJECT}_SB_SRC ${CMAKE_BINARY_DIR}/${NEW_SB_PROJECT}/src/${NEW_SB_PROJECT})
  set(${NEW_SB_PROJECT}_SB_CONFIG)
  set(_SB_${NEW_SB_PROJECT}_ROOT ${SB_INSTALL_PREFIX})
endmacro(SETUP_SUPERBUILD)

# Macro to include dependencies
macro(INCLUDE_SUPERBUILD_DEPENDENCIES)
  foreach(dep ${ARGV})
    string(TOLOWER ${dep} dep_lower)
    include(External_${dep_lower})
  endforeach()
endmacro(INCLUDE_SUPERBUILD_DEPENDENCIES)

# Macro to add a cmake variable to ${proj}_SB_CONFIG (var type: string)
macro(ADD_SUPERBUILD_CMAKE_VAR var)
  if(DEFINED _SB_${var})
    list(APPEND ${proj}_SB_CONFIG
      -D${var}:STRING=${_SB_${var}}
      )
  elseif(DEFINED ${var})
    list(APPEND ${proj}_SB_CONFIG
      -D${var}:STRING=${${var}}
      )
  endif()
endmacro(ADD_SUPERBUILD_CMAKE_VAR)

# Macro to add a configure variable to ${proj}_SB_CONFIG
# optional 3rd argument : suffix to the variable
macro(ADD_SUPERBUILD_CONFIGURE_VAR var name)
  set(suffix "${ARGV2}")
  if(DEFINED _SB_${var})
    list(APPEND ${proj}_SB_CONFIG
      ${name}=${_SB_${var}}${suffix}
      )
  elseif(DEFINED ${var})
    list(APPEND ${proj}_SB_CONFIG
      ${name}=${${var}}${suffix}
      )
  endif()
endmacro(ADD_SUPERBUILD_CONFIGURE_VAR)
