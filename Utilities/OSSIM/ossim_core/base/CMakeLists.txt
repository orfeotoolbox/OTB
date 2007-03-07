FILE(GLOB_RECURSE ossim_base_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_base_HDRS "*.h")
ADD_LIBRARY(ossim_base ${ossim_base_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_base)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_base_HRDS})
