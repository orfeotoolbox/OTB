project(OTBCommon)

configure_file(src/otbConfigure.h.in otbConfigure.h.in)
file(APPEND ${OTBCommon_BINARY_DIR}/otbConfigure.h.in
     "/* Optional third parties activation status */\n")
foreach(_otb_module_activation_option IN LISTS OTB_MODULE_ACTIVATION_OPTION_LIST)
  file(APPEND ${OTBCommon_BINARY_DIR}/otbConfigure.h.in
       "#cmakedefine ${_otb_module_activation_option}\n")
endforeach()

set(OTB_WINDOWS_EXPORT_ALL_SYMBOLS ${CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS})
configure_file(${OTBCommon_BINARY_DIR}/otbConfigure.h.in otbConfigure.h)

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
