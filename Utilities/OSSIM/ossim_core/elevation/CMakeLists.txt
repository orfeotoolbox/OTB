FILE(GLOB_RECURSE ossim_elevation_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_elevation_HDRS "*.h")
ADD_LIBRARY(ossim_elevation ${ossim_elevation_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_elevation)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_elevation_HRDS})
