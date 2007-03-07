FILE(GLOB_RECURSE ossim_polyclip_SRCS "*.c")
FILE(GLOB_RECURSE ossim_polyclip_HDRS "*.h")
ADD_LIBRARY(ossim_polyclip ${ossim_polyclip_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_polyclip)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_polyclip_HRDS})
