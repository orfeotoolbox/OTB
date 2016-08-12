project( OTBMonteverdiGUI )

set( OTBMonteverdiGUI_LIBRARIES OTBMonteverdiGUI )

# folder where ui headers are generated
set( OTBMonteverdiGUI_INCLUDE_DIRS
  ${OTBMonteverdiGUI_BINARY_DIR}/src
  )

otb_module_impl()
