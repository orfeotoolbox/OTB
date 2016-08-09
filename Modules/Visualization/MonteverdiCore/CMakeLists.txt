project( OTBMonteverdiCore )

configure_file(src/ConfigureMonteverdi.h.in ConfigureMonteverdi.h)

set(OTBMonteverdiCore_INCLUDE_DIRS
  ${OTBMonteverdiCore_BINARY_DIR}
  )

set( OTBMonteverdiCore_LIBRARIES OTBMonteverdiCore )

otb_module_impl()

install(FILES
  ${OTBMonteverdiCore_BINARY_DIR}/ConfigureMonteverdi.h
  DESTINATION ${OTBMonteverdiCore_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
