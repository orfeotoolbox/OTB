# This project is designed to be built outside the Insight source tree.
# Code from http://www.insight-journal.org/browse/publication/94
PROJECT(OTBKNN)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# source files for otbknn
#SET(OTBKNN_SRCS
#voronoiFilter.cxx
#classifyKNNCore.cxx
#)

# Adjust the compiler flags to avoid problems with ossim code.
#IF(CMAKE_COMPILER_IS_GNUCXX)
  # Hide warnings in ossim code.
#  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

  # CMake versions before 2.4 did not put the source-file-specific flags
  # after the directory-level flags.  Remove optimization flags from the
  # CMAKE_CXX_FLAGS* variables so they can be overridden.
#  IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)
#    FOREACH(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
#      STRING(REGEX REPLACE
#        "-O[^ ]*( |$)" "" CMAKE_CXX_FLAGS${c} "${CMAKE_CXX_FLAGS${c}}")
#    ENDFOREACH(c)
#  ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)
#ENDIF(CMAKE_COMPILER_IS_GNUCXX)

#ADD_LIBRARY(otbknn ${OTBKNN_SRCS})
#TARGET_LINK_LIBRARIES (otbknn ITKCommon ITKBasicFilters ITKIO)
#IF(OTB_LIBRARY_PROPERTIES)
#  SET_TARGET_PROPERTIES(otbknn PROPERTIES ${OTB_LIBRARY_PROPERTIES})
#ENDIF(OTB_LIBRARY_PROPERTIES)

#IF(NOT OTB_INSTALL_NO_LIBRARIES)
#  INSTALL(TARGETS otbknn
#    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
#    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
#    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
#ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  INSTALL(FILES ${__files}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbknn
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

# Find ITK.

#FIND_PACKAGE(ITK)
#IF(ITK_FOUND)
#  INCLUDE(${ITK_USE_FILE})
#ELSE(ITK_FOUND)
#  MESSAGE(FATAL_ERROR
#          "Cannot build without ITK.  Please set ITK_DIR.")
#ENDIF(ITK_FOUND)


#ADD_EXECUTABLE(classifyKNN2 classifyKNN2.cxx)
#ADD_LIBRARY(voronoiFilter voronoiFilter.cxx)
#ADD_LIBRARY(classifyKNNCore classifyKNNCore.cxx)
#TARGET_LINK_LIBRARIES(classifyKNN2 voronoiFilter classifyKNNCore ITKCommon ITKBasicFilters ITKIO )


