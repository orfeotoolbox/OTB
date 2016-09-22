set( OTBQtAdapters_SRC
  )

add_library( OTBQtAdapters ${OTBQtAdapters_SRC} )

target_link_libraries( OTBQtAdapters 
  ${OTBQt4_LIBRARIES}
  )

otb_module_target( OTBQtAdapters )
