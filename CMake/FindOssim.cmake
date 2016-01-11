# - Find Ossim
# Find the native Ossim includes and library
#
#   OSSIM_FOUND        - True if Ossim found.
#   OSSIM_INCLUDE_DIRS - where to find ossim/init/ossimInit.h, etc.
#   OSSIM_LIBRARIES    - List of libraries when using Ossim.
#

if( OSSIM_INCLUDE_DIR )
    # Already in cache, be silent
    set( Ossim_FIND_QUIETLY TRUE )
endif()

find_path( OSSIM_INCLUDE_DIR
           NAMES ossim/init/ossimInit.h )

if(EXISTS "${OSSIM_INCLUDE_DIR}/ossim/ossimVersion.h")
  file(READ "${OSSIM_INCLUDE_DIR}/ossim/ossimVersion.h" _ossim_version_h_CONTENTS)
  string(REGEX REPLACE ".*# *define OSSIM_VERSION *\"([0-9.]+)\".*" "\\1" OSSIM_VERSION "${_ossim_version_h_CONTENTS}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" OSSIM_MAJOR_VERSION_NUMBER "${OSSIM_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" OSSIM_MINOR_VERSION_NUMBER "${OSSIM_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" OSSIM_PATCH_VERSION_NUMBER "${OSSIM_VERSION}")
  math(EXPR OSSIM_VERSION_NUMBER
    "((${OSSIM_MAJOR_VERSION_NUMBER})*100+${OSSIM_MINOR_VERSION_NUMBER})*100+${OSSIM_PATCH_VERSION_NUMBER}")
else()
  if(NOT Ossim_FIND_QUIETLY)
  message(WARNING "ossimVersion.h not found !")
  endif()
endif()

# Parse a certain header to check if SVN revision is 23537
if(EXISTS "${OSSIM_INCLUDE_DIR}/ossim/imaging/ossimImageRenderer.h")
  file(STRINGS "${OSSIM_INCLUDE_DIR}/ossim/imaging/ossimImageRenderer.h" _ossim_image_renderer_h_CONTENTS REGEX "^// \\$Id: ossimImageRenderer\\.h [0-9.]+ ")
  string(REGEX REPLACE ".*ossimImageRenderer\\.h ([0-9.]+) .*" "\\1" _ossim_svn_revision_hint "${_ossim_image_renderer_h_CONTENTS}")

  if("${_ossim_svn_revision_hint}" LESS "23537")
    message(WARNING "The OSSIM include directory detected by OTB is: '${OSSIM_INCLUDE_DIR}'."
      "This version does not have support for SENTINEL-1 products."
      "Opening an image will result in immediate crash!. "
      "Please consider updating your ossim to latest stable release.(1.8.20-3)")
  else()
    if("${_ossim_svn_revision_hint}" LESS "23664")
      message(WARNING "The OSSIM include directory detected by OTB is '${OSSIM_INCLUDE_DIR}'."
        "This version has a critical bug inside ossimRpcModel."
        "When an input point outside image extent is given the model returns 'NaN'."
        "Please consider updating your ossim to latest stable release.(1.8.20-3)")
    endif()
  endif()
else()
  if(NOT Ossim_FIND_QUIETLY)
    message(WARNING "ossimImageRenderer.h not found !")
  endif()
endif()

find_library(OSSIM_LIBRARY
             NAMES ossim)

# handle the QUIETLY and REQUIRED arguments and set OSSIM_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Ossim DEFAULT_MSG OSSIM_LIBRARY OSSIM_INCLUDE_DIR )

mark_as_advanced( OSSIM_INCLUDE_DIR OSSIM_LIBRARY )

set(OSSIM_LIBRARIES "${OSSIM_LIBRARY};${OPENTHREADS_LIBRARY}")
set(OSSIM_INCLUDE_DIRS ${OSSIM_INCLUDE_DIR})
