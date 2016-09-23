#############################################################################
project( OTBMonteverdiCore )

#############################################################################

set( Monteverdi_FLOATING_TYPE
  "float"
  CACHE
  STRING "The type of data in which input images are read (double/float)."
  )

set(Monteverdi_UI_VERSION "1")

set(Monteverdi_CONFIGURE_FILE "ConfigureMonteverdi.h" )

set(Monteverdi_PROJECT_NAME "Monteverdi")

#############################################################################
configure_file( src/ConfigureMonteverdi.h.in ConfigureMonteverdi.h )

#############################################################################
set( OTBMonteverdiCore_INCLUDE_DIRS
  ${OTBMonteverdiCore_BINARY_DIR}
  )

#############################################################################
set( OTBMonteverdiCore_LIBRARIES OTBMonteverdiCore )

#############################################################################
otb_module_impl()

#############################################################################
install(FILES
  ${OTBMonteverdiCore_BINARY_DIR}/ConfigureMonteverdi.h
  DESTINATION ${OTBMonteverdiCore_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
