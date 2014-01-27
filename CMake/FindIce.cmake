# - Find Ice
#
# Find the native ICE headers and libraries.
#
#  ICE_INCLUDE_DIRS - where to find ice.h, etc.
#  ICE_LIBRARIES    - List of libraries when using ice.
#  ICE_FOUND        - True if ice found.

#=============================================================================
# TODO: Add Copyright notice here.
#=============================================================================

# Look for the header file.
find_path(ICE_INCLUDE_DIR
  NAMES otbGlView.h
  PATH_SUFFIXES otb
)

# Look for the library.
find_library(ICE_LIBRARY
  NAMES OTBIce
  PATH_SUFFIXES otb
)

# handle the QUIETLY and REQUIRED arguments and set ICE_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ICE DEFAULT_MSG ICE_LIBRARY ICE_INCLUDE_DIR)

# Copy the results to the output variables.
if(ICE_FOUND)
  set(ICE_LIBRARIES ${ICE_LIBRARY})
  set(ICE_INCLUDE_DIRS ${ICE_INCLUDE_DIR})
endif()

mark_as_advanced(ICE_INCLUDE_DIR ICE_LIBRARY)
