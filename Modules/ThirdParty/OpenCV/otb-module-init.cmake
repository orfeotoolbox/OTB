find_package ( OpenCV REQUIRED )
mark_as_advanced( OpenCV_DIR )

if("${OpenCV_VERSION}" VERSION_LESS "2.4.0")
  message(WARNING "The OpenCV version detected (${OpenCV_VERSION}) is older than 2.4.0, "
    "it may not be fully compatible with OTB")
endif()

if(OpenCV_VERSION_MAJOR EQUAL 3)
set(OTB_OPENCV_3 1)
else()
set(OTB_OPENCV_3 0)
endif()
