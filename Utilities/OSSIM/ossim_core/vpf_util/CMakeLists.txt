FILE(GLOB_RECURSE ossim_vpf_SRCS "*.c")
FILE(GLOB_RECURSE ossim_vpf_HDRS "*.h")
ADD_LIBRARY(ossim_vpf ${ossim_vpf_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_vpf)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_vpf_HRDS})
