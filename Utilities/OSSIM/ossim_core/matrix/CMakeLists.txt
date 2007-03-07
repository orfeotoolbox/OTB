FILE(GLOB_RECURSE ossim_matrix_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_matrix_HDRS "*.h")
ADD_LIBRARY(ossim_matrix ${ossim_matrix_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_matrix)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_matrix_HRDS})
