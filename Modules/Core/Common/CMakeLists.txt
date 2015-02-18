project(OTBCommon)

configure_file(src/otbConfigure.h.in otbConfigure.h)

set(OTBCommon_INCLUDE_DIRS
  ${OTBCommon_BINARY_DIR}
  )

set(OTBCommon_LIBRARIES OTBCommon)

otb_module_impl()

install(FILES
  ${OTBCommon_BINARY_DIR}/otbConfigure.h
  DESTINATION ${OTBCommon_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
