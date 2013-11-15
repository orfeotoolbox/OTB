# - Try to find Boost include dirs and libraries
# Usage of this module as follows:
#
#     set(Boost_USE_STATIC_LIBS ON)
#     set(Boost_USE_MULTITHREAD OFF)
#     find_package( Boost 1.34.1 COMPONENTS date_time filesystem iostreams ... )
#
# The Boost_ADDITIONAL_VERSIONS variable can be used to specify a list of
# boost version numbers that should be taken into account when searching
# for the libraries. Unfortunately boost puts the version number into the
# actual filename for the libraries, so this might be needed in the future
# when new Boost versions are released.
#
# Currently this module searches for the following version numbers:
# 1.33, 1.33.0, 1.33.1, 1.34, 1.34.0, 1.34.1, 1.35, 1.35.0, 1.35.1, 1.36,
# 1.36.0, 1.36.1
#
# The components list needs to be the actual names of boost libraries, that is
# the part of the actual library files that differ on different libraries. So
# its "date_time" for "libboost_date_time...". Anything else will result in
# errors
#
# You can provide a minimum version number that should be used. If you provide this
# version number and specify the REQUIRED attribute, this module will fail if it
# can't find the specified or a later version. If you specify a version number this is
# automatically put into the considered list of version numbers and thus doesn't need
# to be specified in the Boost_ADDITIONAL_VERSIONS variable
#
# Variables used by this module, they can change the default behaviour and need to be set
# before calling find_package:
#  Boost_USE_MULTITHREAD         Can be set to OFF to use the non-multithreaded
#                                boost libraries. Defaults to ON.
#  Boost_USE_STATIC_LIBS         Can be set to ON to force the use of the static
#                                boost libraries. Defaults to OFF.
#  Boost_ADDITIONAL_VERSIONS     A list of version numbers to use for searching
#                                the boost include directory. The default list
#                                of version numbers is:
#                                1.33, 1.33.0, 1.33.1, 1.34, 1.34.0, 1.34.1,
#                                1.35, 1.35.0, 1.35.1, 1.36, 1.36.0, 1.36.1
#                                If you want to look for an older or newer
#                                version set this variable to a list of
#                                strings, where each string contains a number, i.e.
#                                set(Boost_ADDITIONAL_VERSIONS "0.99.0" "1.35.0")
#  BOOST_ROOT or BOOSTROOT       Preferred installation prefix for searching for Boost,
#                                set this if the module has problems finding the proper Boost installation
#  BOOST_INCLUDEDIR              Set this to the include directory of Boost, if the
#                                module has problems finding the proper Boost installation
#  BOOST_LIBRARYDIR              Set this to the lib directory of Boost, if the
#                                module has problems finding the proper Boost installation
#
#  The last three variables are available also as environment variables
#
#
# Variables defined by this module:
#
#  Boost_FOUND                          System has Boost, this means the include dir was found,
#                                       as well as all the libraries specified in the COMPONENTS list
#  Boost_INCLUDE_DIRS                   Boost include directories, not cached
#  Boost_INCLUDE_DIR                    This is almost the same as above, but this one is cached and may be
#                                       modified by advanced users
#  Boost_LIBRARIES                      Link these to use the Boost libraries that you specified, not cached
#  Boost_LIBRARY_DIRS                   The path to where the Boost library files are.
#  Boost_VERSION                        The version number of the boost libraries that have been found,
#                                       same as in version.hpp from Boost
#  Boost_LIB_VERSION                    The version number in filename form as its appended to the library filenames
#  Boost_MAJOR_VERSION                  major version number of boost
#  Boost_MINOR_VERSION                  minor version number of boost
#  Boost_SUBMINOR_VERSION               subminor version number of boost
#  Boost_LIB_DIAGNOSTIC_DEFINITIONS     Only set on windows. Can be used with add_definitions
#                                       to print diagnostic information about the automatic
#                                       linking done on windows.

# For each component you list the following variables are set.
# ATTENTION: The component names need to be in lower case, just as the boost
# library names however the cmake variables use upper case for the component
# part. So you'd get Boost_SERIALIZATION_FOUND for example.
#
#  Boost_${COMPONENT}_FOUND             True IF the Boost library "component" was found.
#  Boost_${COMPONENT}_LIBRARY           The absolute path of the Boost library "component".
#  Boost_${COMPONENT}_LIBRARY_DEBUG     The absolute path of the debug version of the
#                                       Boost library "component".
#  Boost_${COMPONENT}_LIBRARY_RELEASE   The absolute path of the release version of the
#                                       Boost library "component"
#
#  Copyright (c) 2006-2008 Andreas Schneider <mail@cynapses.org>
#  Copyright (c) 2007      Wengo
#  Copyright (c) 2007      Mike Jackson
#  Copyright (c) 2008      Andreas Pakulat <apaku@gmx.de>
#
#  Redistribution AND use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
option(Boost_USE_MULTITHREADED
  "Use the multithreaded versions of the Boost libraries" ON)

