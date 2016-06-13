include(CMakeParseArguments)

# Macro SETUP_SYSTEM_LIBRARY : defines USE_SYSTEM_XXX option, initialize
# internal variables
macro(SETUP_SYSTEM_LIBRARY)
  cmake_parse_arguments(NEW_SYSLIB  "" "PROJECT;DEFAULT" "" ${ARGN})
  option(USE_SYSTEM_${NEW_SYSLIB_PROJECT}
    "  Use a system build of ${NEW_SYSLIB_PROJECT}."
    ${NEW_SYSLIB_DEFAULT}
    )
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
  #set_property(GLOBAL PROPERTY prop_${project}_DEPENDENCIES "")
  set(${NEW_SB_PROJECT}_DEPENDENCIES "")
  set(${NEW_SB_PROJECT}_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${NEW_SB_PROJECT}/build)
  set(${NEW_SB_PROJECT}_SB_SRC ${CMAKE_BINARY_DIR}/${NEW_SB_PROJECT}/src/${NEW_SB_PROJECT})
  set(${NEW_SB_PROJECT}_SB_CONFIG)
  set(_SB_${NEW_SB_PROJECT}_ROOT ${SB_INSTALL_PREFIX})
endmacro(SETUP_SUPERBUILD)

macro(ADDTO_DEPENDENCIES_IF_NOT_SYSTEM project)
  foreach(dep ${ARGN})
    if(NOT USE_SYSTEM_${dep})
      #      get_property(old_${project}_DEPENDENCIES GLOBAL PROPERTY prop_${project}_DEPENDENCIES)
      list(APPEND ${project}_DEPENDENCIES "${dep}")
      #     set_property(GLOBAL PROPERTY prop_${project}_DEPENDENCIES "${old_${project}_DEPENDENCIES}")
    endif()
    string(TOLOWER ${dep} dep_lower)
    include(External_${dep_lower})
  endforeach()
endmacro(ADDTO_DEPENDENCIES_IF_NOT_SYSTEM)

# Macro to add a cmake variable to ${proj}_SB_CONFIG (var type: string)
macro(ADD_SUPERBUILD_CMAKE_VAR project var_name)
  set (extra_macro_args ${ARGN})
  # Did we get any optional args?
  list(LENGTH extra_macro_args num_extra_args)
  if (${num_extra_args} GREATER 0)
    list(GET extra_macro_args 0 optional_arg)
    set(var_type ${optional_arg})
  else()
    #try a guess for type from variable name using
    #CMake's variable naming convention!
    #convert to string to list so we ZLIB_INCLUDE_DIR as ZLIB, INCLUDE, DIR
    string(REPLACE "_" ";" var_name_parts ${var_name})
    #reverse the list.
    #We are only interested in the last part _DIR(s) or _LIBRARY, _ROOT etc..
    list(REVERSE var_name_parts)
    #simply pop first item to have the last part of var_name
    list(GET var_name_parts 0 var_name_last_part)

    #set var_type.
    if( "${var_name_last_part}" STREQUAL "DIR")
      set(var_type PATH)
    elseif("${var_name_last_part}" STREQUAL "LIBRARY")
      set(var_type FILEPATH)
    else()
      set(var_type STRING)
    endif()
  endif ()
  if(DEFINED _SB_${var_name})
    list(APPEND ${project}_SB_CONFIG
      -D${var_name}:${var_type}=${_SB_${var_name}}
      )
  elseif(DEFINED ${var_name})
    list(APPEND ${project}_SB_CONFIG
      -D${var_name}:${var_type}=${${var_name}}
      )
  endif()

endmacro(ADD_SUPERBUILD_CMAKE_VAR)

# Macro to add a configure variable to ${proj}_SB_CONFIG
# optional 3rd argument : suffix to the variable
macro(ADD_SUPERBUILD_CONFIGURE_VAR project var name)
  set(suffix "${ARGV3}")
  if(DEFINED _SB_${var})
    list(APPEND ${project}_SB_CONFIG
      ${name}=${_SB_${var}}${suffix}
      )
  elseif(DEFINED ${var})
    list(APPEND ${project}_SB_CONFIG
      ${name}=${${var}}${suffix}
      )
  endif()
endmacro(ADD_SUPERBUILD_CONFIGURE_VAR)

macro(SUPERBUILD_PATCH_SOURCE project)
  set(${project}_PATCH_DIR ${CMAKE_SOURCE_DIR}/patches/${project})
  string(TOLOWER ${project} project_)
  if(WIN32)
    set(DIFF_FILE_MATCH_STRING "win")
  else()
    if(APPLE)
      set(DIFF_FILE_MATCH_STRING "macx")
    else() #Linux
      set(DIFF_FILE_MATCH_STRING "linux")
    endif()
  endif() #WIN32
  file(GLOB files_list "${${project}_PATCH_DIR}/${project_}*${DIFF_FILE_MATCH_STRING}*diff")
  if(files_list)
    message(STATUS "  Custom patches required for ${project}")
    ExternalProject_Add_Step(${project} ${project}_custom_patch
      COMMAND
      ${CMAKE_COMMAND}
      -DSOURCE_DIR=${${project}_SB_SRC}
      -DPATCH_DIR=${${project}_PATCH_DIR}
      -DDIFF_FILE_MATCH_STRING=${DIFF_FILE_MATCH_STRING}
      -P ${CMAKE_SOURCE_DIR}/CMake/patch.cmake
      DEPENDEES patch update
      DEPENDERS configure
      )
  endif()

endmacro(SUPERBUILD_PATCH_SOURCE)

macro(SUPERBUILD_UPDATE_CMAKE_VARIABLES PROJECT with_prefix)

  if("${ARGV3}" STREQUAL "")
    string(TOLOWER ${PROJECT} lib_file_we)
  else()
    set(lib_file_we "${ARGV3}")
  endif()

  if("${ARGV4}" STREQUAL "")
    set(include_dir "include")
  else()
    set(include_dir "include/${ARGV4}")
  endif()

  if(WIN32)
    if(${with_prefix})
      set(lib_file lib${lib_file_we}${CMAKE_LINK_LIBRARY_SUFFIX})
    else()
      set(lib_file ${lib_file_we}${CMAKE_LINK_LIBRARY_SUFFIX})
    endif()
  else()
    set(lib_file "lib${lib_file_we}${CMAKE_SHARED_LIBRARY_SUFFIX}")
  endif()

  set(_SB_${PROJECT}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/${include_dir})
  set(_SB_${PROJECT}_LIBRARY ${SB_INSTALL_PREFIX}/lib/${lib_file})

endmacro()