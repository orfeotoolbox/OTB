# Find the AntiGrain Geometry graphical toolkit header files
#
#   AGG_FOUND        - True if AGG found.
#   AGG_INCLUDE_DIRS - where to find agg_pixfmt_rgba.h, etc.
#

if( AGG_INCLUDE_DIR )
    # Already in cache, be silent
    set( AGG_FIND_QUIETLY TRUE )
endif()

find_path( AGG_INCLUDE_DIR agg_pixfmt_rgba.h
  PATH_SUFFIXES agg2
  )

# handle the QUIETLY and REQUIRED arguments and set AGG_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Agg DEFAULT_MSG AGG_INCLUDE_DIR )

mark_as_advanced( AGG_INCLUDE_DIR )

if(AGG_FOUND)
  set(AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIR})
else()
  set(AGG_INCLUDE_DIRS)
endif()

