FILE(GLOB_RECURSE ossim_imaging_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_imaging_HDRS "*.h")
ADD_LIBRARY(ossim_imaging ${ossim_imaging_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_imaging)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_imaging_HRDS})
