project(OTBSupervised)

set(OTBSupervised_LIBRARIES OTBSupervised)

find_package(Shark REQUIRED)
include(${SHARK_USE_FILE})

otb_module_impl()
