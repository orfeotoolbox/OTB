#
# Current inforamtions:
# Version 2.84 released on April Fools' day, 2007.
# Download site : http://www.csie.ntu.edu.tw/~cjlin/libsvm/
#

PROJECT(OTBSVM)

# source files for otbsvm
SET(OTBSVM_SRCS
svm.cxx 
)

ADD_LIBRARY(otbsvm ${OTBSVM_SRCS})
TARGET_LINK_LIBRARIES (otbsvm ITKCommon)


INSTALL(TARGETS otbsvm
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)


FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")

INSTALL(FILES svm.h
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsvm 
    COMPONENT Development)

