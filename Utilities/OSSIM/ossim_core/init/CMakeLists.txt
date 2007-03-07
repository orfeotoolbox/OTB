FILE(GLOB_RECURSE ossim_init_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_init_HDRS "*.h")
ADD_LIBRARY(ossim_init ${ossim_init_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_init)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_init_HRDS})
