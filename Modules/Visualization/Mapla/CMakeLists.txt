project( OTBMapla )

set(OTBMapla_LIBRARIES OTBMapla)

# folder where ui headers are generated
set( OTBMapla_INCLUDE_DIRS
  ${OTBMapla_BINARY_DIR}/src
  )

otb_module_impl()
