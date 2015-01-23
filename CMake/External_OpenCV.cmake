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
  
  if(UNIX)
  
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
      -DBUILD_JPEG:BOOL=OFF
      -DBUILD_OPENEXR:BOOL=OFF
      -DBUILD_PACKAGE:BOOL=ON
      -DBUILD_PERF_TESTS:BOOL=OFF
      -DBUILD_PNG:BOOL=OFF
      -DBUILD_TBB:BOOL=OFF
      -DBUILD_TESTS:BOOL=OFF
      -DBUILD_TIFF:BOOL=OFF
      -DBUILD_ZLIB:BOOL=OFF
    DEPENDS ${${proj}_DEPENDENCIES}
    )
    
  else(MSVC)
  
  endif()
  
  message(STATUS "  Using OpenCV SuperBuild version")
endif()
