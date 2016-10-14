project(OTBShark)

set(OTBShark_SYSTEM_INCLUDE_DIRS ${SHARK_INCLUDE_DIRS})
set(OTBShark_LIBRARIES "${SHARK_LIBRARIES}")

otb_module_impl()
