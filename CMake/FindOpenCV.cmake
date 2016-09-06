find_path( 
cv_INCLUDE_DIR 
opencv/cv.h 
DOC "The directory where opencv/cv.h is installed")

find_path( 
opencv2_INCLUDE_DIR 
opencv2/opencv.hpp 
DOC "The directory where opencv2/opencv.hpp is installed")

set(opencv_core_NAMES opencv_core)
set(opencv_ml_NAMES opencv_ml)

if ( cv_INCLUDE_DIR AND opencv2_INCLUDE_DIR )
	set(OPENCV_INCLUDE_DIRS "${cv_INCLUDE_DIR};${opencv2_INCLUDE_DIR}")
	list(REMOVE_DUPLICATES OPENCV_INCLUDE_DIRS)
	
	if(NOT OpenCV_VERSION)
		file(READ "${opencv2_INCLUDE_DIR}/opencv2/core/version.hpp" _header_content)
		string(REGEX REPLACE ".*# *define +CV_VERSION_EPOCH +([0-9]+).*" "\\1" 
		OpenCV_VERSION_MAJOR ${_header_content})
		string(REGEX REPLACE ".*# *define +CV_VERSION_MAJOR +([0-9]+).*" "\\1" 
		OpenCV_VERSION_MINOR ${_header_content})
		string(REGEX REPLACE ".*# *define +CV_VERSION_MINOR +([0-9]+).*" "\\1" 
		OpenCV_VERSION_PATCH ${_header_content})
		string(REGEX REPLACE ".*# *define +CV_VERSION_REVISION +([0-9]+).*" "\\1" 
		OpenCV_VERSION_TWEAK ${_header_content})
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
DOC "Path to opencv_core library")

find_library(
OPENCV_ml_LIBRARY
NAMES ${opencv_ml_NAMES}
DOC "Path to opencv_ml library")

set(OpenCV_FOUND FALSE)
if ( OPENCV_core_LIBRARY AND OPENCV_ml_LIBRARY )
	set(OPENCV_LIBRARIES "${OPENCV_core_LIBRARY};${OPENCV_ml_LIBRARY}")
endif ()

if( OPENCV_INCLUDE_DIRS AND OPENCV_LIBRARIES )
	set(OpenCV_FOUND TRUE)
endif()

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OPENCV 
  REQUIRED_VARS OPENCV_core_LIBRARY OPENCV_INCLUDE_DIRS
  VERSION_VAR OpenCV_VERSION)
