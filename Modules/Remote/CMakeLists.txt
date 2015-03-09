# Functions to fetch remote modules.
include(OTBModuleRemote)

file(GLOB remotes "*.remote.cmake")
foreach(remote_module ${remotes})
  include(${remote_module})
endforeach()
