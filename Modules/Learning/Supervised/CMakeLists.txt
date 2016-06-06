project(OTBSupervised)

find_package(Shark REQUIRED)
include(${SHARK_USE_FILE})

set(OTBSupervised_LIBRARIES OTBSupervised)

otb_module_impl()
