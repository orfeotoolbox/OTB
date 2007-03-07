FILE(GLOB_RECURSE ossim_support_data_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_support_data_HDRS "*.h")
ADD_LIBRARY(ossim_support_data ${ossim_support_data_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_support_data)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_support_data_HRDS})
