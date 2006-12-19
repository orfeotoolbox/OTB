# Sources of non-templated classes.

FILE(GLOB OTBDisparityMap_SRCS "*.cxx" )

ADD_LIBRARY(OTBDisparityMap ${OTBDisparityMap_SRCS})
TARGET_LINK_LIBRARIES (OTBDisparityMap ITKCommon)

INSTALL_TARGETS(/lib/otb OTBDisparityMap )
INSTALL_FILES(/include/otb/DisparityMap "(\\.h|\\.txx)$")
