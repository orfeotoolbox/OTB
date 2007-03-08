
FILE(GLOB_RECURSE ossim_HDRS "*.h")
INSTALL_FILES(/include/otb/Utilities/OSSIM/include/ "\\.h$" ${ossim_HDRS})
