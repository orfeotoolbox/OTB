project(OTBMuParserX)

set(OTBMuParserX_SYSTEM_INCLUDE_DIRS ${MUPARSERX_INCLUDE_DIRS})
set(OTBMuParserX_LIBRARIES "${MUPARSERX_LIBRARIES}")
set(OTBMuParserX_INCLUDE_DIRS ${OTBMuParserX_BINARY_DIR}/src)

# as advised by Ingo Berg : avoid using those #define with MSVC
if(OTB_CXX_HAS_UNIQUE_PTR OR MSVC)
  set(OTB_MUPARSERX_HIDE_UNIQUE_PTR 0)
else()
  set(OTB_MUPARSERX_HIDE_UNIQUE_PTR 1)
endif()

if(OTB_CXX_HAS_OVERRIDE_SPECIFIER OR MSVC)
  set(OTB_MUPARSERX_HIDE_OVERRIDE 0)
else()
  set(OTB_MUPARSERX_HIDE_OVERRIDE 1)
endif()

if(OTB_CXX_HAS_NULLPTR OR MSVC)
  set(OTB_MUPARSERX_HIDE_NULLPTR 0)
else()
  set(OTB_MUPARSERX_HIDE_NULLPTR 1)
endif()

configure_file( src/mpCompat.h.in src/mpCompat.h )

otb_module_impl()

install(FILES ${OTBMuParserX_BINARY_DIR}/src/mpCompat.h
  DESTINATION ${OTBMuParserX_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