if (Boost_FIND_VERSION_EXACT)
  if (Boost_FIND_VERSION_PATCH)
    set( _boost_TEST_VERSIONS
      "${Boost_FIND_VERSION_MAJOR}.${Boost_FIND_VERSION_MINOR}.${Boost_FIND_VERSION_PATCH}")
  else (Boost_FIND_VERSION_PATCH)
    set( _boost_TEST_VERSIONS
      "${Boost_FIND_VERSION_MAJOR}.${Boost_FIND_VERSION_MINOR}.0"
      "${Boost_FIND_VERSION_MAJOR}.${Boost_FIND_VERSION_MINOR}")
  endif (Boost_FIND_VERSION_PATCH)
else (Boost_FIND_VERSION_EXACT)
  set( _boost_TEST_VERSIONS ${Boost_ADDITIONAL_VERSIONS}
    "1.49.0" "1.49" "1.48.0" "1.48" "1.47.0" "1.47" "1.46.0" "1.46" "1.45.0"
    "1.45" "1.44.0" "1.44" "1.43.0" "1.43" "1.42.0" "1.42"
    "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37"
    "1.36.1" "1.36.0" "1.36" "1.35.1" "1.35.0" "1.35" "1.34.1" "1.34.0"
    "1.34" "1.33.1" "1.33.0" "1.33" )
endif (Boost_FIND_VERSION_EXACT)

# The reason that we failed to find Boost. This will be set to a
# user-friendly message when we fail to find some necessary piece of
# Boost.
set(Boost_ERROR_REASON)

############################################
#
# Check the existence of the libraries.
#
############################################
# This macro was taken directly from the FindQt4.cmake file that is included
# with the CMake distribution. This is NOT my work. All work was done by the
# original authors of the FindQt4.cmake file. Only minor modifications were
# made to remove references to Qt and make this file more generally applicable
#########################################################################

macro(_Boost_ADJUST_LIB_VARS basename)
  if(Boost_INCLUDE_DIR )
    if(Boost_${basename}_LIBRARY_DEBUG AND Boost_${basename}_LIBRARY_RELEASE)
      # if the generator supports configuration types then set
      # optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
      if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        set(Boost_${basename}_LIBRARY optimized ${Boost_${basename}_LIBRARY_RELEASE} debug ${Boost_${basename}_LIBRARY_DEBUG})
      else(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        # if there are no configuration types and CMAKE_BUILD_TYPE has no value
        # then just use the release libraries
        set(Boost_${basename}_LIBRARY ${Boost_${basename}_LIBRARY_RELEASE} )
      endif(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
      set(Boost_${basename}_LIBRARIES optimized ${Boost_${basename}_LIBRARY_RELEASE} debug ${Boost_${basename}_LIBRARY_DEBUG})
    endif(Boost_${basename}_LIBRARY_DEBUG AND Boost_${basename}_LIBRARY_RELEASE)

    # if only the release version was found, set the debug variable also to the release version
    if(Boost_${basename}_LIBRARY_RELEASE AND NOT Boost_${basename}_LIBRARY_DEBUG)
      set(Boost_${basename}_LIBRARY_DEBUG ${Boost_${basename}_LIBRARY_RELEASE})
      set(Boost_${basename}_LIBRARY       ${Boost_${basename}_LIBRARY_RELEASE})
      set(Boost_${basename}_LIBRARIES     ${Boost_${basename}_LIBRARY_RELEASE})
    endif(Boost_${basename}_LIBRARY_RELEASE AND NOT Boost_${basename}_LIBRARY_DEBUG)

    # if only the debug version was found, set the release variable also to the debug version
    if(Boost_${basename}_LIBRARY_DEBUG AND NOT Boost_${basename}_LIBRARY_RELEASE)
      set(Boost_${basename}_LIBRARY_RELEASE ${Boost_${basename}_LIBRARY_DEBUG})
      set(Boost_${basename}_LIBRARY         ${Boost_${basename}_LIBRARY_DEBUG})
      set(Boost_${basename}_LIBRARIES       ${Boost_${basename}_LIBRARY_DEBUG})
    endif(Boost_${basename}_LIBRARY_DEBUG AND NOT Boost_${basename}_LIBRARY_RELEASE)

    if(Boost_${basename}_LIBRARY)
      set(Boost_${basename}_LIBRARY ${Boost_${basename}_LIBRARY} CACHE FILEPATH "The Boost ${basename} library")
      get_filename_component(Boost_LIBRARY_DIRS "${Boost_${basename}_LIBRARY}" PATH)
      set(Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIRS} CACHE FILEPATH "Boost library directory")
      set(Boost_${basename}_FOUND ON CACHE INTERNAL "Whether the Boost ${basename} library found")
    endif(Boost_${basename}_LIBRARY)

  endif(Boost_INCLUDE_DIR )
  # Make variables changeble to the advanced user
  mark_as_advanced(
      Boost_${basename}_LIBRARY
      Boost_${basename}_LIBRARY_RELEASE
      Boost_${basename}_LIBRARY_DEBUG
  )
endmacro(_Boost_ADJUST_LIB_VARS)

#-------------------------------------------------------------------------------


set( _boost_IN_CACHE TRUE)
if(Boost_INCLUDE_DIR)
  foreach(COMPONENT ${Boost_FIND_COMPONENTS})
    string(TOUPPER ${COMPONENT} COMPONENT)
    if(NOT Boost_${COMPONENT}_FOUND)
      set( _boost_IN_CACHE FALSE)
    endif(NOT Boost_${COMPONENT}_FOUND)
  endforeach(COMPONENT)
else(Boost_INCLUDE_DIR)
  set( _boost_IN_CACHE FALSE)
endif(Boost_INCLUDE_DIR)

if(_boost_IN_CACHE)
  # in cache already
  set(Boost_FOUND TRUE)
  foreach(COMPONENT ${Boost_FIND_COMPONENTS})
    string(TOUPPER ${COMPONENT} COMPONENT)
    _Boost_ADJUST_LIB_VARS( ${COMPONENT} )
    set(Boost_LIBRARIES ${Boost_LIBRARIES} ${Boost_${COMPONENT}_LIBRARY})
  endforeach(COMPONENT)
  set(Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIR})
  if(Boost_VERSION AND NOT "${Boost_VERSION}" STREQUAL "0")
    math(EXPR Boost_MAJOR_VERSION "${Boost_VERSION} / 100000")
    math(EXPR Boost_MINOR_VERSION "${Boost_VERSION} / 100 % 1000")
    math(EXPR Boost_SUBMINOR_VERSION "${Boost_VERSION} % 100")
  endif(Boost_VERSION AND NOT "${Boost_VERSION}" STREQUAL "0")
