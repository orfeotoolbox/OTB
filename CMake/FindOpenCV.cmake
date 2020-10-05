#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set(OPENCV_SEARCH_PATH)

if(OpenCV_DIR)
  get_filename_component(OPENCV_SEARCH_PATH "${OpenCV_DIR}" REALPATH)
  if(OPENCV_SEARCH_PATH)
    get_filename_component(OPENCV_SEARCH_PATH "${OPENCV_SEARCH_PATH}" REALPATH)
  endif()
  if(OPENCV_SEARCH_PATH)
    find_path(
      opencv_INCLUDE_DIR
      opencv2/opencv.hpp
      PATHS "${OPENCV_SEARCH_PATH}"
      #no additional paths are added to the search if OpenCV_DIR
      NO_DEFAULT_PATH
      PATH_SUFFIXES "include" "include/opencv4"
      DOC "The directory where opencv/cv.h is installed")
  endif()
endif()

if(NOT opencv_INCLUDE_DIR)
  find_path(
    opencv_INCLUDE_DIR
    opencv2/opencv.hpp
    PATHS "${OpenCV_DIR}"
    PATH_SUFFIXES "include" "include/opencv4"
    DOC "The directory where opencv/cv.h is installed")
endif()

set(opencv_core_NAMES opencv_core)
set(opencv_ml_NAMES opencv_ml)

if (opencv_INCLUDE_DIR)

  set(OPENCV_INCLUDE_DIRS "${opencv_INCLUDE_DIR}")

  if(NOT OpenCV_VERSION)
    file(READ "${opencv_INCLUDE_DIR}/opencv2/core/version.hpp" _header_content)

    # detect the type of version file (2.3.x , 2.4.x, 3.x or 4.x)
    string(REGEX MATCH  ".*# *define +CV_VERSION_EPOCH +([0-9]+).*" has_epoch ${_header_content})
    string(REGEX MATCH  ".*# *define +CV_MAJOR_VERSION +([0-9]+).*" has_old_major ${_header_content})
    string(REGEX MATCH  ".*# *define +CV_MINOR_VERSION +([0-9]+).*" has_old_minor ${_header_content})
    string(REGEX MATCH  ".*# *define +CV_SUBMINOR_VERSION +([0-9]+).*" has_old_subminor ${_header_content})

    if(has_old_major AND has_old_minor AND has_old_subminor)
      #for opencv 2.3.x
      string(REGEX REPLACE ".*# *define +CV_MAJOR_VERSION +([0-9]+).*" "\\1"
        OpenCV_VERSION_MAJOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_MINOR_VERSION +([0-9]+).*" "\\1"
        OpenCV_VERSION_MINOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_SUBMINOR_VERSION +([0-9]+).*" "\\1"
        OpenCV_VERSION_PATCH ${_header_content})
      set(OpenCV_VERSION_TWEAK)
    elseif(has_epoch)
      # for opencv 2.4.x
      string(REGEX REPLACE ".*# *define +CV_VERSION_EPOCH +([0-9]+).*" "\\1"
        OpenCV_VERSION_MAJOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_MAJOR +([0-9]+).*" "\\1"
        OpenCV_VERSION_MINOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_MINOR +([0-9]+).*" "\\1"
        OpenCV_VERSION_PATCH ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_REVISION +([0-9]+).*" "\\1"
        OpenCV_VERSION_TWEAK ${_header_content})
    else()
      string(REGEX REPLACE ".*# *define +CV_VERSION_MAJOR +([0-9]+).*" "\\1"
        OpenCV_VERSION_MAJOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_MINOR +([0-9]+).*" "\\1"
        OpenCV_VERSION_MINOR ${_header_content})
      string(REGEX REPLACE ".*# *define +CV_VERSION_REVISION +([0-9]+).*" "\\1"
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

if(NOT OPENCV_SEARCH_PATH)
  get_filename_component(OPENCV_SEARCH_PATH "${opencv_INCLUDE_DIR}" PATH)
  # include dir is include/opencv4 in v4 UNIX
  if(UNIX AND OpenCV_VERSION_MAJOR EQUAL 4)
    get_filename_component(OPENCV_SEARCH_PATH "${OPENCV_SEARCH_PATH}" PATH)
  endif()
endif()

# Prefer the static library.
find_library(
  OPENCV_core_LIBRARY
  NAMES ${opencv_core_NAMES}
  PATHS ${OPENCV_SEARCH_PATH}
  PATH_SUFFIXES "lib" "lib64" "lib/${CMAKE_LIBRARY_ARCHITECTURE}"
  NO_DEFAULT_PATH
  DOC "Path to opencv_core library")

find_library(
  OPENCV_ml_LIBRARY
  NAMES ${opencv_ml_NAMES}
  PATHS ${OPENCV_SEARCH_PATH}
  PATH_SUFFIXES "lib" "lib64" "lib/${CMAKE_LIBRARY_ARCHITECTURE}"
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
find_package_handle_standard_args(OpenCV
  REQUIRED_VARS OPENCV_core_LIBRARY OPENCV_INCLUDE_DIRS
  VERSION_VAR OpenCV_VERSION)
