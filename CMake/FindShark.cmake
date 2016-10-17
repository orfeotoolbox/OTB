# - Find Shark
# Find the Shark includes and library
#
#   SHARK_FOUND       - True if Shark found.
#   SHARK_INCLUDE_DIR - where to find Core/shark.h
#   SHARK_LIBRARIES   - List of libraries required for shark.
#

if( SHARK_INCLUDE_DIR )
    # Already in cache, be silent
   set( Shark_FIND_QUIETLY TRUE )
endif()

find_path( SHARK_INCLUDE_DIR shark/Core/shark.h PATH_SUFFIXES shark )

find_library( SHARK_LIBRARY NAMES shark )

mark_as_advanced( SHARK_INCLUDE_DIR
                  SHARK_LIBRARY )
find_package( 
	Boost 1.48.0 REQUIRED QUIET COMPONENTS
	system date_time filesystem
	program_options serialization thread
	unit_test_framework
)

if(NOT Boost_FOUND)
	message(FATAL_ERROR "Please make sure Boost 1.48.0 is installed on your system")
endif()

find_file(SHARK_CONFIG_FILE SharkConfig.cmake PATH_SUFFIXES lib/cmake/Shark share/Shark)
if(SHARK_CONFIG_FILE)
  file(STRINGS "${SHARK_CONFIG_FILE}" SHARK_CONFIG_FILE_CONTENTS)
  string(REGEX REPLACE
      ".*set.*\\(SHARK_VERSION_MAJOR..([0-9]+)\"\\).*" "\\1"
       SHARK_VERSION_MAJOR "${SHARK_CONFIG_FILE_CONTENTS}")
  string(REGEX REPLACE
      ".*set.*\\(SHARK_VERSION_MINOR..([0-9]+)\"\\).*" "\\1"
       SHARK_VERSION_MINOR "${SHARK_CONFIG_FILE_CONTENTS}")
  string(REGEX REPLACE
      ".*set.*\\(SHARK_VERSION_PATCH..([0-9]+)\"\\).*" "\\1"
       SHARK_VERSION_PATCH "${SHARK_CONFIG_FILE_CONTENTS}")       

  set(SHARK_VERSION_STRING 
  "${SHARK_VERSION_MAJOR}.${SHARK_VERSION_MINOR}.${SHARK_VERSION_PATCH}")

endif()

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Shark
                                  REQUIRED_VARS SHARK_LIBRARY SHARK_INCLUDE_DIR
                                  VERSION_VAR SHARK_VERSION_STRING)
if(SHARK_FOUND)
  set(SHARK_INCLUDE_DIRS ${SHARK_INCLUDE_DIR} ${Boost_INCLUDE_DIR} )
  set(SHARK_LIBRARIES ${SHARK_LIBRARY} ${Boost_LIBRARIES} )
endif()
