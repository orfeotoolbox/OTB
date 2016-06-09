project(OTBShark)

set(OTBShark_SYSTEM_INCLUDE_DIRS ${Shark_INCLUDE_DIRS})
set(OTBShark_LIBRARIES "${Shark_LIBS}")

otb_module_impl()
