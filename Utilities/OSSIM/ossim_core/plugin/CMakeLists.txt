FILE(GLOB_RECURSE ossim_plugin_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_plugin_HDRS "*.h")
ADD_LIBRARY(ossim_plugin ${ossim_plugin_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_plugin)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_plugin_HRDS})
