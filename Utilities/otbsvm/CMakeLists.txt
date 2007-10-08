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

# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX)
  # Hide warnings in ossim code.
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

  # CMake versions before 2.4 did not put the source-file-specific flags
  # after the directory-level flags.  Remove optimization flags from the
  # CMAKE_CXX_FLAGS* variables so they can be overridden.
  IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)
    FOREACH(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
      STRING(REGEX REPLACE
        "-O[^ ]*( |$)" "" CMAKE_CXX_FLAGS${c} "${CMAKE_CXX_FLAGS${c}}")
    ENDFOREACH(c)
  ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_LIBRARY(otbsvm ${OTBSVM_SRCS})
TARGET_LINK_LIBRARIES (otbsvm ITKCommon)


INSTALL(TARGETS otbsvm
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)


FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

INSTALL(FILES ${__files}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsvm 
    COMPONENT Development)

