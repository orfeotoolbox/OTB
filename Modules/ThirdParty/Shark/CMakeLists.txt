project(OTBShark)

set(OTBShark_SYSTEM_INCLUDE_DIRS ${SHARK_INCLUDE_DIRS})
set(OTBShark_LIBRARIES "${SHARK_LIBRARIES}")

if(MSVC)
  add_definitions(-DBOOST_ALL_NO_LIB)
endif()
otb_module_impl()
