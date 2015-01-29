message(STATUS "Setup OpenCV...")

set(proj OPENCV)

set(DEFAULT_USE_SYSTEM_OPENCV OFF)

option(USE_SYSTEM_OPENCV "Use a system build of OpenCV." ${DEFAULT_USE_SYSTEM_OPENCV})
mark_as_advanced(USE_SYSTEM_OPENCV)

if(USE_SYSTEM_OPENCV)
  message(STATUS "  Using OpenCV system version")
else()
  set(${proj}_DEPENDENCIES)
  set(OPENCV_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(OPENCV_SB_SRC_DIR ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.10/opencv-2.4.10.zip/download"
    URL_MD5 ec63952d3a3dff965d5fdde765926821
    BINARY_DIR ${OPENCV_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DBUILD_DOCS:BOOL=OFF
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_JASPER:BOOL=OFF
      -DWITH_JASPER:BOOL=OFF
      -DBUILD_JPEG:BOOL=OFF
      -DWITH_JPEG:BOOL=OFF
      -DWITH_FFMPEG:BOOL=OFF
      -DWITH_VFW:BOOL=OFF
      -DBUILD_OPENEXR:BOOL=OFF
      -DBUILD_PACKAGE:BOOL=ON
      -DBUILD_PERF_TESTS:BOOL=OFF
      -DBUILD_PNG:BOOL=OFF
      -DBUILD_TBB:BOOL=OFF
      -DBUILD_TESTS:BOOL=OFF
      -DBUILD_TIFF:BOOL=OFF
      -DBUILD_ZLIB:BOOL=OFF
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
    DEPENDS ${${proj}_DEPENDENCIES}
    )
  
  message(STATUS "  Using OpenCV SuperBuild version")
endif()
