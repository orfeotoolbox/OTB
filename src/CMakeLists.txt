#${otb-module} will be the name of this module and will not need to be
#changed when this module is renamed.

set(${otb-module}_SRC
  otbMPIConfig.cpp
)

add_library(${otb-module} ${${otb-module}_SRC})
target_link_libraries(${otb-module}  ${OTBCommon_LIBRARIES})
otb_module_target(${otb-module})
