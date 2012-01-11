# $Id$

# Always add the Resolved Fa to be compiled and tested
add_subdirectory(Resolved)

# Add the failing bugs
IF(BUILD_BUG_TRACKER_TESTS)
  add_subdirectory(Failing)
ENDIF(BUILD_BUG_TRACKER_TESTS)
