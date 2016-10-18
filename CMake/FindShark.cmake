# - Find Shark
# Find the Shark includes and library
#
#   SHARK_FOUND       - True if Shark found.
#   SHARK_INCLUDE_DIR - where to find Core/shark.h
#   SHARK_LIBRARIES   - List of libraries required for shark.
#

set(SHARK_SEARCH_PATH)

set(SHARK_CONFIG_FILE)

if(Shark_DIR)
  get_filename_component(SHARK_SEARCH_PATH "${Shark_DIR}" PATH)
  get_filename_component(SHARK_SEARCH_PATH "${SHARK_SEARCH_PATH}" PATH)
  get_filename_component(SHARK_SEARCH_PATH "${SHARK_SEARCH_PATH}" PATH)

  if(EXISTS "${Shark_DIR}/SharkConfig.cmake")
    set(SHARK_CONFIG_FILE ${Shark_DIR}/SharkConfig.cmake)
  endif()

endif() #if(Shark_DIR)

find_path( SHARK_INCLUDE_DIR shark/Core/Shark.h
  PATHS "${SHARK_SEARCH_PATH}"
  PATH_SUFFIXES include include/shark shark)

find_library( SHARK_LIBRARY
  NAMES shark
  PATHS "${SHARK_SEARCH_PATH}"
  PATH_SUFFIXES lib
  )

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

if(NOT SHARK_CONFIG_FILE)
  find_file(SHARK_CONFIG_FILE SharkConfig.cmake PATH_SUFFIXES lib/cmake/Shark share/Shark)
endif()
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