else(_boost_IN_CACHE)
  # Need to search for boost

  if(WIN32)
    # In windows, automatic linking is performed, so you do not have
    # to specify the libraries.  If you are linking to a dynamic
    # runtime, then you can choose to link to either a static or a
    # dynamic Boost library, the default is to do a static link.  You
    # can alter this for a specific library "whatever" by defining
    # BOOST_WHATEVER_DYN_LINK to force Boost library "whatever" to be
    # linked dynamically.  Alternatively you can force all Boost
    # libraries to dynamic link by defining BOOST_ALL_DYN_LINK.

    # This feature can be disabled for Boost library "whatever" by
    # defining BOOST_WHATEVER_NO_LIB, or for all of Boost by defining
    # BOOST_ALL_NO_LIB.

    # If you want to observe which libraries are being linked against
    # then defining BOOST_LIB_DIAGNOSTIC will cause the auto-linking
    # code to emit a #pragma message each time a library is selected
    # for linking.
    set(Boost_LIB_DIAGNOSTIC_DEFINITIONS
      "-DBOOST_LIB_DIAGNOSTIC" CACHE STRING "Boost diagnostic define")
  endif(WIN32)

  set(_boost_INCLUDE_SEARCH_DIRS
    C:/boost/include
    "C:/boost"
    "$ENV{ProgramFiles}/boost/boost_${Boost_FIND_VERSION_MAJOR}_${Boost_FIND_VERSION_MINOR}_${Boost_FIND_VERSION_PATCH}"
    "$ENV{ProgramFiles}/Boost"
    /sw/local/include
  )

  set(_boost_LIBRARIES_SEARCH_DIRS
    C:/boost/lib
    "C:/boost"
    "$ENV{ProgramFiles}/boost/boost_${Boost_FIND_VERSION_MAJOR}_${Boost_FIND_VERSION_MINOR}_${Boost_FIND_VERSION_PATCH}/lib"
    "$ENV{ProgramFiles}/Boost"
    /sw/local/lib
  )

  # If BOOST_ROOT was defined in the environment, use it.
  if (NOT BOOST_ROOT AND NOT $ENV{BOOST_ROOT} STREQUAL "")
    set(BOOST_ROOT $ENV{BOOST_ROOT})
  endif(NOT BOOST_ROOT AND NOT $ENV{BOOST_ROOT} STREQUAL "")

  # If BOOSTROOT was defined in the environment, use it.
  if (NOT BOOST_ROOT AND NOT $ENV{BOOSTROOT} STREQUAL "")
    set(BOOST_ROOT $ENV{BOOSTROOT})
  endif(NOT BOOST_ROOT AND NOT $ENV{BOOSTROOT} STREQUAL "")

  # If BOOST_INCLUDEDIR was defined in the environment, use it.
  if( NOT $ENV{BOOST_INCLUDEDIR} STREQUAL "" )
    set(BOOST_INCLUDEDIR $ENV{BOOST_INCLUDEDIR})
  endif( NOT $ENV{BOOST_INCLUDEDIR} STREQUAL "" )

  # If BOOST_LIBRARYDIR was defined in the environment, use it.
  if( NOT $ENV{BOOST_LIBRARYDIR} STREQUAL "" )
    set(BOOST_LIBRARYDIR $ENV{BOOST_LIBRARYDIR})
  endif( NOT $ENV{BOOST_LIBRARYDIR} STREQUAL "" )

  if( BOOST_ROOT )
    file(TO_CMAKE_PATH ${BOOST_ROOT} BOOST_ROOT)
    set(_boost_INCLUDE_SEARCH_DIRS
      ${BOOST_ROOT}/include
      ${BOOST_ROOT}
      ${_boost_INCLUDE_SEARCH_DIRS})
    set(_boost_LIBRARIES_SEARCH_DIRS
      ${BOOST_ROOT}/lib
      ${BOOST_ROOT}/stage/lib
      ${_boost_LIBRARIES_SEARCH_DIRS})
  endif( BOOST_ROOT )

  if( BOOST_INCLUDEDIR )
    file(TO_CMAKE_PATH ${BOOST_INCLUDEDIR} BOOST_INCLUDEDIR)
    set(_boost_INCLUDE_SEARCH_DIRS
      ${BOOST_INCLUDEDIR} ${_boost_INCLUDE_SEARCH_DIRS})
  endif( BOOST_INCLUDEDIR )

  if( BOOST_LIBRARYDIR )
    file(TO_CMAKE_PATH ${BOOST_LIBRARYDIR} BOOST_LIBRARYDIR)
    set(_boost_LIBRARIES_SEARCH_DIRS
      ${BOOST_LIBRARYDIR} ${_boost_LIBRARIES_SEARCH_DIRS})
  endif( BOOST_LIBRARYDIR )

  # Try to find Boost by stepping backwards through the Boost versions
  # we know about.
  if( NOT Boost_INCLUDE_DIR )
    # Build a list of path suffixes for each version.
    set(_boost_PATH_SUFFIXES)
    foreach(_boost_VER ${_boost_TEST_VERSIONS})
      # Add in a path suffix, based on the required version, ideally
      # we could read this from version.hpp, but for that to work we'd
      # need to know the include dir already
      if (WIN32 AND NOT CYGWIN)
        set(_boost_PATH_SUFFIX boost_${_boost_VER})
      else (WIN32 AND NOT CYGWIN)
        set(_boost_PATH_SUFFIX boost-${_boost_VER})
      endif (WIN32 AND NOT CYGWIN)

      if(_boost_PATH_SUFFIX MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+")
          string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1_\\2_\\3"
            _boost_PATH_SUFFIX ${_boost_PATH_SUFFIX})
      elseif(_boost_PATH_SUFFIX MATCHES "[0-9]+\\.[0-9]+")
          string(REGEX REPLACE "([0-9]+)\\.([0-9]+)" "\\1_\\2"
            _boost_PATH_SUFFIX ${_boost_PATH_SUFFIX})
      endif(_boost_PATH_SUFFIX MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+")
      list(APPEND _boost_PATH_SUFFIXES "${_boost_PATH_SUFFIX}")
    endforeach(_boost_VER)

    # Look for a standard boost header file.
    find_path(Boost_INCLUDE_DIR
      NAMES         boost/config.hpp
      HINTS         ${_boost_INCLUDE_SEARCH_DIRS}
      PATH_SUFFIXES ${_boost_PATH_SUFFIXES}
      )
  endif( NOT Boost_INCLUDE_DIR )

  if(Boost_INCLUDE_DIR)
    # Extract Boost_VERSION and Boost_LIB_VERSION from version.hpp
    # Read the whole file:
    #
    set(BOOST_VERSION 0)
    set(BOOST_LIB_VERSION "")
    file(READ "${Boost_INCLUDE_DIR}/boost/version.hpp" _boost_VERSION_HPP_CONTENTS)

    string(REGEX REPLACE ".*#define BOOST_VERSION ([0-9]+).*" "\\1" Boost_VERSION "${_boost_VERSION_HPP_CONTENTS}")
    string(REGEX REPLACE ".*#define BOOST_LIB_VERSION \"([0-9_]+)\".*" "\\1" Boost_LIB_VERSION "${_boost_VERSION_HPP_CONTENTS}")

    set(Boost_LIB_VERSION ${Boost_LIB_VERSION} CACHE INTERNAL "The library version string for boost libraries")
    set(Boost_VERSION ${Boost_VERSION} CACHE INTERNAL "The version number for boost libraries")

    if(NOT "${Boost_VERSION}" STREQUAL "0")
      math(EXPR Boost_MAJOR_VERSION "${Boost_VERSION} / 100000")
      math(EXPR Boost_MINOR_VERSION "${Boost_VERSION} / 100 % 1000")
      math(EXPR Boost_SUBMINOR_VERSION "${Boost_VERSION} % 100")

      set(Boost_ERROR_REASON
          "${Boost_ERROR_REASON}Boost version: ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}\nBoost include path: ${Boost_INCLUDE_DIR}")
    endif(NOT "${Boost_VERSION}" STREQUAL "0")
  else(Boost_INCLUDE_DIR)
    set(Boost_ERROR_REASON
      "${Boost_ERROR_REASON}Unable to find the Boost header files. Please set BOOST_ROOT to the root directory containing Boost or BOOST_INCLUDEDIR to the directory containing Boost's headers.")
  endif(Boost_INCLUDE_DIR)

  # Setting some more suffixes for the library
  set(Boost_LIB_PREFIX "")
  if( WIN32 AND Boost_USE_STATIC_LIBS )
    set(Boost_LIB_PREFIX "lib")
  endif( WIN32 AND Boost_USE_STATIC_LIBS )
  set(_boost_COMPILER "-gcc")
  if(MSVC90)
    set(_boost_COMPILER "-vc90")
  elseif(MSVC80)
    set(_boost_COMPILER "-vc80")
  elseif(MSVC71)
    set(_boost_COMPILER "-vc71")
  endif(MSVC90)
  if(MINGW)
    exec_program(${CMAKE_CXX_COMPILER}
      ARGS -dumpversion
      OUTPUT_VARIABLE _boost_COMPILER_VERSION
      )
    string(REGEX REPLACE "([0-9])\\.([0-9])\\.[0-9]" "\\1\\2"
      _boost_COMPILER_VERSION ${_boost_COMPILER_VERSION})
    set(_boost_COMPILER "-mgw${_boost_COMPILER_VERSION}")
  endif(MINGW)
  if(UNIX)
    if(NOT CMAKE_COMPILER_IS_GNUCC)
      # We assume that we have the Intel compiler.
      set(_boost_COMPILER "-il")
    else(NOT CMAKE_COMPILER_IS_GNUCC)
      # Determine which version of GCC we have.
      exec_program(${CMAKE_CXX_COMPILER}
        ARGS -dumpversion
        OUTPUT_VARIABLE _boost_COMPILER_VERSION
        )
      string(REGEX REPLACE "([0-9])\\.([0-9])\\.[0-9]" "\\1\\2"
        _boost_COMPILER_VERSION ${_boost_COMPILER_VERSION})
      if(APPLE)
        if(Boost_MINOR_VERSION)
          if(${Boost_MINOR_VERSION} GREATER 35)
            # In Boost 1.36.0 and newer, the mangled compiler name used
            # on Mac OS X/Darwin is "xgcc".
            set(_boost_COMPILER "-xgcc${_boost_COMPILER_VERSION}")
          else(${Boost_MINOR_VERSION} GREATER 35)
            # In Boost <= 1.35.0, there is no mangled compiler name for
            # the Mac OS X/Darwin version of GCC.
            set(_boost_COMPILER "")
          endif(${Boost_MINOR_VERSION} GREATER 35)
        else(Boost_MINOR_VERSION)
          # We don't know the Boost version, so assume it's
          # pre-1.36.0.
          set(_boost_COMPILER "")
        endif(Boost_MINOR_VERSION)
      else()
        set(_boost_COMPILER "-gcc${_boost_COMPILER_VERSION}")
      endif()
    endif(NOT CMAKE_COMPILER_IS_GNUCC)
  endif(UNIX)

  set(_boost_MULTITHREADED "-mt")

  if( NOT Boost_USE_MULTITHREADED )
    set(_boost_MULTITHREADED "")
  endif( NOT Boost_USE_MULTITHREADED )

  set( _boost_STATIC_TAG "")
  if(WIN32)
    if(MSVC)
      set(_boost_ABI_TAG "g")
    endif(MSVC)
    if( Boost_USE_STATIC_LIBS )
      set( _boost_STATIC_TAG "-s")
    endif( Boost_USE_STATIC_LIBS )
  endif(WIN32)
  set(_boost_ABI_TAG "${_boost_ABI_TAG}d")

  # ------------------------------------------------------------------------
  #  Begin finding boost libraries
  # ------------------------------------------------------------------------
  foreach(COMPONENT ${Boost_FIND_COMPONENTS})
    string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
    set( Boost_${UPPERCOMPONENT}_LIBRARY "Boost_${UPPERCOMPONENT}_LIBRARY-NOTFOUND" )
    set( Boost_${UPPERCOMPONENT}_LIBRARY_RELEASE "Boost_${UPPERCOMPONENT}_LIBRARY_RELEASE-NOTFOUND" )
    set( Boost_${UPPERCOMPONENT}_LIBRARY_DEBUG "Boost_${UPPERCOMPONENT}_LIBRARY_DEBUG-NOTFOUND")

    # Support preference of static libs by adjusting CMAKE_FIND_LIBRARY_SUFFIXES
    if( Boost_USE_STATIC_LIBS )
      set( _boost_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
      if(WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
      else(WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
      endif(WIN32)
    endif( Boost_USE_STATIC_LIBS )

    find_library(Boost_${UPPERCOMPONENT}_LIBRARY_RELEASE
        NAMES  ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_COMPILER}${_boost_MULTITHREADED}-${Boost_LIB_VERSION}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_COMPILER}${_boost_MULTITHREADED}${_boost_STATIC_TAG}-${Boost_LIB_VERSION}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_MULTITHREADED}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_MULTITHREADED}${_boost_STATIC_TAG}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}
        HINTS  ${_boost_LIBRARIES_SEARCH_DIRS}
        PATHS  ${_boost_LIBRARIES_SEARCH_DIRS}
    )

    find_library(Boost_${UPPERCOMPONENT}_LIBRARY_DEBUG
        NAMES  ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_COMPILER}${_boost_MULTITHREADED}-${_boost_ABI_TAG}-${Boost_LIB_VERSION}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_COMPILER}${_boost_MULTITHREADED}${_boost_STATIC_TAG}${_boost_ABI_TAG}-${Boost_LIB_VERSION}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_MULTITHREADED}-${_boost_ABI_TAG}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_MULTITHREADED}${_boost_STATIC_TAG}${_boost_ABI_TAG}
               ${Boost_LIB_PREFIX}boost_${COMPONENT}-${_boost_ABI_TAG}
        HINTS  ${_boost_LIBRARIES_SEARCH_DIRS}
    )

    _Boost_ADJUST_LIB_VARS(${UPPERCOMPONENT})
    if( Boost_USE_STATIC_LIBS )
      set(CMAKE_FIND_LIBRARY_SUFFIXES ${_boost_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})
    endif( Boost_USE_STATIC_LIBS )
  endforeach(COMPONENT)
  # ------------------------------------------------------------------------
  #  End finding boost libraries
  # ------------------------------------------------------------------------

  set(Boost_INCLUDE_DIRS
    ${Boost_INCLUDE_DIR}
  )

  set(Boost_FOUND FALSE)
  if(Boost_INCLUDE_DIR)
    set( Boost_FOUND TRUE )

    # Check the version of Boost against the requested version.
    if (Boost_FIND_VERSION AND NOT Boost_FIND_VERSION_MINOR)
      message(SEND_ERROR "When requesting a specific version of Boost, you must provide at least the major and minor version numbers, e.g., 1.34")
    endif (Boost_FIND_VERSION AND NOT Boost_FIND_VERSION_MINOR)
    if(Boost_MAJOR_VERSION LESS "${Boost_FIND_VERSION_MAJOR}" )
      set( Boost_FOUND FALSE )
      set(_Boost_VERSION_AGE "old")
    elseif(Boost_MAJOR_VERSION EQUAL "${Boost_FIND_VERSION_MAJOR}" )
      if(Boost_MINOR_VERSION LESS "${Boost_FIND_VERSION_MINOR}" )
        set( Boost_FOUND FALSE )
        set(_Boost_VERSION_AGE "old")
      elseif(Boost_MINOR_VERSION EQUAL "${Boost_FIND_VERSION_MINOR}" )
        if( Boost_FIND_VERSION_PATCH AND Boost_SUBMINOR_VERSION LESS "${Boost_FIND_VERSION_PATCH}" )
          set( Boost_FOUND FALSE )
          set(_Boost_VERSION_AGE "old")
        endif( Boost_FIND_VERSION_PATCH AND Boost_SUBMINOR_VERSION LESS "${Boost_FIND_VERSION_PATCH}" )
      endif( Boost_MINOR_VERSION LESS "${Boost_FIND_VERSION_MINOR}" )
    endif( Boost_MAJOR_VERSION LESS "${Boost_FIND_VERSION_MAJOR}" )

    if (Boost_FOUND AND Boost_FIND_VERSION_EXACT)
      # If the user requested an exact version of Boost, check
      # that. We already know that the Boost version we have is >= the
      # requested version.
      set(_Boost_VERSION_AGE "new")

      # If the user didn't specify a patchlevel, it's 0.
      if (NOT Boost_FIND_VERSION_PATCH)
        set(Boost_FIND_VERSION_PATCH 0)
      endif (NOT Boost_FIND_VERSION_PATCH)

      # We'll set Boost_FOUND true again if we have an exact version match.
      set(Boost_FOUND FALSE)
      if(Boost_MAJOR_VERSION EQUAL "${Boost_FIND_VERSION_MAJOR}" )
        if(Boost_MINOR_VERSION EQUAL "${Boost_FIND_VERSION_MINOR}" )
          if(Boost_SUBMINOR_VERSION EQUAL "${Boost_FIND_VERSION_PATCH}" )
            set( Boost_FOUND TRUE )
          endif(Boost_SUBMINOR_VERSION EQUAL "${Boost_FIND_VERSION_PATCH}" )
        endif( Boost_MINOR_VERSION EQUAL "${Boost_FIND_VERSION_MINOR}" )
      endif( Boost_MAJOR_VERSION EQUAL "${Boost_FIND_VERSION_MAJOR}" )
    endif (Boost_FOUND AND Boost_FIND_VERSION_EXACT)

    if(NOT Boost_FOUND)
      # State that we found a version of Boost that is too new or too old.
      set(Boost_ERROR_REASON
        "${Boost_ERROR_REASON}\nDetected version of Boost is too ${_Boost_VERSION_AGE}. Requested version was ${Boost_FIND_VERSION_MAJOR}.${Boost_FIND_VERSION_MINOR}")
      if (Boost_FIND_VERSION_PATCH)
        set(Boost_ERROR_REASON
          "${Boost_ERROR_REASON}.${Boost_FIND_VERSION_PATCH}")
      endif (Boost_FIND_VERSION_PATCH)
      if (NOT Boost_FIND_VERSION_EXACT)
        set(Boost_ERROR_REASON "${Boost_ERROR_REASON} (or newer)")
      endif (NOT Boost_FIND_VERSION_EXACT)
      set(Boost_ERROR_REASON "${Boost_ERROR_REASON}.")
    endif (NOT Boost_FOUND)

    if (Boost_FOUND)
      set(_boost_CHECKED_COMPONENT FALSE)
      set(_Boost_MISSING_COMPONENTS)
      foreach(COMPONENT ${Boost_FIND_COMPONENTS})
        string(TOUPPER ${COMPONENT} COMPONENT)
        set(_boost_CHECKED_COMPONENT TRUE)
        if(NOT Boost_${COMPONENT}_FOUND)
          string(TOLOWER ${COMPONENT} COMPONENT)
          list(APPEND _Boost_MISSING_COMPONENTS ${COMPONENT})
          set( Boost_FOUND FALSE)
        endif(NOT Boost_${COMPONENT}_FOUND)
      endforeach(COMPONENT)
    endif (Boost_FOUND)

    if (_Boost_MISSING_COMPONENTS)
      # We were unable to find some libraries, so generate a sensible
      # error message that lists the libraries we were unable to find.
      set(Boost_ERROR_REASON
        "${Boost_ERROR_REASON}\nThe following Boost libraries could not be found:\n")
      foreach(COMPONENT ${_Boost_MISSING_COMPONENTS})
        set(Boost_ERROR_REASON
          "${Boost_ERROR_REASON}        boost_${COMPONENT}\n")
      endforeach(COMPONENT)

      list(LENGTH Boost_FIND_COMPONENTS Boost_NUM_COMPONENTS_WANTED)
      list(LENGTH _Boost_MISSING_COMPONENTS Boost_NUM_MISSING_COMPONENTS)
      if (${Boost_NUM_COMPONENTS_WANTED} EQUAL ${Boost_NUM_MISSING_COMPONENTS})
        set(Boost_ERROR_REASON
          "${Boost_ERROR_REASON}No Boost libraries were found. You may need to set Boost_LIBRARYDIR to the directory containing Boost libraries or BOOST_ROOT to the location of Boost.")
      else (${Boost_NUM_COMPONENTS_WANTED} EQUAL ${Boost_NUM_MISSING_COMPONENTS})
        set(Boost_ERROR_REASON
          "${Boost_ERROR_REASON}Some (but not all) of the required Boost libraries were found. You may need to install these additional Boost libraries. Alternatively, set Boost_LIBRARYDIR to the directory containing Boost libraries or BOOST_ROOT to the location of Boost.")
      endif (${Boost_NUM_COMPONENTS_WANTED} EQUAL ${Boost_NUM_MISSING_COMPONENTS})
    endif (_Boost_MISSING_COMPONENTS)

    if( NOT Boost_LIBRARY_DIRS AND NOT _boost_CHECKED_COMPONENT )
      # Compatibility Code for backwards compatibility with CMake
      # 2.4's FindBoost module.

      # Look for the boost library path.
      # Note that the user may not have installed any libraries
      # so it is quite possible the Boost_LIBRARY_PATH may not exist.
      set(_boost_LIB_DIR ${Boost_INCLUDE_DIR})

      if("${_boost_LIB_DIR}" MATCHES "boost-[0-9]+")
        get_filename_component(_boost_LIB_DIR ${_boost_LIB_DIR} PATH)
      endif("${_boost_LIB_DIR}" MATCHES "boost-[0-9]+")

      if("${_boost_LIB_DIR}" MATCHES "/include$")
        # Strip off the trailing "/include" in the path.
        get_filename_component(_boost_LIB_DIR ${_boost_LIB_DIR} PATH)
      endif("${_boost_LIB_DIR}" MATCHES "/include$")

      if(EXISTS "${_boost_LIB_DIR}/lib")
        set(_boost_LIB_DIR ${_boost_LIB_DIR}/lib)
      else(EXISTS "${_boost_LIB_DIR}/lib")
        if(EXISTS "${_boost_LIB_DIR}/stage/lib")
          set(_boost_LIB_DIR ${_boost_LIB_DIR}/stage/lib)
        else(EXISTS "${_boost_LIB_DIR}/stage/lib")
          set(_boost_LIB_DIR "")
        endif(EXISTS "${_boost_LIB_DIR}/stage/lib")
      endif(EXISTS "${_boost_LIB_DIR}/lib")

      if(_boost_LIB_DIR AND EXISTS "${_boost_LIB_DIR}")
        set(Boost_LIBRARY_DIRS ${_boost_LIB_DIR} CACHE FILEPATH "Boost library directory")
      endif(_boost_LIB_DIR AND EXISTS "${_boost_LIB_DIR}")

    endif( NOT Boost_LIBRARY_DIRS AND NOT _boost_CHECKED_COMPONENT )

  else(Boost_INCLUDE_DIR)
    set( Boost_FOUND FALSE)
  endif(Boost_INCLUDE_DIR)

  if(Boost_FOUND)
      if(NOT Boost_FIND_QUIETLY)
        message(STATUS "Boost version: ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
      endif(NOT Boost_FIND_QUIETLY)
      if(NOT Boost_FIND_QUIETLY)
        message(STATUS "Found the following Boost libraries:")
      endif(NOT Boost_FIND_QUIETLY)
      foreach( COMPONENT  ${Boost_FIND_COMPONENTS} )
        string( TOUPPER ${COMPONENT} UPPERCOMPONENT )
        if( Boost_${UPPERCOMPONENT}_FOUND )
          if(NOT Boost_FIND_QUIETLY)
            message(STATUS "  ${COMPONENT}")
          endif(NOT Boost_FIND_QUIETLY)
          set(Boost_LIBRARIES ${Boost_LIBRARIES} ${Boost_${UPPERCOMPONENT}_LIBRARY})
        endif( Boost_${UPPERCOMPONENT}_FOUND )
      endforeach(COMPONENT)
  else(Boost_FOUND)
      if(Boost_FIND_REQUIRED)
        message(SEND_ERROR "Unable to find the requested Boost libraries.\n${Boost_ERROR_REASON}")
      endif(Boost_FIND_REQUIRED)
  endif(Boost_FOUND)

  # Under Windows, automatic linking is performed, so no need to specify the libraries.
  if(WIN32)
    if(NOT MINGW)
      set(Boost_LIBRARIES "")
    endif(NOT MINGW)
  endif(WIN32)

  # show the Boost_INCLUDE_DIRS AND Boost_LIBRARIES variables only in the advanced view
  mark_as_advanced(Boost_INCLUDE_DIR
      Boost_INCLUDE_DIRS
      Boost_LIBRARY_DIRS
      Boost_USE_MULTITHREADED
  )
endif(_boost_IN_CACHE)

