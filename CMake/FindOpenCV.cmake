set(OPENCV_SEARCH_PATH)

if(OpenCV_DIR)
  get_filename_component(OPENCV_SEARCH_PATH "${OpenCV_DIR}" PATH)
  if(OPENCV_SEARCH_PATH)
    get_filename_component(OPENCV_SEARCH_PATH "${OPENCV_SEARCH_PATH}" PATH)
  endif()
  if(OPENCV_SEARCH_PATH)
    find_path(
      opencv_INCLUDE_DIR
      opencv/cv.h
      PATHS "${OPENCV_SEARCH_PATH}"
      PATH_SUFFIXES "include"
      DOC "The directory where opencv/cv.h is installed")
  endif()  
endif()

if(NOT opencv_INCLUDE_DIR)
  find_path(
    opencv_INCLUDE_DIR
    opencv/cv.h
    PATHS "${OpenCV_DIR}"
    PATH_SUFFIXES "include"
    DOC "The directory where opencv/cv.h is installed")
endif()

if(NOT EXISTS ${opencv_INCLUDE_DIR}/opencv2/opencv.hpp)
  message(FATAL_ERROR "${opencv_INCLUDE_DIR}/opencv2/opencv.hpp does not exists. "
    "Make sure you have opencv 2.3 or higher. We had searched in ${OPENCV_SEARCH_PATHS}")
endif()

set(opencv_core_NAMES opencv_core)
set(opencv_ml_NAMES opencv_ml)

if ( opencv_INCLUDE_DIR )

  set(OPENCV_INCLUDE_DIRS "${opencv_INCLUDE_DIR}")
  
  if(NOT OpenCV_VERSION)
    file(READ "${opencv_INCLUDE_DIR}/opencv2/core/version.hpp" _header_content)

    string(REGEX MATCH  ".*# *define +CV_VERSION_EPOCH +([0-9]+).*" matched ${_header_content})
    if( matched)
      string(REGEX REPLACE ".*# *define +CV_VERSION_EPOCH +([0-9]+).*" "\\1"
        OpenCV_VERSION_MAJOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_MAJOR +([0-9]+).*" "\\1"
        OpenCV_VERSION_MINOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_MINOR +([0-9]+).*" "\\1"
        OpenCV_VERSION_PATCH ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_REVISION +([0-9]+).*" "\\1"
        OpenCV_VERSION_TWEAK ${_header_content})
    else()
      #for opencv 2.3.x
      string(REGEX REPLACE ".*# *define +CV_MAJOR_VERSION +([0-9]+).*" "\\1"
        OpenCV_VERSION_MAJOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_MINOR_VERSION +([0-9]+).*" "\\1"
        OpenCV_VERSION_MINOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_SUBMINOR_VERSION +([0-9]+).*" "\\1"
        OpenCV_VERSION_PATCH ${_header_content})
      set(OpenCV_VERSION_TWEAK)
    endif()     
    set(OpenCV_VERSION
      "${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}")
  endif()
  
  if(WIN32)
    list(APPEND opencv_core_NAMES
      "opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
    list(APPEND opencv_ml_NAMES
      "opencv_ml${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
  endif()
endif()


# Prefer the static library.
find_library(
OPENCV_core_LIBRARY
NAMES ${opencv_core_NAMES}
PATHS ${OPENCV_SEARCH_PATH}
PATH_SUFFIXES "lib" "lib64" "lib/x86_64-linux-gnu"
NO_DEFAULT_PATH
DOC "Path to opencv_core library")

find_library(
OPENCV_ml_LIBRARY
NAMES ${opencv_ml_NAMES}
PATHS ${OPENCV_SEARCH_PATH}
PATH_SUFFIXES "lib" "lib64" "lib/x86_64-linux-gnu"
NO_DEFAULT_PATH
DOC "Path to opencv_ml library")

set(OpenCV_FOUND FALSE)
if ( OPENCV_core_LIBRARY AND OPENCV_ml_LIBRARY )
  set(OPENCV_LIBRARIES "${OPENCV_core_LIBRARY};${OPENCV_ml_LIBRARY}")
endif ()

if( OPENCV_INCLUDE_DIRS AND OPENCV_LIBRARIES )
  set(OpenCV_FOUND TRUE)
  set(OPENCV_VERSION ${OpenCV_VERSION}) #for compatility
endif()

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenCV
  REQUIRED_VARS OPENCV_core_LIBRARY OPENCV_INCLUDE_DIRS
  VERSION_VAR OpenCV_VERSION)
