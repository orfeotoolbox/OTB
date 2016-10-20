project(OTBMuParserX)

set(OTBMuParserX_SYSTEM_INCLUDE_DIRS ${MUPARSERX_INCLUDE_DIRS})
set(OTBMuParserX_LIBRARIES "${MUPARSERX_LIBRARIES}")
set(OTBMuParserX_INCLUDE_DIRS ${OTBMuParserX_BINARY_DIR}/src)

configure_file( src/mpCompat.h.in src/mpCompat.h )

otb_module_impl()

install(FILES ${OTBMuParserX_BINARY_DIR}/src/mpCompat.h
  DESTINATION ${OTBMuParserX_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
