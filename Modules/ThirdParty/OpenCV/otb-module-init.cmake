find_package ( OpenCV REQUIRED )
mark_as_advanced( OpenCV_DIR )

if("${OpenCV_VERSION}" VERSION_LESS "2.4.0")
  message(WARNING "The OpenCV version detected (${OpenCV_VERSION}) is older than 2.4.0, "
    "it may not be fully compatible with OTB")
endif()
