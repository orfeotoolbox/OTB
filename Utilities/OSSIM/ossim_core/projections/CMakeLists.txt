FILE(GLOB_RECURSE ossim_projections_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_projections_HDRS "*.h")
ADD_LIBRARY(ossim_projections ${ossim_projections_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_projections)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_projections_HRDS})
