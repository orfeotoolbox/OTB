# Find the Google Benchmark library
# Defines:
#  GBENCHMARK_INCLUDE_DIR  - gbenchmark include directory
#  GBENCHMARK_LIBRARY      - gbenchmark library file
#  GBENCHMARK_FOUND        - TRUE if gbenchmark is found

if (GBENCHMARK_INCLUDE_DIR)
 #check cache
  set(GBENCHMARK_FIND_QUIETLY TRUE)
endif ()

if (NOT GBENCHMARK_INCLUDE_DIR)
  find_path(GBENCHMARK_INCLUDE_DIR NAMES benchmark.h PATH_SUFFIXES benchmark)
  set(GBENCHMARK_INCLUDE_DIR ${GBENCHMARK_INCLUDE_DIR}/benchmark CACHE PATH "Google.benchmark include directory")
endif ()

mark_as_advanced(GBENCHMARK_INCLUDE_DIR)

find_library(GBENCHMARK_LIBRARY NAMES benchmark)
mark_as_advanced(GBENCHMARK_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GBENCHMARK
  REQUIRED_VARS GBENCHMARK_LIBRARY GBENCHMARK_INCLUDE_DIR
  )
#VERSION_VAR GBENCHMARK_VERSION_STRING

set(GBENCHMARK_LIBRARIES ${GBENCHMARK_LIBRARY})
set(GBENCHMARK_INCLUDE_DIRS ${GBENCHMARK_INCLUDE_DIR})
