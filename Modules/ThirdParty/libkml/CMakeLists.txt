project(OTBlibkml)

set(OTBlibkml_SYSTEM_INCLUDE_DIRS ${LIBKML_INCLUDE_DIRS})
set(OTBlibkml_LIBRARIES "${LIBKML_LIBRARIES}")

otb_module_impl()
