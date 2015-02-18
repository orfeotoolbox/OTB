project(OTBEdison)
set(OTBEdison_THIRD_PARTY 1)

set(OTBEdison_INCLUDE_DIRS ${OTBEdison_SOURCE_DIR}/src/otbedison)
set(OTBEdison_LIBRARIES otbedison)

otb_module_impl()
