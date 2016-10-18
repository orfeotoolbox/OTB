project(OTBShark)

configure_file( src/otb_shark.h.in src/otb_shark.h )

set(OTBShark_SYSTEM_INCLUDE_DIRS ${SHARK_INCLUDE_DIRS})
set(OTBShark_LIBRARIES "${SHARK_LIBRARIES}")
set(OTBShark_INCLUDE_DIRS
  ${OTBShark_BINARY_DIR}/src
  )

otb_module_impl()

install(FILES ${OTBShark_BINARY_DIR}/src/otb_shark.h
  DESTINATION ${OTBShark_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
