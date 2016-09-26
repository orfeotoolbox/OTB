set( OTBQtAdapters_SRC
  otbQtAdapters.cxx
  )

set( OTBQtAdapters_HEADERS_MOC
  # otbQtAdapters.h
  )

add_to_qt4_i18n_sources( ${OTBQtAdapters_SRCS} )
add_to_qt4_i18n_headers( "../include" ${OTBQtAdapters_SRCS} )

qt4_wrap_cpp( OTBQtAdapters_SRC_MOC ${OTBQtAdapters_HEADERS_MOC} )

include_directories( ${OTBCommon_BINARY_DIR} )

add_library( OTBQtAdapters ${OTBQtAdapters_SRC} ${OTBQtAdapters_SRC_MOC})

target_link_libraries( OTBQtAdapters 
  ${OTBQt4_LIBRARIES}
  )

otb_module_target( OTBQtAdapters )
