FILE(GLOB_RECURSE ossim_parallel_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_parallel_HDRS "*.h")
ADD_LIBRARY(ossim_parallel ${ossim_parallel_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_parallel)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_parallel_HRDS})
