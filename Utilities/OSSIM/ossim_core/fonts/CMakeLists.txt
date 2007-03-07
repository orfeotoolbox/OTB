FILE(GLOB_RECURSE ossim_font_SRCS "*.cpp")
FILE(GLOB_RECURSE ossim_font_HDRS "*.h")
ADD_LIBRARY(ossim_font ${ossim_font_SRCS})

INSTALL_TARGETS(/lib/otb/ ossim_font)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_font_HRDS})
