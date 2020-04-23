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

INCLUDE_ONCE_MACRO(OPENCV)

SETUP_SUPERBUILD(OPENCV)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(OPENCV ZLIB TIFF PNG)

ADD_SUPERBUILD_CMAKE_VAR(OPENCV ZLIB_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(OPENCV ZLIB_LIBRARY)
ADD_SUPERBUILD_CMAKE_VAR(OPENCV TIFF_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(OPENCV TIFF_LIBRARY)
ADD_SUPERBUILD_CMAKE_VAR(OPENCV PNG_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(OPENCV PNG_LIBRARY)

ExternalProject_Add(OPENCV
  PREFIX OPENCV
  URL "https://github.com/opencv/opencv/archive/4.1.1.tar.gz"
  URL_MD5 d9d3138d12e52bb0684f4a5862f6a46c
  BINARY_DIR ${OPENCV_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_DOCS:BOOL=OFF
  -DBUILD_EXAMPLES:BOOL=OFF
  -DBUILD_JASPER:BOOL=OFF
  -DWITH_JASPER:BOOL=OFF
  -DBUILD_JPEG:BOOL=OFF
  -DWITH_JPEG:BOOL=OFF
  -DWITH_FFMPEG:BOOL=OFF
  -DWITH_VFW:BOOL=OFF
  -DBUILD_OPENEXR:BOOL=OFF
  -DBUILD_PACKAGE:BOOL=OFF
  -DBUILD_PERF_TESTS:BOOL=OFF
  -DBUILD_PNG:BOOL=OFF
  -DBUILD_TBB:BOOL=OFF
  -DBUILD_TESTS:BOOL=OFF
  -DBUILD_TIFF:BOOL=OFF
  -DBUILD_ZLIB:BOOL=OFF
  -DWITH_CUDA:BOOL=OFF
  -DWITH_OPENCL:BOOL=OFF
  -DWITH_CUFFT:BOOL=OFF
  -DWITH_GIGEAPI:BOOL=OFF
  -DWITH_GSTREAMER:BOOL=OFF
  -DWITH_GTK:BOOL=OFF
  -DWITH_OPENCLAMDBLAS:BOOL=OFF
  -DWITH_OPENCLAMDFFT:BOOL=OFF
  -DWITH_OPENEXR:BOOL=OFF
  -DWITH_PVAPI:BOOL=OFF
  -DWITH_QT:BOOL=OFF
  -DWITH_UNICAP:BOOL=OFF
  -DWITH_LIBV4L:BOOL=OFF
  -DWITH_V4L:BOOL=OFF
  -DWITH_VTK:BOOL=OFF
  -DWITH_XIMEA:BOOL=OFF
  -DWITH_XINE:BOOL=OFF
  -DBUILD_opencv_apps:BOOL=OFF
  -DBUILD_opencv_calib3d:BOOL=OFF
  -DBUILD_opencv_contrib:BOOL=OFF
  -DBUILD_opencv_core:BOOL=ON
  -DBUILD_opencv_features2d:BOOL=OFF
  -DBUILD_opencv_flann:BOOL=OFF
  -DBUILD_opencv_gpu:BOOL=OFF
  -DBUILD_opencv_highgui:BOOL=OFF
  -DBUILD_opencv_imgproc:BOOL=OFF
  -DBUILD_opencv_java:BOOL=OFF
  -DBUILD_opencv_legacy:BOOL=OFF
  -DBUILD_opencv_ml:BOOL=ON
  -DBUILD_opencv_nonfree:BOOL=OFF
  -DBUILD_opencv_objdetect:BOOL=OFF
  -DBUILD_opencv_ocl:BOOL=OFF
  -DBUILD_opencv_photo:BOOL=OFF
  -DBUILD_opencv_python:BOOL=OFF
  -DBUILD_opencv_stitching:BOOL=OFF
  -DBUILD_opencv_superres:BOOL=OFF
  -DBUILD_opencv_ts:BOOL=OFF
  -DBUILD_opencv_video:BOOL=OFF
  -DBUILD_opencv_videostab:BOOL=OFF
  -DBUILD_opencv_world:BOOL=OFF
  -DWITH_CUBLAS:BOOL=OFF
  -DWITH_DIRECTX:BOOL=OFF
  -DWITH_DSHOW:BOOL=OFF
  -DWITH_EIGEN:BOOL=OFF
  -DWITH_IPP:BOOL=OFF
  -DWITH_ITT:BOOL=OFF
  -DWITH_LAPACK:BOOL=OFF
  -DWITH_MATLAB:BOOL=OFF
  -DWITH_NVCUVID:BOOL=OFF
  -DWITH_WEBP:BOOL=OFF
  -DWITH_WIN32UI:BOOL=OFF
  -DBUILD_PROTOBUF:BOOL=OFF
  -DBUILD_WEBP:BOOL=OFF
  -DBUILD_opencv_java_bindings_generator:BOOL=OFF
  -DBUILD_opencv_python_bindings_generator:BOOL=OFF
  -DBUILD_opencv_python3:BOOL=OFF
  -DBUILD_opencv_python2:BOOL=OFF
  -DCMAKE_INSTALL_LIBDIR:PATH=lib
  ${OPENCV_SB_CONFIG}
  DEPENDS ${OPENCV_DEPENDENCIES}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(OPENCV)

set(_SB_OpenCV_DIR ${SB_INSTALL_PREFIX}/share/OpenCV)
