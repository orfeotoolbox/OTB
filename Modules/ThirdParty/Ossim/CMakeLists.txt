project(OTBOssim)

set(OTBOssim_SYSTEM_INCLUDE_DIRS ${OSSIM_INCLUDE_DIRS})
set(OTBOssim_LIBRARIES "${OSSIM_LIBRARIES}")

otb_module_impl()
