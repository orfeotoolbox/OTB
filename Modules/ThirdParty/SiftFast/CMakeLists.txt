project(OTBSiftFast)
set(OTBSiftFast_THIRD_PARTY 1)

set(OTBSiftFast_INCLUDE_DIRS ${OTBSiftFast_SOURCE_DIR}/src)
set(OTBSiftFast_LIBRARIES otbsiftfast)

otb_module_impl()
