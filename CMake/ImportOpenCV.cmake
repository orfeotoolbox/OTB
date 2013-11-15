message(STATUS "Importing OpenCV...")

option(OTB_USE_OPENCV "Use OpenCV library for classification." OFF)
mark_as_advanced(OTB_USE_OPENCV)

if(OTB_USE_OPENCV)
  message(STATUS "  Enabling OpenCV support")
  find_package(OpenCV
               REQUIRED
               PATHS
                 /opt/local/lib/cmake # MacPort
               )
  message(STATUS "  Found OpenCV ${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}")
  message(STATUS "  OpenCV include directory: ${OpenCV_INCLUDE_DIRS}")
else()
  message(STATUS "  Disabling OpenCV support (OTB_USE_OPENCV is OFF)")
endif()
