#############################################################################
project( OTBMonteverdiCore )

#############################################################################
set( Monteverdi_VERSION_MAJOR "3" )
set( Monteverdi_VERSION_MINOR "5" )
set( Monteverdi_VERSION_PATCH "0" )
set( Monteverdi_VERSION_SUFFIX "" )

set( Monteverdi_VERSION_STRING
  "${Monteverdi_VERSION_MAJOR}.${Monteverdi_VERSION_MINOR}.${Monteverdi_VERSION_PATCH}${Monteverdi_VERSION_SUFFIX}"
  )

set( Monteverdi_FLOATING_TYPE
  "float"
  CACHE
  STRING "The type of data in which input images are read (double/float)."
  )

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
