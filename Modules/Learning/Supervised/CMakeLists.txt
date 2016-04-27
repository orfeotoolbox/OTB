project(OTBSupervised)

set(OTBSupervised_LIBRARIES OTBSupervised)

option(OTB_USE_SHARK "Enable Shark in OTB" OFF)
if(OTB_USE_SHARK)
  find_package(Shark REQUIRED)
  include(${SHARK_USE_FILE})
endif()

otb_module_impl()
