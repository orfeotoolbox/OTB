# - Find GETTEXT
# Find the native GETTEXT includes and library
#
#   GETTEXT_FOUND        - True if GETTEXT found.
#   GETTEXT_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   GETTEXT_LIBRARIES    - List of libraries when using GETTEXT.
#

if( GETTEXT_INCLUDE_DIR )
    # Already in cache, be silent
    set( GETTEXT_FIND_QUIETLY TRUE )
endif()

find_path( GETTEXT_INCLUDE_DIR libintl.h )

find_library( GETTEXT_LIBRARY
              NAMES GETTEXT )

# handle the QUIETLY and REQUIRED arguments and set GETTEXT_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GETTEXT DEFAULT_MSG GETTEXT_LIBRARY GETTEXT_INCLUDE_DIR  )

mark_as_advanced( GETTEXT_INCLUDE_DIR GETTEXT_LIBRARY )

if(GETTEXT_FOUND)
  set(GETTEXT_INCLUDE_DIRS ${GETTEXT_INCLUDE_DIR})
  set(GETTEXT_LIBRARIES ${GETTEXT_LIBRARY})
else()
  set(GETTEXT_INCLUDE_DIRS)
  set(GETTEXT_LIBRARIES)
endif()

