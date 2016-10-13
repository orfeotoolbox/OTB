include(CMakeParseArguments)

macro(INCLUDE_ONCE_MACRO project)
  if( __included_${project}__)
  return()
else()
  set(__included_${project}__ 1)
endif()

if(USE_SYSTEM_${project})
  #message(STATUS "  Using ${project} system version")
  list(APPEND FROM_SYSTEM_LIST ${project})
  return()
endif()
endmacro()

# Macro SETUP_SUPERBUILD
# Initialize useful variables to build a superbuild project
macro(SETUP_SUPERBUILD NEW_SB_PROJECT)
  list(APPEND FROM_SUPERBUILD_LIST ${NEW_SB_PROJECT})
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
    set(${project}_SB_CONFIG
      ${${project}_SB_CONFIG} ${name}=${_SB_${var}}${suffix})
  elseif(DEFINED ${var})
    set(${project}_SB_CONFIG
      ${${project}_SB_CONFIG} ${name}=${${var}}${suffix})
  endif()
endmacro(ADD_SUPERBUILD_CONFIGURE_VAR)

macro(SUPERBUILD_PATCH_SOURCE project)
  set(${project}_PATCH_DIR ${CMAKE_SOURCE_DIR}/patches/${project})
  string(TOLOWER ${project} patch_dir_prefix)
  set(PATCH_ARGS "${ARGV1}")

  if(WIN32)
    set(DIFF_FILE_MATCH_STRING "win")
    if(NOT PATCH_ARGS)
      set(PATCH_ARGS "--binary")
    endif()
  else()
    if(APPLE)
      set(DIFF_FILE_MATCH_STRING "macx")
    else() #Linux
      set(DIFF_FILE_MATCH_STRING "linux")
    endif()
  endif() #WIN32

  #if an optional diff file is mentioned. then only that is applied
  set(DOT_DIFF_FILES "${ARGV2}")

  if(NOT DOT_DIFF_FILES)
    #If DOT_DIFF_FILES is empty,
    #we try to list files ending with all.diff of <DIFF_FILE_MATCH_STRING>.diff
    #the final list is stored in DOT_DIFF_FILES and applied using patch.cmake
    #Custom patches requrired for...

    #glob all files ending with DIFF_FILE_MATCH_STRING.diff
    file(GLOB files_list "${${project}_PATCH_DIR}/${patch_dir_prefix}*${DIFF_FILE_MATCH_STRING}.diff")

    #glob all files ending with all.diff
    file(GLOB all_files_list "${${project}_PATCH_DIR}/${patch_dir_prefix}*all.diff")

    #merge two list for the final one
    list(APPEND files_list ${all_files_list})

    #because we are passing it cmake_command using -D!!
    string(REPLACE ";" " " DOT_DIFF_FILES "${files_list}")
  endif()

  if(DOT_DIFF_FILES)
    message(STATUS "  Custom patches required for ${project}")
    ExternalProject_Add_Step(${project} ${project}_custom_patch
      COMMAND
      ${CMAKE_COMMAND}
      -DSOURCE_DIR=${${project}_SB_SRC}
      -DDOT_DIFF_FILES=${DOT_DIFF_FILES}
      -DPATCH_ARGS=${PATCH_ARGS}
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

macro(package_require_cxx11 project)
  if(NOT OTB_HAS_CXX11)
    message(FATAL_ERROR "${project} requires C++11 support. consider adding --std=c++11 to your cxx compiler flags or disable ${project} ")
  endif()
endmacro()

