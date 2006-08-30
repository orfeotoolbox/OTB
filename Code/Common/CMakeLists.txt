# Sources of non-templated classes.

FILE(GLOB OTBCommon_SRCS "*.cxx" )

ADD_LIBRARY(OTBCommon ${OTBCommon_SRCS})
TARGET_LINK_LIBRARIES (OTBCommon ITKAlgorithms ITKStatistics ITKCommon)

INSTALL_TARGETS(/lib/otb OTBCommon )
INSTALL_FILES(/include/otb/Common "(\\.h|\\.txx)$")
