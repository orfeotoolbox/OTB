FILE(GLOB_RECURSE ossim_vec_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_vec_HDRS "*.h")
ADD_LIBRARY(ossim_vec ${ossim_vec_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_vec)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_vec_HRDS})
