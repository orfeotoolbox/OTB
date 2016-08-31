project(OTBQwt)

set(OTBQwt_SYSTEM_INCLUDE_DIRS ${QWT_INCLUDE_DIRS})
set(OTBQwt_LIBRARIES "${QWT_LIBRARIES}")

otb_module_impl()
