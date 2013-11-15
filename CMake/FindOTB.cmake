# - Find an OTB installation or build tree.

# When OTB is found, the OTBConfig.cmake file is sourced to setup the
# location and configuration of OTB.  Please read this file, or
# OTBConfig.cmake.in from the OTB source tree for the full list of
# definitions.  Of particular interest is OTB_USE_FILE, a CMake source file
# that can be included to set the include directories, library directories,
# and preprocessor macros.  In addition to the variables read from
# OTBConfig.cmake, this find module also defines
#  OTB_DIR  - The directory containing OTBConfig.cmake.
#             This is either the root of the build tree,
#             or the lib/otb directory.
#             This is the only cache entry.
#
#  OTB_FOUND - Whether OTB was found.  If this is true,
#              OTB_DIR is okay.
#
#  USE_OTB_FILE - The full path to the UseOTB.cmake file.
#                 This is provided for backward
#                 compatability.  Use OTB_USE_FILE
#                 instead.

# This is adapted from the FindITK.cmake distributed with cmake
# WARNING: the adaptation is not fully tested yet and needs some rework

set(OTB_DIR_STRING "directory containing OTBConfig.cmake.  This is either the root of the build tree, or PREFIX/lib/otb for an installation.")

# Search only if the location is not already known.
if(NOT OTB_DIR)
  # Get the system search path as a list.
  if(UNIX)
    string(REGEX MATCHALL "[^:]+" OTB_DIR_SEARCH1 "$ENV{PATH}")
  else(UNIX)
    string(REGEX REPLACE "\\\\" "/" OTB_DIR_SEARCH1 "$ENV{PATH}")
  endif(UNIX)
  string(REGEX REPLACE "/;" ";" OTB_DIR_SEARCH2 ${OTB_DIR_SEARCH1})

  # Construct a set of paths relative to the system search path.
  set(OTB_DIR_SEARCH "")
  foreach(dir ${OTB_DIR_SEARCH2})
    set(OTB_DIR_SEARCH ${OTB_DIR_SEARCH} "${dir}/../lib/otb")
  endforeach(dir)

  #
  # Look for an installation or build tree.
  #
  find_path(OTB_DIR OTBConfig.cmake
    # Look for an environment variable OTB_DIR.
    $ENV{OTB_DIR}

    # Look in places relative to the system executable search path.
    ${OTB_DIR_SEARCH}

    # Look in standard UNIX install locations.
    /usr/local/lib/otb
    /usr/lib/otb

    # Read from the CMakeSetup registry entries.  It is likely that
    # OTB will have been recently built.
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild1]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild2]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild3]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild4]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild5]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild6]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild7]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild8]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild9]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild10]

    # Help the user find it if we cannot.
    DOC "The ${OTB_DIR_STRING}"
  )
endif(NOT OTB_DIR)

# If OTB was found, load the configuration file to get the rest of the
# settings.
if(OTB_DIR)
  set(OTB_FOUND 1)
  include(${OTB_DIR}/OTBConfig.cmake)

  # Set USE_OTB_FILE for backward-compatability.
  set(USE_OTB_FILE ${OTB_USE_FILE})
else(OTB_DIR)
  set(OTB_FOUND 0)
  if(OTB_FIND_REQUIRED)
    message(FATAL_ERROR "Please set OTB_DIR to the ${OTB_DIR_STRING}")
  endif(OTB_FIND_REQUIRED)
endif(OTB_DIR)
