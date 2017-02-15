project( OTBMonteverdi )

set( OTBMonteverdi_LIBRARIES OTBMonteverdi )

# folder where ui headers are generated
set( OTBMonteverdi_INCLUDE_DIRS
  ${OTBMonteverdi_BINARY_DIR}/src
  )

otb_module_impl()
