project(OTBOpenCV)

configure_file( src/otb_opencv_api.h.in src/otb_opencv_api.h )

set(OTBOpenCV_SYSTEM_INCLUDE_DIRS ${OPENCV_INCLUDE_DIRS})
set(OTBOpenCV_LIBRARIES "${OPENCV_LIBRARIES}")
set(OTBOpenCV_INCLUDE_DIRS
  ${OTBOpenCV_BINARY_DIR}/src
  )
set(OTBOpenCV_EXPORT_CODE_BUILD "find_package(OpenCV REQUIRED)")
set(OTBOpenCV_EXPORT_CODE_INSTALL ${OTBOpenCV_EXPORT_CODE_BUILD})

otb_module_impl()
